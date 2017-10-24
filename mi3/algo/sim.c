#include <stdio.h>
#include <unistd.h>
#include "dfs.h"

void clear() { printf("\033[H\033[J"); }

void delay_and_clear() {
  sleep(1);
  clear();
}

int main(int argc, char** argv) {
  explore_t state;
  dfs_init(&state, 0, 0, NORTH);
  dfs_mark_obstacle(&state, 1, 1, NORTH);
  dfs_mark_obstacle(&state, 1, 1, EAST);
  dfs_mark_obstacle(&state, 1, 1, SOUTH);
  dfs_mark_obstacle(&state, 1, 1, WEST);

  dfs_mark_obstacle(&state, 1, 4, WEST);
  dfs_mark_obstacle(&state, 2, 4, WEST);
  dfs_mark_obstacle(&state, 3, 4, WEST);

  clear();
  int last_rel_dir;
  do {
    printf("State:\n");
    dfs_print_grid(&state);
    printf("Going: ");
    switch (last_rel_dir) {
      case FORWARDS:
        printf(" F ");
        break;
      case RIGHT:
        printf(" R ");
        break;
      case BACKWARDS:
        printf(" B ");
        break;
      case LEFT:
        printf(" L ");
        break;
      default:
        printf("   ");
    }
    printf("\n");
    delay_and_clear();
  } while ((last_rel_dir = dfs_at_intersection(&state)) != -1);

  dfs_finalize(&state);
  printf("Done:\n");
  dfs_print_grid(&state);
  delay_and_clear();
  sleep(10);
}
