// Define Values
// ----------

#define MAP_HEIGHT 7
#define MAP_WIDTH 7
#define MAX_LIST_SIZE (MAP_HEIGHT * MAP_WIDTH)
#define COST_MAX 10000

#define OPEN 1
#define CLOSE 2

// #define DEBUG_DISPLAY

typedef struct
{
    int x;
    int y;
} point;

typedef struct
{
    point position;
    point parent_index;
    double total_cost;
    double heuristic_cost;
} node;

typedef enum
{
    LEFT = 0,
    RIGHT,
    UPPER,
    UNDER
} adjacent;