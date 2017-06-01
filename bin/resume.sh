#!/bin/bash

pid=`xdotool getwindowfocus getwindowpid`
kill -CONT $pid

exit 0
