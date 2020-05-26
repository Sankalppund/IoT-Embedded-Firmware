Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements in the "Default" configuration of the profiler to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See [Shared document](https://docs.google.com/document/d/1Ro9G2Nsr_ZXDhBYJ6YyF9CPivb--6UjhHRmVhDGySag/edit?usp=sharing) for instructions.* 

1. What is the average current per period?
   Answer: 95.94 microamps.
   <br>Screenshot:  
   ![Avg_current_per_period](https://github.com/CU-ECEN-5823/assignment-4-Sankalppund/blob/master/Screenshots/Avg_current_per_period.JPG)  

2. What is the average current when the Si7021 is Load Power Management OFF?
   Answer: 1.21 microamps.
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/assignment-4-Sankalppund/blob/master/Screenshots/Avg_current_LPM_Off.JPG)  

3. What is the average current when the Si7021 is Load Power Management ON?
   Answer: 1.87 milliamps.
   <br>Screenshot:  
   ![Avg_current_LPM_on](https://github.com/CU-ECEN-5823/assignment-4-Sankalppund/blob/master/Screenshots/Avg_current_LPM_on.JPG)  

4. How long is the Si7021 Load Power Management ON for 1 temperature reading?
   Answer: 149.60 milliseconds.
   <br>Screenshot:  
   ![duration_lpm_on](https://github.com/CU-ECEN-5823/assignment-4-Sankalppund/blob/master/Screenshots/duration_lpm_on.JPG)  

5. What is the total operating time of your design for assignment 4 in hours assuming a 1000mAh supply?

  Operating Time =(1000mAh/0.09594) is approx 10423 hrs.

6. How has the power consumption performance of your design changed since the previous assignment?

Power consumption in assignment 3 was 457.79 microwatt & power consumption in assignment 4 is 316.86 microwatt. So power consumption performance
has been improved by 30.7%.

7. Describe how you have tested your code to ensure you are sleeping in EM1 mode during I2C transfers.

By using SLEEP_SleepBlockBegin(sleepEM2) & SLEEP_SleepBlockEnd(sleepEM2) which ensures to block all energy modes from EM2 and higher and works in EM1 mode
from LPM On->I2C_Write->I2C_Read->Temp_Cal->LPM Off.

