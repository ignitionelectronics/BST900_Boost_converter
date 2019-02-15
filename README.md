# BST900
### Control of Boost Converter over serial interface

This is a fork of B3603 originally by [baruch](https://github.com/baruch/b3603) and later forked by iafilius and [frmaioli](https://github.com/frmaioli/b3603)
This fork extends the code to support the BST900 and BST400 boost converters from MingHe.
The object of the BST900 alternative firmware is to allow control of the boost converter over a serial interface. In my case this is to control the charge current of my home battery system

The plugin top board of BST900 has a convenient unused serial socket (3.3V logic levels), which can be easily connected to a suitable controller. (In my case an ESP8266).
BST900 and BST400 have a different architecture to the original B3603. They use a UCC3803 Current Mode PWM controller on their bottom boards, but they use the same  STM8 based microcontroller top board albeit running slightly different firmware.

### Status
The project is a work in progress. It is functional. Control over the  BST900 using the serial interface is working, but it currently only supports output voltages to 65.5V.

![BST900 with STM8 Development board](docs/BST900_with_STM8_development_board.png) The firmware may be loaded onto a BST900 top board, but because the stock firmware is read protected it is not possible to  save the stock firmware and revert back to it later. However it is possible to load the firmware onto an STM8 development board, and by hooking up
the appropriate pins to the connector, run the firmware without the seven segment display or push buttons. The firmware can thereby be evaluated without making any irrevesible choices.

## Alternative to this firmware
There is another way to control the current delivered by a BST900 without having to change the firmware. It is possible to set the constant current limit using either an analogue or digital potentiometer.

Looking at the BST900. The fourth pin down on the left hand row is the pin used to control the Constant Current limit of UCC38803. This pin carries a digital PWM signal from the top board. However the UCC3803
 will work equally well with an analogue voltage on this pin.
 
 ##### Procedure
 * Put the top board on long pin header sockets to raise it up.
 * Bend Pin 4 of the left hand header out so this pin is open circuited.
 * Insert a wire into Pin 4 of the socket on the lower board and connect to the wiper of a 10k Potentiometer (or Digipot)
 * Connect one side of the potentiometer to GND which is found 2nd terminal on the left in the row of 4 immediately to the left of the buttons.
 * Connect the other side of the potentiometer to +3.3V via a resistor of 15-20k. 3.3V is found at the right hand terminal of the row of 4.
 
 That's it.  Hopefully it will not blow up!  It is probably a good idea to feed the BST900 with a constant current source while testing it out.


## BST900 Architecture

BST900 is based on the UCC3803 Current Mode PWM Controller from TI. It boosts voltages up to 120V and operates at currents up to 15A.
The output voltage is controlled by a PWM signal on pin5 of the upper board in exactly the same way as B3603. Pin4 carries a PWM signal whose width determines the Constant Current limit.

![BST900](docs/BST900_Top_View.png)

![BST900 Bottom View](docs/BST900_Bottom_View.png)


## ToDo

* Implement control for the cooling fan.  - Done
* Rework Constant Current control to suit BST900. - Done
* Implement a start at power on feature to suit my application for charging a home battery system. - Done
* Recalibrate all measurements and PWM to suit BST900 - Done
* Rework code to use a 10mV voltage resolution to allow voltages above 65535mV - In Progress


## Components needed:
* [BST900](https://www.aliexpress.com/item/DC-DC-BST900-0-15A-8-60V-To-10-120V-Boost-Converter-Power-Supply-Module-CC/32838432319.html) -- The unit being reprogrammed
* [STM8 Development board](https://www.aliexpress.com/item/STM8S103F3P6-system-board-STM8S-STM8-development-board-minimum-core-board/32802517941.html) -- Enables firmware to be tried without losing the stock firmware.
* [FT232RL](https://www.aliexpress.com/item/FT232RL-FT232-FTDI-USB-3-3V-5-5V-to-TTL-Serial-Adapter-Module-Mini-Port-for/32896631192.html) -- A usb-to-serial TTL-level
* [STLink V2](https://www.aliexpress.com/item/Hot-Sale-ST-LINK-Stlink-ST-Link-V2-Mini-STM8-STM32-Simulator-Download-Programmer-Programming-With/32684040486.html) -- programmer for the STM8S microcontroller


## Software needed:
* [SDCC v 3.7.0] sudo apt install sdcc
* [stm8flash](https://github.com/vdudouyt/stm8flash) -- STM8 flasher

## Improved Cooling
While the manufacturer specifies the BST900 as operating at up to 15A looking at the size of the supplied heatsink I was not too hopeful about it actually working for long at that current.
It is fairly easy to uprate the cooling by removing the MOSFET and diode pair and mounting them on the lower side of the PCB with a much larger heat sink (use spacers to ensure no short circuit between the heatsink and PCB traces, 
and make sure the Mosfet leads go to the correct places). 
BST900 also has an unused socket for a second diode pair. Adding a second diode will more than halve the heat dissipation of each diode since the lower current will mean the diode is operating at a lower forward voltage. If adding a diode make sure they are both of the
 same type.
 
 ![BST900 Improved Cooling](docs/BST900_Improved_Cooling.png)

# Extracts from the Original project page.

## Schematics

These were done by flex, the discussion can be seen in the EEVBlog forum (link at the bottom).


Top board schematics:

![B3603 Top Board Schematics](docs/B3603_TopBoardSchematics.png)


### Pinouts

Lets name the different pinout components, left and right are as seen looking at the top board with the 7-segment display up:

* MCU
* Left connector -- 8 pins left side, analogue and digital signals
* Right connector -- 8 pins right side +5V power and analogue and digital Ground.
* Serial connector -- 4 pins at left most side. 3.3V logic level
* SWIM connector -- 4 pins at the bottom, just left of the buttons. Used to programme the STM8 chip

#### Pinout from MCU


| MCU pin | MCU Function | Board Connector | Board Connector Pin | Board Connector Name
| ------- | -------------|-----------------|---------------------|-----
| Pin 1 | UART1\_CK/TIM2\_CH1/BEEP/(HS) PD4 | 74HC595 | Pin 3 | DS
| Pin 2 | UART1\_TX | Serial connector | Pin 2 | TX
| Pin 3 | UART1\_RX | Serial connector | Pin 4 | RX
| Pin 4 | NRST | SWIM | Pin 1 | SWIM NRST
| Pin 5 | OSCIN/PA1 | 74HC595 | Pin 11 | SHCP
| Pin 6 | OSCOUT/PA2 | 74HC595 | Pin 12 | STCP
| Pin 7 | Vss (GND) | | |
| Pin 8 | Vcap | | |
| Pin 9 | Vdd | | |
| Pin 10 | SPI\_NSS / TIM2\_CH3 / PA3 (HS) | CV/CC leds |  | CV/CC leds
| Pin 11 | PB5 (T) / I2C\_SDA / TIM1\_BKIN | Left connector | Pin 7 | CV/CC status
| Pin 12 | PB4 (T) / I2C\_SCL / ADC\_ETR | Left connector | Pin 6 | Enable Output + Red (ON) led
| Pin 13 | PC3 (HS) / TIM1\_CH3 [TLI] [TIM1_CH1N]| Left Connector | Pin 8 | Fan Control
| Pin 14 | PC4 (HS) / TIM1\_CH4 / CLK\_CCO / AIN2 / TIM1\_CH2N | Left connector | Pin 1 | Iout sense 16\*(0.01V + Iout\*0.05)
| Pin 15 | PC5 (HS) / SPI\_SCK / TIM2\_CH1 | Left connector | Pin 5 | Vout set
| Pin 16 | PC6 (HS) / SPI\_MOSI / TIM1\_CH1 | Left connector | Pin 4 | Iout set
| Pin 17 | PC7 (HS) / SPI\_MISO / TIM1\_CH2 | Button |  | Buttons
| Pin 18 | PD1 (HS) / SWIM | SWIM | Pin 3 | SWIM & Buttons
| Pin 19 | PD2 (HS) / AIN3 / TIM2\_CH3 | Left connector | Pin 2 | Vout sense
| Pin 20 | PD3 (HS) / AIN4 / TIM2\_CH2 / ADC\_ETR | Left connector | Pin 3 | Vin sense (Vin/16)



## Links

* [Manufacturer product page](http://www.mhinstek.com/product/html/?106.html) (Chinese) ([English translation](https://translate.google.com/translate?sl=auto&tl=en&js=y&prev=_t&hl=en&ie=UTF-8&u=http%3A%2F%2Fwww.mhinstek.com%2Fproduct%2Fhtml%2F%3F106.html&edit-text=))
* [EEVBlog forum discussion](http://www.eevblog.com/forum/reviews/b3603-dcdc-buck-converter-mini-review-and-how-the-set-key-could-be-fatal/)

