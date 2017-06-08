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

* xdotool

.. code:: sh

``sudo apt-get install xdotool``

* wmctrl

.. code:: sh

``sudo apt-get install wmctrl``

* xprop and X11

.. code:: sh

``sudo apt-get install xorg openbox``

-----------
How to use:
-----------

1. Add names of applications that you dont want to suspend or to refresh regurlay to their respective configuration files.
2. Compile (using ``make clean all``) and launch the executable generated of the latest version of the application. 
3. Launch the bash file xprop.sh in a separate terminal.

-------------------
Keyboard Shortcuts
-------------------

The folder /power_up_tool/bin has several bash files. Assign a keyboard shortcut to each of these files and the shortcuts will have the following functionalities:

1. **stop.sh:** Suspends the currently active window application by sending it a SIGSTOP signal.
2. **stop_all.sh:** Suspends all window applications except the currently active one.
3. **resume.sh:** Resumes the currently active window application by sending it a SIGCONT signal.
4. **dont_stop_this.sh:** Adds the PID of the currently active window application to the list of processes to never suspend. (Requires power-up too to be already running to work).
