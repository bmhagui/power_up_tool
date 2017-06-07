=========
Power-Up
=========

Power-up is a tool that will help you extend your battery life and make better use of your processing power. This is achieved by suspending your inactive window applications (SIGSTOP signal is sent to all processes in the background, and a SIGCONT to the one in the foreground).

If there are processes that you do not wish to suspend, please enter their names in the file ~/power_up_tool/power-up/v4/config/conf.txt
The name has to be something specific to the application entended, for example firefox or spotify.

For applications that you want to refresh regularly (e-mail application for example) you can put their names in the file name_refresh.txt
These applications will be refreshed atleast every 5 seconds by default. If you want to change this duration, edit the file version_4.c line13 and specify the number of seconds wanted. Compile and run afterwards.

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
2. Launch the bash file xprop.sh in a terminal.
3. Compile (using ``make clean all``) and launch the executable generated of the latest version of the application. 

-------------------
Keyboard Shortcuts
-------------------

The folder /power_up_tool/bin has several bash files. Assign a keyboard shortcut to each of these files and the shortcuts will have the following functionalities:

1. **dont_stop_this.sh:** Ajoute le PID de la fênetre active à la liste des PIDS des processus à ne pas mettre en veille.
2. **resume.sh:** Envoi un signal SIGCONT au processus de la fênetre active. (Le redémarrer.)
3. **stop.sh:** Envoi un signal SIGSTOP au processus de la fênetre active. (Le mettre en veille.)
4. **stop_all.sh:** Envoi un signal SIGSTOP a tout les processus sauf celle lié à la fênetre active. (Mettre tout en veille sauf processus active.)
