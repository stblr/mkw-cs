// SPDX-License-Identifier: Apache-2.0 OR MIT

mod linker;
mod removal;
mod removals;
mod section;
mod symbol;
mod symbol_map;
mod symbol_names;

use std::env;
use std::fmt::Debug;
use std::fs;
use std::io::Error as IoError;
use std::path::PathBuf;

use object::BigEndian;
use object::Error as ObjectError;
use object::elf::FileHeader32;
use object::read::elf::ElfFile;

use linker::{Linker, LinkerError};
use symbol::ParseSymbolError;
use symbol_map::SymbolMap;
use symbol_names::SymbolNames;

type FileHeader32Be = FileHeader32<BigEndian>;
type ElfFile32Be<'data, R = &'data [u8]> = ElfFile<'data, FileHeader32Be, R>;

fn main() -> Result<(), Error> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 5 {
        return Err(Error::WrongArgCount);
    }

    let symbol_map = fs::read_to_string(&args[1])?;
    let symbol_map = symbol_map.parse::<SymbolMap>()?;

    let symbols_to_remove = fs::read_to_string(&args[2])?;
    let symbols_to_remove = symbols_to_remove.parse::<SymbolNames>().unwrap();

    let symbols_to_replace = fs::read_to_string(&args[3])?;
    let symbols_to_replace = symbols_to_replace.parse::<SymbolNames>().unwrap();

    let elf_file = fs::read(&args[4])?;
    let elf_file = ElfFile32Be::parse(&*elf_file)?;

    let linker = Linker::new(symbol_map, symbols_to_remove, symbols_to_replace, elf_file);
    let output = linker.run()?;

    let mut output_path = PathBuf::from(&args[4]);
    output_path.set_extension("bin");
    fs::write(&output_path, output)?;

    Ok(())
}

#[derive(Debug)]
enum Error {
    Io(IoError),
    Linker(LinkerError),
    Object(ObjectError),
    ParseSymbol(ParseSymbolError),
    WrongArgCount,
}

impl From<IoError> for Error {
    fn from(e: IoError) -> Error {
        Error::Io(e)
    }
}

impl From<LinkerError> for Error {
    fn from(e: LinkerError) -> Error {
        Error::Linker(e)
    }
}

impl From<ObjectError> for Error {
    fn from(e: ObjectError) -> Error {
        Error::Object(e)
    }
}

impl From<ParseSymbolError> for Error {
    fn from(e: ParseSymbolError) -> Error {
        Error::ParseSymbol(e)
    }
}
