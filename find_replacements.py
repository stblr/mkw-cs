#!/usr/bin/env python3


from argparse import ArgumentParser
from elftools.elf.elffile import ELFFile


parser = ArgumentParser()
parser.add_argument('in_path')
parser.add_argument('renamings_path')
parser.add_argument('patches_path')
args = parser.parse_args()

elf = ELFFile(open(args.in_path, 'rb'))

replacements_section_index = None
for index, section in enumerate(elf.iter_sections()):
    if section.name == 'replacements':
        replacements_section_index = index

symtab = elf.get_section_by_name('.symtab')
symbols = []
for symbol in symtab.iter_symbols():
    if symbol['st_shndx'] != replacements_section_index:
        continue

    if symbol['st_info']['type'] != 'STT_FUNC':
        continue

    symbols += [symbol]
renamings = open(args.renamings_path, 'w')
for symbol in symbols:
    renamings.write(symbol.name + ' my_' + symbol.name + '\n')

patches = open(args.patches_path, 'w')
patches.write(
    '#include "game/host_system/Patcher.hxx"\n' +
    '\n' +
    'namespace System {\n' +
    'namespace Patcher {\n' +
    '\n'
);
for symbol in symbols:
    patches.write(
        'extern "C" u8 ' + symbol.name + ';\n' +
        'extern "C" u8 my_' + symbol.name + ';\n'
    );
patches.write(
    '\n' +
    'const u32 patchCount = ' + str(len(symbols)) + ';\n' +
    'const Patch patches[] = {\n'
);
for symbol in symbols:
    patches.write('    { &' + symbol.name + ', &my_' + symbol.name + ' },\n')
patches.write('};\n')
patches.write('\n')
patches.write('} // namespace Patcher\n')
patches.write('} // namespace System\n')
