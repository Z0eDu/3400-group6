// #ifndef GROUP_6_3400_DFS
// #define GROUP_6_3400_DFS

#include <stdio.h>

#define MAP_ROWS 4
#define MAP_COLS 5
#define MAX_STACK_DEPTH (MAP_ROWS * MAP_COLS)
#define DIR_COUNT 4

enum { UNVISITED, VISITED, OBSTACLE };

enum { NORTH = 0, EAST, SOUTH, WEST };
enum { FORWARDS = 0, RIGHT, BACKWARDS, LEFT };

const int DIRECTIONS[DIR_COUNT] = {FORWARDS, RIGHT, BACKWARDS, LEFT};
const int RELATIVE_DIRECTIONS[DIR_COUNT] = {FORWARDS, RIGHT, BACKWARDS, LEFT};

typedef struct {
  int row;
  int col;
  int dir;
} point_t;

typedef struct {
  int visited[MAP_ROWS][MAP_COLS];
  point_t stack[MAX_STACK_DEPTH];
  int stack_head;
  point_t cur_pos;
} explore_t;

void dfs_init(explore_t* state, int start_row, int start_col, int start_dir) {
  for (size_t row = 0; row < MAP_ROWS; row++) {
    for (size_t col = 0; col < MAP_COLS; col++) {
      state->visited[row][col] = UNVISITED;
    }
  }
  state->cur_pos.dir = start_dir;
  state->cur_pos.row = start_row;
  state->cur_pos.col = start_col;
  state->stack_head = 0;
}

void dfs_mark_obstacle(explore_t* state, int row, int col) {
  state->visited[row][col] = OBSTACLE;
}

int dfs_in_bounds(int row, int col) {
  return row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS;
}

int dfs_resolve_dir(explore_t* state, int rel_dir) {
  return (state->cur_pos.dir + rel_dir) % DIR_COUNT;
}

int dfs_get_offset(explore_t* state, int rel_dir, point_t* out) {
  int abs_dir = dfs_resolve_dir(state, rel_dir);
  int row, col;
  switch (abs_dir) {
    case NORTH:
      row = state->cur_pos.row - 1;
      col = state->cur_pos.col;
      break;
    case EAST:
      row = state->cur_pos.row;
      col = state->cur_pos.col + 1;
      break;
    case SOUTH:
      row = state->cur_pos.row + 1;
      col = state->cur_pos.col;
      break;
    case WEST:
      row = state->cur_pos.row;
      col = state->cur_pos.col - 1;
      break;
    default:
      row = -1;
      col = -1;
  }

  out->row = row;
  out->col = col;
  out->dir = abs_dir;

  return dfs_in_bounds(row, col);
}

int dfs_should_explore(explore_t* state, int rel_dir, point_t* out) {
  return dfs_get_offset(state, rel_dir, out) &&
         state->visited[out->row][out->col] == UNVISITED;
}

int dfs_point_loc_equals(const point_t* a, int row, int col) {
  return a->row == row && a->col == col;
}

int dfs_absolute_direction(const point_t* source, const point_t* terminal) {
  int dr = terminal->row - source->row;
  int dc = terminal->col - source->col;

  if (dr == 1 && dc == 0) return SOUTH;
  if (dr == 0 && dc == 1) return EAST;
  if (dr == -1 && dc == 0) return NORTH;
  if (dr == 0 && dc == -1) return WEST;

  return -1;
}

int dfs_relative_offset(int base_dir, int total_dir) {
  return (total_dir - base_dir + DIR_COUNT) % DIR_COUNT;
}

int dfs_at_intersection(explore_t* state) {
  state->visited[state->cur_pos.row][state->cur_pos.col] = VISITED;

  // Look at all the neighbors
  for (int i = 0; i < DIR_COUNT; i++) {
    point_t target;
    if (dfs_should_explore(state, RELATIVE_DIRECTIONS[i], &target)) {
      // We are going to go; push our current location onto the stack
      state->stack[state->stack_head++] = state->cur_pos;
      int next_dir = dfs_absolute_direction(&state->cur_pos, &target);
      state->cur_pos = target;
      state->cur_pos.dir = next_dir;
      return RELATIVE_DIRECTIONS[i];
    }
  }

  // If we have no unvisited neighbors, pop the stack
  // If the stack is empty, we done!
  if (state->stack_head == 0) {
    return -1;
  }

  point_t next_pos = state->stack[--state->stack_head];
  int next_dir = dfs_absolute_direction(&state->cur_pos, &next_pos);
  int rel_dir = dfs_relative_offset(state->cur_pos.dir, next_dir);
  state->cur_pos = next_pos;
  state->cur_pos.dir = next_dir;
  return rel_dir;
}

void dfs_print_grid(const explore_t* state) {
  for (size_t row = 0; row < MAP_ROWS; row++) {
    for (size_t col = 0; col < MAP_COLS; col++) {
      if (dfs_point_loc_equals(&state->cur_pos, row, col)) {
        switch (state->cur_pos.dir) {
          case NORTH:
            printf(" ^ ");
            break;
          case EAST:
            printf(" > ");
            break;
          case SOUTH:
            printf(" v ");
            break;
          case WEST:
            printf(" < ");
            break;
          default:
            printf("   ");
        }
      } else {
        switch (state->visited[row][col]) {
          case VISITED:
            printf(" + ");
            break;
          case UNVISITED:
            printf(" ? ");
            break;
          case OBSTACLE:
            printf(" X ");
            break;
          default:
            printf("   ");
        }
      }
    }
    printf("\n");
  }
}

int main(int argc, char** argv) {
  explore_t state;
  dfs_init(&state, 0, 0, NORTH);
  dfs_mark_obstacle(&state, 1, 1);
  dfs_mark_obstacle(&state, 0, 2);

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
}

// #endif /* end of include guard:  */
