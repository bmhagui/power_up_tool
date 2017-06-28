=========
Power_Up
=========

Power_up is a tool that will help you extend your battery life and make better use of your processing power. This is achieved by pausing your inactive window applications (SIGSTOP signal is sent to all processes in the background, and a SIGCONT to the one in the foreground).

If there are processes that you do not wish to pause, please enter their names in the file **black_list.conf** located at **~/.config/power_up/** after the first start-up of the application.
The name has to be something specific to the application entended, for example firefox or spotify.

For applications that you want to refresh regularly (e-mail application for example) you can put their names in the file **refresh_list.conf** located in the same directory as above. 
These applications will be refreshed atleast every 5 seconds *(default value)*. If you want to change this duration

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

2. Navigate to **power_up** application folder. Compile (using ``make all``) and launch the executable generated of the latest version of the application.
   
3. Configuration files are now set up in **~/.config/power_up/**
   
   black_list.conf contains the names of applications you never want to pause.
   
   refresh_list.conf the aplications you want to suspend but activate every REFRESH_RATE_S (in seconds) for a cycle of power_up.

4. To change the application refresh frequency or the time after which the applications are paused:

   Launch the application with the flag **--configure-pause-and-refresh-rates or -c**.

   ``./power_up --configure-pause-and-refresh-rates``


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
