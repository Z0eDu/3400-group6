# C Programming: Arduinos & Computers

One main component of a working 3400 robot is the maze solving algorithm. In order to design the algorithm, it helps to be able to simulate it on a computer, without a robot present. This makes it significantly easier to isolate and debug problems. There are 2 main ways to do this:

1. Write the simulator and algorithm in a language like Python or Matlab. Once it works, port it to C/C++ to run on the Arduino.
2. Write the algorithm only once in C/C++, and run it both on a computer, and on the Arduino.

While the first approach may sound easier, it may not be ideal. The first approach requires implementing the algorithm twice: once in a language like Python for the simulation, and once in C for the Arduino. Furthermore, even if the simulation works, the C port of it _may contain bugs_, which you can only detect on the Arduino, making it far harder to debug! While method 1 may be initially be easier, method 2 will ultimately end up requiring less work.

To use method 2, you need to compile C code for you Arduino and your computer. To do so, you need to separate the computer and Arduino specific parts from the common algorithmic part. The key to doing that is to split the code across multiple files.

Our [Depth First Search (DFS) library](../mi4) is a good example of how to do this:

* The Arduino project (in a folder with the same name), is [`robot_move/robot_move.ino`](../mi4/robot_move/robot_move.ino). The main Arduino project can use the functions declared in [`dfs.h`](../mi4/robot_move/dfs.h) by including an `#include "dfs.h"` directive. The Arduino IDE will then build an object file for `dfs.c`, and link your sketch against it.
* The common code is in [`dfs.c`](../mi4/robot_move/dfs.c). All functions which you want to be accessible from other files must be declared in the header file, [`dfs.h`](../mi4/robot_move/dfs.h).
* The simulation code, which _uses_ the functions in [`dfs.c`](../mi4/robot_move/dfs.c) is [`sim.c`](../mi4/sim.c). The [`Makefile`](../mi4/Makefile) builds the `dfs.o` object file from the [`dfs.c`](../mi4/robot_move/dfs.c) source file, and then compiles and links `sim.c` against it to produce an executable executable called `sim`. You can then easily build the simulation code by invoking `make` in the [`mi4` directory](../mi4).
