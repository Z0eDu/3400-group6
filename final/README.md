# Final Competition

<!-- Good introductions and problem motivations  
Clarity and thoroughness of documentation  
Intuitive and nice layouts  
Use of the right tools for analysis and unit testing  
Presence of schematics when applicable  
Presence of simulation results when applicable  
Presence and evaluation of experimental results  
Use of graphs and tables to display results, as well as proper axis and unit labeling  
Presence of design flaws  
Comparative discussion of simulation and experimental results  
Presence of conclusions   -->

For ECE 3400, we built a maze solving robot. The robot was controlled by an Arduino Uno, which wirelessly communicated with an Arduino Uno connected to a DE0 Nano FPGA, which drove a VGA monitor displaying a map of the maze. The robot also used a microphone to start on a 660 Hz tone, and used infrared photodiodes to detect and classify treasures: infrared beacons flashing at 7, 12, and 17 kHz. It used infrared distance sensors to detect walls, and infrared line sensors to follow lines and keep track of its position as it traversed the grid. We hope that the documentation provided on our website will provide a comprehensive overview of the system we built as well as a compilation of what we learned throughout this semester.

## System

### Robot
<img src="https://docs.google.com/uc?id=1u3O5pn8g4v8mFneFVIyXRM5UH_Pw-CvC"  width="650">

The robot's main processing unit is the Arduino Uno. The Arduino Uno interfaces with line sensors, treasure sensors, distance sensors, and a microphone. Because the Arduino only has 5 analog input pins, we used an analog multiplexer to route far more than 5 signals into the analog input pins. As we were testing, we noticed that line sensors placed on the mux would occasionally get delayed readings. As a result, we moved a few of the line sensor pins to directly connect to the Arduino to ensure that our robot could detect intersections consistently. The 2 servo motors where driven by PWM pins on the Arduino. The Arduino was powered off of a 9V battery, and the servos were powered off a 5 volt rechargeable power bank. Finally, the Arduino used a Nordic RF24 radio to communicate with the base station Arduino.

### Base Station
<img src="https://docs.google.com/uc?id=19J9ya6bEyLajjWw7duF759bAyg2q0A3w"  width="650">  

The base station consisted of 2 main components: another Arduino Uno, and a DE0 Nano FPGA. The FPGA is necessary because the Arduino can not sustain the required bandwidth to operate a VGA display. The Arduino was rather simple: it received the radio signals, encoded as explained in milestone 4, and sent them to the FPGA. The FPGA maintained a dual port 32 element RAM, in which it stored a 4x5 grid of values, representing the squares. For each square, it stored the state, the treasure status, and the surrounding walls. We were able to extend the implementation of our monitor display to show any arbitrary image. To do this, we wrote a Python script that allows us to resize an input image, and output each pixel's value in hex that is stored in a Verilog single port ROM.

In addition, the FPGA code contained a Direct Digital Synthesis module (DDS), which played the done signal once the robot had finished. The FPGA outputed a digital 8 bit signal to an R2R DAC, which was connected to speakers.

## Design Methodology
We worked on each piece of the robot independently, and as evidenced by our milestones, got all the pieces working nicely. We tested each piece individually, then added it to the final robot. In putting all the pieces together, the first thing we did was to integrate the code for line and wall detection in Milestone 3. Once we confirmed that our sensors were reading correct inputs, we were able to seamlessly integrate our Depth First Search simulation code onto our robot. This was easily done as we had created a Depth First Search library that allows our algorithm to be run locally on both the computer and on the Arduino. The last piece of our robot integration was the treasures. We created three treasure detection "breakout boards" and tested that the ISR in our treasure code was not breaking the rest of the core robot functionality. With the exception of the microphone, our robot was fully integrated and working by Milestone 4.

Towards the end, we tested the entire robot as a system, and it performed as expected. Our milestone 4 performed flawlessly.

## Final Design
Our final design and robot worked very nicely, at least the day before competition:

<iframe src="https://drive.google.com/file/d/1Rs3jYRBsrQxttRoGFLB5iE3Q41FV31Ir/preview" width="640" height="480"></iframe>

## Conclusion

At competition, we had multiple problems:
1.  Our transmission system failed. The robot was unable to send transmissions to the base station.
2.  Wall, treasure, and start tone detection became very unreliable. We removed the treasure circuits to reduce problems, but that did not help.

During our rounds, the robot failed to start, and after it did, often ran into walls. Furthermore, it had just worked perfectly the day before. While we are not sure what caused this, we believe the most likely cause was some accidental short circuit shortly before competition which fried some pins on the Arduino.

The main reason such a thing is likely is because of the number of wires routed throughout our robot. Had we used a protoboard, and even a PCB, we could have made it significantly more reliable. Given an opportunity for future work and improvements, we would have also taken advantage of the resources available to us such as the Skunkworks lab and designed custom parts to make our robot more mechanically robust. 

Finally, we would like to thank Kirstin and the TAs for always being helpful and supportive. Thank you for a great semester! 
