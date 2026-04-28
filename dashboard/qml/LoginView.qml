import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: loginPage
    color: "#f0f2f5"

    signal loginSuccess(string username)

    // Auto-connect to MQTT on load
    Component.onCompleted: {
        console.log("LoginView loaded, auto-connecting MQTT...")
        mqtt.brokerHost = "localhost"
        mqtt.brokerPort = 1883
        mqtt.connectToBroker()
        backend.useMqtt = true
    }

    // Settings Dialog
    Dialog {
        id: settingsDialog
        title: "MQTT Settings"
        modal: true
        anchors.centerIn: parent
        width: 400
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent
            spacing: 12

            Label {
                text: "MQTT Broker Configuration"
                font.pixelSize: 16
                font.bold: true
                color: "#333"
            }

            GridLayout {
                columns: 2
                Layout.fillWidth: true
                columnSpacing: 12
                rowSpacing: 10

                Label { text: "Broker Host:"; font.pixelSize: 13; color: "#555" }
                TextField {
                    id: brokerHostField
                    Layout.fillWidth: true
                    text: mqtt.brokerHost
                    font.pixelSize: 13
                    placeholderText: "e.g. localhost or 192.168.1.100"
                }

                Label { text: "Broker Port:"; font.pixelSize: 13; color: "#555" }
                TextField {
                    id: brokerPortField
                    Layout.fillWidth: true
                    text: mqtt.brokerPort.toString()
                    font.pixelSize: 13
                    validator: IntValidator { bottom: 1; top: 65535 }
                }
            }

            // Connection status
            RowLayout {
                spacing: 10
                Rectangle {
                    width: 16; height: 16; radius: 8
                    color: mqtt.connected ? "#4CAF50" : "#f44336"
                }
                Label {
                    text: mqtt.connectionStatus
                    font.pixelSize: 13
                    font.bold: true
                    color: mqtt.connected ? "#4CAF50" : "#f44336"
                }
            }

            // Connect / Disconnect
            RowLayout {
                spacing: 10

                Button {
                    text: "Connect"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    font.pixelSize: 13
                    Material.background: "#4CAF50"
                    Material.foreground: "white"
                    enabled: !mqtt.connected
                    onClicked: {
                        mqtt.brokerHost = brokerHostField.text
                        mqtt.brokerPort = parseInt(brokerPortField.text)
                        mqtt.connectToBroker()
                        backend.useMqtt = true
                    }
                }

                Button {
                    text: "Disconnect"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    font.pixelSize: 13
                    Material.background: "#f44336"
                    Material.foreground: "white"
                    enabled: mqtt.connected
                    onClicked: {
                        mqtt.disconnectFromBroker()
                        backend.useMqtt = false
                    }
                }
            }

            // Mode indicator
            Label {
                text: mqtt.connected
                    ? "✓ Data will come from MQTT broker"
                    : "✗ Data will use internal simulation"
                font.pixelSize: 12
                color: mqtt.connected ? "#4CAF50" : "#888"
            }
        }

        onAccepted: {
            console.log("Settings saved")
        }
    }

    // Main Login Card
    Rectangle {
        anchors.centerIn: parent
        width: 420
        height: 460
        radius: 16
        color: "white"
        border.color: "#e0e0e0"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 35
            spacing: 14

            // Title row with settings gear
            RowLayout {
                Layout.fillWidth: true

                Item { Layout.fillWidth: true }

                Label {
                    text: "LOG IN"
                    font.pixelSize: 28
                    font.bold: true
                    color: "#333"
                }

                Item { Layout.fillWidth: true }

                // Settings gear button
                Button {
                    id: settingsBtn
                    flat: true
                    text: "⚙"
                    font.pixelSize: 22
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 44
                    onClicked: settingsDialog.open()

                    ToolTip.visible: hovered
                    ToolTip.text: "MQTT Settings"
                }
            }

            Label {
                text: "MQTT Sensor Dashboard"
                font.pixelSize: 14
                color: "#888"
                Layout.alignment: Qt.AlignHCenter
            }

            // Connection status bar
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 32
                radius: 6
                color: mqtt.connected ? "#E8F5E9" : "#FFF3E0"
                border.color: mqtt.connected ? "#4CAF50" : "#FF9800"

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 8

                    Rectangle {
                        width: 10; height: 10; radius: 5
                        color: mqtt.connected ? "#4CAF50" : "#FF9800"
                    }

                    Label {
                        text: mqtt.connected
                            ? "Connected to " + mqtt.brokerHost + ":" + mqtt.brokerPort
                            : "Connecting to broker..."
                        font.pixelSize: 11
                        color: mqtt.connected ? "#2E7D32" : "#E65100"
                    }
                }
            }

            Item { Layout.preferredHeight: 5 }

            // Username
            Label { text: "Username"; font.pixelSize: 13; color: "#555" }
            TextField {
                id: usernameField
                placeholderText: "Enter username"
                Layout.fillWidth: true
                font.pixelSize: 14
                leftPadding: 12
                Keys.onReturnPressed: passwordField.forceActiveFocus()
            }

            // Password
            Label { text: "Password"; font.pixelSize: 13; color: "#555" }
            TextField {
                id: passwordField
                placeholderText: "Enter password"
                echoMode: TextInput.Password
                Layout.fillWidth: true
                font.pixelSize: 14
                leftPadding: 12
                Keys.onReturnPressed: doLogin()
            }

            // Error message
            Label {
                id: errorLabel
                text: ""
                color: "#f44336"
                font.pixelSize: 12
                Layout.alignment: Qt.AlignHCenter
                visible: text.length > 0
            }

            // Login button
            Button {
                text: "LOG IN"
                Layout.fillWidth: true
                Layout.preferredHeight: 48
                font.pixelSize: 16
                font.bold: true
                Material.background: "#1976D2"
                Material.foreground: "white"
                onClicked: doLogin()
            }

            Label {
                text: "Hint: any username, password = 1234"
                font.pixelSize: 10
                color: "#aaa"
                Layout.alignment: Qt.AlignHCenter
            }

            Item { Layout.fillHeight: true }
        }
    }

    Connections {
        target: backend
        function onLoginFailed(reason) { errorLabel.text = reason }
    }

    function doLogin() {
        errorLabel.text = ""
        if (backend.login(usernameField.text, passwordField.text)) {
            loginPage.loginSuccess(usernameField.text)
        }
    }
}