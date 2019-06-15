cmake_minimum_required(VERSION 2.8)

project(PSUController)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -lwiringPi -pthread  -finline-limit=150")
set(OUTPUT_NAME "main")
set(project_sources main.cpp
        MCP4922.cpp
        MCP3204.cpp
        Converters.cpp
        Controller.cpp
	PersistentConfiguration.cpp
	InputBoard.cpp
        OutputBoard.cpp
        BatteryUtils.cpp
        Logger.cpp
        Tester.cpp)

add_executable(${PROJECT_NAME} ${project_sources})
