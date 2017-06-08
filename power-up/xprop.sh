#!/bin/bash

xprop -root -spy | awk -W interactive '/NET_ACTIVE_WINDOW\(/ {print $5}' > ~/power_up_tool/power-up/latest_version_power-up/config/notif/window_change.txt
#xprop -root -spy _NET_ACTIVE_WINDOW | cut -f5 -d' ' > ~/power_up_tool/power-up/v4/config/notif/window_change.txt

exit 0
