

## **Bubble display project**




Figure 1. Project structure (Presentation Process, 2013)

# Requirements

The aim of this project is to create a reaction time tester using a LED, push button and a Teensy LC interfaced with a 4-digit LED bubble display. The display must show time in hundredths of a second and must have less than a ten-millisecond deviation over a ten second timespan. Other conditions that the final product must adhere to are, denouncing of push buttons, use of external interrupts, use of N-Fets and no delay/blocking functions.

# Design

The components needed to complete this project are the following.

 - Current limiting resistors
   
   Gate-source resistors
   
   Normally open push buttons
   
   Light emitting diode “LED”
   
   Teensy LC
   
   N channel mosfet’s below is

a schematic of the reaction timer which was used to wire the final design seen in figure 11.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image004.jpg)<![endif]> 

Figure 2. Wiring schematic for reaction timer

## Current limiting resistors

A current limiting series resistor needs to be connected to the output pin with the LED. The current flowing in an LED is an exponential function of voltage across the LED. If a series resistor is not added the current can overload the LED and blow it. The following calculation is done to find the correct series resistor for a LED colour and type.

[<![if !vml]>![Green LED datasheet ](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image006.jpg)<![endif]>](https://user-images.githubusercontent.com/53545740/63909001-4dfe3300-ca75-11e9-9df3-d3eb50594236.JPG)

Figure 3. Green LED datasheet. From (Vishay, 2019)

Vs - Supply Voltage Vf - Forward voltage of the LED. This is the voltage required to turn on the LED.  
If - Forward Current of the LED. This is the current required by the LED for operation.

Using the following equation, a suitable series resistor value can be found. R = (Vs - Vf) / If Substituting the values in for the green LED R = (3.3 - 2.4) / 20mA = 45 ohms

Because we did not have any 45-ohm resistors available the closest value resistor was used which was 130 ohms

## Seven segment bubble display

The seven segment LED bubble display used for this project is the QDSP6064, which is a four-digit display. Seven segment means that there are seven individual segments that can be lit up to display a particular number plus a separate segment for the decimal point as seen in figure 4. Each segment has its own LED which must have a current limiting resistor is series with it. The QDSP6064 display is a common cathode configuration which means the segments have a common ground and a 3.3V signal from the teensy LC to the anode pin of each LED will cause them to light up. For multiple digit displays there are always 8 anode pins and the number of cathode pins is determined by the number of digits the display can represent. To display more than one digit on the display, the LED’s are pulsed at a high frequency. Each common cathode is linked to a digit. Only one cathode can be active at a time otherwise the display will duplicate the same number across all digits. Recommended operation is to cycle through the digits activating the cathode to ground and triggering the required segments to create the desired number.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image008.jpg)<![endif]>

Figure 4. Common cathode 7 segment configuration (Robomart, 2019)

To calculate the current limiting resistors for the display the forward voltage and reverse current need to be obtained from the data sheet as seen in figure 5. The current limiting resistance can be found using the same equation as above. (3.3V – 1.6V) / 10mA = 170 ohms.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image010.jpg)<![endif]>

Figure 5. Data sheet for QDSP-6064 LED display (Hewlett Packard Components, n.d.)

## N channel Mosfet’s

The teensy LC pins can only handle 5 - 20mA through each pin. The display used for this project has a peak current of 110mA which could damage the teensy. To ensure the current from the display is sufficiently drained n channel mosfet’s are used to drain the current from the common cathode to ground as seen in figure 2. The gate is connected to the teensy LC, the source to ground and drain to the cathode pin. To connect the cathode to ground and drain the current, the pin connected to the gate needs to be set high. This will complete the path from the cathode directly to ground and no current will need to be drained through the pins of the controller.

## Gate – source resistors

As seen in figure 2 a ten kiloohm resistor is connected from the gate of the moseft to ground. This is to keep the pin at zero volts when the output pin is set to low. When a pin is in the low state is has high impedance therefore, it is susceptible to surrounding noise. This noise can cause the moseft to switch by itself which is undesirable. To stop this the ten kiloohm resistor drains all noise to ground and creates a steady zero volts when the pin is low keeping the gate closed.

## Button configuration

The microcontrollers default pin mode is the input state. When the controller is in the input state it has high impedance, which means the circuit allows a relatively small amount of current through for the applied voltage. When a button is connected from an input pin to ground, a series current limiting resistor does not need to be added because of the high impedance limiting the current. However, a pullup resistor does need to be added to the line as seen in figure 1. Without a pullup resistor the input line acts as an antenna which picks up the surrounding interference. The interference will cause the pin to float and appear noisy and can cause undesired readings on the microcontroller. To stop the floating a pullup resistor typically between 1K-10K ohms is connected from the input line to the 3.3V pin on the controller (see figure 3). This causes the input line to have a constant 3.3V along it while the button is not pushed which is read as logic 1 by the microcontroller. When the button is pushed the current is sinking through the connection to ground and the voltage drops to 0V which is read as a logic 0 by the microcontroller.

[<![if !vml]>![Pull Up resistor](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image012.jpg)<![endif]>](https://user-images.githubusercontent.com/53545740/64058032-13bd9e80-cbf9-11e9-9c66-45fe7edba810.JPG)

Figure 6. Pull up resistor configuration and transmission signal.

When using mechanical buttons as an input for a microcontroller a process called de-bouncing needs to be performed. When a mechanical button is pushed there is always a 'bounce'. This is caused from the spring inside the button taking time to settle. The typical reading for a mechanical button being pushed can be seen in figure 4. We cannot remove the bouncing effect however, because we are using a microcontroller, we can add additional code to the program to ignore the bounce. This is done by creating an 'ignore time' which tells the program to ignore any input changes for a set time after the initial change. The ignore time is different for each button however, for the button used in this project an ignore time of 80-120 milliseconds was used.

[<![if !vml]>![Button Bounce](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image014.jpg)<![endif]>](https://user-images.githubusercontent.com/53545740/64058038-246e1480-cbf9-11e9-8b42-ee225b6b79e3.JPG)

Figure 7. Button de-bounce waveform. From (Electronics tutorials,2018)

# Implementation

## Pseudo code

The following is a brief pseudo code used to base the project off. A library called SevSeg Written by Dean Reading, 2012 was used to control the process of lighting up each segment to generate a digit. The code that was created for this project controls the function of the reaction timer. The SevSeg library is then imported and the necessary data is given for the display to light up accordingly.

<![if !supportLists]>· <![endif]>Add libraries

<![if !supportLists]>· <![endif]>Set variables

<![if !supportLists]>· <![endif]>Set pin modes

<![if !supportLists]>· <![endif]>Configure necessary variables for the library

<![if !supportLists]>· <![endif]>Generate a random time before the LED turns on

<![if !supportLists]>· <![endif]>When the LED turns on start timer and show on display

<![if !supportLists]>· <![endif]>If the reset button is pressed. Reset the game and generate new random start time

<![if !supportLists]>· <![endif]>If the pause button is pressed. Pause time and display the time the button was pressed

<![if !supportLists]>· <![endif]>Wait until the reset button has been pushed to start new game.

## Final code

Figure 8 below shows the global variables, libraries and setup for the program. Most of the setup is from the template provided by the library so the display works correctly.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image016.jpg)<![endif]>

Figure 8. Part 1 of code. Creating variables and including libraries

Part two is the main part of the program controlling the reaction timer. The first part of the loop starts with a random number being generated controlling the time before the LED turns on. When the LED turns on the timer start counting up while the pause button is not pressed. Every 8155 microseconds the display value is increased by one. This accounts for one hundredth of a second. The reason the time isn’t 10000 microseconds is because the code doesn’t run instantaneously therefore some correction needs to be done to account for other tasks being performed.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image018.jpg)<![endif]>

Figure 9. Part 2 of code. Create string to show on display and interrupts for pause and reset button.

The display value is turned into a string where it is controlled by the SevSeg library to produce the digits on the LED display. The %04d adds leading zeros to the string filling out the display. If the reset button is pressed the display resets, LED is turned off and the random start time is changed. An interrupt service routine is used for the pause and reset buttons. They are debounced by reading the first falling edge then ignoring any other falling edges for a debounce time.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image019.png)<![endif]>

Figure 10. Changing the way, the common cathode sinks current for mosfet design.

Because the design of this project uses N-channel mosfet’s to sink the current from the common cathode some small changes needed to be added to the library to allow for the used of the mosfet. When the digit is on (see figure 10) the gate of the mosfet needs to be active connecting the path to ground. When the digit is off the gate can be closed and the path to ground also disconnected.

## Wiring

Figure 11 shows the final layout for the reaction timer. The reset button is on the top left and pause button on the bottom right.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image021.jpg)<![endif]>

Figure 11. Wiring of the final product

# Verification

Figure 12 below is the addition code used to test the accuracy of the timer. The brief states that the timer must be accurate to 10ms over a 10 second time span. The code below freezes the timer when it reaches a set time. This time can be compared with an oscilloscope reading of the LED turning from on to off.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image023.jpg)<![endif]>

Figure 12. Code used for testing the accuracy of the timer

Figure 13 shows the oscilloscope reading for 10 second timespan. The measurement shows the actual time as being 9.920s which is within the acceptable 10ms threshold.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image025.jpg)<![endif]>

Figure 13. Oscilloscope reading of LED to be compared with timer

# Maintenance

## Improvements

Many improvements could be made to this project if it was to be redone. The accuracy of the timer is the first thing that could be improved. Because typical reaction times are within a few tenths of a second a 10ms error could cause inaccurate readings. Some features could also be added with multiple different LEDs and corresponding buttons being used to create a different situation for the human reaction.

## Conclusion

The aim of this project was to create a reaction time tester using a LED, push button and a Teensy LC interfaced with a 4-digit LED bubble display. The display had to show time in hundredths of a second and have less than a ten-millisecond deviation over a ten second timespan. Other conditions that the final product needed to adhere to were, debouncing of push buttons, use of external interrupts, use of N-Fets and no delay/blocking functions. The final product met all the specifications and constraints in the brief. Some future improvements and additions could be added in future however, the current setup illustrates a working prototype within the brief that has the capability of improvements.

# References

Electronic Tutorials. (2018, April 25). Input Interfacing Circuits Connect to the Real World. Retrieved from [https://www.electronics-tutorials.ws/io/input-interfacing-circuits.html](https://www.electronics-tutorials.ws/io/input-interfacing-circuits.html)

Hewlett Packard Components. (n.d.). _QDSP-6064 4 digit display_. Retrieved from https://cdn.sparkfun.com/datasheets/Components/LED/BB_QDSP_DS.pdf

Presentation Process. (2013, October 25). _Waterfall model_. Retrieved from Youtube: https://www.youtube.com/watch?v=_ZKvvaZEFKE

Robomart. (2019). _Robomart_. Retrieved from 7 Segment LED Display Common Cathode for Arduino/Raspberry-Pi/Robotics: https://www.robomart.com/7-segment-led-display-common-cathode

Vishay. (2019, January 1). High Efficiency LED. Retrieved from [https://www.vishay.com/docs/83009/tlhg4900.pdf](https://www.vishay.com/docs/83009/tlhg4900.pdf)

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTU2MzQzMDAwNCw5MTIxNDUyMTBdfQ==
-->