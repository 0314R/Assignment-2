#ifndef LIST_H
#define LIST_H

typedef struct Node
{
    int readyAt;
    int currentProcessIndex;
    struct Node *next;
} Node;

void freeList(struct Node *head);
void sortedInsert(struct Node **head_ref, struct Node *new_node);
struct Node *newNode(int readyAt, int currentProcessIndex);
struct Node *pop(struct Node **list);

#endif