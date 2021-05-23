#!/bin/bash

#
# LJBG © -- LuaJIT FFI bindings generator.
# Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
#

set -e

$1 $2 --output $3
cmp $3 $4
