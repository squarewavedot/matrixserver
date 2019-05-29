# LEDCube matrixserver

This is a screenserver for the purpose of beeing used with differently orientated LED Matrix panels. It currently has implementations for the LEDCube project, but it can also be used in simple, planar screen orientations, as well as other complex screen orientations

# Dependencies

on raspbian and ubuntu:
`sudo apt install git libeigen3-dev cmake wiringpi libboost-all-dev libasound2-dev libprotobuf-dev protobuf-compiler libmagick++-dev`

# Building

make sure you have cloned with submodules `git clone --recursive`
tested on ubuntu, raspbian & macOS

`cd build`
`cmake ..`
`make`

# Quickstart

TODO

## The Project is divided into multiple modules:

* common
        * protobuf message definition for communication
        * multiple communication classes
                * IPC (boost message queue, currently the most efficent local communication)
                * UnixSocket
                * TCPSocket (remote communication possible)
        * Screen & Color classes

* renderer
	* different renderers for interface with physical or virual displays
	* currently implemented:
		* RGBMatrixRenderer: hardwareinterface via hzeller rpi-rgb-matrix library
		* SimulatorRendere: softwareinterface for the CubeSimulator project (see x)
		* FPGARenderer: currently not implemented, meant to be the new hardwareinterface to LED Panels
* server & application
	* server logic
	* application interface library (applications link against this)
	* cubeapplication interface with convenient setPixel3D etc. methods

* exampleApplications

* server_* folders:
	* these are main.cpp with setups for servers with the different renderers
		* server_testapp
			* if you have installed OpenCV this target will be available. It shows the Screens as simple OpenCV windows (useful for debugging)
		* server_simulator
			* meant to be used with locally installed simulator (start simulator first)  [https://github.com/squarewavedot/CubeSimulator]

