//
// LJBG © -- LuaJIT FFI bindings generator.
// Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
//

#include "argument_parser.h"
#include <string.h>

const char output_option_long[] = "--output";
const char output_option_short[] = "-o";

const char args_option_long[] = "--args";
const char args_option_short[] = "-a";

const char help_option_long[] = "--help";
const char help_option_short[] = "-h";

Arguments ParseArguments(int argc, char* argv[])
{
    Arguments args = {NULL, NULL, NULL, 0, false};

    if (argc < 2)
    {
        args.print_usage_and_quit = true;
        return args;
    }

    int idx = 1;

    if ((argc == 2) && ((strcmp(help_option_short, argv[idx]) == 0) || (strcmp(help_option_long, argv[idx]) == 0)))
    {
        args.print_usage_and_quit = true;
        return args;
    }

    // First argument has to be the input file.
    args.input_file = argv[idx];
    idx++;

    // We need at least 2 more arguments to proceed, thus +1.
    if (idx + 1 >= argc)
    {
        return args;
    }

    if ((strcmp(output_option_short, argv[idx]) == 0) || (strcmp(output_option_long, argv[idx]) == 0))
    {
        args.output_file = argv[idx + 1];
        idx += 2;
    }

    // We need at least 2 more arguments to proceed, thus +1.
    if (idx + 1 >= argc)
    {
        return args;
    }

    if ((strcmp(args_option_short, argv[idx]) == 0) || (strcmp(args_option_long, argv[idx]) == 0))
    {
        args.compilation_arguments = (const char* const*)&argv[idx + 1];
        args.compilation_argument_count = argc - idx - 1;
    }
    return args;
}
