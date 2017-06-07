=========
Power-Up
=========

Power-up is a tool that will help you extend your battery life and make better use of your processing power. This is achieved by suspending your inactive window applications (SIGSTOP signal is sent to all processes in the background, and a SIGCONT to the one in the foreground).

If there are processes that you do not wish to suspend, please enter their names in the file ~/power_up_tool/power-up/v4/config/conf.txt
The name has to be something specific to the application entended, for example firefox or spotify.

-------------
Requirements:
-------------

* xdotool

.. code:: sh

sudo apt-get install xdotool

* wmctrl

.. code:: sh

sudo apt-get install wmctrl

* xprop and X11

.. code:: sh

sudo apt-get install xorg openbox

-----------
How to use:
-----------

1. Launch the bash file xprop.sh in a terminal.
2. Compile (using ``make clean all``) and launch the executable generated of the latest version of the application. 

