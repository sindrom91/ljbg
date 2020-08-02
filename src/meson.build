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

bindgen_lib = library('bindings_generator',
                      ['bindings_generator.c' ,'argument_parser.c'],
                      dependencies : [clang_dep, m_dep],
                      include_directories : [llvm_incdir])

bindgen_dep = declare_dependency(include_directories : '.',
                         link_with : bindgen_lib)

executable('ljbg',
           'ljbg.c',
           dependencies : bindgen_dep)
