import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: monitoringPage
    color: "#f0f2f5"

    signal goBackToMain()

    Component.onCompleted: {
        console.log("MonitoringView loaded successfully!")
    }

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
                    text: "PROCESS PARAMETERS MONITORING VIEW"
                    font.pixelSize: 18
                    font.bold: true
                    color: "white"
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: "Operator: " + backend.currentUser
                    font.pixelSize: 14
                    color: "white"
                }
            }
        }

        // Content
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 16
            spacing: 12

            // Parameter Cards Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    radius: 10
                    color: "#E3F2FD"
                    border.color: "#1976D2"
                    border.width: 1
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 2
                        Label { text: "Vessel Temperature"; font.pixelSize: 11; font.bold: true; color: "#1976D2" }
                        Label { text: backend.vesselTemp.toFixed(1) + " °C"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.vesselTempSetpoint.toFixed(1) + " °C"; font.pixelSize: 10; color: "#888" }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    radius: 10
                    color: "#E8F5E9"
                    border.color: "#388E3C"
                    border.width: 1
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 2
                        Label { text: "Pump Pressure"; font.pixelSize: 11; font.bold: true; color: "#388E3C" }
                        Label { text: backend.pumpPressure.toFixed(0) + " Pa"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.pumpPressureSetpoint.toFixed(0) + " Pa"; font.pixelSize: 10; color: "#888" }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    radius: 10
                    color: "#FFF3E0"
                    border.color: "#F57C00"
                    border.width: 1
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 2
                        Label { text: "Pump RPM"; font.pixelSize: 11; font.bold: true; color: "#F57C00" }
                        Label { text: backend.pumpRpm.toFixed(0) + " rpm"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.pumpRpmSetpoint.toFixed(0) + " rpm"; font.pixelSize: 10; color: "#888" }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    radius: 10
                    color: "#E0F7FA"
                    border.color: "#00838F"
                    border.width: 1
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 2
                        Label { text: "Chiller Temp"; font.pixelSize: 11; font.bold: true; color: "#00838F" }
                        Label { text: backend.chillerTemp.toFixed(1) + " °C"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.chillerTempSetpoint.toFixed(1) + " °C"; font.pixelSize: 10; color: "#888" }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    radius: 10
                    color: "#F3E5F5"
                    border.color: "#7B1FA2"
                    border.width: 1
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 2
                        Label { text: "Dry Air Flow"; font.pixelSize: 11; font.bold: true; color: "#7B1FA2" }
                        Label { text: backend.dryAirFlow.toFixed(2) + " kg/m³"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.dryAirFlowSetpoint.toFixed(2) + " kg/m³"; font.pixelSize: 10; color: "#888" }
                    }
                }
            }

            // Run Timer
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 55
                radius: 10
                color: "white"
                border.color: "#e0e0e0"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 16

                    Label {
                        text: "Run Time:"
                        font.pixelSize: 16
                        font.bold: true
                        color: "#333"
                    }

                    Label {
                        text: backend.runTimeString
                        font.pixelSize: 26
                        font.bold: true
                        font.family: "Courier"
                        color: backend.running ? "#1976D2" : (backend.emergencyStopped ? "#f44336" : "#666")
                    }

                    Item { Layout.fillWidth: true }

                    Label {
                        text: backend.emergencyStopped ? "EMERGENCY STOPPED" :
                              (backend.running ? "RUNNING" : "STOPPED")
                        font.pixelSize: 14
                        font.bold: true
                        color: backend.emergencyStopped ? "#f44336" :
                               (backend.running ? "#4CAF50" : "#999")
                    }
                }
            }

            // Chart — takes full width
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 12
                color: "white"
                border.color: "#e0e0e0"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 4

                    Label {
                        text: "Process Parameters Over Time"
                        font.pixelSize: 14
                        font.bold: true
                        color: "#333"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    // Legend
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 16

                        Row {
                            spacing: 4
                            Rectangle { width: 14; height: 3; color: "#1976D2"; anchors.verticalCenter: parent.verticalCenter }
                            Label { text: "Vessel Temp"; font.pixelSize: 10; color: "#555" }
                        }
                        Row {
                            spacing: 4
                            Rectangle { width: 14; height: 3; color: "#388E3C"; anchors.verticalCenter: parent.verticalCenter }
                            Label { text: "Pressure ÷1k"; font.pixelSize: 10; color: "#555" }
                        }
                        Row {
                            spacing: 4
                            Rectangle { width: 14; height: 3; color: "#F57C00"; anchors.verticalCenter: parent.verticalCenter }
                            Label { text: "RPM ÷10"; font.pixelSize: 10; color: "#555" }
                        }
                        Row {
                            spacing: 4
                            Rectangle { width: 14; height: 3; color: "#00838F"; anchors.verticalCenter: parent.verticalCenter }
                            Label { text: "Chiller Temp"; font.pixelSize: 10; color: "#555" }
                        }
                        Row {
                            spacing: 4
                            Rectangle { width: 14; height: 3; color: "#7B1FA2"; anchors.verticalCenter: parent.verticalCenter }
                            Label { text: "AirFlow ×10"; font.pixelSize: 10; color: "#555" }
                        }
                    }

                    // Canvas Chart
                    Canvas {
                        id: chartCanvas
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        onPaint: {
                            var ctx = getContext("2d")
                            var w = width
                            var h = height
                            var margin = { top: 10, right: 20, bottom: 30, left: 55 }
                            var plotW = w - margin.left - margin.right
                            var plotH = h - margin.top - margin.bottom

                            ctx.clearRect(0, 0, w, h)

                            ctx.fillStyle = "#fafafa"
                            ctx.fillRect(margin.left, margin.top, plotW, plotH)

                            var times = backend.timeHistory
                            var count = times ? times.length : 0

                            if (count < 2) {
                                ctx.fillStyle = "#999"
                                ctx.font = "16px sans-serif"
                                ctx.textAlign = "center"
                                ctx.fillText("Waiting for data...", w / 2, h / 2)
                                return
                            }

                            var vesselData = backend.vesselTempHistory
                            var pressureData = backend.pumpPressureHistory
                            var rpmData = backend.pumpRpmHistory
                            var chillerData = backend.chillerTempHistory
                            var airFlowData = backend.dryAirFlowHistory

                            var minT = times[0]
                            var maxT = times[count - 1]
                            if (maxT <= minT) maxT = minT + 1

                            var maxY = 10
                            for (var i = 0; i < count; i++) {
                                var vt = vesselData[i]
                                var pr = pressureData[i] / 1000.0
                                var rpm = rpmData[i] / 10.0
                                var ct = chillerData[i]
                                var af = airFlowData[i] * 10.0
                                maxY = Math.max(maxY, vt, pr, rpm, ct, af)
                            }
                            maxY = maxY * 1.15

                            // Grid lines and Y labels
                            ctx.strokeStyle = "#e0e0e0"
                            ctx.lineWidth = 0.5
                            ctx.fillStyle = "#888"
                            ctx.font = "10px sans-serif"
                            ctx.textAlign = "right"
                            for (var g = 0; g <= 5; g++) {
                                var gy = margin.top + plotH - (g / 5) * plotH
                                ctx.beginPath()
                                ctx.moveTo(margin.left, gy)
                                ctx.lineTo(margin.left + plotW, gy)
                                ctx.stroke()
                                ctx.fillText((maxY * g / 5).toFixed(1), margin.left - 5, gy + 3)
                            }

                            // X axis labels
                            ctx.textAlign = "center"
                            for (var xl = 0; xl <= 5; xl++) {
                                var xt = minT + (maxT - minT) * xl / 5
                                var xx = margin.left + (xl / 5) * plotW
                                ctx.fillText(xt.toFixed(0) + "s", xx, h - 8)
                                ctx.beginPath()
                                ctx.moveTo(xx, margin.top)
                                ctx.lineTo(xx, margin.top + plotH)
                                ctx.stroke()
                            }

                            function drawLine(data, color, scaleFn) {
                                ctx.strokeStyle = color
                                ctx.lineWidth = 2
                                ctx.beginPath()
                                for (var j = 0; j < count; j++) {
                                    var px = margin.left + ((times[j] - minT) / (maxT - minT)) * plotW
                                    var val = scaleFn(data[j])
                                    var py = margin.top + plotH - (val / maxY) * plotH
                                    if (j === 0) ctx.moveTo(px, py)
                                    else ctx.lineTo(px, py)
                                }
                                ctx.stroke()
                            }

                            drawLine(vesselData,   "#1976D2", function(v) { return v })
                            drawLine(pressureData, "#388E3C", function(v) { return v / 1000.0 })
                            drawLine(rpmData,      "#F57C00", function(v) { return v / 10.0 })
                            drawLine(chillerData,  "#00838F", function(v) { return v })
                            drawLine(airFlowData,  "#7B1FA2", function(v) { return v * 10.0 })

                            ctx.strokeStyle = "#ccc"
                            ctx.lineWidth = 1
                            ctx.strokeRect(margin.left, margin.top, plotW, plotH)
                        }
                    }
                }
            }

            // Bottom Row: Emergency Stop + Finish Run + Save Run
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                spacing: 12

                // Emergency Stop — compact, same height as param cards
                Rectangle {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 100
                    radius: 10
                    color: "#FFEBEE"
                    border.color: "#f44336"
                    border.width: 2

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Rectangle {
                            width: 60
                            height: 60
                            radius: 30
                            color: "#f44336"

                            Label {
                                anchors.centerIn: parent
                                text: "STOP"
                                font.pixelSize: 14
                                font.bold: true
                                color: "white"
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    console.log("EMERGENCY STOP!")
                                    backend.emergencyStop()
                                }
                            }
                        }

                        Label {
                            text: "EMERGENCY\nSTOP"
                            font.pixelSize: 11
                            font.bold: true
                            color: "#c62828"
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Finish Run
                Button {
                    text: "Finish RUN"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 50
                    font.pixelSize: 14
                    font.bold: true
                    Material.background: "#FF9800"
                    Material.foreground: "white"
                    enabled: backend.running
                    onClicked: {
                        console.log("Finish run clicked")
                        backend.finishRun()
                    }
                }

                // Save Run
                Button {
                    text: "Save RUN"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 50
                    font.pixelSize: 14
                    font.bold: true
                    Material.background: "#1976D2"
                    Material.foreground: "white"
                    enabled: !backend.running
                    onClicked: {
                        console.log("Save run clicked")
                        backend.saveRun()
                        monitoringPage.goBackToMain()
                    }
                }
            }
        }
    }

    // Repaint chart when data changes
    Connections {
        target: backend
        function onSensorDataChanged() {
            chartCanvas.requestPaint()
        }
    }
}