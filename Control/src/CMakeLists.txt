cmake_minimum_required(VERSION 2.8)

project(PSUController)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -lwiringPi -pthread -finline-limit=150")
set(OUTPUT_NAME "main")
SET(CMAKE_BUILD_TYPE Debug)

set(project_sources main.cpp
		utils/MCP4922.cpp
		utils/MCP3204.cpp
		io_abstraction/Converters.cpp

		io_abstraction/PersistentConfiguration.cpp
		io_abstraction/InputBoard.cpp
		io_abstraction/OutputBoard.cpp
		features/BatteryUtils.cpp
		features/Logger.cpp
		utils/IPCHandler.cpp utils/IPCHandler.hpp)

add_executable(${PROJECT_NAME} ${project_sources})
