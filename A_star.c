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

// External Function Prototype
// ----------

extern void InitOpenList();
extern void InitCloseList();
extern void AddOpenList(node);
extern void AddCloseList(node);
extern int GetOpenList(node *, int);
extern int GetCloseList(node *, int);
extern void EraseOpenList();
extern void EraseCloseList();
extern int SearchOpenList(node);
extern int SearchCloseList(node);
extern void CheckOpenList(void);
extern void CheckCloseList(void);
extern void SortOpenList(void);
extern int GetOpen_i(void);

// Function Prototype
// ----------

int AStar(point, point);
int IsNodeSamePosition(point, point);
void SetNodeXY(point *, int, int);
void SetAdjacentNode(point, node *);
void CalcHeuristic(node *, point);
void CalcAdjacentNodeTotalCost(node *, double, double *, point);
int JudgeAdjacentNode(node *, double, double *);
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
    int display_i;
    int ret_adjacent;
    node now_node;
    node adjacent_node[4];
    double temp_total_cost;

    now_node.position = start_point;
    now_node.total_cost = 0;
    AddOpenList(now_node);

    while (GetOpenList(&now_node, 0) != -1)
    {
        GetOpenList(&now_node, 0);
        EraseOpenList(0);

        if (IsNodeSamePosition(now_node.position, goal_point))
        {
            AddCloseList(now_node);
            route_check = 1;
        }

        SetAdjacentNode(now_node.position, adjacent_node);

        for (i = 0; i < 4; i++)
        {
            if ((adjacent_node->position.x == -1) || (adjacent_node[i].position.y == -1))
            {
                // next position check
                continue;
            }

            CalcAdjacentNodeTotalCost(&adjacent_node[i], now_node.total_cost, &temp_total_cost, goal_point);
            ret_adjacent = JudgeAdjacentNode(&adjacent_node[i], adjacent_node[i].total_cost, &temp_total_cost);
            if (ret_adjacent == 1)
            {
                AddAdjacentNode(&adjacent_node[i], temp_total_cost, now_node);
            }
        }
        SortOpenList();

#ifdef DEBUG_DISPLAY
        printf("--- after sort ---\r\n");
        display_i = GetOpen_i();
        for (i = 0; i < display_i; i++)
        {
            GetOpenList(&now_node, i);
            printf("[%d] = (%d, %d)\t", i, now_node.position.x, now_node.position.y);
            printf("cost = %f\r\n", i, now_node.total_cost);
        }
#endif
    }

    if (route_check == 1)
    {
        while(!IsNodeSamePosition(start_point, now_node.parent_index))
        {
            printf("[%d, %d]\r\n", now_node.parent_index.x, now_node.parent_index.y);
            now_node.position = now_node.parent_index;
            i = SearchCloseList(now_node);
            GetCloseList(&now_node, i);
        }
    }

    return route_check;
}

int IsNodeSamePosition(point node_a, point node_b)
{
    if ((node_a.x == node_b.x) && (node_a.y == node_b.y))
    {
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
    // printf("now_mode x = %d, y = %d\r\n", now_node.x, now_node.y);
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
}

int JudgeAdjacentNode(node *adjacent_node, double total_cost, double *temp_total_cost)
{
    int ret = 0; // 1: add OK. 0: don't add to list
    int open_index;
    int close_index;
    node temp_node;

    open_index = SearchOpenList(*adjacent_node);
    close_index = SearchCloseList(*adjacent_node);

    if ((open_index == -1) && (close_index == -1))
    {
        ret = 1;
    }
    else if (open_index != 0)
    {
        GetOpenList(&temp_node, open_index);

        if (adjacent_node->total_cost < temp_node.total_cost)
        {
            ret = 1;
            EraseOpenList(open_index);
        }
    }
    else if (close_index != 0)
    {
        GetCloseList(&temp_node, close_index);

        if (adjacent_node->total_cost < temp_node.total_cost)
        {
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