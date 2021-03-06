Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does a single LED draw when the output drive is set to "Strong" with the original code?**

The current drawn by single LED when the output drive is set to "Strong" is "0.49 mA"

**2. After commenting out the standard output drive and uncommenting "Weak" drive, how much current does a single LED draw?**

The current drawn by single LED when the output drive is set to "Weak" is "0.48 mA"

**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, 
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

There is NO meaningful difference in current drawn in both "Strong" and "Weak" drive. The reason being both LEDs are connected to pullup resistors of 3 kilo Ohms and potential difference across both LEDs are same.
Thus current drawn is same in both mode. 
Also as per observation from que 1 and 2 the current drawn by LEDs is almost 0.5mA which is less than current provided in strong drive which is 10mA and current in weak drive which is 1mA.
Thats the reason of having no meaningful difference in current drawn in both "Strong" and "Weak" drive.


**4. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with only LED1 turning on in the main loop?**

With "Weak" drive LEDs, "Average Current = 4.68 mA" and "Energy = 7.73 microWattHour"

**5. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with both LED1 and LED0 turning on in the main loop?**

With "Weak" drive LEDs and with both LED1 and LED0 turning on in main loop, "Average Current = 4.84mA" and "Average Energy = 8.18 microWattHour"

