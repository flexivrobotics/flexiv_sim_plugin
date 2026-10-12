# Flexiv Elements Studio Setup

Flexiv Elements Studio runs the simulated robot controller. 
Set it up once before running any external-simulator workspace.

## Install Elements Studio on Ubuntu

Elements Studio runs on Ubuntu 22.04. On a newer Ubuntu, run it in an Ubuntu 22.04 container instead of installing it: [isaac_sim_ws](https://github.com/flexivrobotics/isaac_sim_ws/tree/v2.1.x#run-elements-studio-in-a-container) provides `launch_elements_studio.sh` for that. Then skip step 3 below, and run step 4 from the container's shell.

1. [Contact Flexiv](https://www.flexiv.com/contact) to obtain the installation package of Elements Studio.
2. Extract the package to a non-root directory.
3. Install Elements Studio:

       bash setup_FlexivElements.sh

4. Switch physics engine from the default built-in to external:

       bash switch_physics_engine.sh

   Select *External* when prompted.

## Create a simulated robot in Elements Studio

1. Start Flexiv Elements Studio from the application menu.
2. In the Robot Connection window, select *Simulator*, and click *CREATE*.
3. Choose "Create according to the selected robot type" and select one from the list, then click *CONFIRM*. A new simulated robot will be added to the simulator list.
4. Toggle on the *Connect* button for the newly added one, then wait for loading.
5. When loading is finished, you'll see a robot at its upright pose, with an "Exception" error at the bottom right corner. This is expected because the external simulator is not started yet. But if you see a normally operating robot, that again means you are running the wrong version of Elements Studio that only supports the built-in physics engine.
6. To enable RDK, Click Settings → Remote Mode → Select a Mode → Ethernet. In the resulting popup window, restart the simulated robot.
7. Toggle the virtual motion bar slider button to the "auto" position. In the resulting popup window, select "AUTO (REMOTE)".
8. At the bottom of the window, click on the small robot icon with a "SIM" tag on it, then a small window will pop up, note down the displayed robot serial number.

