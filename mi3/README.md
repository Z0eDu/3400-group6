# Milestone 3: 

## Mux Implementation

```cpp
void muxSelect(int state){
   Mux_State = state;
   digitalWrite(MUX_sel0, bitRead(state,0) ? HIGH : LOW);
   digitalWrite(MUX_sel1, bitRead(state,1) ? HIGH : LOW);
   digitalWrite(MUX_sel2, bitRead(state,2) ? HIGH : LOW);
}
```

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
<script type="text/javascript" src="https://asciinema.org/a/lCf5EbSOk9DVuMELJmQMCry2a.js" id="asciicast-lCf5EbSOk9DVuMELJmQMCry2a" async></script>


