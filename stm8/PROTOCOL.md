# Serial Protocol

This is a description of the serial protocol for this alternative firmware.

## Configuration

The serial works at 38400 8N1, the low speed is intended to allow MCU to handle
the flow without interfering too much with the rest of the work.

## Startup

The controller will send a welcome message of the model number followed by the
firmware revision  For example "BST900 V:1.0.0".

## Commands

All commands are line-feed or carriage-return terminated, and are not case sensitive. If the input buffer
is filled with no LF or CR character the entire line is dumped and a message is
sent on the output to indicate this failure.

## Responses

Invalid commands will be given a response "E!"<br/>
Valid commands will be replied with the appropriate response terminated with "OK". If the ECHO option is enabled, responses to a valid command will be
 "<COMMAND> - OK". This is to allow connected devices to confirm that all previous commands have been successfully executed.

### Menu Help

* Send: "HELP"
* Receive a list of all valid terminal commands.

### System Query

* Send: "SYSTEM"
* Receive: <br/>
"M: BST900<br/>
V: 1.0.0<br/>
N: Unnamed<br/>
O: OFF<br/>
E: OFF<br/>
AC: ON<br/>
OK"


This is the system information giving the model number, the firmware revision, the given name,
 the Output enable status, the command echo status, and the auto commit status.

### Commit configuration

* Send: "COMMIT"
* Receive: "COMMIT - OK" or "OK"

If auto commit is off this command will change the operating parameters according to the changes done since the last commit.
If auto commit is on, this command is not going to change anything.

### Auto-commit Set

* Send: "AUTOCOMMIT <YES/NO> | <1/0>"
* Receive: "AUTOMMIT - OK" or "OK"

Set the auto commit setting.

### Echo set

* Send "ECHO 1" or "ECHO 0"
* Receive "ECHO - OK" or "OK"

Includes the command name being acknowledged in the response.

### Name Set

* Send: "SNAME"
* Receive:<br/>
"SNAME: <name><br/>
 SNAME - OK"

Set the name to what the user gave. The size is limited to 16 characters and
they must be printable characters.

### Calibration Values

* Send: "CALIBRATION"
* Receive: details on calibration, mostly useful to debug and comparison of units<br/>
VIN  ADC 910000/0<br/>
VOUT ADC 1777000/0<br/>
COUT ADC 65300/0<br/>
VOUT PWM 2430/0<br/>
COUT PWM 6200/0<br/>
CALIBRATION - OK

### Output Enable/Disable

* Send: "OUTPUT 0" or "OUTPUT 1"
* Receive: "OK" for Output Disabled,  or <br/>
"PWM VOLTAGE X.XX XXXX<br/>
PWM CURRENT X.XX XXXX<br/>
OUTPUT - OK" for Output Enabled

OUTPUT 0 disables the output and OUTPUT 1 enables the output.

### Voltage Set

* Send: "VOLTAGE XXXXX"
* Receive: "VOLTAGE - OK" or "OK"

Set the maximum voltage level in mV.<br/>
Use the "CONFIG" command to confirm.

### Current Set

* Send: "CURRENT XXXXX"
* Receive: "CURRENT - OK" or "OK"

Set the maximum current level in mA.

### Default at startup

* Send: "DEFAULT 0" or "DEFAULT 1"
* Receive: "DEFAULT - OK" or "OK"

Set the default for the output at startup, either enabled or disabled.


### Query configuration

* Send: "CONFIG"
* Receive: <br/>
"OUTPUT: <OFF>|<ON><br/>
VSET <Voutmax><br/>
CSET <Ioutmax><br/>
CONFIG - OK"

Report all the config variables:

* Output -- Output enabled "ON" or disabled "OFF"

* Send: "LIMITS"
* Receive: <br/>
 VMIN <Minimum output voltage><br/>
 VMAX <Maximum output voltage><br/>
 VSTEP <Incremental step voltage><br/>
 CMIN <Minimum Current><br/>
 CMAX <Maximum output current<br/>
 CSTEP <Incremental step current>

### Status Report

* Send: "STATUS"
* Receive: "STATUS:\r\nOUTPUT: <Output>\r\nVOLTAGE IN: <Vin>\r\nVOLTAGE OUT: <Vout>\r\nVOLTAGE OUT: <Iout>\r\nCONSTANT: <CCCV>\r\n"

Reports all state variables:<br/>
OUTPUT <ON>|<OFF><br/>
VIN <Voltage Input to the unit><br/>
VOUT <Actual voltage output><br/>
COUT <Actual current output><br/>
CONSTANT <CURRENT> if we are in constant current mode, or <VOLTAGE> if we are in constant voltage mode<br/>
STATUS - OK

### Save configuration

* Send: "SAVE"
* Receive: "SAVE - OK" or "OK"

Saves current configuration to EEPROM

### Load configuration

* Send: "LOAD"
* Receive: "LOAD - OK" or "OK"

Loads configuration from EEPROM

### Restore Defaults

* Send: FACTORY
* Receive "OK" or "FACTORY - OK"

Returns all configuration to their default values.

### Help

* Send: "HELP"
* Receive: <br/>
List of all valid commands
