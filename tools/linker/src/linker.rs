// SPDX-License-Identifier: Apache-2.0 OR MIT

use std::convert::TryFrom;

use object::{Object, ObjectSection, ObjectSymbol, Relocation, RelocationKind, SymbolIndex, SymbolKind};
use object::elf::{R_PPC_ADDR16_HA, R_PPC_ADDR16_LO, R_PPC_REL24};
use object::read::RelocationTarget;

use crate::ElfFile32Be;
use crate::removal::{Removal, RemovalRemoveError};
use crate::removals::{Removals, RemovalInsertError};
use crate::section::Section;
use crate::symbol_map::SymbolMap;
use crate::symbol_names::SymbolNames;

#[derive(Debug)]
pub struct Linker<'data> {
    symbol_map: SymbolMap,
    symbols_to_remove: SymbolNames,
    symbols_to_replace: SymbolNames,
    elf_file: ElfFile32Be<'data>,
    removals: Removals,
    insertions: Vec<Insertion>,
    patches: Vec<Patch>,
}

impl Linker<'_> {
    pub fn new(symbol_map: SymbolMap, symbols_to_remove: SymbolNames, symbols_to_replace: SymbolNames, elf_file: ElfFile32Be) -> Linker {
        Linker {
            symbol_map,
            symbols_to_remove,
            symbols_to_replace,
            elf_file,
            removals: Removals::new(),
            insertions: vec![],
            patches: vec![],
        }
    }

    pub fn run(mut self) -> Result<Vec<u8>, LinkerError> {
        self.setup_removals()?;
        self.setup_replacements()?;
        self.setup_insertions()?;

        self.process_deletions();
        self.process_insertions()?;

        Ok(self.serialize_patches())
    }

    fn setup_removals(&mut self) -> Result<(), LinkerError> {
        for symbol_name in self.symbols_to_remove.iter() {
            let symbol = self.symbol_map.find(symbol_name).ok_or(LinkerError::SymbolNotFound)?;
            let removal = Removal::from(symbol);
            self.removals.try_insert(removal)?;
        }

        Ok(())
    }

    fn setup_replacements(&mut self) -> Result<(), LinkerError> {
        for symbol_name in self.symbols_to_replace.iter() {
            let symbol = self.symbol_map.find(symbol_name).ok_or(LinkerError::SymbolNotFound)?;
            let elf_symbol = self.elf_file
                .symbols()
                .find(|elf_symbol| elf_symbol.name() == Ok(symbol_name))
                .ok_or(LinkerError::SymbolNotFound)?;

            let elf_section_index = elf_symbol.section_index().ok_or(LinkerError::InvalidSectionIndex)?;
            let elf_section = self.elf_file.section_by_index(elf_section_index).unwrap();

            let elf_section_name = elf_section.name().map_err(|_| LinkerError::InvalidSectionName)?;
            if Section::try_from_elf_name(elf_section_name) != Some(symbol.section()) {
                return Err(LinkerError::MismatchingSections)?;
            }

            let mut removal = Removal::from(symbol);
            let start = removal.try_remove(elf_symbol.size() as u32)?;
            self.removals.try_insert(removal)?;

            let elf_symbol_index = elf_symbol.index();
            let insertion = Insertion { start, elf_symbol_index };
            self.insertions.push(insertion);
        }

        Ok(())
    }

    fn setup_insertions(&mut self) -> Result<(), LinkerError> {
        for elf_symbol in self.elf_file.symbols() {
            if elf_symbol.kind() != SymbolKind::Text && elf_symbol.kind() != SymbolKind::Data {
                continue;
            }

            let elf_symbol_name = elf_symbol.name().map_err(|_| LinkerError::InvalidSymbolName)?;
            if self.symbols_to_replace.contains(elf_symbol_name) {
                continue;
            }

            if elf_symbol.is_weak() && self.symbol_map.contains(elf_symbol_name) {
                continue;
            }

            let elf_section_index = elf_symbol.section_index().unwrap();
            let elf_section = self.elf_file.section_by_index(elf_section_index).unwrap();

            let elf_section_name = elf_section.name().map_err(|_| LinkerError::InvalidSectionName)?;
            let section = Section::try_from_elf_name(elf_section_name).ok_or(LinkerError::InvalidSectionName)?;

            let start = self.removals.try_remove(elf_symbol.size() as u32, section)?;
            let elf_symbol_index = elf_symbol.index();
            let insertion = Insertion { start, elf_symbol_index };
            self.insertions.push(insertion);
        }

        Ok(())
    }

    fn process_deletions(&mut self) {
        for removal in self.removals.iter() {
            let start = removal.start();
            let data = vec![0; removal.size() as usize];
            let patch = Patch { start, data };
            self.patches.push(patch);
        }
    }

    fn process_insertions(&mut self) -> Result<(), LinkerError> {
        for insertion in &self.insertions {
            let elf_symbol = self.elf_file.symbol_by_index(insertion.elf_symbol_index).unwrap();

            let elf_section_index = elf_symbol.section_index().unwrap();
            let elf_section = self.elf_file.section_by_index(elf_section_index).unwrap();

            let data = match elf_section.data_range(elf_symbol.address(), elf_symbol.size()) {
                Ok(Some(data)) => data,
                _ => return Err(LinkerError::InvalidSymbolRange),
            };
            let mut data = data.to_vec();

            for (offset, relocation) in elf_section.relocations() {
                if offset < elf_symbol.address() {
                    continue;
                }
                if offset >= elf_symbol.address() + elf_symbol.size() {
                    continue;
                }

                let offset = offset.checked_sub(elf_symbol.address()).ok_or(LinkerError::InvalidRelocationOffset)? as u32;
                let address = insertion.start + offset;
                self.process_relocation(&mut data, offset, &relocation, address)?;
            }

            let start = insertion.start;
            let patch = Patch { start, data };
            self.patches.push(patch);
        }

        Ok(())
    }

    fn process_relocation(&self, data: &mut [u8], offset: u32, relocation: &Relocation, address: u32) -> Result<(), LinkerError> {
        let target_elf_symbol_index = match relocation.target() {
            RelocationTarget::Symbol(target_elf_symbol_index) => target_elf_symbol_index,
            _ => return Err(LinkerError::InvalidRelocationTarget),
        };
        let target_elf_symbol = self.elf_file.symbol_by_index(target_elf_symbol_index).unwrap();
        let target_elf_symbol_name = target_elf_symbol.name().map_err(|_| LinkerError::InvalidSymbolName)?;
        let target_address = self.find_symbol_start(target_elf_symbol_name)?;

        match relocation.kind() {
            RelocationKind::Absolute => {
                Self::apply_relocation_u32(data, offset, |_| target_address)?;
            },
            RelocationKind::Elf(R_PPC_ADDR16_HA) => {
                Self::apply_relocation_u16(data, offset, |_| {
                    ((target_address >> 16) + (target_address >> 15 & 1)) as u16
                })?;
            },
            RelocationKind::Elf(R_PPC_ADDR16_LO) => {
                Self::apply_relocation_u16(data, offset, |_| {
                    target_address as u16
                })?;
            }
            RelocationKind::Elf(R_PPC_REL24) => {
                Self::apply_relocation_u32(data, offset, |val| {
                    val & 0xfc000003 | target_address.wrapping_sub(address) & 0x03fffffc
                })?;
            },
            _ => return Err(LinkerError::UnsupportedRelocationKind),
        }

        Ok(())
    }

    fn find_symbol_start(&self, symbol_name: &str) -> Result<u32, LinkerError> {
        self.insertions
            .iter()
            .find_map(|insertion| {
                let elf_symbol = self.elf_file.symbol_by_index(insertion.elf_symbol_index).unwrap();
                let elf_symbol_name = elf_symbol.name().unwrap();
                if elf_symbol_name != symbol_name {
                    return None;
                }

                Some(insertion.start)
            })
            .or_else(|| self.symbol_map.find(symbol_name).map(|symbol| symbol.start()))
            .ok_or(LinkerError::SymbolNotFound)
    }

    fn apply_relocation_u16<F: Fn(u16) -> u16>(data: &mut [u8], offset: u32, f: F) -> Result<(), LinkerError> {
        let offset = offset as usize;
        let slice = data.get_mut(offset..offset + 2).ok_or(LinkerError::InvalidRelocationOffset)?;
        let arr = <&mut [u8; 2]>::try_from(slice).unwrap();
        let val = u16::from_be_bytes(*arr);
        let val = f(val);
        *arr = u16::to_be_bytes(val);
        Ok(())
    }

    fn apply_relocation_u32<F: Fn(u32) -> u32>(data: &mut [u8], offset: u32, f: F) -> Result<(), LinkerError> {
        let offset = offset as usize;
        let slice = data.get_mut(offset..offset + 4).ok_or(LinkerError::InvalidRelocationOffset)?;
        let arr = <&mut [u8; 4]>::try_from(slice).unwrap();
        let val = u32::from_be_bytes(*arr);
        let val = f(val);
        *arr = u32::to_be_bytes(val);
        Ok(())
    }

    fn serialize_patches(mut self) -> Vec<u8> {
        self.patches.sort_unstable_by_key(|patch| patch.start);

        self.patches.dedup_by(|p1, p0| {
            let can_be_merged = p0.start + p0.data.len() as u32 == p1.start;
            if can_be_merged {
                p0.data.append(&mut p1.data);
            }
            can_be_merged
        });

        let mut serialized = vec![];
        for patch in self.patches {
            serialized.extend(u32::to_be_bytes(patch.start));
            serialized.extend(u32::to_be_bytes(patch.data.len() as u32));
            serialized.extend(patch.data);
        }
        serialized
    }
}

#[derive(Debug)]
struct Insertion {
    start: u32,
    elf_symbol_index: SymbolIndex,
}

#[derive(Debug)]
struct Patch {
    start: u32,
    data: Vec<u8>,
}

#[derive(Debug)]
pub enum LinkerError {
    InvalidRelocationOffset,
    InvalidRelocationTarget,
    InvalidSectionIndex,
    InvalidSectionName,
    InvalidSymbolName,
    InvalidSymbolRange,
    MismatchingSections,
    SymbolNotFound,
    UnsupportedRelocationKind,
    RemovalInsert(RemovalInsertError),
    RemovalRemove(RemovalRemoveError),
}

impl From<RemovalInsertError> for LinkerError {
    fn from(e: RemovalInsertError) -> LinkerError {
        LinkerError::RemovalInsert(e)
    }
}

impl From<RemovalRemoveError> for LinkerError {
    fn from(e: RemovalRemoveError) -> LinkerError {
        LinkerError::RemovalRemove(e)
    }
}
