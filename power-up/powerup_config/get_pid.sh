#!/bin/bash`

#> ~/power_up_tool/power-up/v4/config/conf_pid.txt
while read -r line
do
	name="$line"
	pid=`wmctrl -l -p | grep -i -m1 $name | cut -f4 -d' '`
	#pid=`ps -aux | grep -i -m 1 $name | cut -f5 -d' '`
	if [[ -z "${pid// }" ]]; then
        	pgrep -i $name >> ~/.config/powerup_config/conf_pid.txt
	else
		echo $pid >> ~/.config/powerup_config/conf_pid.txt	
	fi

done < ~/.config/powerup_config/conf.txt

> ~/.config/powerup_config/refresh_pid.txt
while read -r line
do
	name="$line"
	pid=`wmctrl -l -p | grep -i -m1 $name | cut -f4 -d' '`
	#pid=`ps -aux | grep -i -m 1 $name | cut -f5 -d' '`
	if [[ -z "${pid// }" ]]; then
        	pgrep -i $name >> ~/.config/powerup_config/refresh_pid.txt
	else
		echo $pid >> ~/.config/powerup_config/refresh_pid.txt	
	fi	

done < ~/.config/powerup_config/name_refresh.txt

exit 0
