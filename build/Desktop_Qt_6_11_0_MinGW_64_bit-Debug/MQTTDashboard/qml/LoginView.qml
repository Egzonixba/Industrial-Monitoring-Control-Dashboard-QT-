import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: loginPage
    color: "#f0f2f5"

    signal loginSuccess(string username)

    Rectangle {
        anchors.centerIn: parent
        width: 420
        height: 450
        radius: 16
        color: "white"
        border.color: "#e0e0e0"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 40
            spacing: 16

            Label {
                text: "LOG IN"
                font.pixelSize: 28
                font.bold: true
                color: "#333"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: "MQTT Sensor Dashboard"
                font.pixelSize: 14
                color: "#888"
                Layout.alignment: Qt.AlignHCenter
            }

            Item { Layout.preferredHeight: 10 }

            Label {
                text: "Username"
                font.pixelSize: 13
                color: "#555"
            }
            TextField {
                id: usernameField
                placeholderText: "Enter username"
                Layout.fillWidth: true
                font.pixelSize: 14
                leftPadding: 12
                Keys.onReturnPressed: passwordField.forceActiveFocus()
            }

            Label {
                text: "Password"
                font.pixelSize: 13
                color: "#555"
            }
            TextField {
                id: passwordField
                placeholderText: "Enter password"
                echoMode: TextInput.Password
                Layout.fillWidth: true
                font.pixelSize: 14
                leftPadding: 12
                Keys.onReturnPressed: doLogin()
            }

            Label {
                id: errorLabel
                text: ""
                color: "red"
                font.pixelSize: 12
                Layout.alignment: Qt.AlignHCenter
                visible: text.length > 0
            }

            Item { Layout.preferredHeight: 5 }

            Button {
                text: "OK"
                Layout.fillWidth: true
                Layout.preferredHeight: 48
                font.pixelSize: 16
                font.bold: true
                Material.background: Material.Blue
                Material.foreground: "white"
                onClicked: doLogin()
            }

            Label {
                text: "Hint: any username, password = 1234"
                font.pixelSize: 11
                color: "#aaa"
                Layout.alignment: Qt.AlignHCenter
            }

            Item { Layout.fillHeight: true }
        }
    }

    Connections {
        target: backend
        function onLoginFailed(reason) {
            errorLabel.text = reason
        }
    }

    function doLogin() {
        errorLabel.text = ""
        console.log("Attempting login for:", usernameField.text)
        if (backend.login(usernameField.text, passwordField.text)) {
            console.log("Login OK, emitting signal")
            loginPage.loginSuccess(usernameField.text)
        }
    }
}