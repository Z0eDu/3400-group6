# Milestone 4:

## Base Station

For this assignment, we had to modify the base station to display walls and treasures, and to indicate when the robot is done. For the walls, we modified the FPGA code to display white bars along the sides of the squares. We represented treasures as smaller squares displayed in the center of each grid square, and used different colors to distinguish the types of treasures. For the done signal, we wanted a way to signal that the robot was done, without removing the solved maze from the screen. The simplest way to do that was to change the wall color from white to purple when the robot finished. Once the robot finished, we also enabled our sound module from milestone 3, which caused it to play a C scale.

Here is a picture of the base station displaying every possible type of square, with all the possible treasures, and all the possible wall arrangements:

<img src="https://docs.google.com/uc?id=1Z-kXtSD_p57yaE_aZIT3XiW-nkVjkkhb"  width="400">

For squares, we used the following color scheme:
* Black: unvisited
* Green: visited
* Blue: current position
* Red: unreachable

For treasures, we used the following color scheme:
* 7 kHz: orange
* 12 kHz: white
* 17 kHz: purple

For walls, we used the following color scheme:
* White: robot still exploring the maze
* Purple: robot done, maze final

## Demo

Here is a video of the final system:

<iframe src="https://drive.google.com/file/d/1jSv64BG5JyUz64KameMMnVCxGJmFDhmi/preview" width="640" height="480"></iframe>


## Work Distribution

*   Ayomi:
*   Drew:
*   Emily:  
*   Eric: Mux hardware implementation
*   Jacob: Maze exploration algorithm
*   Joo Yeon: Mux software implementation
