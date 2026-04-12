cmake_minimum_required(VERSION 3.16)

project(MQTTDashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2 Charts)

qt_standard_project_setup(REQUIRES 6.5)

# Suppress the QTP0004 policy warning for qml subdirs
qt_policy(SET QTP0004 OLD)

qt_add_executable(appMQTTDashboard
    main.cpp
    backend.h
    backend.cpp
)

qt_add_qml_module(appMQTTDashboard
    URI MQTTDashboard
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/LoginView.qml
        qml/MainOperationView.qml
        qml/MonitoringView.qml
)

target_link_libraries(appMQTTDashboard PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Charts
)

install(TARGETS appMQTTDashboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)