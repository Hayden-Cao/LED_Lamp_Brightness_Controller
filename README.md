# LED_Lamp_Brightness_Controller
LED Lamp Brightness Controller with Bluetooth Control on a PIC18F4321 Microcontroller

**Overview**

This project uses a HM-10 bluetooth module and a rotary encoder to provide inputs that will change the duty cycle of a PWM signal. This PWM signal feeds into the gate of a TIP122 transistor that will connect a 12V LED bulb to GND whenever the PWM signal is high.
A PIC18F4321 microcontroller will be used to generate the PWM signal and to process the input data from the HM-10 Module through UART and the rotary encoder.

****

Circuit Schematic:

<img width="830" alt="image" src="https://github.com/pileofhay/LED_Lamp_Brightness_Controller/assets/130268332/0365521b-c376-42d5-b13d-398694968bbb">

