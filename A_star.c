#include <stdio.h>
#include <math.h>
#include "A_star.h"

// Glogal Values
// ----------
#if 1
const int G_MAP[MAP_HEIGHT][MAP_WIDTH] = {
    // 0: impassable, >0: cost
    {0, 0, 0, 0, 0, 0, 0}, // 0 ~ 6
    {0, 1, 1, 1, 1, 1, 0}, // 7 ~ 13
    {0, 1, 0, 1, 1, 0, 0}, // 14 ~ 20
    {0, 1, 0, 0, 1, 1, 0}, // 21 ~ 27
    {0, 1, 1, 0, 1, 1, 0}, // 29 ~ 34
    {0, 1, 1, 1, 1, 1, 0}, // 35 ~ 41
    {0, 0, 0, 0, 0, 0, 0}  // 42 ~ 48
};
#else
const int G_MAP[MAP_HEIGHT][MAP_WIDTH] = {
    // 0: impassable, >0: cost
    {0, 0, 0, 0, 0, 0, 0}, // 0 ~ 6
    {0, 1, 1, 0, 1, 1, 0}, // 7 ~ 13
    {0, 1, 0, 0, 1, 0, 0}, // 14 ~ 20
    {0, 1, 0, 0, 1, 1, 0}, // 21 ~ 27
    {0, 1, 1, 0, 1, 1, 0}, // 29 ~ 34
    {0, 1, 1, 0, 1, 1, 0}, // 35 ~ 41
    {0, 0, 0, 0, 0, 0, 0}  // 42 ~ 48
};
#endif

static node open_list[MAX_LIST_SIZE];
static node close_list[MAX_LIST_SIZE];
static int open_i = 0;
static int close_i = 0;

// Function Prototype list
// ----------

extern void InitOpenList();
extern void InitCloseList();
extern void AddOpenList(node);
extern void AddCloseList(node);
extern int GetOpenList(node *, int);
extern int GetCloseList(node *, int);
extern void EraseOpenList(int);
extern void EraseCloseList(int);
extern int SearchOpenList(node);
extern int SearchCloseList(node);
extern void CheckOpenList(void);
extern void CheckCloseList();
extern int GetOpen_i(void);

// Function Prototype sort
// ----------

extern void SortOpenList();
extern void quick_sort(node *, int, int);

// Function Prototype
// ----------

int AStar(point, point);
int IsNodeSamePosition(point, point);
void SetNodeXY(point *, int, int);
void SetAdjacentNode(point, node *);
void CalcHeuristic(node *, point);
void CalcAdjacentNodeTotalCost(node *, double, double *, point);
int JudgeAdjacentNode(node *, double);
void AddAdjacentNode(node *, double, node);

// Function Body
// ----------

int main()
{
    point goal_point;
    point start_point;

    int result;

    goal_point.x = 1;
    goal_point.y = 1;
    start_point.x = 4;
    start_point.y = 2;

    InitOpenList();
    InitCloseList();

    result = AStar(start_point, goal_point);

    if (result == 1)
    {
        printf("Goal found!");
    }
    else
    {
        printf("Goal is not found");
    }
}

int AStar(point start_point, point goal_point)
{
    int route_check = 0;
    int i;
    int ret_adjacent;
    node now_node;
    node before_node;
    node adjacent_node[4];
    double temp_total_cost;

    now_node.position = start_point;
    now_node.total_cost = 0;
    AddOpenList(now_node);

    while (GetOpenList(&now_node, 0) != -1)
    {
#ifdef DEBUG_DISPLAY
        printf("loop in ----------\r\n");
        printf("now_node: (%d, %d) total = %f\r\n", now_node.position.x, now_node.position.y, now_node.total_cost);
#endif
        if (IsNodeSamePosition(now_node.position, goal_point))
        {
            AddCloseList(now_node);
            route_check = 1;
            break;
        }

        SetAdjacentNode(now_node.position, adjacent_node);

        for (i = 0; i < 4; i++)
        {
            if ((adjacent_node[i].position.x == -1) || (adjacent_node[i].position.y == -1))
            {
                // next position check
                continue;
            }

#ifdef DEBUG_DISPLAY
            printf(" adj[%d] x = %d, y = %d\r\n", i, adjacent_node[i].position.x, adjacent_node[i].position.y);
#endif
            CalcAdjacentNodeTotalCost(&adjacent_node[i], now_node.total_cost, &temp_total_cost, goal_point);

            ret_adjacent = JudgeAdjacentNode(&adjacent_node[i], temp_total_cost);
            if (ret_adjacent == 1)
            {
#ifdef DEBUG_DISPLAY
                printf("total_cost change\r\n");
#endif
                AddAdjacentNode(&adjacent_node[i], temp_total_cost, now_node);
            }
        }
        EraseOpenList(0);
        AddCloseList(now_node);
        SortOpenList();
#ifdef DEBUG_DISPLAY
        printf("---------- open check end\r\n");
#endif
    }

    if (route_check == 1)
    {
        now_node.position = goal_point;
        printf("[%d, %d]\r\n", now_node.position.x, now_node.position.y);
        do
        {
            before_node.position = now_node.parent_index;
            i = SearchCloseList(before_node);
            GetCloseList(&now_node, i);
            printf("[%d, %d]\r\n", now_node.position.x, now_node.position.y);
#ifdef DEBUG_DISPLAY
            printf("   position(%d, %d)\r\n", now_node.position.x, now_node.position.y);
            printf("   parent  (%d, %d)\r\n", now_node.parent_index.x, now_node.parent_index.y);
#endif
        } while (!IsNodeSamePosition(start_point, now_node.position));
    }

    return route_check;
}

int IsNodeSamePosition(point node_a, point node_b)
{
#ifdef DEBUG_DISPLAY
    printf("a.(%d, %d) : b.(%d, %d)\r\n", node_a.x, node_a.y, node_b.x, node_b.y);
#endif
    if ((node_a.x == node_b.x) && (node_a.y == node_b.y))
    {
#ifdef DEBUG_DISPLAY
        // printf("same position found\r\n");
#endif
        return 1;
    }
    else
    {
        return 0;
    }
}

void SetNodeXY(point *position, int x, int y)
{
    position->x = x;
    position->y = y;
}

void SetAdjacentNode(point now_node, node *adjacent)
{
    // printf("set adjacent node now_mode x = %d, y = %d\r\n", now_node.x, now_node.y);
    if ((now_node.x > 0) && (G_MAP[now_node.y][now_node.x - 1] != 0))
    {
        // left is ok
        SetNodeXY(&adjacent[LEFT].position, now_node.x - 1, now_node.y);
    }
    else
    {
        SetNodeXY(&adjacent[LEFT].position, -1, -1);
    }

    if (((now_node.x + 1) < MAP_WIDTH) && (G_MAP[now_node.y][now_node.x + 1] != 0))
    {
        // right is ok
        SetNodeXY(&adjacent[RIGHT].position, now_node.x + 1, now_node.y);
    }
    else
    {
        SetNodeXY(&adjacent[RIGHT].position, -1, -1);
    }

    if ((now_node.y > 0) && (G_MAP[now_node.y - 1][now_node.x] != 0))
    {
        // upper is ok
        SetNodeXY(&adjacent[UPPER].position, now_node.x, now_node.y - 1);
    }
    else
    {
        SetNodeXY(&adjacent[UPPER].position, -1, -1);
    }

    if (((now_node.y + 1) < MAP_HEIGHT) && (G_MAP[now_node.y + 1][now_node.x] != 0))
    {
        // under is ok
        SetNodeXY(&adjacent[UNDER].position, now_node.x, now_node.y + 1);
    }
    else
    {
        SetNodeXY(&adjacent[UNDER].position, -1, -1);
    }
}

void CalcHeuristic(node *adjacent_node, point goal_point)
{
    int i;
    int temp_x;
    int temp_y;

    temp_x = adjacent_node->position.x - goal_point.x;
    temp_y = adjacent_node->position.y - goal_point.y;

    adjacent_node->heuristic_cost = sqrt((temp_x * temp_x) + (temp_y * temp_y));
}

void CalcAdjacentNodeTotalCost(node *adjacent_node, double now_cost, double *temp_total_cost, point goal_point)
{
    double edge_cost;

    // MEMO: already checked map cost is not 0 at SetAdjacentNode()

    CalcHeuristic(adjacent_node, goal_point);
    edge_cost = G_MAP[adjacent_node->position.y][adjacent_node->position.x];
    *temp_total_cost = edge_cost + adjacent_node->heuristic_cost + now_cost;

#ifdef DEBUG_DISPLAY
    // printf("calc total cost: %f\r\n", *temp_total_cost);
#endif
}

int JudgeAdjacentNode(node *adjacent_node, double total_cost)
{
    int ret = 0; // 1: add OK. 0: don't add to list
    int open_index;
    int close_index;
    node temp_node;

#ifdef DEBUG_DISPLAY
    // printf("adj node (%d, %d)\r\n", adjacent_node->position.x, adjacent_node->position.y);
#endif

    open_index = SearchOpenList(*adjacent_node);
    close_index = SearchCloseList(*adjacent_node);

    if ((open_index == -1) && (close_index == -1))
    {
#ifdef DEBUG_DISPLAY
        printf(" first position\r\n");
#endif
        ret = 1;
    }
    else if (open_index != -1)
    {
        if (open_index)
            GetOpenList(&temp_node, open_index);

#ifdef DEBUG_DISPLAY
        printf("  compare OPEN total = %f, now_total = %f\r\n", total_cost, temp_node.total_cost);
#endif
        if (total_cost < temp_node.total_cost)
        {
#ifdef DEBUG_DISPLAY
            printf("   erase\r\n");
#endif
            ret = 1;
            EraseOpenList(open_index);
        }
    }
    else if (close_index != -1)
    {
        GetCloseList(&temp_node, close_index);

#ifdef DEBUG_DISPLAY
        printf("  compare CLOSE total = %f, now_total = %f\r\n", total_cost, temp_node.total_cost);
#endif
        if (total_cost < temp_node.total_cost)
        {
#ifdef DEBUG_DISPLAY
            printf("   erase\r\n");
#endif
            ret = 1;
            EraseCloseList(close_index);
        }
    }

    return ret;
}

void AddAdjacentNode(node *adjacent_node, double total_cost, node now_node)
{
    adjacent_node->parent_index = now_node.position;
    adjacent_node->total_cost = total_cost;
    AddOpenList(*adjacent_node);
}