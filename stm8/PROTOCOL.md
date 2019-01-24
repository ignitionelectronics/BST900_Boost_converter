# Serial Protocol

This is a description of the serial protocol for this alternative firmware.

## Configuration

The serial works at 9600 8N1, the low speed is intended to allow MCU to handle
the flow without interfering too much with the rest of the work.

## Startup

The controller will send a welcome message of the model number followed by the
firmware revision  For example "B3603 V:2.0.0".

## Commands

All commands are line-feed or carriage-return terminated, and are not case sensitive. If the input buffer
is filled with no LF or CR character the entire line is dumped and a message is
sent on the output to indicate this failure.

## Responses

Invalid commands will be given a response "E!"
Valid commands will be replied with the appropriate response terminated with "OK".

### Menu Help

* Send: "HELP"
* Receive a list of all valid terminal commands.

### System Query

* Send: "SYSTEM"
* Receive: 
"M: B3603
V: 2.0.0
N: Unnamed
O: OFF
AC: ON

OK"


This is the system information giving the model number, the firmware revision, the given name,
and the Output enable status.

### Version Query

* Send: "VERSION"
* Receive: "VERSION: X.XX"

### System Configuration Query

* Send: "SYSTEM"
* Receive: "SYSTEM:\r\nMODEL: <model>\r\nVERSION: <version>\r\nNAME: <name>\r\nONSTARTUP: <ON/OFF>\r\nAUTOCOMMIT: <YES/NO>\r\n"

Get the system information: model, version, name, auto-on on startup and auto commit.

### Commit configuration

* Send: "COMMIT"
* Receive: "COMMIT: DONE\r\n"

If auto commit is off this command will change the operating parameters according to the changes done since the last commit.
If auto commit is on, this command is not going to change anything.

### Auto-commit Set

* Send: "AUTOCOMMIT <YES/NO>"
* Receive: "AUTOMMIT: YES" or "AUTOCOMMIT: NO"

Set the auto commit setting.

### Name Set

* Send: "SNAME"
* Receive: "SNAME: <name>"

Set the name to what the user gave. The size is limited to 16 characters and
they must be printable characters.

### Calibration Values

* Send: "CALIBRATION"
* Receive: detailed on calibration, mostly useful to debug and comparison of units

### Output Enable/Disable

* Send: "OUTPUT 0" or "OUTPUT 1"
* Receive: "OK" for Output Disabled,  or 
"PWM VOLTAGE X.XXX XXXX
PWM CURRENT X.XXX XXXX" for Output Enabled

OUTPUT 0 disables the output and OUTPUT 1 enables the output.

### Voltage Set

* Send: "VOLTAGE XXXXX"
* Receive: "OK"

Set the maximum voltage level in mV.
Use the "CONFIG" command to confirm.

### Current Set

* Send: "CURRENT XXXXX"
* Receive: "OK"

Set the maximum current level in mA.

### Default at startup

* Send: "DEFAULT 0" or "DEFAULT 1"
* Receive: "DEFAULT: DISABLED" or "DEFAULT: ENABLED"

Set the default for the output at startup, either enabled or disabled.

Default disabled is better for safety, default enabled is useful when the unit
powers an always on device and needs to always work without a manual
intervention when the power comes back after a power outage.



### Query configuration

* Send: "CONFIG"
* Receive: "CONFIG:\r\nOUTPUT: <Output>\r\nVOLTAGE SET: <Voutmax>\r\nCURRENT SET: <Ioutmax>\r\nVOLTAGE SHUTDOWN: <Vshutdown>\r\nCURRENT SHUTDOWN: <Cshutdown>\r\n"

Report all the config variables:

* Output -- Output enabled "ON" or disabled "OFF"

* Send: "LIMITS"
* Receive: 
* VMIN -- Minimum output voltage
* VMAX -- Maximum output voltage
* VSTEP -- Incremental step voltage
* CMIN -- Minimum Current
* CMAX -- Maximum output current
* CSTEP -- Incremental step current

### Status Report

* Send: "STATUS"
* Receive: "STATUS:\r\nOUTPUT: <Output>\r\nVOLTAGE IN: <Vin>\r\nVOLTAGE OUT: <Vout>\r\nVOLTAGE OUT: <Iout>\r\nCONSTANT: <CCCV>\r\n"

Reports all state variables:

* OUTPUT -- Output enabled "ON" or disabled "OFF"
* VIN -- Voltage Input to the unit
* VOUT -- Actual voltage output
* COUT -- Actual current output
* CONSTANT -- "CURRENT" if we are in constant current, "VOLTAGE" if we are in constant voltage

### Restore Defaults

* Send: FACTORY
* Receive "OK"

Returns all configuration to their default values.

