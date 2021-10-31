#!/usr/bin/env python3


import os

from vendor.ninja_syntax import Writer


n = Writer(open('build.ninja', 'w'))

n.variable('ninja_required_version', '1.3')
n.newline()

n.variable('builddir', 'build')
n.newline()

n.variable('configure', os.path.join('.', 'configure.py'))
n.newline()

n.rule(
    'configure',
    command = '$configure',
    generator = True,
)
n.build(
    'build.ninja',
    'configure',
    implicit = [
        '$configure',
        os.path.join('vendor', 'ninja_syntax.py'),
    ],
)
n.newline()

n.variable('cc', os.path.join('llvm-project', 'build', 'bin', 'clang'))
n.variable('cxx', os.path.join('llvm-project', 'build', 'bin', 'clang'))
n.variable('ld', os.path.join('llvm-project', 'build', 'bin', 'clang'))
n.variable('find', os.path.join('.', 'find_replacements.py'))
n.variable('objcopy', os.path.join('llvm-project', 'build', 'bin', 'llvm-objcopy'))
n.newline()

cflags = [
    '-fdata-sections',
    '-ffunction-sections',
    '-fno-asynchronous-unwind-tables',
    '-fshort-wchar',
    '-iquote', 'source',
    '-isystem', 'source',
    '-nostdlib',
    '-O2',
    '-target', 'powerpc-kuribo-eabi',
    '-Wall',
]
cxxflags = [
    '$cflags',
    '-fno-exceptions',
    '-fno-rtti',
]
packflags = [
    '-fuse-ld=lld',
    '-nostdlib',
    '-target', 'powerpc-kuribo-eabi',
]
linkflags = [
    '$packflags',
    '-Wl,--gc-sections',
    '-Wl,--nmagic',
    '-Wl,--no-dynamic-linker',
    '-Wl,--oformat,binary',
    '-Wl,-T,rmcp.ld',
]
n.variable('cflags', ' '.join(cflags))
n.variable('cxxflags', ' '.join(cxxflags))
n.variable('packflags', ' '.join(packflags))
n.variable('linkflags', ' '.join(linkflags))
n.newline()

n.rule(
    'cc',
    command = '$cc -MMD -MT $out -MF $out.d $cflags -c $in -o $out',
    depfile = '$out.d',
    deps = 'gcc',
    description = 'CC $out',
)
n.newline()

n.rule(
    'cxx',
    command = '$cxx -MMD -MT $out -MF $out.d $cxxflags -c $in -o $out',
    depfile = '$out.d',
    deps = 'gcc',
    description = 'CXX $out',
)
n.newline()

n.rule(
    'pack',
    command = '$ld $packflags -r $in -o $out',
    description = 'PACK $out',
)
n.newline()

n.rule(
    'find',
    command = '$find $in $out',
    description = 'FIND $out',
)
n.newline()

n.rule(
    'rename',
    command = '$objcopy --redefine-syms=$renamings $packed $out',
    description = 'RENAME $out',
)
n.newline()

linkparams = [
    '-Wl,--entry=$entry',
    '-Wl,-image-base=$base',
    '-Wl,--section-start=.text.$entry=$base',
]
n.rule(
    'link',
    command = '$ld $linkflags ' + ' '.join(linkparams) + ' $in -o $out',
    description = 'LINK $out',
)
n.newline()

sourcefiles = {
    'loader': [
        os.path.join('game', 'host_system', 'Loader.cxx'),
        os.path.join('game', 'host_system', 'Rel.cxx'),
    ],
    'client': [
        os.path.join('game', 'host_system', 'BootStrapScene.cxx'),
        os.path.join('game', 'host_system', 'Dol.cxx'),
        os.path.join('game', 'host_system', 'Patcher.cxx'),
        os.path.join('game', 'host_system', 'Payload.cxx'),
        os.path.join('game', 'host_system', 'Rel.cxx'),
        os.path.join('game', 'host_system', 'SceneCreatorStatic.cxx'),
        os.path.join('game', 'host_system', 'SceneManager.cxx'),
        os.path.join('game', 'host_system', 'System.cxx'),
    ],
    'server': [],
}
ofiles = {target: [] for target in sourcefiles}

for target in sourcefiles:
    for sourcefile in sourcefiles[target]:
        base, ext = os.path.splitext(sourcefile)
        ofile = os.path.join('$builddir', target, base + '.o')
        rule = {
            '.c': 'cc',
            '.cxx': 'cxx',
        }[ext]
        n.build(
            ofile,
            rule,
            os.path.join('source', sourcefile),
        )
        ofiles[target] += [ofile]
    n.newline()

n.build(
    os.path.join('$builddir', 'loader.bin'),
    'link',
    ofiles['loader'],
    variables = {
        'entry': 'main__Q26System6LoaderFv',
        'base': '0x80004000',
    },
)
n.newline()

n.build(
    os.path.join('$builddir', 'client', 'packed.o'),
    'pack',
    ofiles['client'],
)
n.newline()
ofiles['client'] = []

n.build(
    [
        os.path.join('$builddir', 'client', 'renamings.txt'),
        os.path.join('$builddir', 'client', 'Patches.cxx'),
    ],
    'find',
    os.path.join('$builddir', 'client', 'packed.o'),
    implicit = '$find',
)
n.newline()

n.build(
    os.path.join('$builddir', 'client', 'renamed.o'),
    'rename',
    [
        os.path.join('$builddir', 'client', 'packed.o'),
        os.path.join('$builddir', 'client', 'renamings.txt'),
    ],
    variables = {
        'packed': os.path.join('$builddir', 'client', 'packed.o'),
        'renamings': os.path.join('$builddir', 'client', 'renamings.txt'),
    },
)
n.newline()
ofiles['client'] += [os.path.join('$builddir', 'client', 'renamed.o')]

n.build(
    os.path.join('$builddir', 'client', 'Patches.o'),
    'cxx',
    os.path.join('$builddir', 'client', 'Patches.cxx'),
)
n.newline()
ofiles['client'] += [os.path.join('$builddir', 'client', 'Patches.o')]

n.build(
    os.path.join('$builddir', 'client.bin'),
    'link',
    ofiles['client'],
    variables = {
        'entry': 'main__Q26System7PayloadFv',
        'base': '0x8076db60',
    },
)
