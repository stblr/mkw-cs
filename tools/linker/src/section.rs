// SPDX-License-Identifier: Apache-2.0 OR MIT

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Section {
    Text,
    Rodata,
    Data,
    Bss,
}

impl Section {
    pub fn try_from_virtual_address(virtual_address: u32) -> Option<Section> {
        match virtual_address {
            0x800072c0..=0x80244ddf => Some(Section::Text),
            0x80244ec0..=0x8025857f => Some(Section::Rodata),
            0x80258580..=0x802a403f => Some(Section::Data),
            0x802a4080..=0x80384bff => Some(Section::Bss),
            _ => None,
        }
    }

    pub fn try_from_elf_name(elf_section_name: &str) -> Option<Section> {
        match elf_section_name.split(".").skip(1).next()? {
            "text" => Some(Section::Text),
            "rodata" => Some(Section::Rodata),
            "data" => Some(Section::Data),
            "bss" => Some(Section::Bss),
            _ => None,
        }
    }
}
