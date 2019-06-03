# LEDCube matrixserver

This is a screenserver for the purpose of being used with differently orientated LED Matrix panels. 
It currently has been implemented for the LEDCube project, but it can also be used in simple, 
planar screen orientations, as well as other complex screen orientations.  

# Dependencies

on raspbian and ubuntu:
`sudo apt install git libeigen3-dev cmake wiringpi libboost-all-dev libasound2-dev libprotobuf-dev protobuf-compiler`

# Building

make sure you have cloned with submodules `git clone --recursive`  
tested on ubuntu, raspbian & macOS

`cd build && cmake .. && make`

# Quickstart

If you have an IceBreaker board with HUB75 adapter you can at first load the FPGA with the `rgb_panel` project example (https://github.com/squarewavedot/ice40-playground/tree/master/projects/rgb_panel)   
Then hook it up to the Raspberry Pi via USB, and compile and start the `Server_FPGA` target.  
In another Terminal compile and start the `cubetestapp` or `PixelFlow` or any other target from the exampleApplications folder.

## The Project is divided into multiple modules:

* common

	* protobuf message definition for communication
        * multiple communication classes
	
                * IPC (boost message queue, currently the most efficient local communication)
                * UnixSocket
                * TCPSocket (remote communication possible)
        * Screen & Color classes

* renderer
	* different renderers for interface with physical or virtual displays
	* multiple renderers can be used at the same time. i.e. SimulatorRenderer(remote over network) && HardwareRenderer
	* currently implemented:
		* RGBMatrixRenderer: hardwareinterface via hzeller rpi-rgb-matrix library
		* SimulatorRenderer: softwareinterface for the CubeSimulator project (see [https://github.com/squarewavedot/CubeSimulator])
		* FPGARendererFTDI: FTDI protocol implementation of FPGA rendering. Meant to be used together with [https://github.com/squarewavedot/ice40-playground.git]
* server & application
	* server logic
	* application interface library (applications link against this)
	* cubeapplication interface with convenient setPixel3D etc. methods

* exampleApplications

* server_* folders:
	* these are main.cpp with setups for servers with the different renderers
	    * server_FPGA
	        * the Icebreaker USB FTDI Renderer
	    * server_FPGA_RPISPI
	        * the Icebreaker native Raspberry Pi SPI renderer (more infos soon)
		* server_testapp
			* if you have installed OpenCV this target will be available. It shows the Screens as simple OpenCV windows (useful for debugging)
		* server_simulator
			* meant to be used with locally installed simulator (start simulator first)  [https://github.com/squarewavedot/CubeSimulator]

