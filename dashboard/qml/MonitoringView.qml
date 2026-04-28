import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: monitoringPage
    color: "#f0f2f5"

    signal goBackToMain()

    // Setpoint Dialog
    Dialog {
        id: setpointDialog
        title: "Adjust Setpoint"
        modal: true
        anchors.centerIn: parent
        width: 380
        standardButtons: Dialog.Ok | Dialog.Cancel

        property string paramName: ""
        property string paramUnit: ""
        property double currentValue: 0
        property color accentColor: "#1976D2"
        property var callback: null

        ColumnLayout {
            anchors.fill: parent
            spacing: 14

            Label {
                text: setpointDialog.paramName
                font.pixelSize: 18
                font.bold: true
                color: setpointDialog.accentColor
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                radius: 6
                color: "#f5f5f5"

                Label {
                    anchors.centerIn: parent
                    text: "Current: " + setpointDialog.currentValue.toFixed(2) + " " + setpointDialog.paramUnit
                    font.pixelSize: 14
                    color: "#555"
                }
            }

            Label { text: "New Setpoint Value:"; font.pixelSize: 13; color: "#555" }

            TextField {
                id: newSetpointField
                Layout.fillWidth: true
                Layout.preferredHeight: 48
                placeholderText: "Enter value in " + setpointDialog.paramUnit
                font.pixelSize: 16
                validator: DoubleValidator { bottom: -1000; top: 1000000 }
            }
        }

        onAccepted: {
            if (callback && newSetpointField.text.length > 0) {
                callback(parseFloat(newSetpointField.text))
            }
        }

        onOpened: {
            newSetpointField.text = currentValue.toFixed(2)
            newSetpointField.forceActiveFocus()
            newSetpointField.selectAll()
        }

        function openFor(name, unit, current, color, cb) {
            paramName = name
            paramUnit = unit
            currentValue = current
            accentColor = color
            callback = cb
            open()
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
                    text: "Process Monitoring"
                    font.pixelSize: 20
                    font.bold: true
                    color: "white"
                }

                Item { Layout.fillWidth: true }

                RowLayout {
                    spacing: 6
                    Rectangle {
                        width: 10; height: 10; radius: 5
                        color: mqtt.connected ? "#4CAF50" : "#FF9800"
                    }
                    Label {
                        text: mqtt.connected ? "MQTT" : "SIM"
                        font.pixelSize: 11; color: "white"; opacity: 0.8
                    }
                }

                Rectangle { width: 1; height: 30; color: "white"; opacity: 0.3 }

                Label {
                    text: "Operator: " + backend.currentUser
                    font.pixelSize: 14; color: "white"
                }
            }
        }

        // Content
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 12
            spacing: 8

            // Parameter Cards
            RowLayout {
                Layout.fillWidth: true
                spacing: 6

                // Vessel Temperature
                Rectangle {
                    id: vesselCard
                    Layout.fillWidth: true; Layout.preferredHeight: 140; radius: 10; border.width: 2
                    property int alarm: backend.vesselTempAlarm
                    color: alarm !== 0 ? "#FFEBEE" : "#E3F2FD"
                    border.color: alarm !== 0 ? "#f44336" : "#1976D2"
                    SequentialAnimation on opacity { running: vesselCard.alarm !== 0
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.5; duration: 500 }
                        NumberAnimation { to: 1.0; duration: 500 }
                    }

                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 10; spacing: 2
                        RowLayout {
                            Label { text: "Vessel Temperature"; font.pixelSize: 10; font.bold: true; color: "#1976D2" }
                            Item { Layout.fillWidth: true }
                            Label { visible: vesselCard.alarm !== 0; text: vesselCard.alarm > 0 ? "⚠ HIGH" : "⚠ LOW"; font.pixelSize: 9; font.bold: true; color: "#f44336" }
                        }
                        Label { text: backend.vesselTemp.toFixed(1) + " °C"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.vesselTempSetpoint.toFixed(1) + " °C"; font.pixelSize: 9; color: "#888" }
                        Item { Layout.fillHeight: true }
                        Button {
                            text: "Adjust Setpoint"; Layout.fillWidth: true; Layout.preferredHeight: 30; font.pixelSize: 10
                            Material.background: "#1976D2"; Material.foreground: "white"
                            onClicked: setpointDialog.openFor("Vessel Temperature", "°C", backend.vesselTempSetpoint, "#1976D2", function(v) { backend.vesselTempSetpoint = v })
                        }
                    }
                }

                // Pump Pressure
                Rectangle {
                    id: pressureCard
                    Layout.fillWidth: true; Layout.preferredHeight: 140; radius: 10; border.width: 2
                    property int alarm: backend.pumpPressureAlarm
                    color: alarm !== 0 ? "#FFEBEE" : "#E8F5E9"
                    border.color: alarm !== 0 ? "#f44336" : "#388E3C"
                    SequentialAnimation on opacity {
                        running: pressureCard.alarm !== 0
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.5; duration: 500 }
                        NumberAnimation { to: 1.0; duration: 500 }
                    }

                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 10; spacing: 2
                        RowLayout {
                            Label { text: "Pump Pressure"; font.pixelSize: 10; font.bold: true; color: "#388E3C" }
                            Item { Layout.fillWidth: true }
                            Label { visible: pressureCard.alarm !== 0; text: pressureCard.alarm > 0 ? "⚠ HIGH" : "⚠ LOW"; font.pixelSize: 9; font.bold: true; color: "#f44336" }
                        }
                        Label { text: backend.pumpPressure.toFixed(0) + " Pa"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.pumpPressureSetpoint.toFixed(0) + " Pa"; font.pixelSize: 9; color: "#888" }
                        Item { Layout.fillHeight: true }
                        Button {
                            text: "Adjust Setpoint"; Layout.fillWidth: true; Layout.preferredHeight: 30; font.pixelSize: 10
                            Material.background: "#388E3C"; Material.foreground: "white"
                            onClicked: setpointDialog.openFor("Pump Pressure", "Pa", backend.pumpPressureSetpoint, "#388E3C", function(v) { backend.pumpPressureSetpoint = v })
                        }
                    }
                }

                // Pump RPM
                Rectangle {
                    id: rpmCard
                    Layout.fillWidth: true; Layout.preferredHeight: 140; radius: 10; border.width: 2
                    property int alarm: backend.pumpRpmAlarm
                    color: alarm !== 0 ? "#FFEBEE" : "#FFF3E0"
                    border.color: alarm !== 0 ? "#f44336" : "#F57C00"
                    SequentialAnimation on opacity {
                        running: rpmCard.alarm !== 0
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.5; duration: 500 }
                        NumberAnimation { to: 1.0; duration: 500 }
                    }

                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 10; spacing: 2
                        RowLayout {
                            Label { text: "Pump RPM"; font.pixelSize: 10; font.bold: true; color: "#F57C00" }
                            Item { Layout.fillWidth: true }
                            Label { visible: rpmCard.alarm !== 0; text: rpmCard.alarm > 0 ? "⚠ HIGH" : "⚠ LOW"; font.pixelSize: 9; font.bold: true; color: "#f44336" }
                        }
                        Label { text: backend.pumpRpm.toFixed(0) + " rpm"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.pumpRpmSetpoint.toFixed(0) + " rpm"; font.pixelSize: 9; color: "#888" }
                        Item { Layout.fillHeight: true }
                        Button {
                            text: "Adjust Setpoint"; Layout.fillWidth: true; Layout.preferredHeight: 30; font.pixelSize: 10
                            Material.background: "#F57C00"; Material.foreground: "white"
                            onClicked: setpointDialog.openFor("Pump RPM", "rpm", backend.pumpRpmSetpoint, "#F57C00", function(v) { backend.pumpRpmSetpoint = v })
                        }
                    }
                }

                // Chiller Temp
                Rectangle {
                    id: chillerCard
                    Layout.fillWidth: true; Layout.preferredHeight: 140; radius: 10; border.width: 2
                    property int alarm: backend.chillerTempAlarm
                    color: alarm !== 0 ? "#FFEBEE" : "#E0F7FA"
                    border.color: alarm !== 0 ? "#f44336" : "#00838F"
                    SequentialAnimation on opacity {
                        running: chillerCard.alarm !== 0
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.5; duration: 500 }
                        NumberAnimation { to: 1.0; duration: 500 }
                    }

                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 10; spacing: 2
                        RowLayout {
                            Label { text: "Chiller Temp"; font.pixelSize: 10; font.bold: true; color: "#00838F" }
                            Item { Layout.fillWidth: true }
                            Label { visible: chillerCard.alarm !== 0; text: chillerCard.alarm > 0 ? "⚠ HIGH" : "⚠ LOW"; font.pixelSize: 9; font.bold: true; color: "#f44336" }
                        }
                        Label { text: backend.chillerTemp.toFixed(1) + " °C"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.chillerTempSetpoint.toFixed(1) + " °C"; font.pixelSize: 9; color: "#888" }
                        Item { Layout.fillHeight: true }
                        Button {
                            text: "Adjust Setpoint"; Layout.fillWidth: true; Layout.preferredHeight: 30; font.pixelSize: 10
                            Material.background: "#00838F"; Material.foreground: "white"
                            onClicked: setpointDialog.openFor("Chiller Temperature", "°C", backend.chillerTempSetpoint, "#00838F", function(v) { backend.chillerTempSetpoint = v })
                        }
                    }
                }

                // CO2 Flow
                Rectangle {
                    id: co2Card
                    Layout.fillWidth: true; Layout.preferredHeight: 140; radius: 10; border.width: 2
                    property int alarm: backend.co2FlowAlarm
                    color: alarm !== 0 ? "#FFEBEE" : "#F3E5F5"
                    border.color: alarm !== 0 ? "#f44336" : "#7B1FA2"
                    SequentialAnimation on opacity {
                        running: co2Card.alarm !== 0
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.5; duration: 500 }
                        NumberAnimation { to: 1.0; duration: 500 }
                    }

                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 10; spacing: 2
                        RowLayout {
                            Label { text: "CO₂ Flow"; font.pixelSize: 10; font.bold: true; color: "#7B1FA2" }
                            Item { Layout.fillWidth: true }
                            Label { visible: co2Card.alarm !== 0; text: co2Card.alarm > 0 ? "⚠ HIGH" : "⚠ LOW"; font.pixelSize: 9; font.bold: true; color: "#f44336" }
                        }
                        Label { text: backend.co2Flow.toFixed(0) + " Pa"; font.pixelSize: 22; font.bold: true; color: "#333" }
                        Label { text: "SP: " + backend.co2FlowSetpoint.toFixed(0) + " Pa"; font.pixelSize: 9; color: "#888" }
                        Item { Layout.fillHeight: true }
                        Button {
                            text: "Adjust Setpoint"; Layout.fillWidth: true; Layout.preferredHeight: 30; font.pixelSize: 10
                            Material.background: "#7B1FA2"; Material.foreground: "white"
                            onClicked: setpointDialog.openFor("CO₂ Flow", "Pa", backend.co2FlowSetpoint, "#7B1FA2", function(v) { backend.co2FlowSetpoint = v })
                        }
                    }
                }
            }

            // Run Timer
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: 50; radius: 10
                color: "white"; border.color: "#e0e0e0"

                RowLayout {
                    anchors.fill: parent; anchors.margins: 12; spacing: 16
                    Label { text: "Run Time:"; font.pixelSize: 15; font.bold: true; color: "#333" }
                    Label {
                        text: backend.runTimeString; font.pixelSize: 24; font.bold: true; font.family: "Courier"
                        color: backend.running ? "#1976D2" : (backend.emergencyStopped ? "#f44336" : "#666")
                    }
                    Item { Layout.fillWidth: true }
                    Label {
                        text: backend.emergencyStopped ? "⚠ EMERGENCY STOPPED" : (backend.running ? "● RUNNING" : "■ STOPPED")
                        font.pixelSize: 13; font.bold: true
                        color: backend.emergencyStopped ? "#f44336" : (backend.running ? "#4CAF50" : "#999")
                    }
                }
            }

            // Chart
            Rectangle {
                Layout.fillWidth: true; Layout.fillHeight: true; radius: 12
                color: "white"; border.color: "#e0e0e0"

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 10; spacing: 2

                    Label { text: "Process Parameters Over Time"; font.pixelSize: 13; font.bold: true; color: "#333"; Layout.alignment: Qt.AlignHCenter }

                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter; spacing: 14
                        Row { spacing: 3; Rectangle { width: 12; height: 3; color: "#1976D2"; anchors.verticalCenter: parent.verticalCenter } Label { text: "Vessel T"; font.pixelSize: 9; color: "#555" } }
                        Row { spacing: 3; Rectangle { width: 12; height: 3; color: "#388E3C"; anchors.verticalCenter: parent.verticalCenter } Label { text: "Pressure÷100"; font.pixelSize: 9; color: "#555" } }
                        Row { spacing: 3; Rectangle { width: 12; height: 3; color: "#F57C00"; anchors.verticalCenter: parent.verticalCenter } Label { text: "RPM÷100"; font.pixelSize: 9; color: "#555" } }
                        Row { spacing: 3; Rectangle { width: 12; height: 3; color: "#00838F"; anchors.verticalCenter: parent.verticalCenter } Label { text: "Chiller T"; font.pixelSize: 9; color: "#555" } }
                        Row { spacing: 3; Rectangle { width: 12; height: 3; color: "#7B1FA2"; anchors.verticalCenter: parent.verticalCenter } Label { text: "CO₂÷100"; font.pixelSize: 9; color: "#555" } }
                    }

                    Canvas {
                        id: chartCanvas; Layout.fillWidth: true; Layout.fillHeight: true

                        onPaint: {
                            var ctx = getContext("2d"); var w = width; var h = height
                            var margin = { top: 10, right: 20, bottom: 30, left: 55 }
                            var plotW = w - margin.left - margin.right; var plotH = h - margin.top - margin.bottom
                            ctx.clearRect(0, 0, w, h); ctx.fillStyle = "#fafafa"; ctx.fillRect(margin.left, margin.top, plotW, plotH)

                            var times = backend.timeHistory; var count = times ? times.length : 0
                            if (count < 2) { ctx.fillStyle = "#999"; ctx.font = "16px sans-serif"; ctx.textAlign = "center"; ctx.fillText("Waiting for data...", w/2, h/2); return }

                            var vd = backend.vesselTempHistory, pd = backend.pumpPressureHistory, rd = backend.pumpRpmHistory, cd = backend.chillerTempHistory, ad = backend.co2FlowHistory
                            var minT = times[0], maxT = times[count-1]; if (maxT <= minT) maxT = minT + 1
                            var maxY = 10
                            for (var i = 0; i < count; i++) maxY = Math.max(maxY, vd[i], pd[i]/100, rd[i]/100, Math.abs(cd[i]), ad[i]/100)
                            maxY *= 1.15

                            ctx.strokeStyle = "#e0e0e0"; ctx.lineWidth = 0.5; ctx.fillStyle = "#888"; ctx.font = "10px sans-serif"; ctx.textAlign = "right"
                            for (var g = 0; g <= 5; g++) { var gy = margin.top + plotH - (g/5)*plotH; ctx.beginPath(); ctx.moveTo(margin.left, gy); ctx.lineTo(margin.left+plotW, gy); ctx.stroke(); ctx.fillText((maxY*g/5).toFixed(1), margin.left-5, gy+3) }
                            ctx.textAlign = "center"
                            for (var xl = 0; xl <= 5; xl++) { var xt = minT + (maxT-minT)*xl/5; var xx = margin.left + (xl/5)*plotW; ctx.fillText(xt.toFixed(0)+"s", xx, h-8); ctx.beginPath(); ctx.moveTo(xx, margin.top); ctx.lineTo(xx, margin.top+plotH); ctx.stroke() }

                            function drawLine(data, clr, fn) { ctx.strokeStyle = clr; ctx.lineWidth = 2; ctx.beginPath(); for (var j = 0; j < count; j++) { var px = margin.left+((times[j]-minT)/(maxT-minT))*plotW; var py = margin.top+plotH-(fn(data[j])/maxY)*plotH; if(j===0) ctx.moveTo(px,py); else ctx.lineTo(px,py) } ctx.stroke() }
                            drawLine(vd, "#1976D2", function(v){return v}); drawLine(pd, "#388E3C", function(v){return v/100}); drawLine(rd, "#F57C00", function(v){return v/100}); drawLine(cd, "#00838F", function(v){return Math.abs(v)}); drawLine(ad, "#7B1FA2", function(v){return v/100})
                            ctx.strokeStyle = "#ccc"; ctx.lineWidth = 1; ctx.strokeRect(margin.left, margin.top, plotW, plotH)
                        }
                    }
                }
            }

            // Bottom Controls
            RowLayout {
                Layout.fillWidth: true; Layout.preferredHeight: 60; spacing: 12

                // Emergency Stop
                Rectangle {
                    Layout.preferredWidth: 180; Layout.preferredHeight: 56; radius: 10
                    color: "#FFEBEE"; border.color: "#f44336"; border.width: 2

                    RowLayout {
                        anchors.fill: parent; anchors.margins: 8; spacing: 8
                        Rectangle {
                            width: 40; height: 40; radius: 20; color: "#f44336"
                            Label { anchors.centerIn: parent; text: "STOP"; font.pixelSize: 10; font.bold: true; color: "white" }
                            MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor; onClicked: backend.emergencyStop() }
                        }
                        Label { text: "EMERGENCY\nSTOP"; font.pixelSize: 10; font.bold: true; color: "#c62828" }
                    }
                }

                Item { Layout.fillWidth: true }

                Button {
                    text: "Finish Run"; Layout.preferredWidth: 160; Layout.preferredHeight: 48; font.pixelSize: 14; font.bold: true
                    Material.background: "#FF9800"; Material.foreground: "white"; enabled: backend.running
                    onClicked: backend.finishRun()
                }

                Button {
                    text: "Save Run"; Layout.preferredWidth: 160; Layout.preferredHeight: 48; font.pixelSize: 14; font.bold: true
                    Material.background: "#1976D2"; Material.foreground: "white"; enabled: !backend.running
                    onClicked: { backend.saveRun(); monitoringPage.goBackToMain() }
                }
            }
        }
    }

    Connections {
        target: backend
        function onSensorDataChanged() { chartCanvas.requestPaint() }
    }
}