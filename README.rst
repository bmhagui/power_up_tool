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

1. Extract the application to the home directory.

2. Open a terminal, navigate to the bin folder of the application. Run the bash file **install.sh**. **(This step is only to do the first time you use the application.)**
   
3. Configuration files are now set up in **~/.config/config_powerup/** black_list.conf contains the names of applications you want to never suspend and refresh_list.conf the aplications you want to suspend but activate every REFRESH_RATE_S (in seconds) for a cycle of the application.
   
4. Finding the name of applications that you wish to add to one of those lists can be done through the following commands:
   MORE DETAILS COMING HERE, FLAGS WILL BE USED INSTEAD

   ``wmctrl -l -p``

   Then you can go through a list of window applications and their window titles. You can choose something unique to the application (firefox for example) and add it to the configuration file if you wish.
   If there is no unique part of the window title to the application, use:

   ``ps -aux | grep "insert a name here"``

   You need to use several names related to the application and find something that will help identify and add it to one of the configuration files if you wish.

5. Navigate to **power-up** application folder. Compile (using ``make clean all``) and launch the executable generated of the latest version of the application.
