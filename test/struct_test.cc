//
// LJBG © -- LuaJIT FFI bindings generator.
//
// Copyright (C) 2020 Stefan Pejic
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the  "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

extern "C"
{
#include "bindings_generator.h"
}
#include <gtest/gtest.h>

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
    EXPECT_STREQ(expected_output, actual_output);

    free(actual_output);
}

#define PREFIX "local ffi = require(\"ffi\")\n\nffi.cdef[[\n"
#define SUFFIX "]]\n"

TEST(Structs, RegularStruct)
{
    const char input_content[] = "struct s {\n    int x;\n};\n";
    const char input_filename[] = "test_struct.h";
    const char output_filename[] = "test_struct.lua";
    const char expected_output[] = PREFIX "struct s {\n    int x;\n};\n" SUFFIX;
    RunTest(input_filename, output_filename, NULL, 0, input_content, expected_output);
}
