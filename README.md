# LED_Lamp_Brightness_Controller
LED Lamp Brightness Controller with Bluetooth Control on a PIC18F4321 Microcontroller

**Overview**

This project uses a HM-10 bluetooth module and a rotary encoder to provide inputs that will change the duty cycle of a PWM signal. This PWM signal feeds into the gate of a TIP122 transistor that will connect a 12V LED bulb to GND whenever the PWM signal is high.
A PIC18F4321 microcontroller will be used to generate the PWM signal and to process the input data from the HM-10 Module through UART and the rotary encoder.

****

Circuit Schematic:

![image](https://github.com/pileofhay/LED_Lamp_Brightness_Controller/assets/130268332/644c8cd1-9ff5-4e48-b362-1df7766c0853)



****

Design Flaws:

One design flaw is that I am relying on an external app called Dabble to provide the terminal that communicates with the HM-10 bluetooth module. The user has to pair their device with the HM-10 each time the app is opened making the user experience clunky and slow. Additionally, the terminal is not a user friendly user interface when compared to user interfaces like a slider. 

Another flaw is that during the PWM ranges of about 80% duty cycle, I observed that the led lamp had the same brightness as the lamp when driven with a 100% duty cycle PWM signal. This occurs because the LED lamp was drawing more current than before and was drawing about 1.2A when it normally draws 0.7A at full brightness. The LED seems to be drawing more current to reach max brightness to make up for the small difference in voltage.

****

Planned Improvements:

Adding an LCD display to showcase the percent of brightness

Limiting the range of the PWM duty cycle to prevent entering the region of increased current draw.

External peripherals such as a clock or temperature sensor can be added for extra information to be displayed on the LCD

****

Prototype Demonstration:

To View Click on the Image to open the Youtube Link

[![IMAGE ALT TEXT](http://img.youtube.com/vi/NrTSRcypTo4/0.jpg)](http://www.youtube.com/watch?v=NrTSRcypTo4 "LED Lamp Brightness Controller Prototype Demonstration")
