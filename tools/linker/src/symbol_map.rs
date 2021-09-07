// SPDX-License-Identifier: Apache-2.0 OR MIT

use std::str::FromStr;

use crate::symbol::{ParseSymbolError, Symbol};

#[derive(Debug)]
pub struct SymbolMap {
    symbols: Box<[Symbol]>,
}

impl SymbolMap {
    pub fn contains(&self, symbol_name: &str) -> bool {
        self.find(symbol_name).is_some()
    }

    pub fn find(&self, symbol_name: &str) -> Option<&Symbol> {
        self.symbols
            .iter()
            .find(|symbol| symbol.name() == symbol_name)
    }
}

impl FromStr for SymbolMap {
    type Err = ParseSymbolError;

    fn from_str(s: &str) -> Result<SymbolMap, ParseSymbolError> {
        let symbols = s
            .lines()
            .map(FromStr::from_str)
            .collect::<Result<_, _>>()?;

        Ok(SymbolMap { symbols })
    }
}
