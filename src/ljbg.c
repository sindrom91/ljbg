//
// LJBG © -- LuaJIT FFI bindings generator.
// Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
//

#include "bindings_generator.h"

int main(int argc, char* argv[])
{
    const Arguments args = ParseArguments(argc, argv);

    if (args.print_usage_and_quit)
    {
        return 1;
    }

    GenerateBindings(args);

    return 0;
}
