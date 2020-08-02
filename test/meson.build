gtest_dep = dependency('gtest', main : true, required : false)

if gtest_dep.found()
  struct_test = executable('struct_test',
                           'struct_test.cc',
                           dependencies : [gtest_dep, bindgen_dep])

  test('structure tests', struct_test)
endif
