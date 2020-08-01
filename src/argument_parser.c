#include "argument_parser.h"
#include <string.h>

const char output_option_long[] = "--output";
const char output_option_short[] = "-o";

const char args_option_long[] = "--args";
const char args_option_short[] = "-a";

const char help_option_long[] = "--help";
const char help_option_short[] = "-h";

Arguments ParseArguments(int argc, char *argv[])
{
    Arguments args = {NULL, NULL, NULL, 0, true};

    if ((argc == 1) || (strcmp(help_option_short, argv[1]) == 0) || (strcmp(help_option_long, argv[2]) == 0))
    {
        args.print_usage_and_quit = true;
        return args;
    }

    int active_index = 1;

    args.input_file = argv[active_index];
    ++active_index;

    if (active_index + 1 >= argc)
    {
        return args;
    }

    if ((strcmp(output_option_short, argv[active_index]) == 0) || (strcmp(output_option_long, argv[active_index]) == 0))
    {
        args.output_file = argv[active_index + 1];
        active_index += 2;
    }

    if (active_index + 1 >= argc)
    {
        return args;
    }

    if ((strcmp(args_option_short, argv[active_index]) == 0) || (strcmp(args_option_long, argv[active_index]) == 0))
    {
        args.compilation_arguments = (const char *const *)&argv[active_index + 1];
        args.compilation_argument_count = argc - active_index - 1;
    }
    return args;
}
