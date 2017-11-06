# Milestone 3:

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
#define LEFT_OUT_st 0
#define RIGHT_OUT_st 1
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

## Maze Exploration Algorithm

We used a depth first search to explore the maze. Unlike a standard depth first search, it terminates as soon as the whole maze is explored instead of returning to the starting point. We chose a depth first search because a breadth first search would have been far too slow on a physical robot because it would have to backtrack constantly.

We implemented the algorithm in C, in [`dfs.c`](robot_move/dfs.c). [`sim.c`](sim.c) uses the DFS functions to simulate the robot navigating through a maze, and prints out a pretty ASCII art map. Our robot, in [`robot_move.ino`](robot_move/robot_move.ino) does exactly the same thing as the simulator, except it actually moves the robot! By writing our algorithm in C, which can run on the Arduino, and using the code for the simulator as well as the actual implementation on the robot, we halved the time and effort it would have taken had we built the robot code separately from the algorithm.

While the [`dfs.c`](robot_move/dfs.c) file is rather long (about 300 lines of dense algorithmic C), the main features it provides are:
* `void dfs_mark_rel_obstacle(explore_t* state, int rel_dir)`: marks an obstacle (wall) at the given relative direction from the current robot position.
* `void dfs_mark_treasure(explore_t* state, int treasure)`: marks a treasure with the specified frequency at the current robot position.
* `int dfs_at_intersection(explore_t* state)`: computes the direction the robot should go next. Called when the robot is at an intersection to decide which way to turn. Returns `FORWARDS`, `RIGHT`, `BACKWARDS`, or `LEFT`. Returns -1 when maze exploration is complete (as soon as there are no more reachable squares that have not been visited).
* `void dfs_finalize(explore_t* state)`: marks on the map which squares are reachable and which are not.

The following clip shows the output of our simulation on a sample maze. The robot is represented by the `^`, `>`, `<`, and `v` symbols, which indicate its direction of movement (it moves in the direction of the pointy part of the symbol). Unexplored squares are represented by a `?`, and explored spaces are represented by a `+`. Once the robot is done mapping the maze, unreachable areas are indicated by a `#`. Walls are indicated by either `|` or `*` depending on their orientation.

<script type="text/javascript" src="https://asciinema.org/a/3V5Omsrkrczqsp4YI2fZJ9qUo.js" id="asciicast-3V5Omsrkrczqsp4YI2fZJ9qUo" async data-size="big"></script>


## Demo

We ran our algorithm on the robot, and overall it worked very nicely! Here is a video of a full depth first search of a maze with no walls:

JENNIE PUT YOUR VIDEO HERE

And here is a video of a DFS of a maze with walls:

<iframe src="https://drive.google.com/file/d/0ByCM4xElwbIeN3FodElQUE9VSzQ/preview" width="640" height="480"></iframe>

As you can see, the robot failed to completely traverse the maze because it simply missed the last turn and thought it moved two more intersections. This was due to the degradation of the accuracy of our line sensors when putting them through the mux. We increased thresholds but still could not get the same consistency. However, we were able to improve the consistency markedly from when we first hooked up the mux. The major issue was that we forgot to set up the digital select pins for output, and we did not select the analog pin for input on the Arduino to receive the output of our mux. Not doing this caused our mux to only output one of the inputs despite switching the select signals at varying rates and give other weird results. 

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

## Work Distribution

*   Ayomi:
*   Drew: Robot with walled in maze demo/challenges with maze exploration
*   Emily:  
*   Eric: Mux hardware implementation
*   Jacob: Maze exploration algorithm
*   Joo Yeon: Mux software implementation
