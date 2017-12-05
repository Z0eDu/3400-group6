# Milestone 3:

## Code Integration 

The first thing we did was integrate our code from the different labs and milestones: in particular the line following code from Milestone 1 and the wall detection from Milestone 2. We added the relevant parts to a new file. We remapped the pins and created global constant variables that each sensor would need, so that the robot could use the line sensors and wall sensors at the same time. We connected the two inside line sensors directly to the Arduino at the analog pins A2 and A3. The two outside line sensors we connected to a mux, and the output is the analog pin A5 on the Arduino.
```
#define LEFT_OUT A5
#define LEFT_IN A3
#define RIGHT_IN A2
#define RIGHT_OUT A5

#define MUX A5 //input pin setup
#define MUX_sel0 2
#define MUX_sel1 3
#define MUX_sel2 4

#define LEFT_OUT_st 0 // For mux select
#define RIGHT_OUT_st 1
```

## Sensor Integration

As we had to integrate the code from line following and wall detection, we naturally had to integrate the line sensors and IR sensors onto our robot. We preserved our sensor configurations from Milestone 1 and 2 because it was effective and worked well. For line detection, four line sensors are used: the inner 2 sensors are used for line following, and the outer 2 sensors are used to detect intersections. For wall detection, three sensors are used and are positioned on the front, left, and right of the robot respectively. 

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoV2ZNczVQX0NRQWc"  width="400">

We decided to use a 8-to-1 mux to be able to accomodate multiple analog sensors. For this milestone, we were able to get the mux to correctly select between our two outer line sensors, and had the two inner line sensors and three wall sensors connected directly to the Arduino. The analog pin mappings are shown below.

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoWnpIUXJWX2h6SlU"  width="400">

To power the robot, we had all the sensors connected to the Arduino 5V, which was provided by the 9V battery. The two servos were connected to the 5V provided by the battery pack. The two rails share a common ground that is connected to the Arduino ground. 


## Mux Implementation

While integrating the robot, we realized that we need to use more analog inputs than the number of analog input ports.
We always need inputs from the inner two line sensors for line following and the three treasure detecting sensors. There are only 6 analog input ports, so the microphone, the outer two line sensors, and the three wall sensors have to share the last analog input port. Therefore we decided to use a mux to choose which one will use the port.

In order to do so, we outputed three bits of mux select value that corresponds to the analog input that we want to choose.

```cpp
void muxSelect(int state){
   Mux_State = state;
   digitalWrite(MUX_sel0, bitRead(state,0) ? HIGH : LOW);
   digitalWrite(MUX_sel1, bitRead(state,1) ? HIGH : LOW);
   digitalWrite(MUX_sel2, bitRead(state,2) ? HIGH : LOW);
}
```
These are the state values for the analog inputs.

```cpp
#define MIC_st 0
#define LEFT_OUT_st 1
#define RIGHT_OUT_st 2
#define WALL_LEFT_st 3
#define WALL_RIGHT_st 4
#define WALL_FRONT_st 5
```

Whenever we had to use one of the microphone, the outer two line sensors, and the three wall sensors, we included a line muxSelect(the sensor_st) to change the mux select value.

```cpp
int lineStatus() {
  muxSelect(LEFT_OUT_st);
  int left = nsr(LEFT_OUT);
  muxSelect(RIGHT_OUT_st);
  int right = nsr(RIGHT_OUT);

  if (left < LINE_THRESHOLD || right < LINE_THRESHOLD) {
    return LINE_FOLLOW_STOP;
  } else {
    return LINE_FOLLOW_GOOD;
  }
}
```
Since we figured out that the mux increased the normalized line sensor reading, we increased the threshold value for the outer line sensors.

On the robot, the output of the mux was connected to analog pin A5. Both VCC and GND were connected to the arduinos corresponding 5V output and ground, the enable was set to ground as well. The outer line sensors and wall sensors were then rewired to input pins 1-5. The select pins were wired to digital pins 2-4 on the arduino. After testing the values with and without the mux, it became apparent the values were being affected by the mux. We resolved this by simply changing the threshold, but this may have also been caused by it being a used mux. Testing of this mux's functionality before the project will ensue.

## Maze Exploration Algorithm

We used a depth first search to explore the maze. Unlike a standard depth first search, it terminates as soon as the whole maze is explored instead of returning to the starting point. We chose a depth first search because a breadth first search would have been far too slow on a physical robot because it would have to backtrack constantly.

We implemented the algorithm in C, in [`dfs.c`](robot_move/dfs.c). [`sim.c`](sim.c) uses the DFS functions to simulate the robot navigating through a maze, and prints out a pretty ASCII art map. Our robot, in [`robot_move.ino`](robot_move/robot_move.ino) does exactly the same thing as the simulator, except it actually moves the robot! By writing our algorithm in C, which can run on the Arduino, and using the code for the simulator as well as the actual implementation on the robot, we halved the time and effort it would have taken had we built the robot code separately from the algorithm.

While the [`dfs.c`](robot_move/dfs.c) file is rather long (about 300 lines of dense algorithmic C), the main features it provides are:
* `void dfs_mark_rel_obstacle(explore_t* state, int rel_dir)`: marks an obstacle (wall) at the given relative direction from the current robot position.
* `void dfs_mark_treasure(explore_t* state, int treasure)`: marks a treasure with the specified frequency at the current robot position.
* `int dfs_at_intersection(explore_t* state)`: computes the direction the robot should go next. Called when the robot is at an intersection to decide which way to turn. Returns `FORWARDS`, `RIGHT`, `BACKWARDS`, or `LEFT`. Returns -1 when maze exploration is complete (as soon as there are no more reachable squares that have not been visited).
* `void dfs_finalize(explore_t* state)`: marks on the map which squares are reachable and which are not.

The following clip shows the output of our simulation on a sample maze. The robot is represented by the `^`, `>`, `<`, and `v` symbols, which indicate its direction of movement (it moves in the direction of the pointy part of the symbol). Unexplored squares are represented by a `?`, and explored spaces are represented by a `+`. Once the robot is done mapping the maze, unreachable areas are indicated by a `#`. Walls are indicated by either `|` or `*` depending on their orientation. The indicator that shows the robot is done exploring is when the robot stops. This is a valid indication because it otherwise would continue moving in simulation.

<script type="text/javascript" src="https://asciinema.org/a/3V5Omsrkrczqsp4YI2fZJ9qUo.js" id="asciicast-3V5Omsrkrczqsp4YI2fZJ9qUo" async data-size="big"></script>

Rather than change this into a separate code that runs on the Arduino, we imported this C code into our Arduino code to run the same DFS algorithm as shown in the demo above. We made the C code act just like the robot, so we had to change nothing when using it for the robot. See our second tutorial to see how we did this!


## Demo

We ran our algorithm on the robot, and overall it worked very nicely! Here is a video of a full depth first search of a maze with no walls. As with simulation, the indicator that shows the robot is done exploring is when the robot stops. This is a valid indication that is both simple and straightforward to understand.

<iframe src="https://drive.google.com/file/d/0B1QvEdmy23tjajdXOE5tTVB2aFE/preview" width="640" height="480"></iframe>

And here is a video of a DFS of a maze with walls. The robot indicates that it is done by simply stopping:

<iframe src="https://drive.google.com/file/d/0ByCM4xElwbIeN3FodElQUE9VSzQ/preview" width="640" height="480"></iframe>

As you can see, the robot failed to completely traverse the maze because it just missed the last turn and thought it moved two more intersections. This was due to the degradation of the consitency of our line sensors when putting them through the mux (you can even hear Drew say "It's not going to work anyway"). We increased thresholds but still could not get the same consistency. However, we were able to improve the consistency markedly from when we first hooked up the mux. The major issue was that we forgot to set up the digital select pins for output, and we did not set up the analog pin for input on the Arduino to receive the output of our mux. Not doing this caused our mux to only output one of the mux inputs despite switching the select signals at varying rates--this also gave other weird results that we could not explain. 

In addition, in this video is is clear that the robot's 180 degree turn was very bad, so we went back in and changed it from simply two left turns to have a specific function to turn 180 degrees. This allowed us to reduce the amount that the robot goes forward when turning, and reduced the 180 degree turning radius for the robot. Here is the function, followed by a video of how it works. 

```cpp
void rotate180() {
  //drive forward for 300 ms
  drive(10, 10);     
  delay(300);
  drive(0,0);      
  
  //start driving with the left wheel going backward and the right going forward
  int dir = -1;
  int vl = dir * DRIVE_TURN_SPEED;
  int vr = - dir * DRIVE_TURN_SPEED;
  drive(vl, vr);
   
  //drive until the outer right sensor gets to the line, 
  //then until the left inner sensor gets to the line
 
  muxSelect(RIGHT_OUT_st);
  while(nsr(RIGHT_OUT) > 70);
  while(nsr(LEFT_IN) > 40);
   
  //drive forward for 100 ms
  drive(10, 10);
  delay(100);
  
  //turn again in the same way as before
  drive(vl, vr);
  
  muxSelect(RIGHT_OUT_st);
  while(nsr(RIGHT_OUT) > 70);
  while(nsr(LEFT_IN) > 40);
  
  drive(0, 0);
}
```

<iframe src="https://drive.google.com/file/d/0ByCM4xElwbIeelR5dkJXaXBtY1U/preview" width="640" height="480"></iframe>

## Update

We were able to fix our inconcistencies by raising up the line sensors. They were almost touching the ground, which caused them to sense their own shadows and get thrown off from time to time if they hit the ground. Here is the robot successfylly navigating the same maze:

<iframe src="https://drive.google.com/file/d/1J9mfxvdShdzv5FZRr6nGyIn-4kUWKguF/preview" width="640" height="480"></iframe>

Here is the robot successfully navigating a different maze (this was taken later on in the semester).

<iframe src="https://drive.google.com/file/d/1jSv64BG5JyUz64KameMMnVCxGJmFDhmi/preview" width="640" height="480"></iframe>


## Work Distribution

*   Ayomi: Code integration
*   Drew: Robot with walled in maze demo/challenges with maze exploration
*   Emily: Sensor integration and tuning
*   Eric: Mux hardware implementation
*   Jacob: Maze exploration algorithm
*   Joo Yeon: Mux software implementation
