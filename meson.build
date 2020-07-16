#
# LJBG © -- LuaJIT FFI bindings generator.
#
# Copyright (C) 2020 Stefan Pejic
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the  "Software"), to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
# OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

project('ljbg', 'c')

cc = meson.get_compiler('c')

llvm_dep = dependency('llvm', version : '>=7.0', required : true)
llvm_libdir = llvm_dep.get_configtool_variable('libdir')
llvm_incdir = llvm_dep.get_configtool_variable('includedir')

clang_dep = cc.find_library('clang',
                            dirs : llvm_libdir,
                            has_headers: 'clang-c/Index.h',
                            header_include_directories : include_directories(llvm_incdir),
                            required : true)

m_dep = cc.find_library('m', required : true)

executable('ljbg',
           ['ljbg.c', 'argtable3.c'],
           dependencies : [clang_dep, m_dep],
           include_directories : [llvm_incdir],
           c_args : '-fPIC')
