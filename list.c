#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct Node *newNode(int readyAt, int currentProcessIndex)
{
    /* allocate node */
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    /* put in the data  */
    new_node->readyAt = readyAt;
    new_node->currentProcessIndex = currentProcessIndex;
    new_node->next = NULL;

    return new_node;
}

struct Node *pop(struct Node **list)
{
    if (*list == NULL)
    {
        return NULL;
    }

    struct Node *head = *list;
    (*list) = (*list)->next;

    return head;
}

void sortedInsert(struct Node **list, struct Node *new_node)
{
    struct Node *current;
    /* Special case for the head end */
    if (*list == NULL || (*list)->readyAt >= new_node->readyAt)
    {
        new_node->next = *list;
        *list = new_node;
    }
    else
    {
        /* Locate the node before the point of insertion */
        current = *list;
        while (current->next != NULL && current->next->readyAt < new_node->readyAt)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}