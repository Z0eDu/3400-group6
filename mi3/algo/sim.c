#include <stdio.h>
#include "dfs.h"

int main(int argc, char** argv) {
  explore_t state;
  dfs_init(&state, 0, 0, NORTH);
  dfs_mark_obstacle(&state, 1, 1);
  dfs_mark_obstacle(&state, 0, 2);
  dfs_mark_obstacle(&state, 2, 2);
  dfs_mark_obstacle(&state, 1, 3);

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
  } while ((last_rel_dir = dfs_at_intersection(&state)) != -1);

  dfs_finalize(&state);
  printf("Done:\n");
  dfs_print_grid(&state);
}