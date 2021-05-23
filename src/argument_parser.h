//
// LJBG © -- LuaJIT FFI bindings generator.
// Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
//

#include <stdbool.h>

typedef struct
{
    const char* input_file;
    const char* output_file;
    const char* const* compilation_arguments;
    int compilation_argument_count;
    bool print_usage_and_quit;
} Arguments;

Arguments ParseArguments(int argc, char* argv[]);
