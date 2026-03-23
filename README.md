# Industrial Monitoring & Control Dashboard (Qt 6)

## Overview
This project is a Qt 6 desktop application that simulates an industrial monitoring and control system (HMI-style dashboard). It displays live environmental and process data (e.g., temperature, humidity, system status) and allows basic user control such as starting/stopping a process or adjusting thresholds.

The project is inspired by manufacturing environments where operators monitor and interact with systems through control interfaces.

---

## Features (MVP)
- Real-time sensor data display (temperature, humidity)
- System status indicator (Running / Stopped)
- Start / Stop control button
- QML UI connected to C++ backend
- Basic MQTT communication

---

## Tech Stack
- **Qt 6 (Qt Quick / QML)**
- **C++ Backend**
- **MQTT Protocol**
- **Python (sensor data simulator)**
- **Desktop target (Windows/Linux)**

---

## Architecture

### Components
- **Qt Application**
  - QML UI (frontend)
  - C++ backend (logic + communication)

- **Data Source**
  - Python script simulating sensor data
  - MQTT broker (e.g., Mosquitto)

### Data Flow
- Python Script → MQTT Broker → Qt App (subscribe to sensor data)
- Qt App → MQTT Broker → Python Script (send control commands)

### Responsibilities

**QML (UI):**
- Display live data
- Provide user controls (buttons, inputs)

**C++ Backend:**
- Handle MQTT communication
- Process incoming data
- Send commands
- Expose data to QML (signals/slots, Q_PROPERTY)

---

## UI Overview

### Main Dashboard
- Temperature display
- Humidity display
- System status indicator
- Start / Stop button
- Connection status

### Settings Screen (Optional)
- Threshold input (e.g., temperature limit)
- Apply button

---

## Getting Started

### Prerequisites
- Qt 6.10+ (Qt Creator recommended)
- CMake
- MQTT broker (e.g., Mosquitto)
- Python 3 (for simulator)

### Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/qt-advanced-egzon-barja.git
   cd qt-advanced-egzon-barja
