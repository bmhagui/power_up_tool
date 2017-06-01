#!/bin/bash

wmctrl -l -p | cut -f4 -d' ' > active_windows.txt

focused_window=`xdotool getwindowfocus getwindowpid`
#echo active $focused_window
while read -r line
do
	pid="$line"
	#echo $pid
	if [ $pid = $focused_window ]
	then
		kill -CONT $pid
	else
		kill -STOP $pid
	fi
done < active_windows.txt

exit 0
