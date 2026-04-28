cmake_minimum_required(VERSION 3.16)

project(PlantSimulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

qt_add_executable(PlantSimulator
    main.cpp
    plantsimulator.h
    plantsimulator.cpp
    simplemqtt.h
    simplemqtt.cpp
)

target_link_libraries(PlantSimulator PRIVATE
    Qt6::Core
    Qt6::Network
)