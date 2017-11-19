#ifndef GROUP_6_3400_DFS
#define GROUP_6_3400_DFS

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ALGO_IN_SIMULATION
#define PRINT(...)
#else
#define PRINT(...) printf(__VA_ARGS__)
#endif

// Size of map
#define MAP_ROWS 4
#define MAP_COLS 5
// Maximum size of the stack while performing the DFS
#define MAX_STACK_DEPTH (MAP_ROWS * MAP_COLS)
// Number of directions
#define DIR_COUNT 4

// Represents state of a square
enum { UNVISITED, VISITED, ISOLATED };
enum { TREASURE_NO, TREASURE_7KHZ, TREASURE_12KHZ, TREASURE_17KHZ };
enum { WALL, NO_WALL };

// Absolute directions.
// North: lower row, same column
// East: same row, higher column
// South: higher row, same column
// West: same row, lower column
enum { NORTH = 0, EAST, SOUTH, WEST };
// Relative directions
enum { FORWARDS = 0, RIGHT, BACKWARDS, LEFT };

typedef struct {
  int north;
  int west;
} wall_t;

// Represents a point and direction
typedef struct {
  int row;
  int col;
  int dir;
} point_t;

// Represents the state of a DFS exploration
typedef struct {
  int visited[MAP_ROWS][MAP_COLS];
  wall_t obstacles[MAP_ROWS + 1][MAP_COLS + 1];
  // -1 indicates no treasure
  int treasure[MAP_ROWS][MAP_COLS];
  point_t stack[MAX_STACK_DEPTH];
  int stack_head;
  point_t cur_pos;
} explore_t;

#define WALL_NORTH_MASK (0x1 << 2)
#define WALL_EAST_MASK (0x1 << 1)
#define WALL_SOUTH_MASK (0x1 << 0)
#define WALL_WEST_MASK (0x1 << 3)

typedef struct {
  // #: not reachable
  // ?: not visited
  // +: explored
  // ^: robot facing north
  // >: robot facing east
  // v: robot facing south
  // <: robot facing west
  char state;
  // Stores wall information in the low 4 bits
  // Wall X is present if WALL_X_MASK & walls != 0
  // Where X is one of NORTH, EAST, SOUTH, WEST
  char walls;
  // the treasure frequency in kHz
  // 0 if no treasure
  char treasure;
} grid_info_t;

/**
 * Effect: initializes a DFS state, with the robot starting in the given
 * start location, with the given absolute direction.
 */
void dfs_init(explore_t* state, int start_row, int start_col, int start_dir);

/**
 * Effect: marks an obstacle on the grid, at the specified location
 */
void dfs_mark_obstacle(explore_t* state, int row, int col, int dir);

/**
 * Effect: marks an obstacle at a space adjacent to the robot in the specified
 * relative direction.
 */
void dfs_mark_rel_obstacle(explore_t* state, int rel_dir);

/**
 * Effect: marks that the current location has a treasure of type treasure.
 * Should be one of:
 * { TREASURE_NO, TREASURE_7KHZ, TREASURE_12KHZ, TREASURE_17KHZ }
 */
void dfs_mark_treasure(explore_t* state, int treasure);

/**
 * Returns: true if the specified location is in bounds
 */
int dfs_in_bounds(int row, int col);

/**
 * Returns: the absolute direction which is the result of resolving rel_dir
 * with respect to the robot's current direction.
 */
int dfs_resolve_dir(explore_t* state, int rel_dir);

/**
 * Computes: the location as a result of moving the rover in the specified
 * relative direction from its current location.
 * Stores the result in out.
 * Returns: true if the location is valid, false otherwise.
 */
int dfs_get_offset(explore_t* state, int rel_dir, point_t* out);

int dfs_obstacle_between(explore_t* state, const point_t* source,
                         const point_t* terminal);

/**
 * Computes: the location as a result of moving the rover in the specified
 * relative direction from its current location.
 * Stores the result in out.
 * Returns: true if the location is valid and unvisited, false otherwise.
 */
int dfs_should_explore(explore_t* state, int rel_dir, point_t* out);

/**
 * Returns: true if the row and column equal the value stored in the point.
 */
int dfs_point_loc_equals(const point_t* a, int row, int col);

/**
 * Returns: the absolute direction the robot must travel in to move from source
 * to terminal.
 */
int dfs_absolute_direction(const point_t* source, const point_t* terminal);

/**
 * Returns: the relative direction from base_dir to total_dir.
 */
int dfs_relative_offset(int base_dir, int total_dir);

/**
 * Param:  state the state
 * Returns: 1 if no more unvisited reachable squares, 0 otherwise.
 */
int dfs_done(explore_t* state);

/**
 * Effect: moves the robot to the next location, and returns the relative
 * direction
 * it should move in to get there.
 * Returns: -1 when the entire maze explored.
 */
int dfs_at_intersection(explore_t* state);

/**
 * Effect: called after dfs_at_intersection returns -1 to mark some positions
 * as isloated.
 */
void dfs_finalize(explore_t* state);

/**
 * Effect: prints the grid.
 * ? represents unvisited, + represents visited, X represents obstacle, #
 * represents an isolated space (unreachable). dfs_finalize has to be called to
 * get that.
 * The robot is represented by ^, <, >, v, which points in the direction of the
 * robot.
 */
void dfs_print_grid(const explore_t* state);

/**
 * Returns: the info about the requested spot
 */
grid_info_t dfs_get_grid_info(explore_t* state, const int row, const int col);

unsigned short dfs_get_grid_info_to_transmit(explore_t* state, const int row,
                                             const int col);

/**
 * Prints the treasures at each location.
 */
void dfs_print_treasure(const explore_t* state);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* end of include guard:  */
