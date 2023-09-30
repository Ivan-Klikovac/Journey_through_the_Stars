#include <jtts.h>

float speed_modifier;

Entity* create_player(Position start_pos)
{
    Entity* new_player = calloc(1, sizeof(Entity));

    strcpy(new_player->name, "Player");
    new_player->pos.y = start_pos.y;
    new_player->pos.x = start_pos.x;
    new_player->c = '@';
    new_player->color = COLOR_PAIR(WHITE);
    new_player->hp = 50;
    new_player->max_hp = 50;
    new_player->level = 1;
    new_player->damage = 8;
    new_player->armor = 4;
    new_player->speed = 1;
    new_player->poise = new_player->speed;
    new_player->vision_range = 15;
    new_player->inventory = calloc(16, sizeof(Item*));
    new_player->inventory_size = 16;

    speed_modifier = 1.0 / new_player->speed;

    return new_player;
}

void change_player_speed(float new_speed)
{
    float change = new_speed / player->speed;
    change = 1 / change;
    speed_modifier *= change;
}

void handle_input(int input)
{
    Position new_pos = {player->pos.y, player->pos.x};

    switch(input)
    {
        case 'w':
            new_pos.y--;
            break;
        case 's':
            new_pos.y++;
            break;
        case 'a':
            new_pos.x--;
            break;
        case 'd':
            new_pos.x++;
            break;
        default:
            break;
    }

    move_player(new_pos);
}

void move_player(Position new_pos)
{
    if(is_entity_on_pos(new_pos))
    {
        Entity* target = get_entity_from_pos(new_pos);
        attack(player, target);

        return;
    }

    if(is_item_on_pos(new_pos))
    {
        item_pickup_all(player, new_pos);
    } // picking up doesn't take a turn, you can move within the same turn

    if(map[new_pos.y][new_pos.x].walkable)
    {
        clear_fov(player);
        player->pos.y = new_pos.y;
        player->pos.x = new_pos.x;
        make_fov(player);

        return;
    }
}

int is_entity_on_pos(Position pos) // is another entity on this tile already?
{
    for(int i = 0; i < MAX_MONSTERS; i++)
    {
        if(monsters[i] != NULL)
        {
            if((monsters[i]->pos.y == pos.y) && (monsters[i]->pos.x == pos.x))
                return 1;
        }
    }

    return 0;
}

int is_item_on_pos(Position pos)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(items[i] != NULL)
        {
            if((items[i]->pos.y == pos.y) && (items[i]->pos.x == pos.x))
                return 1;
        }
    }

    return 0;
}

Entity* get_entity_from_pos(Position pos)
{
    for(int i = 0; i < MAX_MONSTERS; i++)
    {
        if(monsters[i] != NULL)
        {
            if((monsters[i]->pos.y == pos.y) && (monsters[i]->pos.x == pos.x))
                return monsters[i];
        }
    }

    return NULL;
}

Item* get_item_from_pos(Position pos) // make pickup of multiple items from the same tile in the same turn possible
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(items[i] != NULL)
        {
            if((items[i]->pos.y == pos.y) && (items[i]->pos.x == pos.x))
                return items[i];
        }
    }

    return NULL;
}