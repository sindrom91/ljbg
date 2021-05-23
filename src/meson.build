#
# LJBG © -- LuaJIT FFI bindings generator.
# Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
#

cc = meson.get_compiler('c')

libllvm = dependency('llvm', version : '>=7.0', required : true)
libclang_searchpath = libllvm.get_configtool_variable('libdir')
libclang_includedir = libllvm.get_configtool_variable('includedir')
libclang = cc.find_library('clang',
                            dirs : libclang_searchpath,
                            has_headers: 'clang-c/Index.h',
                            header_include_directories : include_directories(libclang_includedir),
                            required : true)

libm = cc.find_library('m', required : true)

ljbg = executable('ljbg',
                  ['ljbg.c', 'bindings_generator.c' ,'argument_parser.c'],
                  dependencies : [libclang, libm],
                  include_directories : [libclang_includedir])
