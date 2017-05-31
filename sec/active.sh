#!/bin/bash

#test that there is a pipe and if not creates it
pipe=/tmp/testpipe

trap "rm -f $pipe" EXIT

if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi

ls -la $pipe

