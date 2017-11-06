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

DREW PUT YOUR VIDEO HERE

## Work Distribution

*   Ayomi:
*   Drew:
*   Emily:  
*   Eric: Mux hardware implementation
*   Jacob: Maze exploration algorithm
*   Joo Yeon: Mux software implementation
