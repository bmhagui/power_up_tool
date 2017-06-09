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

1. Run the bash file **install.sh** in a terminal. **This step is only to do the first time you use the application.**
   
2. Configuration files are now set up in **~/.config/powerup_config/** You can modify the file conf.txt to add names of processes that you do not want to ever suspend. And names_refresh.txt has a list of processes that you wish to refresh regularly.
   
3. Finding the name of applications that you wish to add to one of those lists can be done through the following commands

   ``wmctrl -l -p``

   Then you can go through a list of window applications and their window titles. You can choose something unique to the application (firefox for example) and add it to the configuration file if you wish.
   If there is no unique part of the window title to the application, use:

   ``ps -aux | grep "insert a name here"``

   You need to use several names related to the application and find something that will help identify and add it to one of the configuration files if you wish.

4. Go back to power-up application folder. More specifically the folder **latest_version_power-up.** Compile (using ``make clean all``) and launch the executable generated of the latest version of the application.
   
5. Launch the bash file xprop.sh in a separate terminal.

-------------------
Keyboard Shortcuts
-------------------

The folder /power_up_tool/bin has several bash files. Assign a keyboard shortcut to each of these files and the shortcuts will have the following functionalities:

1. **stop.sh:** Suspends the currently active window application by sending it a SIGSTOP signal.
2. **stop_all.sh:** Suspends all window applications except the currently active one.
3. **resume.sh:** Resumes the currently active window application by sending it a SIGCONT signal.
4. **dont_stop_this.sh:** Adds the PID of the currently active window application to the list of processes to never suspend. (Requires power-up too to be already running to work).
