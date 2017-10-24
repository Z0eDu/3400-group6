sim: dfs.o sim.c
	gcc sim.c dfs.o -Wall -Werror -Wswitch-enum -o sim
dfs.o: dfs.c dfs.h
	gcc -c dfs.c -Wall -Werror -Wswitch-enum -o dfs.o
format: dfs.c
	clang-format -i --style=google dfs.c dfs.h sim.c
clean:
	rm -f *.o sim
