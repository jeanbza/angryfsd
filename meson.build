project('angryfsd', 'cpp')

# this appears to do nothing :/
add_project_arguments('-Wno-reorder', '-Wsign-compare', language: 'cpp')

executable('angryfsd', 'angryfsd.cpp',
    dependencies: [
        dependency('fuse3'),
    ],
)