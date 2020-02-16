#ifndef PQ_HEADER
#define PQ_HEADER

#include <stdbool.h>

typedef struct queue
{
    unsigned long size;
    struct node *head;
} Queue;

typedef struct node
{
    void *data;
    int priority;
    struct node *next;
} Node;

Queue *queue_new();
Node *queue_insert(Queue *queue, void *data);
Node *queue_insert_sorted(Queue *queue, void *data, int priority);
void *queue_pop(Queue *queue);
void *queue_peek(Queue *queue);
void queue_free(Queue **queue);
bool queue_is_empty(Queue *queue);

#endif