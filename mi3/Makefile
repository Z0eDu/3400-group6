sim: dfs.o sim.c
	gcc sim.c dfs.o -Wall -Werror -Wswitch-enum -o sim
dfs.o: robot_move/dfs.c robot_move/dfs.h
	gcc -c robot_move/dfs.c -Wall -Werror -Wswitch-enum -o dfs.o
format: robot_move/dfs.c robot_move/dfs.h sim.c
	clang-format -i --style=google robot_move/dfs.c robot_move/dfs.h sim.c
clean:
	rm -f *.o sim
