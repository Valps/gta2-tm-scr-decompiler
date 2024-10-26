project('scr_decompiler', 'cpp', default_options: ['cpp_std=c++17'])
pkg = import('pkgconfig')

lib = library('scrd', 'scr_decompiler.cpp', install: true, gnu_symbol_visibility: 'hidden')
install_headers('scr_decompiler_api.h')
libscrd_dep = declare_dependency(include_directories: '.', link_with: lib)
pkg.generate(lib, name: meson.project_name(), description: 'Decompiler for GTA2 SCR scripts.')

exe = executable('scr_decompiler', 'decompiler_tool.cpp', dependencies: libscrd_dep)
