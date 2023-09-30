#include <jtts.h>

Room create_room(int y, int x, int height, int width)
{
    Room new_room;

    new_room.pos.y = y;
    new_room.pos.x = x;
    new_room.height = height;
    new_room.width = width;
    new_room.center.y = y + (int)(height / 2);
    new_room.center.x = x + (int)(width / 2);

    return new_room;
}

void add_room_to_map(Room room)
{
    for(int y = room.pos.y; y < room.pos.y + room.height; y++)
    {
        for(int x = room.pos.x; x < room.pos.x + room.width; x++)
        {
            map[y][x].c = '.';
            map[y][x].walkable = 1;
            map[y][x].transparent = 1;
        }
    }
}

void connect_room_centers(Position center1, Position center2)
{
    Position temp;
    temp.x = center1.x;
    temp.y = center1.y;

    while(1)
    {
        if(abs((temp.x - 1) - center2.x) < abs(temp.x - center2.x))
            temp.x--;
        else if(abs((temp.x + 1) - center2.x) < abs(temp.x - center2.x))
            temp.x++;
        else if(abs((temp.y + 1) - center2.y) < abs(temp.y - center2.y))
            temp.y++;
        else if(abs((temp.y - 1) - center2.y) < abs(temp.y - center2.y))
            temp.y--;
        else break;

        map[temp.y][temp.x].c = '.';
        map[temp.y][temp.x].walkable = 1;
        map[temp.y][temp.x].transparent = 1;
    }
}