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


## Maze Exploration Algorithm
<script type="text/javascript" src="https://asciinema.org/a/3V5Omsrkrczqsp4YI2fZJ9qUo.js" id="asciicast-3V5Omsrkrczqsp4YI2fZJ9qUo" async data-size="big"></script>


## Work Distribution

*   Ayomi:
*   Drew:
*   Emily:  
*   Eric: Mux hardware implementation
*   Jacob: Maze exploration algorithm
*   Joo Yeon: Mux software implementation
