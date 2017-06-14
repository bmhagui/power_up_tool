=========
Power-Up
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

-----------
How to use:
-----------

1. Extract the application.

2. Open a terminal, navigate to the bin folder of the application. Run the bash file **bash install.sh**. **(This step is only to do the first time you use the application, if launched again it will empty your configuration files.)**
   
3. Configuration files are now set up in **~/.config/config_powerup/**
   
   black_list.conf contains the names of applications you want to never suspend and
   
   refresh_list.conf the aplications you want to suspend but activate every REFRESH_RATE_S (in seconds) for a cycle of the application.
   
4. Finding the name or PID of applications that you wish to add to one of those lists can be done through the following commands:

   Start the application with the flag **--add-black or -b**, click on a window application to add it to the black list. Press 1 then enter to continue selecting window applications or 0 to launch.

   Start the application with the flag **--add-refresh or -r**, click on a window application to add it to the refresh list. Press 1 then enter to continue selecting window applications or 0 to launch.

   Start the application with the flag **--list or -l**, to display a list of opened window applications and their PIDs.  

5. Navigate to **power-up** application folder. Compile (using ``make clean all``) and launch the executable generated of the latest version of the application.


-----------
-----------
Example by the creator of XdoTool:
https://stackoverflow.com/questions/1108926/produce-keyboard-events-key-hits/2978666#2978666
---------------------------------------------------
use the flag --help or -h for further instructions.
---------------------------------------------------
