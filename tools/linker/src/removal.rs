// SPDX-License-Identifier: Apache-2.0 OR MIT

use std::cmp::Ordering;

use crate::section::Section;
use crate::symbol::Symbol;

#[derive(Clone, Copy, Debug)]
pub struct Removal {
    start: u32,
    size: u32,
    section: Section,
}

impl Removal {
    pub fn start(&self) -> u32 {
        self.start
    }

    pub fn end(&self) -> u32 {
        self.start + self.size
    }

    pub fn size(&self) -> u32 {
        self.size
    }

    pub fn section(&self) -> Section {
        self.section
    }

    pub fn try_merge(self, other: Removal) -> Result<Removal, RemovalMergeError> {
        if self.section != other.section {
            return Err(RemovalMergeError::DifferentSections);
        }

        match self.end().cmp(&other.start) {
            Ordering::Less => Err(RemovalMergeError::Separate),
            Ordering::Equal => {
                Ok(Removal {
                    start: self.start,
                    size: self.size + other.size,
                    section: self.section,
                })
            },
            Ordering::Greater => Err(RemovalMergeError::Overlapping),
        }
    }

    pub fn try_remove(&mut self, size: u32) -> Result<u32, RemovalRemoveError> {
        self.size = self.size.checked_sub(size).ok_or(RemovalRemoveError::NotEnoughSpace)?;
        let old_start = self.start;
        self.start += size;
        Ok(old_start)
    }
}

#[derive(Debug)]
pub enum RemovalMergeError {
    DifferentSections,
    Overlapping,
    Separate,
}

#[derive(Debug)]
pub enum RemovalRemoveError {
    NotEnoughSpace
}

impl From<&Symbol> for Removal {
    fn from(symbol: &Symbol) -> Removal {
        Removal {
            start: symbol.start(),
            size: symbol.size(),
            section: symbol.section(),
        }
    }
}
