#!/bin/sh -v
DIRNAME=`dirname "$0"`
cd "$DIRNAME"
CMD=`ls MoMCustomizer-*`
export LD_LIBRARY_PATH="$PWD"
./$CMD
