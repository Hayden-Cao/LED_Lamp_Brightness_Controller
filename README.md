# LED_Lamp_Brightness_Controller
LED Lamp Brightness Controller with Bluetooth Control on a PIC18F4321 Microcontroller

Prototype Demonstration:

To View click on the Image to open the Youtube Link

[![IMAGE ALT TEXT](http://img.youtube.com/vi/xRQ9Ijzu4HU/0.jpg)](http://www.youtube.com/watch?v=xRQ9Ijzu4HU)

**Overview**

This project uses a HM-10 bluetooth module and a rotary encoder to provide inputs that will change the duty cycle of a PWM signal. This PWM signal feeds into the gate of a TIP122 transistor that will connect a 12V LED bulb to GND whenever the PWM signal is high.
A PIC18F4321 microcontroller will be used to generate the PWM signal and to process the input data from the HM-10 Module through UART and the rotary encoder.

An IOS app called Dabble is used to provide the serial terminal to communicate with the HM-10 module. When a number between 0-249 is sent through the Dabble terminal, the PIC18 will receive a UART communication where the data is parsed and used to control the PWM duty cycle.

****

Circuit Schematic:

![image](https://github.com/pileofhay/LED_Lamp_Brightness_Controller/assets/130268332/644c8cd1-9ff5-4e48-b362-1df7766c0853)


****

Potential Improvements:

Adding an LCD display to showcase the percent of brightness

External peripherals such as a clock or temperature sensor can be added for extra information to be displayed on the LCD

****

