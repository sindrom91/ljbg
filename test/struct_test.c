//
// LJBG -- LuaJIT FFI bindings generator.
//
// Copyright (C) 2020 Stefan Pejic
//

#include "bindings_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void WriteContentToFile(const char* filename, const char* content)
{
    FILE* f = fopen(filename, "wb");
    fputs(content, f);
    fclose(f);
}

char* ReadContentFromFile(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    const size_t fsize = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    char* content = (char*)malloc(fsize);
    const size_t unused = fread(content, 1, fsize, f);
    (void)unused;
    fclose(f);
    content[fsize] = '\0';
    return content;
}

void ExpectStrEq(const char* const expected, const char* const actual)
{
    if (strcmp(expected, actual) == 0)
    {
        printf("\033[1;32m");
        printf("# TEST PASSED!\n");
        printf("\033[0m");
    }
    else
    {
        printf("\033[1;31m");
        printf("# TEST FAILED!\n");
        printf("\033[0m");
    }
}

void RunTest(const char* input_filename,
             const char* output_filename,
             const char** compilation_arguments,
             const int compilation_argument_count,
             const char* input_content,
             const char* expected_output)
{
    WriteContentToFile(input_filename, input_content);

    Arguments args;
    args.input_file = input_filename;
    args.output_file = output_filename;
    args.compilation_arguments = compilation_arguments;
    args.compilation_argument_count = compilation_argument_count;
    args.print_usage_and_quit = false;
    GenerateBindings(args);

    char* actual_output = ReadContentFromFile(output_filename);
    ExpectStrEq(expected_output, actual_output);

    free(actual_output);
}

#define PREFIX "local ffi = require(\"ffi\")\n\nffi.cdef[[\n"
#define SUFFIX "]]\n"

void RunStructTest()
{
    const char input_content[] = "struct s {\n    int x;\n};\n";
    const char input_filename[] = "test_struct.h";
    const char output_filename[] = "test_struct.lua";
    const char expected_output[] = PREFIX "struct s {\n    int x;\n};\n" SUFFIX;
    RunTest(input_filename, output_filename, NULL, 0, input_content, expected_output);
}

int main() { RunStructTest(); }
