#include <stdlib.h>
#include <string.h>
#include "history.h"

// private declarations

void history_queue_push(Queue *q, Game *g);
bool history_queue_pop(Queue *q, Game *g);
void history_queue_clear(Queue *q);

// public api

History *history_new()
{
    History *h = malloc(sizeof(History));
    h->undo = queue_new();
    h->redo = queue_new();
    return h;
}

void history_add(History *h, Game *g)
{
    history_queue_push(h->undo, g);
    history_queue_clear(h->redo);
}

bool history_undo(History *h, Game *g)
{
    if (h->undo->size == 0)
        return false;
    history_queue_push(h->redo, g);
    history_queue_pop(h->undo, g);
    return true;
}

bool history_redo(History *h, Game *g)
{
    if (h->redo->size == 0)
        return false;
    history_queue_push(h->undo, g);
    history_queue_pop(h->redo, g);
    return true;
}

void history_clear(History *h)
{
    history_queue_clear(h->undo);
    history_queue_clear(h->redo);
}

void history_free(History **h)
{
    history_clear(*h);
    queue_free(&(*h)->undo);
    queue_free(&(*h)->redo);
    free((*h));
    (*h) = NULL;
}

// private

void history_queue_push(Queue *q, Game *g)
{
    Game *game = malloc(sizeof(Game));
    (*game) = (*g);
    queue_insert(q, game);
}

bool history_queue_pop(Queue *q, Game *g)
{
    if (q->size == 0)
        return false;

    Game *game = queue_pop(q);
    (*g) = (*game);
    free(game);
    return true;
}

void history_queue_clear(Queue *q)
{
    while (q->size > 0)
    {
        Game *g = queue_pop(q);
        free(g);
    }
}
