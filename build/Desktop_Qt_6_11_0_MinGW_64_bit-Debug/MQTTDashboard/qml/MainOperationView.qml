import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: mainOpPage
    color: "#f0f2f5"

    signal startRunRequested()
    signal logoutRequested()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header Bar
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

                Label {
                    text: "Operator: " + backend.currentUser
                    font.pixelSize: 14
                    color: "white"
                }

                Button {
                    text: "Logout"
                    flat: true
                    Material.foreground: "white"
                    onClicked: {
                        console.log("Logout clicked")
                        mainOpPage.logoutRequested()
                    }
                }
            }
        }

        // Content Area - Scrollable
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: contentColumn.height + 32
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            ColumnLayout {
                id: contentColumn
                width: parent.width - 32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 16
                spacing: 16

                // Row 1: Batch/Recipe + Process Parameters + Pump Control
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    // Batch & Recipe Card
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

                            Label { text: "Batch ID"; font.pixelSize: 12; color: "#666" }
                            TextField {
                                id: batchIdField
                                placeholderText: "Enter Batch ID"
                                Layout.fillWidth: true
                                text: backend.batchId
                                onTextChanged: backend.batchId = text
                            }

                            Label { text: "Recipe Selection"; font.pixelSize: 12; color: "#666" }
                            ComboBox {
                                id: recipeCombo
                                Layout.fillWidth: true
                                model: backend.recipeList
                                onCurrentTextChanged: backend.recipe = currentText
                                Component.onCompleted: {
                                    if (backend.recipeList.length > 0)
                                        backend.recipe = currentText
                                }
                            }

                            Item { Layout.fillHeight: true }
                        }
                    }

                    // Process Parameters Card — taller now
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
                                text: "Set Process Parameters"
                                font.pixelSize: 16
                                font.bold: true
                                color: "#333"
                            }

                            GridLayout {
                                columns: 2
                                Layout.fillWidth: true
                                columnSpacing: 12
                                rowSpacing: 10

                                Label { text: "Vessel Temperature (°C)"; font.pixelSize: 12; color: "#555" }
                                TextField {
                                    Layout.preferredWidth: 120
                                    text: backend.vesselTempSetpoint.toFixed(1)
                                    validator: DoubleValidator { bottom: 0; top: 500 }
                                    onEditingFinished: backend.vesselTempSetpoint = parseFloat(text)
                                }

                                Label { text: "Pump Pressure (Pa)"; font.pixelSize: 12; color: "#555" }
                                TextField {
                                    Layout.preferredWidth: 120
                                    text: backend.pumpPressureSetpoint.toFixed(0)
                                    validator: DoubleValidator { bottom: 0; top: 1000000 }
                                    onEditingFinished: backend.pumpPressureSetpoint = parseFloat(text)
                                }

                                Label { text: "Pump RPM Setpoint"; font.pixelSize: 12; color: "#555" }
                                TextField {
                                    Layout.preferredWidth: 120
                                    text: backend.pumpRpmSetpoint.toFixed(0)
                                    validator: DoubleValidator { bottom: 0; top: 10000 }
                                    onEditingFinished: backend.pumpRpmSetpoint = parseFloat(text)
                                }

                                Label { text: "Chiller Temperature (°C)"; font.pixelSize: 12; color: "#555" }
                                TextField {
                                    Layout.preferredWidth: 120
                                    text: backend.chillerTempSetpoint.toFixed(1)
                                    validator: DoubleValidator { bottom: -50; top: 100 }
                                    onEditingFinished: backend.chillerTempSetpoint = parseFloat(text)
                                }

                                Label { text: "Dry Air Flow (kg/m³)"; font.pixelSize: 12; color: "#555" }
                                TextField {
                                    Layout.preferredWidth: 120
                                    text: backend.dryAirFlowSetpoint.toFixed(2)
                                    validator: DoubleValidator { bottom: 0; top: 100 }
                                    onEditingFinished: backend.dryAirFlowSetpoint = parseFloat(text)
                                }
                            }

                            Item { Layout.fillHeight: true }
                        }
                    }

                    // Pump Operation & Status Card
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 380
                        radius: 12
                        color: "white"
                        border.color: "#e0e0e0"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 12

                            RowLayout {
                                spacing: 40
                                Layout.fillWidth: true

                                ColumnLayout {
                                    spacing: 8

                                    Label {
                                        text: "Pump Operation"
                                        font.pixelSize: 16
                                        font.bold: true
                                        color: "#333"
                                    }

                                    Button {
                                        text: "Flush PUMP"
                                        Layout.preferredWidth: 150
                                        Layout.preferredHeight: 40
                                        Material.background: "#FF9800"
                                        Material.foreground: "white"
                                        enabled: !backend.flushing
                                        onClicked: backend.flushPump()
                                    }

                                    Button {
                                        text: "Pump ON"
                                        Layout.preferredWidth: 150
                                        Layout.preferredHeight: 40
                                        Material.background: "#4CAF50"
                                        Material.foreground: "white"
                                        enabled: !backend.pumpRunning
                                        onClicked: backend.pumpOn()
                                    }

                                    Button {
                                        text: "Pump OFF"
                                        Layout.preferredWidth: 150
                                        Layout.preferredHeight: 40
                                        Material.background: "#f44336"
                                        Material.foreground: "white"
                                        enabled: backend.pumpRunning
                                        onClicked: backend.pumpOff()
                                    }
                                }

                                ColumnLayout {
                                    spacing: 10

                                    Label {
                                        text: "Pump Status"
                                        font.pixelSize: 16
                                        font.bold: true
                                        color: "#333"
                                    }

                                    Row {
                                        spacing: 8
                                        Rectangle {
                                            width: 14; height: 14; radius: 7
                                            color: backend.flushing ? "#FF9800" : "#ccc"
                                        }
                                        Label { text: "Flushing sequence"; font.pixelSize: 12; color: "#555" }
                                    }

                                    Row {
                                        spacing: 8
                                        Rectangle {
                                            width: 14; height: 14; radius: 7
                                            color: backend.pumpRunning ? "#4CAF50" : "#ccc"
                                        }
                                        Label { text: "Pump running"; font.pixelSize: 12; color: "#555" }
                                    }

                                    Row {
                                        spacing: 8
                                        Rectangle {
                                            width: 14; height: 14; radius: 7
                                            color: !backend.pumpRunning ? "#f44336" : "#ccc"
                                        }
                                        Label { text: "Pump is OFF"; font.pixelSize: 12; color: "#555" }
                                    }
                                }
                            }

                            Item { Layout.fillHeight: true }
                        }
                    }
                }

                // Spacer to push Start Run down
                Item {
                    Layout.preferredHeight: 20
                }

                // Row 2: Start Run Button
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    radius: 12
                    color: "white"
                    border.color: "#e0e0e0"

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 12

                        Label {
                            text: "Start Run"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#333"
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Button {
                            text: "  START  "
                            Layout.preferredWidth: 200
                            Layout.preferredHeight: 50
                            font.pixelSize: 18
                            font.bold: true
                            Material.background: "#4CAF50"
                            Material.foreground: "white"
                            Layout.alignment: Qt.AlignHCenter
                            onClicked: {
                                console.log("START button clicked!")
                                backend.startRun()
                                console.log("Emitting startRunRequested signal")
                                mainOpPage.startRunRequested()
                            }
                        }
                    }
                }

                // Bottom spacer
                Item {
                    Layout.preferredHeight: 16
                }
            }
        }
    }
}