#include <jtts.h>

void make_fov(Entity* player)
{
    int y, x, distance;
    int radius = player->vision_range;
    Position target;

    map[player->pos.y][player->pos.x].visible = 1;
    map[player->pos.y][player->pos.x].seen = 1;

    for(y = player->pos.y - radius; y < player->pos.y + radius; y++)
    {
        for(x = player->pos.x - radius; x < player->pos.x + radius; x++)
        {
            target.y = y;
            target.x = x;
            distance = get_distance(player->pos, target);

            if(distance < radius)
            {
                if(is_in_map(y, x) && line_of_sight(player->pos, target))
                {
                    map[y][x].visible = 1;
                    map[y][x].seen = 1;
                }
            }
        }
    }

    for(int i = 0; i < MAX_MONSTERS; i++)
    {
        if(monsters[i] != NULL)
        {
            distance = get_distance(player->pos, monsters[i]->pos);
            if(distance < radius)
            {
                if(line_of_sight(player->pos, monsters[i]->pos))
                {
                    monsters[i]->visible = 1;
                }
            }
        }
    }

    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(items[i] != NULL)
        {
            distance = get_distance(player->pos, items[i]->pos);
            if(distance < radius)
            {
                if(line_of_sight(player->pos, items[i]->pos))
                {
                    items[i]->visible = 1;
                }
            }
        }
    }
}

void clear_fov(Entity* player)
{
    int y, x;
    int radius = 15;

    for(y = player->pos.y - radius; y < player->pos.y + radius; y++)
    {
        for(x = player->pos.x - radius; x < player->pos.x + radius; x++)
        {
            if(is_in_map(y, x))
                map[y][x].visible = 0;
        }
    }

    for(int i = 0; i < MAX_MONSTERS; i++)
    {
        if(monsters[i] != NULL)
            monsters[i]->visible = 0;
    }

    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(items[i] != NULL)
            items[i]->visible = 0;
    }
}

int get_distance(Position origin, Position target)
{
    double dy, dx;
    int distance;
    dx = target.x - origin.x;
    dy = target.y - origin.y;
    distance = floor(sqrt((dx * dx) + (dy * dy)));

    return distance;
}

int is_in_map(int y, int x)
{
    if((0 < y && y < MAP_HEIGHT - 1) && (0 < x && x < MAP_WIDTH - 1))
    {
        return 1;
    }

    return 0;
}

int line_of_sight(Position origin, Position target)
{
    // adapted from https://roguebasin.com/index.php/Simple_Line_of_Sight

    int t, x, y, abs_delta_x, abs_delta_y, sign_x, sign_y, delta_x, delta_y;
    
    delta_x = origin.x - target.x;
    delta_y = origin.y - target.y;

    abs_delta_x = abs(delta_x);
    abs_delta_y = abs(delta_y);

    sign_x = sign(delta_x);
    sign_y = sign(delta_y);

    x = target.x;
    y = target.y;

    if(abs_delta_x > abs_delta_y)
    {
        t = abs_delta_y * 2 - abs_delta_x;

        do
        {
            if(t >= 0)
            {
                y += sign_y;
                t -= abs_delta_x * 2;
            }

            x += sign_x;
            t += abs_delta_y * 2;

            if(x == origin.x && y == origin.y)
            {
                return 1;
            }
        }
        while (map[y][x].transparent);

        return 0;
    }

    else
    {
        t = abs_delta_x * 2 - abs_delta_y;

        do
        {
            if(t >= 0)
            {
                x += sign_x;
                t -= abs_delta_y * 2;
            }

            y += sign_y;
            t += abs_delta_x * 2;

            if(x == origin.x && y == origin.y)
            {
                return 1;
            }
        }
        while(map[y][x].transparent);

        return 0;
    }
}

int sign(int a)
{
    return (a < 0) ? -1 : 1;
}