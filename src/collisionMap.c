#include "collisionMap.h"
#include "sceneManager.h"

int map_collides_rect(SDL_Rect *r)
{
    collisionMap *colMap = &scm_get_scm()->current_Scene->colMap;

    int left = r->x / TILE_SIZE;
    int right = (r->x + r->w - 1) / TILE_SIZE;
    int top = r->y / TILE_SIZE;
    int bottom = (r->y + r->h - 1) / TILE_SIZE;

    if (left < 0 || top < 0 ||
        right >= colMap->width || bottom >= colMap->height)
    {
        return 1;
    }

    for (int y = top; y <= bottom; y++)
    {
        for (int x = left; x <= right; x++)
        {
            if (colMap->tiles[y * colMap->width + x] != 0)
            {
                return colMap->tiles[y * colMap->width + x];
            }
        }
    }

    return 0;
}
