#
# LJBG © -- LuaJIT FFI bindings generator.
# Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
#

test_runner = find_program('test_runner.sh', required : true)

test('struct', test_runner, workdir : meson.current_build_dir(),
     args : [ljbg.full_path(), files('struct.h'), 'struct_result.lua', files('struct_expected.lua')])
test('union', test_runner, workdir : meson.current_build_dir(),
     args : [ljbg.full_path(), files('union.h'), 'union_result.lua', files('union_expected.lua')])
test('enum', test_runner, workdir : meson.current_build_dir(),
     args : [ljbg.full_path(), files('enum.h'), 'enum_result.lua', files('enum_expected.lua')])
test('function', test_runner, workdir : meson.current_build_dir(),
     args : [ljbg.full_path(), files('function.h'), 'function_result.lua', files('function_expected.lua')])
