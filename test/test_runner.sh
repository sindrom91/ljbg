#!/bin/bash

set -e

$1 $2 --output $3
cmp $3 $4
