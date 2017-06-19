#!/bin/bash`

> ~/.config/power-up/black_list_pid.conf
while read -r line
do
	name="$line"
	pid=`wmctrl -l -p | grep -i -m1 $name | cut -f4 -d' '`
	#pid=`ps -aux | grep -i -m 1 $name | cut -f5 -d' '`
	if [[ -z "${pid// }" ]]; then
        	pgrep -i $name >> ~/.config/power-up/black_list_pid.conf
	else
		echo $pid >> ~/.config/power-up/black_list_pid.conf
	fi

done < ~/.config/power-up/black_list.conf

> ~/.config/power-up/refresh_list_pid.conf
while read -r line
do
	name="$line"
	pid=`wmctrl -l -p | grep -i -m1 $name | cut -f4 -d' '`
	#pid=`ps -aux | grep -i -m 1 $name | cut -f5 -d' '`
	if [[ -z "${pid// }" ]]; then
        	pgrep -i $name >> ~/.config/power-up/refresh_list_pid.conf
	else
		echo $pid >> ~/.config/power-up/refresh_list_pid.conf	
	fi	

done < ~/.config/power-up/refresh_list.conf

exit 0
