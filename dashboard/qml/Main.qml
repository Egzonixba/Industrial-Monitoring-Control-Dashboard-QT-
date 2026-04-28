import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    id: appWindow
    visible: true
    width: 1400
    height: 900
    minimumWidth: 1200
    minimumHeight: 800
    title: "MQTT Sensor Dashboard"
    x: 100
    y: 100

    Material.theme: Material.Light
    Material.accent: Material.Blue
    color: "#f0f2f5"

    property int currentView: 0

    onCurrentViewChanged: console.log("View changed to:", currentView)

    Loader {
        id: loginLoader
        anchors.fill: parent
        active: appWindow.currentView === 0
        visible: active
        source: "LoginView.qml"
    }
    Connections {
        target: loginLoader.item
        ignoreUnknownSignals: true
        function onLoginSuccess(username) {
            appWindow.currentView = 1
        }
    }

    Loader {
        id: mainOpLoader
        anchors.fill: parent
        active: appWindow.currentView === 1
        visible: active
        source: "MainOperationView.qml"
    }
    Connections {
        target: mainOpLoader.item
        ignoreUnknownSignals: true
        function onGoToMonitoring() {
            appWindow.currentView = 2
        }
        function onLogoutRequested() {
            backend.logout()
            appWindow.currentView = 0
        }
    }

    Loader {
        id: monitoringLoader
        anchors.fill: parent
        active: appWindow.currentView === 2
        visible: active
        source: "MonitoringView.qml"
    }
    Connections {
        target: monitoringLoader.item
        ignoreUnknownSignals: true
        function onGoBackToMain() {
            appWindow.currentView = 1
        }
    }
}