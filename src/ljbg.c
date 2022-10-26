//
// LJBG © -- LuaJIT FFI bindings generator.
// Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
//

#include <stdio.h>

#include "bindings_generator.h"

int main(int argc, char* argv[])
{
    const Arguments args = ParseArguments(argc, argv);

    if (args.print_usage_and_quit)
    {
        printf("Usage:\n");
        printf("ljbg <input> [-o/--output <output>] [--args <arg> <arg> ...]\n");
        printf("\nArguments are expected in this exact order!\n");
        return StatusSuccess;
    }

    return GenerateBindings(args);
}
