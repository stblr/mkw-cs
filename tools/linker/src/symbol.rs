// SPDX-License-Identifier: Apache-2.0 OR MIT

use std::num::ParseIntError;
use std::str::FromStr;

use crate::section::Section;

#[derive(Clone, Debug)]
pub struct Symbol {
    start: u32,
    size: u32,
    name: String,
    section: Section,
}

impl Symbol {
    pub fn start(&self) -> u32 {
        self.start
    }

    pub fn size(&self) -> u32 {
        self.size
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn section(&self) -> Section {
        self.section
    }
}

impl FromStr for Symbol {
    type Err = ParseSymbolError;

    fn from_str(s: &str) -> Result<Symbol, ParseSymbolError> {
        let mut vals = s.split_ascii_whitespace();

        let start = vals.next().ok_or(ParseSymbolError::NotEnoughTokens)?;
        let start = u32::from_str_radix(start, 16)?;

        let size = vals.next().ok_or(ParseSymbolError::NotEnoughTokens)?;
        let size = u32::from_str_radix(size, 16)?;

        let name = vals.next().ok_or(ParseSymbolError::NotEnoughTokens)?;
        let name = name.to_owned();

        if vals.next().is_some() {
            return Err(ParseSymbolError::TooManyTokens);
        }

        let section = match Section::try_from_virtual_address(start) {
            Some(section) => section,
            None => return Err(ParseSymbolError::UnknownSection),
        };

        Ok(Symbol {
            start,
            size,
            name,
            section,
        })
    }
}

#[derive(Debug)]
pub enum ParseSymbolError {
    NotEnoughTokens,
    TooManyTokens,
    UnknownSection,
    ParseInt(ParseIntError),
}

impl From<ParseIntError> for ParseSymbolError {
    fn from(e: ParseIntError) -> ParseSymbolError {
        ParseSymbolError::ParseInt(e)
    }
}
