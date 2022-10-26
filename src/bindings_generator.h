//
// LJBG © -- LuaJIT FFI bindings generator.
// Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
//

#include "argument_parser.h"

typedef enum
{
    StatusSuccess = 0,
    StatusBadInputFile = 1,
    StatusBadOutputFile = 2,
    StatusCompilationError = 3,
} StatusCodes;

StatusCodes GenerateBindings(const Arguments args);
