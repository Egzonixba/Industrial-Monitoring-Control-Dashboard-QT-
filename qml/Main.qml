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

    // Navigation state: 0=login, 1=mainOp, 2=monitoring
    property int currentView: 0

    Component.onCompleted: {
        console.log("Main window loaded, currentView:", currentView)
    }

    onCurrentViewChanged: {
        console.log("View changed to:", currentView)
    }

    // Login View
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
            console.log("Signal received: loginSuccess for", username)
            appWindow.currentView = 1
        }
    }

    // Main Operation View
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
        function onStartRunRequested() {
            console.log("Signal received: startRunRequested")
            appWindow.currentView = 2
        }
        function onLogoutRequested() {
            console.log("Signal received: logoutRequested")
            backend.logout()
            appWindow.currentView = 0
        }
    }

    // Monitoring View
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
            console.log("Signal received: goBackToMain")
            appWindow.currentView = 1
        }
    }
}



