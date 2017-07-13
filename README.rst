=========
Power_Up
=========

Power_up is a tool that will help extend your battery life and make better use of your processing power. This is achieved by pausing your inactive window applications (SIGSTOP signal is sent to all processes in the background, and a SIGCONT to the one in the foreground).

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

1. Download and extract power_up.

2. Navigate to **power_up** application folder. Compile (using ``make all``) and launch the executable generated of the latest version of the application.
   
3. Configuration files are now set up in **~/.config/power_up/**

   **black_list.conf** contains the names of applications you never want to pause.
  
   **refresh_list.conf** the aplications you want to pause but activate every REFRESH_RATE_S (in seconds) for atleast REFRESH_RATE_S (in seconds). 

   The names added to either lists have to be something specific to the application entended, for example firefox or spotify.
   Use power_up with the flag -l to display the list of names of currently running applications.

-------
Flags:
-------

**--refresh-list** or **-r**
* If one or more arguments are added with this flag then those arguments will be added to **refresh_list.conf**. If there is no argument then the next window you click on will be added to the refresh list.
  
**--black-list** or **-b**
* If one or more arguments are added with this flag then those arguments will be added to **black_list.conf**. If there is no argument then the next window you click on will be added to the black list.
  
**--kill-power-up** or **--k**
* Terminate the power_up tool currently running.
  
**--list-apps** or **-l**
* Displays a list of open window applications and their PIDs.
  
**--toggle-active-window** or **-t**
* Add the currently open application to the blacklist or remove it if it is already on there.
  
**--wait-for <seconds>** or **--w <seconds>**
* Waits the entered number of seconds before launching the app. If the app is alraedy running, this will stop the app and re-run it in the second terminal after <seconds>.
  
**--configure-pause-and-refresh-rates** or **-c**
* Choose after how many seconds to pause the background apps, the frequency of refreshing your refresh-list apps and for how long to keep them active.
  
**--verbose** or **-v**
* Launch the app in verbose mode, print out the name of each application and whether it was paused or continued.
  
**--help** or **-h**
* Prints out details about the available flags and how to use them.

------------------
Auto-completition:
------------------

For auto-completiton you will need to do the following:

1. Use the Makefile with the option install ``make install``.
   This will copy a file called **power_up** to the directory **/etc/bash_completion.d/**.

2. Source the file mentionned previously with the following command:
   ``. /etc/bash_completion.d/power_up``

3. You can now type use [TAB][TAB] to autocomplete the flags you would like to use with power_up
   
---------------------------------------------------
use the flag --help or --h for further instructions.
---------------------------------------------------
