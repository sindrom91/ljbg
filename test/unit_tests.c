//
// LJBG -- LuaJIT FFI bindings generator.
//
// Copyright (C) 2020 Stefan Pejic
//

#include "bindings_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void WriteContentToFile(const char* const filename, const char* const content)
{
    FILE* const f = fopen(filename, "wb");
    fputs(content, f);
    fclose(f);
}

char* ReadContentFromFile(const char* const filename)
{
    FILE* const f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    const size_t fsize = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    char* const content = (char* const)malloc(fsize + 1);
    const size_t unused = fread(content, 1, fsize, f);
    (void)unused;
    fclose(f);
    content[fsize] = '\0';
    return content;
}

bool ExpectStrEq(const char* const expected, const char* const actual, const char* const test_name)
{
    if (strcmp(expected, actual) == 0)
    {
        printf("\033[1;32m");
        printf("# %s: TEST PASSED!\n", test_name);
        printf("\033[0m");
        return true;
    }
    else
    {
        printf("\033[1;31m");
        printf("# %s: TEST FAILED!\n", test_name);
        printf("\033[0m");
        printf("EXPECTED:\n%s\n", expected);
        printf("ACTUAL:\n%s\n", actual);
        return false;
    }
}

bool RunTest(const char* const input_filename,
             const char* const output_filename,
             const char* const* const compilation_arguments,
             const int compilation_argument_count,
             const char* const input_content,
             const char* const expected_output,
             const char* const test_name)
{
    WriteContentToFile(input_filename, input_content);

    Arguments args;
    args.input_file = input_filename;
    args.output_file = output_filename;
    args.compilation_arguments = compilation_arguments;
    args.compilation_argument_count = compilation_argument_count;
    args.print_usage_and_quit = false;
    GenerateBindings(args);

    char* const actual_output = ReadContentFromFile(output_filename);
    const bool hasPassed = ExpectStrEq(expected_output, actual_output, test_name);

    free(actual_output);

    return hasPassed;
}

#define PREFIX "local ffi = require(\"ffi\")\n\nffi.cdef[[\n"
#define SUFFIX "\n]]\n"

bool RunStructTest()
{
    const char input_content[] = "struct s {\n    int x;\n};\n";
    const char input_filename[] = "test_struct.h";
    const char output_filename[] = "test_struct.lua";
    const char expected_output[] = PREFIX "struct s {\n    int x;\n};" SUFFIX;
    const char test_name[] = "test_struct";
    return RunTest(input_filename, output_filename, NULL, 0, input_content, expected_output, test_name);
}

bool RunUnionTest()
{
    const char input_content[] = "union u {\n    int x;\n    float y;\n};\n";
    const char input_filename[] = "test_union.h";
    const char output_filename[] = "test_union.lua";
    const char expected_output[] = PREFIX "union u {\n    int x;\n    float y;\n};" SUFFIX;
    const char test_name[] = "test_union";
    return RunTest(input_filename, output_filename, NULL, 0, input_content, expected_output, test_name);
}

bool RunEnumTest()
{
    const char input_content[] = "enum e {\n    FIRST,\n    SECOND\n};\n";
    const char input_filename[] = "test_enum.h";
    const char output_filename[] = "test_enum.lua";
    const char expected_output[] = PREFIX "enum e {\n    FIRST,\n    SECOND\n};" SUFFIX;
    const char test_name[] = "test_enum";
    return RunTest(input_filename, output_filename, NULL, 0, input_content, expected_output, test_name);
}

bool RunFunctionTest()
{
    const char input_content[] = "int Foo(int x, int y);";
    const char input_filename[] = "test_function.h";
    const char output_filename[] = "test_function.lua";
    const char expected_output[] = PREFIX "int Foo(int x, int y);" SUFFIX;
    const char test_name[] = "test_function";
    return RunTest(input_filename, output_filename, NULL, 0, input_content, expected_output, test_name);
}

int main(int argc, char** argv)
{
    bool isSuccessful = true;

    if (argc == 1)
    {
        isSuccessful &= RunStructTest();
        isSuccessful &= RunUnionTest();
        isSuccessful &= RunEnumTest();
        isSuccessful &= RunFunctionTest();
    }
    else if (strcmp("--struct", argv[1]) == 0)
    {
        isSuccessful = RunStructTest();
    }
    else if (strcmp("--union", argv[1]) == 0)
    {
        isSuccessful = RunUnionTest();
    }
    else if (strcmp("--enum", argv[1]) == 0)
    {
        isSuccessful = RunEnumTest();
    }
    else if (strcmp("--function", argv[1]) == 0)
    {
        isSuccessful = RunFunctionTest();
    }
    else
    {
        printf("INVALID ARGUMENT %s\n", argv[1]);
        isSuccessful = false;
    }

    if (isSuccessful)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
