// Define Values
// ----------

#define MAP_HEIGHT 7
#define MAP_WIDTH 7
#define MAX_LIST_SIZE (MAP_HEIGHT * MAP_WIDTH)
#define COST_MAX 10000

#define OPEN 1
#define CLOSE 2

typedef struct
{
    int x;
    int y;
} node;

typedef struct
{
    node position;
    node parent_index;
    double heuristic_cost;
    double total_cost;
} list;

typedef enum
{
    LEFT = 0,
    RIGHT,
    UPPER,
    UNDER
} adjacent;