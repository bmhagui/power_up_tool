#!/bin/bash`

while read -r line
do
	name="$line"
	pid=`wmctrl -l -p | grep -i -m1 $name | cut -f4 -d' '`
	#pid=`ps -aux | grep -i -m 1 $name | cut -f5 -d' '`
	if [[ -z "${pid// }" ]]; then
        	pgrep -i $name >> ~/.config/config_powerup/black_list_pid.conf
	else
		echo $pid >> ~/.config/config_powerup/black_list_pid.conf
	fi

done < ~/.config/config_powerup/black_list.conf

> ~/.config/config_powerup/refresh_list_pid.conf
while read -r line
do
	name="$line"
	pid=`wmctrl -l -p | grep -i -m1 $name | cut -f4 -d' '`
	#pid=`ps -aux | grep -i -m 1 $name | cut -f5 -d' '`
	if [[ -z "${pid// }" ]]; then
        	pgrep -i $name >> ~/.config/config_powerup/refresh_list_pid.conf
	else
		echo $pid >> ~/.config/config_powerup/refresh_list_pid.conf	
	fi	

done < ~/.config/config_powerup/refresh_list.conf

exit 0
