#include <stdio.h>
#include <math.h>
#include "A_star.h"

// Glogal Values
// ----------

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

// Function Prototype
// ----------

int AStar(node, node, list *, list *);
int IsNodeSamePosition(node, node);
void SetAdjacentNode(node, list *);
void CalcHeuristic(list *, node);
void CalcAdjacentNodeTotalCost(list *, double, list *, list *, int, int );

void InitList(list *);
void AddList(list *, int *, list);
void GetList(list *, int *, list *);
void erase_list(list *, int *);

// Function Body
// ----------

int main()
{
    node goal_node;
    node start_node;
    list open_list[MAX_LIST_SIZE];
    list close_list[MAX_LIST_SIZE];

    int result;

    goal_node.x = 1;
    goal_node.y = 1;
    start_node.x = 4;
    start_node.y = 2;

    InitList(open_list);
    InitList(close_list);

    result = AStar(start_node, goal_node, open_list, close_list);

    if (result == 1)
    {
        printf("Goal found!");
    }
    else
    {
        printf("Goal is not found");
    }
}

int AStar(node start_node, node goal_node, list *open_list, list *close_list)
{
    list now_list;
    list adjacent_node[4];
    int open_count = 0;
    int close_count = 0;
    int adjacent_count;
    double cost;

    now_list.position = start_node;
    now_list.total_cost = 0;
    AddList(open_list, &open_count, now_list);

    while (open_count != 0)
    {
        GetList(open_list, &open_count, &now_list);
        erase_list(open_list, &open_count);

        if (IsNodeSamePosition(now_list.position, goal_node))
        {
            AddList(close_list, &close_count, now_list);
            return 1;
        }

        SetAdjacentNode(now_list.position, adjacent_node);

/* for DEBUG
        printf("left : %d, right : %d, upper : %d, under : %d\r\n",
               adjacent_node[LEFT].position.x, adjacent_node[RIGHT].position.x, adjacent_node[UPPER].position.x, adjacent_node[UNDER].position.x);
*/
        CalcHeuristic(adjacent_node, goal_node);

/* for DEBUG
        printf("left : %f, right : %f, upper : %f, under : %f\r\n",
               adjacent_node[LEFT].heuristic_cost, adjacent_node[RIGHT].heuristic_cost, adjacent_node[UPPER].heuristic_cost, adjacent_node[UNDER].heuristic_cost);
*/

        CalcAdjacentNodeTotalCost(adjacent_node, now_list.total_cost, open_list, close_list, open_count, close_count);

        // Next: Equall Cell Check
        // |
        // AddList(close_list, &close_count, now_list)
        // |
        // Sort
    }

    return 0;
}

int IsNodeSamePosition(node now_node, node goal)
{
    if ((now_node.x == goal.x) && (now_node.y == goal.y))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void SetAdjacentNode(node now_node, list *adjacent)
{
    // printf("now_mode x = %d, y = %d\r\n", now_node.x, now_node.y);
    if ((now_node.x > 0) && (G_MAP[now_node.y][now_node.x - 1] != 0))
    {
        // left is ok
        adjacent[LEFT].position.x = now_node.x - 1;
        adjacent[LEFT].position.y = now_node.y;
    }
    else
    {
        adjacent[LEFT].position.x = -1;
        adjacent[LEFT].position.y = -1;
    }

    if (((now_node.x + 1) < MAP_WIDTH) && (G_MAP[now_node.y][now_node.x + 1] != 0))
    {
        // right is ok
        adjacent[RIGHT].position.x = now_node.x + 1;
        adjacent[RIGHT].position.y = now_node.y;
    }
    else
    {
        adjacent[RIGHT].position.x = -1;
        adjacent[RIGHT].position.y = -1;
    }

    if ((now_node.y != 0) && (G_MAP[now_node.y - 1][now_node.x] != 0))
    {
        // upper is ok
        adjacent[UPPER].position.x = now_node.x;
        adjacent[UPPER].position.y = now_node.y - 1;
    }
    else
    {
        adjacent[UPPER].position.x = -1;
        adjacent[UPPER].position.y = -1;
    }

    if (((now_node.y + 1) < MAP_HEIGHT) && (G_MAP[now_node.y + 1][now_node.x] != 0))
    {
        // under is ok
        adjacent[UNDER].position.x = now_node.x;
        adjacent[UNDER].position.y = now_node.y + 1;
    }
    else
    {
        adjacent[UNDER].position.x = -1;
        adjacent[UNDER].position.y = -1;
    }
}

void CalcHeuristic(list *adjacent_node, node goal_node)
{
    int i;
    int temp_x;
    int temp_y;

    for (i = 0; i < 4; i++)
    {
        if ((adjacent_node[i].position.x == -1) || (adjacent_node[i].position.y == -1))
        {
            // next position check
            continue;
        }

        temp_x = adjacent_node[i].position.x - goal_node.x;
        temp_y = adjacent_node[i].position.y - goal_node.y;
        adjacent_node[i].heuristic_cost = sqrt((temp_x * temp_x) + (temp_y * temp_y));
    }
}

void CalcAdjacentNodeTotalCost(list *adjacent_node, double now_cost, list *open_list, list *close_list, int open_count, int close_count)
{
    double edge_cost;
    double total_cost;
    int i;
    int temp_x;
    int temp_y;

    for (i = 0; i < 4; i++)
    {
        if ((adjacent_node[i].position.x == -1) || (adjacent_node[i].position.y == -1))
        {
            // next position check
            continue;
        }

        // MEMO: already checked map cost is not 0 at SetAdjacentNode()
        edge_cost = G_MAP[adjacent_node[i].position.y][adjacent_node[i].position.x];
        total_cost = edge_cost + adjacent_node[i].heuristic_cost + now_cost;
    }
}

#if 0
int JudgeNode(list *open_list, list *close_list, list adjacent_node, double total_cost, int *open_count, int *close_count)
{
    int i;
    int temp_list;

    for(i=0;i<open_count;i++)
    {
        // GetList(temp_list, cou)
    }
}
#endif

// List function

void InitList(list *list)
{
    int i;
    for (i = 0; i < MAX_LIST_SIZE; i++)
    {
        list[i].position.x = -1;
        list[i].position.y = -1;
        list[i].heuristic_cost = COST_MAX;
        list[i].total_cost = 0;
        list[i].parent_index.x = -1;
        list[i].parent_index.y = -1;
    }
}

void AddList(list *add_list, int *count, list now_list)
{
    add_list[*count] = now_list;
    *count += 1;
}

void GetList(list *get_list, int *count, list *now_list)
{
    if (*count <= 0)
    {
        return;
    }

    *now_list = get_list[*count - 1];
}

void erase_list(list *list, int *count)
{
    list[*count].position.x = -1;
    list[*count].position.y = -1;
    list[*count].heuristic_cost = COST_MAX;
    list[*count].total_cost = 0;
    list[*count].parent_index.x = -1;
    list[*count].parent_index.y = -1;

    *count -= 1;
}