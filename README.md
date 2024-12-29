# RFID-Electronic-Lock

### 使用元件

<details>
  <summary>RDM6300</summary>

  ![RDM6300 PINOUT](./Diagrams/RFID%20RDM6300%20Pinout%20Diagram.png){width=100}![RDM6300 Coil](./Diagrams/RDM6300%20Copper%20Coil.jpg){width=100}

  |  RDM6300 |  Coil  | PIC18F4520 |
  |----------- |----------- |------------|
  | ``ANT1``   | ``Black Line``   | X   |
  | ``ANT2``   | ``Red Line``  | X   |
  | ``Vcc(down)``   | X   | ``Vdd``|
  | ``GND(down)``   | X   | ``Vss``|
  | ``TX``   | X   | ``RC7/RX``  |

</details>
<details>
  <summary>Button</summary>

|  Button | PIC18F4520 |
  |----------- |------------|
  | ``One side``  | ``RB0/INT0`` → ``Resistor`` → ``Vdd``<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;→ ``One side``|
  | ``Another side``  | ``Vss``|

</details>
<details>
  <summary>LED</summary>

  |  LED1 | PIC18F4520 |
  |----------- |------------|
  | ``Long Lag``  | ``RA1/AN1`` → ``Resistor`` → ``Long Lag``|
  | ``Short Lag``  | ``Vss``|

  |  LED2 | PIC18F4520 |
  |----------- |------------|
  | ``Long Lag``  | ``RA2/AN2`` → ``Resistor`` → ``Long Lag``|
  | ``Short Lag``  | ``Vss``|

</details>
<details>
  <summary>Buzzer</summary>

|  Buzzer | PIC18F4520 |
  |----------- |------------|
  | ``Vcc``  | ``Vdd``|
  | ``GND``  | ``Vss``|
  | ``I/O``  | ``RC2/CCP1``|

</details>
<details>
  <summary>Variable Resistor</summary>

|  Variable Resistor | PIC18F4520 |
  |----------- |------------|
  | ``One side``  | ``Vdd``|
  | ``Center``  | ``RA0/AN0``|
  | ``Another side``  | ``Vss``|
</details>

