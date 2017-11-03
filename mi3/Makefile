sim: dfs.o sim.c
	gcc sim.c dfs.o -Wall -Werror -Wswitch-enum -o sim -DALGO_IN_SIMULATION
dfs.o: robot_move/dfs.c robot_move/dfs.h
	gcc -c robot_move/dfs.c -Wall -Werror -Wswitch-enum -o dfs.o -DALGO_IN_SIMULATION
format: robot_move/dfs.c robot_move/dfs.h sim.c
	clang-format -i --style=google robot_move/dfs.c robot_move/dfs.h sim.c
clean:
	rm -f *.o sim
