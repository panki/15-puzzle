// Priority queue on linked list
#include <stdlib.h>
#include <assert.h>
#include "pq.h"

Queue *queue_new()
{
    Queue *q = malloc(sizeof(Queue));
    q->head = NULL;
    q->size = 0;
    return q;
}

Node *queue_new_node(void *data, int priority)
{
    Node *n = malloc(sizeof(Node));
    n->data = data;
    n->priority = priority;
    n->next = NULL;
    return n;
}

Node *queue_insert(Queue *queue, void *data)
{
    Node *n = queue_new_node(data, 0);
    n->next = queue->head;
    queue->head = n;
    queue->size++;
    return n;
}

Node *queue_insert_sorted(Queue *queue, void *data, int priority)
{
    Node *temp = queue_new_node(data, priority);

    if (queue->size == 0)
    {
        // Empty queue
        queue->head = temp;
    }
    else if (queue->head->priority > priority)
    {
        // Insert before head
        temp->next = queue->head;
        queue->head = temp;
    }
    else
    {
        // Insert in the middle or after tail
        Node *node = queue->head;
        while (node->next != NULL && node->next->priority < priority)
        {
            node = node->next;
        }
        temp->next = node->next;
        node->next = temp;
    }
    queue->size++;
    return temp;
}

void *queue_pop(Queue *queue)
{
    if (queue->size > 0)
    {
        Node *temp = queue->head;
        void *data = temp->data;
        queue->head = queue->head->next;
        queue->size--;
        free(temp);
        return data;
    }
    return NULL;
}

void *queue_peek(Queue *queue)
{
    return queue->size > 0 ? queue->head->data : NULL;
}

void queue_free(Queue **queue)
{
    assert(queue);
    while ((*queue)->size > 0)
        queue_pop(*queue);
    free(*queue);
    (*queue) = NULL;
}

bool queue_is_empty(Queue *queue)
{
    return queue->size == 0;
}
