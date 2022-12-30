#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

typedef struct
{
    int scrap_amount;
    // 1 = me, 0 = foe, -1 = neutral
    int owner;
    int units;
    int recycler;
    int can_build;
    int can_spawn;
    int in_range_of_recycler;

} InformationMap;

typedef struct
{
    int north;
    int south;
    int east;
    int west;

} AreaInfluence;

typedef struct
{
    InformationMap *infoMap;
    int width;
    int height;

} Map;

typedef struct
{
    int x;
    int y;
} Point;

char *action_build = "BUILD";
char *action_spawn = "SPAWN";
char *action_move = "MOVE";
char *action_wait = "WAIT";

int g_nbrRobot = 4;

void getMapInformation(Map *map)
{
    int my_matter;
    int opp_matter;
    int position;
    scanf("%d%d", &my_matter, &opp_matter);
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            int scrap_amount;
            int owner; // 1 = me, 0 = foe, -1 = neutral
            int units;
            int recycler;
            int can_build;
            int can_spawn;
            int in_range_of_recycler;

            position = i * map->width + j;

            scanf("%d%d%d%d%d%d%d", &scrap_amount,
                  &owner,
                  &units,
                  &recycler,
                  &can_build,
                  &can_spawn,
                  &in_range_of_recycler);

            map->infoMap[position].scrap_amount = scrap_amount;
            map->infoMap[position].owner = owner;
            map->infoMap[position].units = units;
            map->infoMap[position].recycler = recycler;
            map->infoMap[position].can_build = can_build;
            map->infoMap[position].can_spawn = can_spawn;
            map->infoMap[position].in_range_of_recycler = in_range_of_recycler;
        }
    }
}

int find_build_point(Map *map, Point *point)
{
    int position;
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            position = i * map->width + j;
            if (map->infoMap[position].can_build)
            {
                point->x = j;
                point->y = i;

                return i * map->width + j;
            }
        }
    }
    return -1;
}

void build_recycler(Map *map)
{
    Point point = {0, 0};
    if (find_build_point(map, &point) != -1)
    {
        printf("%s %d %d\n", action_build, point.x, point.y);
        return;
    }
}

void areaInfluence(Map *map, int position, AreaInfluence *area)
{
    int x = position % map->width;
    int y = position / map->width;

    area->north = (y - 1) * map->width + x;
    area->south = (y + 1) * map->width + x;
    area->east = y * map->width + (x + 1);
    area->west = y * map->width + (x - 1);
}

int findRobot(Map *map, int *tabRobot)
{
    int position;
    int index = 0;
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            position = i * map->width + j;
            if (map->infoMap[position].units > 0 && map->infoMap[position].owner == 1)
            {
                tabRobot[index] = position;
                ++index;
            }
        }
    }
    return -1;
}

int bestArea(Map *map, AreaInfluence *area)
{
    int position;
    int best_scrap_amount = 0;

    if (map->infoMap[area->north].scrap_amount > map->infoMap[area->south].scrap_amount)
    {
        position = area->north;
        best_scrap_amount = map->infoMap[area->north].scrap_amount;
    }
    else
    {
        position = area->south;
        best_scrap_amount = map->infoMap[area->south].scrap_amount;
    }

    if (best_scrap_amount < map->infoMap[area->east].scrap_amount)
    {
        position = area->east;
        best_scrap_amount = map->infoMap[area->east].scrap_amount;
    }

    if (best_scrap_amount < area->west)
    {
        position = area->west;
    }

    return position;
}

void move(Map *map)
{
    int *tabRobot = malloc(sizeof(int) * g_nbrRobot);
    int index = 0;
    Point srcpoint = {0, 0};
    Point dstpoint = {0, 0};
    int found;
    int position;
    AreaInfluence area;

    found = findRobot(map, tabRobot);

    while (index < g_nbrRobot && found != -1)
    {
        position = tabRobot[index];
        fprintf(stderr, "robot position %d", position);

        areaInfluence(map, position, &area);
        int best = bestArea(map, &area);
        srcpoint.x = position % map->width;
        srcpoint.y = position / map->width;
        dstpoint.x = best % map->width;
        dstpoint.y = best / map->width;
        areaInfluence(map, position, &area);
        fprintf(stderr, "%s 1 %d %d %d %d\n", action_move, srcpoint.x, srcpoint.y, dstpoint.x, dstpoint.y);
        printf("%s 1 %d %d %d %d\n", action_move, srcpoint.x, srcpoint.y, dstpoint.x, dstpoint.y);
        ++index;
    }
}

int main()
{
    int width;
    int height;
    Map map;

    scanf("%d%d", &width, &height);

    map.width = width;
    map.height = height;
    map.infoMap = malloc(sizeof(InformationMap) * width * height);
    getMapInformation(&map);

    build_recycler(&map);
    // game loop
    while (1)
    {

        move(&map);
        getMapInformation(&map);

        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");
    }

    return 0;
}