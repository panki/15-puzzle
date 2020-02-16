#ifndef HISTORY_HEADER
#define HISTORY_HEADER

#include <game.h>
#include <pq.h>

typedef struct History
{
    Queue *undo;
    Queue *redo;
} History;

History *history_new();
void history_add(History *h, Game *g);
bool history_undo(History *h, Game *g);
bool history_redo(History *h, Game *g);
void history_clear(History *h);
void history_free(History **h);

#endif