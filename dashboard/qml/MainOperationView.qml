import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: mainOpPage
    color: "#f0f2f5"

    signal goToMonitoring()
    signal logoutRequested()

    Connections {
        target: backend
        function onPumpSequenceComplete() {
            mainOpPage.goToMonitoring()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            color: "#1976D2"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16

                Label {
                    text: "Main Operation View"
                    font.pixelSize: 20
                    font.bold: true
                    color: "white"
                }

                Item { Layout.fillWidth: true }

                // MQTT status indicator
                RowLayout {
                    spacing: 6
                    Rectangle {
                        width: 10; height: 10; radius: 5
                        color: mqtt.connected ? "#4CAF50" : "#FF9800"
                    }
                    Label {
                        text: mqtt.connected ? "MQTT" : "SIM"
                        font.pixelSize: 11
                        color: "white"
                        opacity: 0.8
                    }
                }

                Rectangle { width: 1; height: 30; color: "white"; opacity: 0.3 }

                Label {
                    text: "Operator: " + backend.currentUser
                    font.pixelSize: 14
                    color: "white"
                }

                Button {
                    text: "Logout"
                    flat: true
                    font.pixelSize: 13
                    Material.foreground: "white"
                    onClicked: mainOpPage.logoutRequested()
                }
            }
        }

        // Content
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: contentCol.height + 32
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            ColumnLayout {
                id: contentCol
                width: parent.width - 32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 16
                spacing: 16

                // Row: Batch/Recipe + Process Params + Pump
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    // Batch & Recipe
                    Rectangle {
                        Layout.preferredWidth: 300
                        Layout.preferredHeight: 380
                        radius: 12
                        color: "white"
                        border.color: "#e0e0e0"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 12

                            Label {
                                text: "Batch & Recipe"
                                font.pixelSize: 16
                                font.bold: true
                                color: "#333"
                            }

                            Rectangle { Layout.fillWidth: true; height: 1; color: "#e0e0e0" }

                            Label { text: "Batch ID"; font.pixelSize: 12; color: "#666" }
                            TextField {
                                placeholderText: "Enter Batch ID"
                                Layout.fillWidth: true
                                font.pixelSize: 13
                                text: backend.batchId
                                onTextChanged: backend.batchId = text
                            }

                            Label { text: "Recipe Selection"; font.pixelSize: 12; color: "#666" }
                            ComboBox {
                                id: recipeCombo
                                Layout.fillWidth: true
                                font.pixelSize: 13
                                model: backend.recipeList
                                property bool ready: false
                                Component.onCompleted: {
                                    backend.recipe = currentText
                                    backend.applyRecipe(currentText)
                                    ready = true
                                }
                                onCurrentIndexChanged: {
                                    if (ready) {
                                        console.log("Recipe selected:", currentText)
                                        backend.recipe = currentText
                                        backend.applyRecipe(currentText)
                                    }
                                }
                            }

                            Item { Layout.fillHeight: true }
                        }
                    }

                    // Process Parameters
                    Rectangle {
                        Layout.preferredWidth: 380
                        Layout.preferredHeight: 380
                        radius: 12
                        color: "white"
                        border.color: "#e0e0e0"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 10

                            Label {
                                text: "Process Parameters"
                                font.pixelSize: 16
                                font.bold: true
                                color: "#333"
                            }

                            Label {
                                text: "Set by selected recipe"
                                font.pixelSize: 11
                                color: "#999"
                            }

                            Rectangle { Layout.fillWidth: true; height: 1; color: "#e0e0e0" }

                            GridLayout {
                                columns: 2
                                Layout.fillWidth: true
                                columnSpacing: 16
                                rowSpacing: 14

                                Label { text: "Vessel Temperature"; font.pixelSize: 12; color: "#555" }
                                Label { text: backend.vesselTempSetpoint.toFixed(1) + " °C"; font.pixelSize: 14; font.bold: true; color: "#1976D2" }

                                Label { text: "Pump Pressure"; font.pixelSize: 12; color: "#555" }
                                Label { text: backend.pumpPressureSetpoint.toFixed(0) + " Pa"; font.pixelSize: 14; font.bold: true; color: "#388E3C" }

                                Label { text: "Pump RPM"; font.pixelSize: 12; color: "#555" }
                                Label { text: backend.pumpRpmSetpoint.toFixed(0) + " rpm"; font.pixelSize: 14; font.bold: true; color: "#F57C00" }

                                Label { text: "Chiller Temperature"; font.pixelSize: 12; color: "#555" }
                                Label { text: backend.chillerTempSetpoint.toFixed(1) + " °C"; font.pixelSize: 14; font.bold: true; color: "#00838F" }

                                Label { text: "CO₂ Flow"; font.pixelSize: 12; color: "#555" }
                                Label { text: backend.co2FlowSetpoint.toFixed(0) + " Pa"; font.pixelSize: 14; font.bold: true; color: "#7B1FA2" }
                            }

                            Item { Layout.fillHeight: true }
                        }
                    }

                    // Pump Control
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 380
                        radius: 12
                        color: "white"
                        border.color: "#e0e0e0"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 16

                            Label {
                                text: "Pump Control"
                                font.pixelSize: 16
                                font.bold: true
                                color: "#333"
                            }

                            Rectangle { Layout.fillWidth: true; height: 1; color: "#e0e0e0" }

                            Item { Layout.fillHeight: true }

                            // Pump button
                            Button {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 70
                                font.pixelSize: 18
                                font.bold: true
                                enabled: !backend.flushing && !backend.pumpRunning
                                text: {
                                    if (backend.flushing) return "FLUSHING... " + backend.flushCountdown
                                    if (backend.pumpRunning) return "PUMP ACTIVE ✓"
                                    return "START PUMP"
                                }
                                Material.background: {
                                    if (backend.flushing) return "#FF9800"
                                    if (backend.pumpRunning) return "#4CAF50"
                                    return "#4CAF50"
                                }
                                Material.foreground: "white"
                                onClicked: backend.startPumpSequence()
                            }

                            // Status
                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 60
                                radius: 8
                                color: {
                                    if (backend.flushing) return "#FFF3E0"
                                    if (backend.pumpRunning) return "#E8F5E9"
                                    return "#f5f5f5"
                                }
                                border.color: {
                                    if (backend.flushing) return "#FF9800"
                                    if (backend.pumpRunning) return "#4CAF50"
                                    return "#e0e0e0"
                                }

                                RowLayout {
                                    anchors.centerIn: parent
                                    spacing: 10

                                    Rectangle {
                                        width: 14; height: 14; radius: 7
                                        color: {
                                            if (backend.flushing) return "#FF9800"
                                            if (backend.pumpRunning) return "#4CAF50"
                                            return "#ccc"
                                        }

                                        SequentialAnimation on opacity {
                                            running: backend.flushing
                                            loops: Animation.Infinite
                                            NumberAnimation { to: 0.3; duration: 400 }
                                            NumberAnimation { to: 1.0; duration: 400 }
                                        }
                                    }

                                    Label {
                                        font.pixelSize: 13
                                        font.bold: true
                                        color: {
                                            if (backend.flushing) return "#E65100"
                                            if (backend.pumpRunning) return "#2E7D32"
                                            return "#999"
                                        }
                                        text: {
                                            if (backend.flushing) return "Flushing pump... " + backend.flushCountdown + "s remaining"
                                            if (backend.pumpRunning) return "Pump running — initiating run..."
                                            return "Pump idle — press Start Pump"
                                        }
                                    }
                                }
                            }

                            Item { Layout.fillHeight: true }
                        }
                    }
                }

                // Process Diagram
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 350
                    radius: 12
                    color: "white"
                    border.color: "#e0e0e0"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 8

                        Label {
                            text: "Process Flow Diagram"
                            font.pixelSize: 16
                            font.bold: true
                            color: "#333"
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Rectangle { Layout.fillWidth: true; height: 1; color: "#e0e0e0" }

                        Image {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            source: "process_diagram.png"
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            mipmap: true
                        }
                    }
                }

                Item { Layout.preferredHeight: 16 }
            }
        }
    }
}
