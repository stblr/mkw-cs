#!/usr/bin/env python3


import os
from vendor.ninja_syntax import Writer


n = Writer(open('build.ninja', 'w'))

n.variable('ninja_required_version', '1.3')
n.newline()

n.variable('builddir', 'build')
n.newline()

n.variable('cc', os.path.join('tools', 'clang'))
n.variable('cxx', os.path.join('tools', 'clang'))
n.variable('port', os.path.join('.', 'port.py'))
n.variable('ld', os.path.join('tools', 'clang'))
n.newline()

cflags = [
    '-fdata-sections',
    '-ffunction-sections',
    '-fno-asynchronous-unwind-tables',
    '-fno-zero-initialized-in-bss',
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
linkflags = [
    '-fuse-ld=lld',
    '-nostdlib',
    '-target', 'powerpc-kuribo-eabi',
    '-Wl,--gc-sections',
    '-Wl,--nmagic',
    '-Wl,--no-dynamic-linker',
    '-Wl,--oformat,binary',
]
n.variable('cflags', ' '.join(cflags))
n.variable('cxxflags', ' '.join(cxxflags))
n.variable('linkflags', ' '.join(linkflags))
n.newline()

n.rule(
    'cc',
    command = '$cc -MMD -MT $out -MF $out.d $cflags -D $target -c $in -o $out',
    depfile = '$out.d',
    deps = 'gcc',
    description = 'CC $out',
)
n.newline()

n.rule(
    'cxx',
    command = '$cxx -MMD -MT $out -MF $out.d $cxxflags -D $target -c $in -o $out',
    depfile = '$out.d',
    deps = 'gcc',
    description = 'CXX $out',
)
n.newline()

n.rule(
    'port',
    command = '$port $region $in $out',
    description = 'PORT $out'
)
n.newline()

linkparams = [
    '-Wl,--entry=$entry',
    '-Wl,-image-base=$base',
    '-Wl,--section-start=.text.$entry=$base',
    '-Wl,-T,$script',
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
        os.path.join('game', 'host_system', 'RkSystem.cxx'),
        os.path.join('game', 'host_system', 'SceneCreatorStatic.cxx'),
        os.path.join('game', 'host_system', 'SceneManager.cxx'),
    ],
    'server': [
        os.path.join('game', 'host_system', 'Dol.cxx'),
        os.path.join('game', 'host_system', 'Patcher.cxx'),
        os.path.join('game', 'host_system', 'Payload.cxx'),
        os.path.join('game', 'host_system', 'Rel.cxx'),
        os.path.join('game', 'host_system', 'RkSystem.cxx'),
    ],
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
            variables = {
                'target': target.upper(),
            },
        )
        ofiles[target] += [ofile]
    n.newline()

for region in ['P', 'E', 'J', 'K']:
    n.build(
        os.path.join('$builddir', f'RMC{region}.ld'),
        'port',
        os.path.join('.', 'symbols.txt'),
        variables = {
            'region': region,
        },
        implicit = '$port',
    )
    n.newline()

for region in ['P', 'E', 'J', 'K']:
    n.build(
        os.path.join('$builddir', f'loader{region}.bin'),
        'link',
        ofiles['loader'],
        variables = {
            'entry': 'main__Q26System6LoaderFv',
            'base': '0x80003f00',
            'script': os.path.join('$builddir', f'RMC{region}.ld'),
        },
        implicit = os.path.join('$builddir', f'RMC{region}.ld'),
    )
    n.newline()

for target in ['client', 'server']:
    for region in ['P', 'E', 'J', 'K']:
        n.build(
            os.path.join('$builddir', f'{target}{region}.bin'),
            'link',
            ofiles[target],
            variables = {
                'entry': 'main__Q26System7PayloadFv',
                'base': {
                    'P': '0x8076db60',
                    'E': '0x80769400',
                    'J': '0x8076cca0',
                    'K': '0x8075bfe0',
                }[region],
                'script': os.path.join('$builddir', f'RMC{region}.ld'),
            },
            implicit = os.path.join('$builddir', f'RMC{region}.ld'),
        )
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
