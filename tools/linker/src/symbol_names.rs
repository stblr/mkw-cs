// SPDX-License-Identifier: Apache-2.0 OR MIT

use std::convert::Infallible;
use std::str::FromStr;

#[derive(Debug)]
pub struct SymbolNames {
    symbol_names: Box<[Box<str>]>,
}

impl SymbolNames {
    pub fn contains(&self, symbol_name: &str) -> bool {
        self.iter().any(|s| s == symbol_name)
    }

    pub fn iter(&self) -> impl Iterator<Item=&str> {
        self.symbol_names.iter().map(|s| &**s)
    }
}

impl FromStr for SymbolNames {
    type Err = Infallible;

    fn from_str(s: &str) -> Result<SymbolNames, Infallible> {
        let symbol_names = s
            .lines()
            .map(|s| Box::from(s))
            .collect();

        Ok(SymbolNames { symbol_names })
    }
}
