#!/bin/bash

xprop -root -spy | awk -W interactive '/NET_ACTIVE_WINDOW\(/ {print $5}' > /home/bhagui/stage_52/power-up/notif/test.txt

exit 0
