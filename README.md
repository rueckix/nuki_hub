## About

The NUKI Hub software acts as a bridge between a NUKI Lock and a smart home system. Exposes the lock state (and more) via MQTT, and allows to execute commands like locking and unlocking.
Optionally, a NUKI Opener is also supported.

## Installation

Flash the firmware to an ESP32. The easiest way to install is to use the web installer:<br>
https://technyon.github.io/nuki_hub/
<br>(Needs a web serial compatible browser like Chrome/Opera/Edge).

As an alternative, download a release:<br>
https://github.com/technyon/nuki_hub/releases
<br>Read the included readme.txt for installation instructions for either Espressif Flash Download Tools or esptool.

## Setup

The firmware uses the Wifi Manager to configure the WiFi network. Power up the ESP32, a new Access Point should appear. Connect to this access point and in a browser navigate to "192.168.4.1". Use the web interface configure your Wifi network.

After configuring the Wifi, the ESP should automatically connect to your network. Use the web interface to setup the MQTT broker; just navigate to the IP-Address assigned to the ESP32 via DHCP (often found in the web interface of the internet router).<br>
To configure MQTT, enter the adress of your MQTT broker and eventually a username and a password if required. The firmware supports SSL encryption for MQTT, however most people and especially home users don't use this. In that case leave all fields about "MQTT SSL" blank.

## Paring

Just enable pairing mode on the NUKI lock (press button for a few seconds) and power on the ESP32. Pairing should be automatic. When pairing is successful, the web interface should show "Paired: Yes" (reload page in browser). MQTT nodes like lock state and battery level should now reflect the reported values from the lock.

## MQTT Interface

### Lock

- lock/action: Allows to execute lock actions. After receiving the action, the value is set to "ack". Possible actions: unlock, lock, unlatch, lockNgo, lockNgoUnlatch, fullLock, fobAction1, fobAction2, fobAction3
- lock/state: Reports the current lock state as a string. Possible values are: uncalibrated, locked, unlocked, unlatched, unlockedLnga, unlatching, bootRun, motorBlocked
- lock/trigger: The trigger of the last action: autoLock, automatic, button, manual, system
- lock/completionStatus: Status of the last action as reported by NUKI lock (needs bluetooth connection): success, motorBlocked, canceled, tooRecent, busy, lowMotorVoltage, clutchFailure, motorPowerFailure, incompleteFailure, invalidCode, otherError, unknown
- lock/authorizationId: If enabled in the web interface, this node returns the authorization id of the last lock action
- lock/authorizationName: If enabled in the web interface, this node returns the authorization name of the last lock action
- lock/commandResult: Result of the last action as reported by NUKI library: success, failed, timeOut, working, notPaired, error, undefined
- lock/doorSensorState: State of the door sensor: unavailable, deactivated, doorClosed, doorOpened, doorStateUnknown, calibrating

- battery/level: Battery level in percent
- battery/critical: 1 if battery level is critical, otherwise 0
- battery/charging: 1 if charging, otherwise 0
- battery/voltage: Current Battery voltage (V)
- battery/drain: The drain of the last lock action in Milliwattseconds (mWs)
- battery/maxTurnCurrent: The highest current of the turn motor during the last lock action (A)

### Opener

- lock/action: Allows to execute lock actions. After receiving the action, the value is set to "ack". Possible actions: activateRTO, deactivateRTO, electricStrikeActuation, activateCM, deactivateCM, fobAction1, fobAction2, fobAction3
- lock/state: Reports the current lock state as a string. Possible values are: locked, RTOactive, open, opening, uncalibrated
- lock/trigger: The trigger of the last action: autoLock, automatic, button, manual, system
- lock/completionStatus: Status of the last action as reported by NUKI lock (needs bluetooth connection): success, motorBlocked, canceled, tooRecent, busy, lowMotorVoltage, clutchFailure, motorPowerFailure, incompleteFailure, invalidCode, otherError, unknown
- lock/authorizationId: If enabled in the web interface, this node returns the authorization id of the last lock action
- lock/authorizationName: If enabled in the web interface, this node returns the authorization name of the last lock action
- lock/commandResult: Result of the last action as reported by NUKI library: success, failed, timeOut, working, notPaired, error, undefined
- lock/doorSensorState: State of the door sensor: unavailable, deactivated, doorClosed, doorOpened, doorStateUnknown, calibrating

- battery/voltage: Reports the current battery voltage in Volts.
- battery/critical: 1 if battery level is critical, otherwise 0

### Misc
- presence/devices: List of detected bluetooth devices as CSV. Can be used for presence detection

## Over-the-air Update (OTA)
After initially flashing the firmware via serial connection, further updates can be deployed via OTA update from a Web Browser. In the configuration portal, scroll down to "Firmware update" and click "Open". Then Click "Browse" and select the new "nuki_hub.bin" file and select "Upload file". After about a minute the new firmware should be installed.

## MQTT Encryption

The communication via MQTT can be SSL encrypted. To enable SSL encryption, supply the necessary information in the MQTT Configuration page.
The following configurations are supported:<br>

CA, CERT and KEY are empty -> No encryption<br>
CA is filled but CERT and KEY are empty -> Encrypted MQTT<br>
CA, CERT and KEY are filled -> Encrypted MQTT with client vaildation<br>

## Connecting via LAN (Optional)

If you prefer to connect to the MQTT Broker via LAN instead of WiFi, you can use a Wiznet W5x00 Module (W5100, W5200, W5500 are supported).
To connect, just wire the module and connect the LAN cable:

- Connect W5x00 to ESP32 SPI0:<br>
W5x00 SCK to GPIO18<br>
W5x00 MISO to GPIOGPIO19<br>
W5x00 MOSI to GPIO23<br>
W5x00 CS/SS to GPIO5
- Additionally connect:<br>
W5x00 reset to GPIO33
- Last but not least, on the ESP32 bridge GPIO26 and GND. This let's the firmware know that a LAN Module is connected

Wifi is now disabled, and the module doesn't boot into WifiManager anymore.

## Disclaimer

This is a third party software for NUKI smart door locks. This project or any of it's authors aren't associated with Nuki Home Solutions GmbH. Please refer for official products and offical support to their website:

https://nuki.io/

For further license details, check the included LICENSE file.
