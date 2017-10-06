# EGM
Externally Guided Motion (EGM) for RobotStudio.

Note: when compiling on Windows using VisualStudio, you need to turn off the Precompiled Headers option.

For Linux, you need to install Ncurses library by :
sudo apt-get install ncurses-dev,
and Google Protocol Buffers by:
sudo apt-get install protobuf-compiler (this will install version 2.x).
Also, you do not need stdafx.* and targetver.h which are specific for Windows.

Be sure to set correct remote IP address for external device in RobotStudio.

The ros directory contains a simple ROS node which interfaces to EGM by a Xbox360 Joystick , using 
joint position control.

Be sure to check if the Joystick is configured successfully first (reference: http://wiki.ros.org/joy/Tutorials/ConfiguringALinuxJoystick).

# Tutorial
EGM Compile:
https://docs.google.com/document/d/1D8IZryG13nEBPCiD3mWVf8wryvLYuSUlSD63wkiGr8o/edit?usp=sharing

RobotStudio Setup:
https://docs.google.com/document/d/1wvwB2GgZjKCHqpVx5xUWfZQm0sD64qzcKVFJn2B3254/edit?usp=sharing 
