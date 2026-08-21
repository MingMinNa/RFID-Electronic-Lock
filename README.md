# RFID Electronic Lock

[![Static Badge](https://img.shields.io/badge/MPLAB%20X-v5.20-blue)](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide) [![Static Badge](https://img.shields.io/badge/MPLAB%20XC8%20Compiler-v2.5-blue)](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/xc8) ![Static Badge](https://img.shields.io/badge/PICkit-4-blue)

An electronic lock system based on the **RDM6300** RFID reader and **PIC18F4520** microcontrollers.

## Overview

This project implements a simple RFID-based electronic lock.
- The RDM6300 reads the RFID card ID and processes it according to the current operating mode.
- Two LEDs indicate the current mode, while the buzzer provides feedback. The servo motor simulates unlocking.

### Demo

[Watch Demo Video](./Video/Demo_Video.mp4)

### Notes
- A dual-microcontroller architecture is used because the PIC18F4520 has only one timer, `TMR2`, available for PWM operation, while both the buzzer and servo require PWM.  
- The two PIC18F4520 devices communicate via UART, with a computer acting as an intermediary.

## Operating Modes

The system provides three operating modes:

| Mode           | LED1 | LED2 | Function                                 |
| -------------- | :--: | :--: | ---------------------------------------- |
| **Register**   |  ON  |  ON  | Add an RFID ID to the database           |
| **Deregister** |  ON  |  OFF | Remove an RFID ID from the database      |
| **Check**      |  OFF |  ON  | Verify an RFID ID and simulate unlocking |

The push button switches between Register and Deregister modes,   
while the variable resistor switches between Check mode and the Register/Deregister modes.

### Register Mode

| Result                  | Status  |
| ----------------------- | ------- |
| Registration Successful | Success |
| ID Already Registered   | Error   |
| ID Database Full        | Error   |
| Invalid ID              | Error   |

### Deregister Mode

| Result                    | Status  |
| ------------------------- | ------- |
| Deregistration Successful | Success |
| ID Not Found              | Error   |
| ID Database Empty         | Error   |
| Invalid ID                | Error   |

### Check Mode

| Result           | Status                 |
| ---------------- | ---------------------- |
| Check Successful | Success + Servo Unlock |
| ID Not Found     | Error                  |
| Invalid ID       | Error                  |

A different buzzer sound is played for successful and failed operations.  
In **Check Mode**, a successful verification activates the servo motor to simulate unlocking.


## Circuit Diagram
![Circuit Diagram](./Images/Circuit%20Diagram.png)

## Hardware Components

### PIC18F4520

Two **PIC18F4520** microcontrollers are used in this project.

<img src="./Images/Components/PIC18F4520.jpg" alt="PIC18F4520" height="200"> <img src="./Images/Components/PIC18F4520 Pinout Diagram.png" alt="PIC18F4520 Pinout" height="200">

* **Device 1** handles the main system logic, including RFID reading, mode selection, LEDs, button, variable resistor, and buzzer.
* **Device 2** handles the USB-to-TTL interface and SG90 servo motor.
* The two devices communicate via UART.

> The dual-microcontroller architecture allows the buzzer and servo to use PWM independently.

### RDM6300 RFID Reader

The **RDM6300** is a 125 kHz RFID reader used to read the card ID.

<img src="./Images/Components/RFID%20RDM6300%20Pinout%20Diagram.png" alt="RDM6300 PINOUT" width="280"> <img src="./Images/Components/RDM6300%20Copper%20Coil.jpg" alt="RDM6300 Coil" width="200"> <img src="./Images/Components/125kHz RFID Card.jpg" alt="125kHz RFID Card" width="320" height="200">

| RDM6300 | Connection        |
| ------- | ----------------- |
| `ANT1`  | Coil — Black Wire |
| `ANT2`  | Coil — Red Wire   |
| `VCC (below)`   | Device 1 `VDD`    |
| `GND (below)`   | Device 1 `VSS`    |
| `TX`    | Device 1 `RC7/RX` |

### Push Button

The push button is used to switch between operating modes.

<img src="./Images/Components/Button.jpg" alt="Button" width="200">

| Button     | Connection                    |
| ---------- | ----------------------------- |
| One side   | `RB0/INT0`                    |
| Other side | `VSS`                         |
| Pull-up    | `RB0/INT0` → Resistor → `VDD` |

### LEDs

Two LEDs indicate the current operating mode.

<img src="./Images/Components/LED.jpg" alt="LED" width="200">

|  LED | PIC18F4520 |
|------|------------------------------|
| LED1 | `RA1/AN1` → Resistor → Anode |
| LED2 | `RA2/AN2` → Resistor → Anode |

Both LED cathodes are connected to `VSS`.

### Buzzer

The buzzer provides audio feedback for successful and failed operations. 

<img src="./Images/Components/Buzzer.jpg" alt="Buzzer" width="200">

| Buzzer | PIC18F4520 |
| ------ | ---------- |
| `VCC`  | `VDD`      |
| `GND`  | `VSS`      |
| `I/O`  | `RC2/CCP1` |

### Variable Resistor

The variable resistor is used for operating-mode selection through the ADC.

<img src="./Images/Components/Variable Resistor.jpg" alt="Variable Resistor" width="200" height="200">

| Variable Resistor | PIC18F4520 |
| ----------------- | ---------- |
| One side          | `VDD`      |
| Center            | `RA0/AN0`  |
| Other side        | `VSS`      |

### USB-to-TTL Serial Cable

The USB-to-TTL cable connects the system to the computer for serial communication.

<img src="./Images/Components/USB to TTL Cable.jpg" alt="TTL Cable" width="200" height="200">

| TTL Wire | Device 1 | Device 2 |
| -------- | -------- | -------- |
| Red      | `VDD`    | ✗        |
| Black    | `VSS`    | ✗        |
| Green    | ✗        | `RC7/RX` |
| White    | `RC6/TX` | ✗        |

### SG90 Servo Motor

The SG90 servo simulates the physical unlocking mechanism.

<img src="./Images/Components/SG90 Servo Motor.jpg" alt="TTL Cable" width="200" height="200">

| SG90 Wire | PIC18F4520 |
| --------- | ---------- |
| Orange    | `RC2/CCP1` |
| Red       | `VDD`      |
| Brown     | `VSS`      |
