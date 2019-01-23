cmake_minimum_required(VERSION 2.8)

project(PSUControllerSim)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lwiringPi -O3 -flto")
set(OUTPUT_NAME "main")
set(project_sources main.cpp
        SPISlaveSimulator.cpp
        MCP4922.cpp
        MCP3204.cpp
        Converters.cpp
        Controller.cpp
	PersistentConfiguration.cpp
	InputBoard.cpp
	OutputBoard.cpp)

add_executable(${PROJECT_NAME} ${project_sources})
