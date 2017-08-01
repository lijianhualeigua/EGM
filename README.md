# EGM
Externally Guided Motion (EGM) for RobotStudio.

Note: when compiling on Windows using VisualStudio, you need to turn off the Precompiled Headers option.

For Linux, you need to install Ncurses library by :
sudo apt-get install ncurses-dev,
and Google Protocol Buffers by:
sudo apt-get install protobuf-compiler (this will install version 2.x).
Also, you do not need stdafx.* and targetver.h which are specific for Windows.

Be sure to set correct remote IP address for external device in RobotStudio.
