#include <jtts.h>

Tile** create_map_tiles(void)
{
    Tile** tiles = calloc(MAP_HEIGHT, sizeof(Tile*));

    for(int y = 0; y < MAP_HEIGHT; y++)
    {
        tiles[y] = calloc(MAP_WIDTH, sizeof(Tile));

        for(int x = 0; x < MAP_WIDTH; x++)
        {
            tiles[y][x].c = '#';
            tiles[y][x].color = COLOR_PAIR(VISIBLE_COLOR);
            tiles[y][x].walkable = 0;
            tiles[y][x].transparent = 0;
            tiles[y][x].visible = 0;
            tiles[y][x].seen = 0;
        }
    }

    return tiles;
}

Position setup_map(void)
{
    int y, x, height, width, n_rooms;
    n_rooms = (rand() % 11) + 5;
    Room* rooms = calloc(n_rooms, sizeof(Room));
    Position start_pos;

    for(int i = 0; i < n_rooms; i++)
    {
        y = (rand() % (MAP_HEIGHT - 10)) + 1;
        x = (rand() % (MAP_WIDTH - 20)) + 1;
        height = (rand() % 7) + 3;
        width = (rand() % 15) + 5;
        rooms[i] = create_room(y, x, height, width);
        add_room_to_map(rooms[i]);

        if(i > 0)
        {
            connect_room_centers(rooms[i-1].center, rooms[i].center);
        }
    }

    start_pos.y = rooms[0].center.y;
    start_pos.x = rooms[0].center.x;

    free(rooms);

    return start_pos;
}

void free_map(void)
{
    for(int y = 0; y < MAP_HEIGHT; y++)
    {
        free(map[y]);
    }

    free(map);
}