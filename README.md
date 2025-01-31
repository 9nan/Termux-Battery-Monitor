# Termux Battery Monitor

## Overview
Termux Battery Monitor is a lightweight system that tracks the battery percentage of an Android device running Termux and sends the data to a Node.js server. The server stores and provides this battery information via API endpoints.

## Features
- Retrieves battery percentage using Termux commands.
- Sends battery data to a Node.js server every 10 seconds.
- Provides an API to fetch the latest battery status.
- Includes a health check (`/ping`) to verify server connectivity.

## Project Structure
```
/battery.sh  - Script to send battery percentage from Termux
/Server.js   - Node.js server to receive and serve battery data
/esp.ino     - (Potentially for an ESP8266/ESP32 integration)
```

## Installation & Usage

### 1. Setting up the Server
#### Requirements
- Node.js installed on your server or PC.

#### Steps
1. Install dependencies:
   ```sh
   npm install express cors
   ```
2. Start the server:
   ```sh
   node Server.js
   ```
   The server will listen on port `21132`.

### 2. Setting up Termux Client
#### Requirements
- Termux app installed.
- `jq` installed (`pkg install jq`).

#### Steps
1. Grant battery status permission:
   ```sh
   termux-battery-status
   ```
2. Run the script:
   ```sh
   bash battery.sh
   ```
   This will send battery data every 10 seconds to the server.

### 3. API Endpoints
| Endpoint         | Method | Description                      |
|-----------------|--------|----------------------------------|
| `/update-battery` | POST   | Receives battery data from Termux |
| `/battery`        | GET    | Retrieves the latest battery data |
| `/ping`          | GET    | Checks if the server is running |

## Future Improvements
- Web dashboard for battery monitoring.
- Support for multiple devices.
- ESP8266/ESP32 integration.

## License
This project is licensed under the MIT License.
