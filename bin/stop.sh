#!/bin/bash

pid=`xdotool getwindowfocus getwindowpid`
kill -STOP $pid

exit 0
