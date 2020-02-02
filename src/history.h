#ifndef HISTORY_HEADER
#define HISTORY_HEADER

#include <game.h>
#include <pq.h>

typedef Queue History;

History *history_new();
Game history_pop(History *h);
void history_push(History *h, Game *g);
void history_clear(History *h);
void history_free(History **h);

#endif