Power_up is a tool that will suspend your inactive processes by sending a SIGSTOP to all processes in the background and a SIGCONT to the one in the foreground.

If there are processes that you do not wish to suspend, please enter their PIDs in the file /v4/config/config_pid.txt

you will need the following packages for this to work:

-xdotool
sudo apt-get install xdotool

-wmctrl
sudo apt-get install wmctrl

-xprop and X11
sudo apt-get install xorg openbox


HOW TO USE:
First of all launch the bash file xprop.sh in a termianl and then compile and launch the executable of version_4.c

