#!/bin/bash

xdotool search --onlyvisible . behave %@ mouse-enter getwindowpid > visible_windows &


exit 0
