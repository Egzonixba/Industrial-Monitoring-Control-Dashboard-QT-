cmake_minimum_required(VERSION 3.16)

project(MQTTDashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2 Charts Network)

qt_standard_project_setup(REQUIRES 6.5)
qt_policy(SET QTP0004 OLD)

qt_add_executable(appMQTTDashboard
    main.cpp
    backend.h
    backend.cpp
    mqttmanager.h
    mqttmanager.cpp
)

qt_add_qml_module(appMQTTDashboard
    URI MQTTDashboard
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/LoginView.qml
        qml/MainOperationView.qml
        qml/MonitoringView.qml
    RESOURCES
        qml/process_diagram.png
)

target_link_libraries(appMQTTDashboard PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Charts
    Qt6::Network
)