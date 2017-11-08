#include "dfs.h"
#include <stdio.h>

// The directions
const int DIRECTIONS[DIR_COUNT] = {NORTH, EAST, SOUTH, WEST};
// The relative directions
const int RELATIVE_DIRECTIONS[DIR_COUNT] = {FORWARDS, RIGHT, BACKWARDS, LEFT};

void dfs_init(explore_t* state, int start_row, int start_col, int start_dir) {
  for (size_t row = 0; row < MAP_ROWS; row++) {
    for (size_t col = 0; col < MAP_COLS; col++) {
      state->visited[row][col] = UNVISITED;
      state->treasure[row][col] = TREASURE_NO;
    }
  }
  for (size_t row = 0; row < MAP_ROWS + 1; row++) {
    for (size_t col = 0; col < MAP_COLS + 1; col++) {
      state->obstacles[row][col].north = NO_WALL;
      state->obstacles[row][col].west = NO_WALL;
    }
  }
  for (size_t col = 0; col < MAP_COLS + 1; col++) {
    state->obstacles[0][col].north = WALL;
    state->obstacles[MAP_ROWS][col].north = WALL;
  }
  for (size_t row = 0; row < MAP_ROWS + 1; row++) {
    state->obstacles[row][0].west = WALL;
    state->obstacles[row][MAP_COLS].west = WALL;
  }
  state->cur_pos.dir = start_dir;
  state->cur_pos.row = start_row;
  state->cur_pos.col = start_col;
  state->stack_head = 0;
}

void dfs_mark_obstacle(explore_t* state, int row, int col, int dir) {
  if (row < 0 || row >= MAP_ROWS + 1 || col < 0 || col >= MAP_COLS + 1) {
    return;
  }

  if (dir == NORTH) {
    state->obstacles[row][col].north = WALL;
  } else if (dir == WEST) {
    state->obstacles[row][col].west = WALL;
  } else if (dir == SOUTH) {
    state->obstacles[row + 1][col].north = WALL;
  } else if (dir == EAST) {
    state->obstacles[row][col + 1].west = WALL;
  }
}

void dfs_mark_rel_obstacle(explore_t* state, int rel_dir) {
  int abs_dir = dfs_resolve_dir(state, rel_dir);
  int row = state->cur_pos.row;
  int col = state->cur_pos.col;
  if (abs_dir == SOUTH) {
    row++;
    abs_dir = NORTH;
  } else if (abs_dir == EAST) {
    col++;
    abs_dir = WEST;
  }
  dfs_mark_obstacle(state, row, col, abs_dir);
}

void dfs_mark_treasure(explore_t* state, int treasure) {
  state->treasure[state->cur_pos.row][state->cur_pos.col] = treasure;
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

int dfs_obstacle_between(explore_t* state, const point_t* source,
                         const point_t* terminal) {
  int abs_dir = dfs_absolute_direction(source, terminal);
  switch (abs_dir) {
    case NORTH:
      return state->obstacles[source->row][source->col].north == WALL;
    case EAST:
      return state->obstacles[terminal->row][terminal->col].west == WALL;
    case SOUTH:
      return state->obstacles[terminal->row][terminal->col].north == WALL;
    case WEST:
      return state->obstacles[source->row][source->col].west == WALL;
    default:
      return 1;
  }
}

int dfs_should_explore(explore_t* state, int rel_dir, point_t* out) {
  return dfs_get_offset(state, rel_dir, out) &&
         !dfs_obstacle_between(state, &state->cur_pos, out) &&
         state->visited[out->row][out->col] != VISITED;
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

int dfs_done(explore_t* state) {
  // Go back through the stack, and look for squares with unvisted
  // neighbors
  state->stack[state->stack_head++] = state->cur_pos;
  int result = 1;
  for (int i = state->stack_head - 1; i >= 0 && result; i--) {
    state->cur_pos = state->stack[i];
    for (int i = 0; i < DIR_COUNT && result; i++) {
      point_t target;
      if (dfs_should_explore(state, RELATIVE_DIRECTIONS[i], &target)) {
        result = 0;
      }
    }
  }

  state->cur_pos = state->stack[--state->stack_head];
  return result;
}

int dfs_at_intersection(explore_t* state) {
  state->visited[state->cur_pos.row][state->cur_pos.col] = VISITED;

  // If we've visited every reachable square
  if (dfs_done(state)) {
    return -1;
  }

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

void dfs_finalize(explore_t* state) {
  for (size_t row = 0; row < MAP_ROWS; row++) {
    for (size_t col = 0; col < MAP_COLS; col++) {
      if (state->visited[row][col] == UNVISITED) {
        state->visited[row][col] = ISOLATED;
      }
    }
  }
}

void dfs_print_grid(const explore_t* state) {
  for (size_t row = 0; row < MAP_ROWS + 1; row++) {
    for (size_t col = 0; col < MAP_COLS + 1; col++) {
      if (state->obstacles[row][col].north == WALL) {
        if (col == MAP_COLS) {
          PRINT("*");
        } else {
          PRINT("****");
        }
      } else {
        PRINT("    ");
      }
    }
    PRINT("\n");
    for (size_t col = 0; col < MAP_COLS + 1; col++) {
      if (row != MAP_ROWS && state->obstacles[row][col].west == WALL) {
        PRINT("| ");
      } else {
        PRINT("  ");
      }

      if (row != MAP_ROWS && col != MAP_COLS) {
        if (dfs_point_loc_equals(&state->cur_pos, row, col)) {
          switch (state->cur_pos.dir) {
            case NORTH:
              PRINT("^");
              break;
            case EAST:
              PRINT(">");
              break;
            case SOUTH:
              PRINT("v");
              break;
            case WEST:
              PRINT("<");
              break;
            default:
              PRINT(" ");
          }
        } else {
          switch (state->visited[row][col]) {
            case VISITED:
              PRINT("+");
              break;
            case UNVISITED:
              PRINT("?");
              break;
            case ISOLATED:
              PRINT("#");
              break;
            default:
              PRINT(" ");
          }
        }
      } else {
        PRINT(" ");
      }
      PRINT(" ");
    }

    PRINT("\n");
  }
}

void dfs_print_treasure(const explore_t* state) {
  for (size_t row = 0; row < MAP_ROWS; row++) {
    for (size_t col = 0; col < MAP_COLS; col++) {
      switch (state->treasure[row][col]) {
        case TREASURE_NO:
          PRINT(" 0 ");
          break;
        case TREASURE_7KHZ:
          PRINT(" 7 ");
          break;
        case TREASURE_12KHZ:
          PRINT(" 12 ");
          break;
        case TREASURE_17KHZ:
          PRINT(" 17 ");
          break;
        default:
          PRINT("   ");
      }
    }
    PRINT("\n");
  }
}

grid_info_t dfs_get_grid_info(explore_t* state, const int row, const int col) {
  grid_info_t info;
  if (dfs_point_loc_equals(&state->cur_pos, row, col)) {
    switch (state->cur_pos.dir) {
      case NORTH:
        info.state = '^';
        break;
      case EAST:
        info.state = '>';
        break;
      case SOUTH:
        info.state = 'v';
        break;
      case WEST:
        info.state = '<';
        break;
    }
  } else {
    switch (state->visited[row][col]) {
      case VISITED:
        info.state = '+';
        break;
      case UNVISITED:
        info.state = '?';
        break;
      case ISOLATED:
        info.state = '#';
        break;
    }
  }

  switch (state->treasure[row][col]) {
    case TREASURE_NO:
      info.treasure = 0;
      break;
    case TREASURE_7KHZ:
      info.treasure = 7;
      break;
    case TREASURE_12KHZ:
      info.treasure = 12;
      break;
    case TREASURE_17KHZ:
      info.treasure = 17;
      break;
  }

  info.walls = 0;
  info.walls |= state->obstacles[row][col].north == WALL ? WALL_NORTH_MASK : 0;
  info.walls |= state->obstacles[row][col].west == WALL ? WALL_WEST_MASK : 0;
  info.walls |=
      state->obstacles[row + 1][col].north == WALL ? WALL_SOUTH_MASK : 0;
  info.walls |=
      state->obstacles[row][col + 1].west == WALL ? WALL_EAST_MASK : 0;

  return info;
}
