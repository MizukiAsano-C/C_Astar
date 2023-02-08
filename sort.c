#include <stdio.h>
#include "A_star.h"

// External Function Prototype
// ----------

extern int GetOpenList(node *, int);
extern int GetOpen_i(void);

// Function Prototype
// ----------
#if 0
void SortOpenList();
void quick_sort(node *, int, int);

// Function Body
// ----------

void SortOpenList(void)
{
    node temp_open_list[MAX_LIST_SIZE];
    int i;
    int max = GetOpen_i();

    for (i=0;i<max;i++)
    {
        GetOpenList(&temp_open_list[i], i);
    }

    quick_sort(temp_open_list, 0, max - 1);
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
#endif