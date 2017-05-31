#!/bin/bash

xprop -root | grep _NET_ACTIVE_WINDOW\(WINDOW\) > /home/bhagui/sortie.txt

exit 0
