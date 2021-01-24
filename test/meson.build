unit_tests = executable('unit_tests',
                        'unit_tests.c',
                        dependencies : [bindgen_dep])

test('struct test', unit_tests, args : ['--struct'])
test('union test', unit_tests, args : ['--union'])
test('enum test', unit_tests, args : ['--enum'])
test('function test', unit_tests, args : ['--function'])
