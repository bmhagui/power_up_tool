#!/bin/bash

xprop -root -spy | awk -W interactive '/NET_ACTIVE_WINDOW\(/ {print $5}' > ~/stage_52/power-up/v4/config/notif/window_change.txt

exit 0
