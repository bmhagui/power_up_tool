=========
Power_Up
=========

Power-up is a tool that will help you extend your battery life and make better use of your processing power. This is achieved by suspending your inactive window applications (SIGSTOP signal is sent to all processes in the background, and a SIGCONT to the one in the foreground).

If there are processes that you do not wish to suspend, please enter their names in the file ~/power_up_tool/power-up/latest_version_power-up/config/conf.txt
The name has to be something specific to the application entended, for example firefox or spotify.

For applications that you want to refresh regularly (e-mail application for example) you can put their names in the file name_refresh.txt
These applications will be refreshed atleast every 5 seconds by default. If you want to change this duration, edit the file libpower.h line13 (variable REFRESH_RATE_S) and specify the number of seconds wanted. Re-Compile and run afterwards.

-------------
Requirements:
-------------

* xprop and X11 (installed by default)
  
* xdotool

.. code:: sh

``sudo apt-get install xdotool``

* wmctrl

.. code:: sh

``sudo apt-get install wmctrl``

* mawk

.. code:: sh

``sudo apt-get install mawk``

-----------
How to use:
-----------

1. Extract the application.

2. Navigate to **power-up** application folder. Compile (using ``make clean all``) and launch the executable generated of the latest version of the application.
   
3. Configuration files are now set up in **~/.config/config_powerup/**
   
   black_list.conf contains the names of applications you want to never suspend and
   
   refresh_list.conf the aplications you want to suspend but activate every REFRESH_RATE_S (in seconds) for a cycle of the application.
   
4. Finding the name or PID of applications that you wish to add to one of those lists can be done through the following commands:

   Start the application with the flag **--add-black or -b**, click on a window application to add it to the black list. Press 1 then enter to continue selecting window applications or 0 to launch.

   Start the application with the flag **--add-refresh or -r**, click on a window application to add it to the refresh list. Press 1 then enter to continue selecting window applications or 0 to launch.

   Start the application with the flag **--list or -l**, to display a list of opened window applications and their PIDs.  

   Start the application with the flag **--toggle or -t**, to add current window application to black_list(If the application is already in the black_list it will be removed).

------------------
Auto-completition:
------------------

For auto-completiton you will need to do the following:

1. Use the Makefile with the option install ``make install``.
   This will copy a file called **power_up** to the directory **/etc/bash_completion.d/**.

2. Source the file mentionned previously with the following command:
   ``. /etc/bash_completion.d/power_up``

3. You can now type use [TAB][TAB] to autocomplete the flags you would like to use with power-up
   
---------------------------------------------------
use the flag --help or --h for further instructions.
---------------------------------------------------
