#!/bin/bash

mkdir -v ~/.config/config_powerup
mkdir -v ~/.config/config_powerup/notif
> ~/.config/config_powerup/notif/window_change.conf
> ~/.config/config_powerup/black_list.conf
> ~/.config/config_powerup/black_list_pid.conf
> ~/.config/config_powerup/open_windows.conf
> ~/.config/config_powerup/refresh_list.conf
> ~/.config/config_powerup/refresh_list_pid.conf

#cp ../config_powerup/ -ruv ~/.config/

exit 0
