#include <stdio.h>
#include <math.h>
#include "A_star.h"

// Glogal Values
// ----------
static node open_list[MAX_LIST_SIZE];
static node close_list[MAX_LIST_SIZE];
static int open_i = 0;
static int close_i = 0;

// External Function Prototype
// ----------
extern int IsNodeSamePosition(point, point);

// Function Prototype
// ----------

void InitOpenList();
void InitCloseList();
void AddOpenList(node);
void AddCloseList(node);
int GetOpenList(node *, int);
int GetCloseList(node *, int);
void EraseOpenList(int);
void EraseCloseList(int);
int SearchOpenList(node);
int SearchCloseList(node);

void CheckOpenList(void);
void CheckCloseList();

int GetOpen_i(void);

void SortOpenList();
void quick_sort(node *, int, int);

// Function Body
// ----------

void InitOpenList(void)
{
    int i;
    open_i = 0;

    for (i = 0; i < MAX_LIST_SIZE; i++)
    {
        open_list[i].position.x = -1;
        open_list[i].position.y = -1;
        open_list[i].heuristic_cost = COST_MAX;
        open_list[i].total_cost = 0;
        open_list[i].parent_index.x = -1;
        open_list[i].parent_index.y = -1;
    }
}

void InitCloseList(void)
{
    int i;
    close_i = 0;

    for (i = 0; i < MAX_LIST_SIZE; i++)
    {
        close_list[i].position.x = -1;
        close_list[i].position.y = -1;
        close_list[i].heuristic_cost = COST_MAX;
        close_list[i].total_cost = 0;
        close_list[i].parent_index.x = -1;
        close_list[i].parent_index.y = -1;
    }
}

void AddOpenList(node new_node)
{
    if (open_i > MAX_LIST_SIZE)
    {
        return;
    }

    open_list[open_i] = new_node;
    // printf("add open list. open_i : %d [%d, %d] total : %f\r\n",
    //        open_i, open_list[open_i].position.x, open_list[open_i].position.y, open_list[open_i].total_cost);
    open_i += 1;
}

void AddCloseList(node new_node)
{
    if (close_i > MAX_LIST_SIZE)
    {
        return;
    }
    close_list[close_i] = new_node;
    close_i += 1;
}

int GetOpenList(node *target_node, int iterator)
{
    *target_node = open_list[iterator];

#ifdef DEBUG_DISPLAY
    // printf(" get i = %d (%d, %d), total = %f\r\n",iterator, target_node->position.x, target_node->position.y, target_node->total_cost);
#endif
    if ((target_node->position.x == -1) || (target_node->position.y == -1))
    {
        return -1;
    }

    return 1;
}

int GetCloseList(node *target_node, int iterator)
{
    *target_node = close_list[iterator];

#ifdef DEBUG_DISPLAY
    printf(" get i = [%d] (%d, %d), parent (%d, %d)\r\n",iterator, target_node->position.x, target_node->position.y, target_node->parent_index.x, target_node->parent_index.y);
#endif
    if ((target_node->position.x == -1) || (target_node->position.y == -1))
    {
        return -1;
    }

    return 1;
}

int SearchOpenList(node target_node)
{
    int i;

    if (open_i <= 0)
    {
        printf("iterator = 0\r\n");
        return -1;
    }

    for (i = 0; i < open_i; i++)
    {
#ifdef DEBUG_DISPLAY
        // printf(" @ search (%d, %d)\r\n", target_node.position.x, target_node.position.y);
#endif

        if (IsNodeSamePosition(open_list[i].position, target_node.position))
        {
            return i;
        }
    }

    return -1;
}

int SearchCloseList(node target_node)
{
    int i;

    if (close_i <= 0)
    {
        return -1;
    }

#ifdef DEBUG_DISPLAY
        printf("search start ---\r\n");
#endif
    for (i = 0; i < close_i; i++)
    {
        if (IsNodeSamePosition(close_list[i].position, target_node.position))
        {
#ifdef DEBUG_DISPLAY
            printf("---Search end: [%d](%d, %d)\r\n", i, target_node.position.x, target_node.position.y);
#endif
            return i;
        }
    }

    return -1;
}

void EraseOpenList(int iterator)
{
    int i;

    if (iterator >= open_i)
    {
        return;
    }

    for (i = iterator; i < open_i; i++)
    {
        open_list[i] = open_list[i + 1];
    }

    open_list[open_i].position.x = -1;
    open_list[open_i].position.y = -1;
    open_i -= 1;
}

void EraseCloseList(int iterator)
{
    int i;

    if (iterator >= close_i)
    {
        return;
    }

    for (i = iterator; i < close_i; i++)
    {
        close_list[i] = close_list[i + 1];
    }

    close_list[close_i].position.x = -1;
    close_list[close_i].position.y = -1;
    close_i -= 1;
}

void CheckOpenList(void)
{
    int i;
    node temp_node;

    for (i = 0; i < open_i; i++)
    {
        printf("[%d] = (%d, %d)\t", i, open_list[i].position.x, open_list[i].position.y);
        printf("total_cost = %f\r\n", open_list[i].total_cost);
    }
}

void CheckCloseList(void)
{
    int i;

    for (i = 0; i < close_i; i++)
    {
        printf("[%d] = (%d, %d)\r\n", i, close_list[i].position.x, close_list[i].position.y);
        printf("  parent(%d, %d)\r\n", close_list[i].parent_index.x, close_list[i].parent_index.y);
    }
}

int GetOpen_i(void)
{
    return open_i;
}

void SortOpenList(void)
{
    quick_sort(open_list, 0, open_i - 1);

    // CheckOpenList();
}

void quick_sort(node *target_list, int left, int right)
{
    double standard;
    node temp_node;
    node left_node;
    int i_left;
    int j_right;

    if (left >= right)
    {
        return;
    }

    standard = target_list[left].total_cost;
    left_node = target_list[left];
    i_left = left;
    j_right = right + 1;

    while (1)
    {
        do
        {
            i_left++;
        } while (target_list[i_left].total_cost < standard);

        do
        {
            j_right--;
        } while (target_list[j_right].total_cost > standard);

        if (i_left >= j_right)
        {
            break;
        }

        temp_node = target_list[i_left];
        target_list[i_left] = target_list[j_right];
        target_list[j_right] = temp_node;
    }

    target_list[left] = target_list[j_right];
    target_list[j_right] = left_node;

    quick_sort(target_list, left, j_right - 1);
    quick_sort(target_list, j_right + 1, right);
}