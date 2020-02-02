#include <stdlib.h>
#include <string.h>
#include "history.h"

History *history_new()
{
    History *h = queue_new();
    return h;
}

Game history_pop(History *h)
{
    Game *game = queue_pop(h);
    Game g = (*game);
    free(game);
    return g;
}

void history_push(History *h, Game *g)
{
    Game *game = malloc(sizeof(Game));
    memcpy(game, g, sizeof(Game));
    queue_insert(h, game);
}

void history_clear(History *h)
{
    while (h->size > 0)
    {
        Game *g = queue_pop(h);
        free(g);
    }
}

void history_free(History **h)
{
    history_clear(*h);
    queue_free(h);
}