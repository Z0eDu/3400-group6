# Lab 4: Radio Communication and Map Drawing

### Objectives
* Extend work from Lab 3 to use the FPGA draw a full maze on VGA monitor
* Update VGA monitor with information received from radio
* Get robot and video controller to talk to each other to simulate actual maze information 


#### Grading
2 points: Sending information wirelessly between Arduino’s  
2 points: Sending the entire maze wirelessly  
3 points: Updating the maze array, dependent only on the updated robot information  
1 point: Displaying a full 4-by-5 grid array on the screen  
3 points: Communicating maze information from the Arduino to the FPGA  
2 points: Display the robot location on the screen  
2 points: Distinguish what sites have been visited and which haven’t on the screen  

## Teams
**Team 1 (Radio)**: Ayomi, Emily, Eric

**Team 2 (FPGA)**: Drew, Jacob, Joo Yeon


## Radio

### Setup 

### Sending Entire Maze

### Sending New Information Only

### Sending Robot Position

Instead of sending (x,y) coordinates to determine the position of the robot in the 4 x 5 maze, we decided that we would send integer positions to denote robot position. Each square of the maze will be numbered as follows:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0Ewod3hvaVg2UWw5X1E" width="300">

<img src="https://docs.google.com/uc?id=" width="350">

Each message that Arduino A transmits to Arduino B is a 8-bit unsigned char that denotes robot position, state, and a clock bit. The 8-bit message is broken down as follows. The lower two least-significant bits differentiates between three states: current robot position, explored, and unexplored -- with decimal values 0, 1, and 2 respectively. The third bit is a clock line that we lower as the Arduino is writing all the bits to the FPGA, and the brought high once all the bits are written. The last most-significant bits indicate the position of the grid, with decimal values 0 to 19 as shown in our grid above. 

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwocnlMbHItclEwR1k" width="450">


## FPGA


Sending robot position

The FPGA team will be working to draw a grid that shows the current position of the robot. To do this, they will need you to relay a coordinate position to them over the radio.

Implement a communication method to send the current position of the robot from one Arduino to the other. Design for a 4 x 5 maze and decide on the appropriate coordinate system to use.

Your goal is to send the current position of the robot from Arduino A to Arduino B. Then Arduino B will transfer that information to the FPGA so that the current position of the robot appears on the VGA monitor.




## Work Distribution

*   Ayomi: 
*   Drew: 
*   Emily: 
*   Eric: 
*   Jacob: 
*   Joo Yeon: 
