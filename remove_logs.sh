#!/bin/bash
# 
# remove_logs.sh
# 
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
#
# Description: This script remove *.log files created in the current direcotry.
#
# Date: 2023-09-22
#

if test -f colored.log || test -f debug.log || 
  test -f hello.log || test -f module.log || 
  test -f trace.log || test -f trace_level.log; then
  rm -r *.log
fi

