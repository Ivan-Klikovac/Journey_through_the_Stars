#include <jtts.h>

Entity** monsters;

void init_monsters()
{
    monsters = calloc(MAX_MONSTERS, sizeof(Entity*));
}

void process_ai(void) // this should be executed after every player turn
{
    for(int i = 0; i < MAX_MONSTERS; i++)
    {
        if(monsters[i] != NULL)
        {
            while(monsters[i]->poise >= 1)
            {
                monsters[i]->poise -= 1;

                if(monsters[i]->state == NEUTRAL)
                {
                    continue;
                }

                if(monsters[i]->state == WANDERING)
                {
                    Position pos;
                    pos.y = monsters[i]->pos.y;
                    pos.x = monsters[i]->pos.x;
                    int random = rand() % 2;

                    if(random) // 50% chance to move horizontally, 50% chance to move vertically
                    {
                        random = rand() % 2;
                        if(random == 0) random--; // so that it's either -1 or 1
                        pos.y += random;
                    }

                    else
                    {
                        random = rand() % 2;
                        if(random == 0) random--;
                        pos.x += random;
                    }

                    move_monster(monsters[i], pos);

                    if(calculate_heuristic(monsters[i]->pos, player->pos) <= monsters[i]->vision_range)
                    {
                        monsters[i]->state = HUNTING;
                    }

                    continue;
                }

                if(monsters[i]->state == HUNTING)
                {
                    if((monsters[i]->pos.y == player->pos.y && (monsters[i]->pos.x + 1 == player->pos.x || monsters[i]->pos.x - 1 == player->pos.x)) 
                    || (monsters[i]->pos.x == player->pos.x && (monsters[i]->pos.y + 1 == player->pos.y || monsters[i]->pos.y - 1 == player->pos.y)))
                    {
                        attack(monsters[i], player);
                    }

                    else // not in range
                    {
                        int direction = find_path(monsters[i]->pos, player->pos);
                        Position pos;
                        pos.y = monsters[i]->pos.y;
                        pos.x = monsters[i]->pos.x;

                        if(direction == UP) pos.y++;
                        else if(direction == DOWN) pos.y--;
                        else if(direction == RIGHT) pos.x++;
                        else if(direction == LEFT) pos.x--;
                        //else continue; // invalid direction

                        move_monster(monsters[i], pos);
                    }

                    if(calculate_heuristic(monsters[i]->pos, player->pos) > monsters[i]->vision_range)
                    {
                        monsters[i]->state = WANDERING;
                    }

                    continue;
                }
            }

            monsters[i]->poise += monsters[i]->speed * speed_modifier;
            draw();
        }
    }
}

void move_monster(Entity* monster, Position new_pos)
{
    if(map[new_pos.y][new_pos.x].walkable && !is_entity_on_pos(new_pos))
    {
        monster->pos.y = new_pos.y;
        monster->pos.x = new_pos.x;
    }
}

void attack(Entity* attacker, Entity* target)
{
    // do damage
    int damage_dealt = attacker->damage * ((float)100 / (100 + 10 * target->armor));
    target->hp -= damage_dealt;

    // print log
    char* buffer = malloc(64);
    sprintf(buffer, "%s hit %s for %d damage", attacker->name, target->name, damage_dealt);
    add_log_entry(buffer);

    // add enemy to sidelog
    if(attacker == player)
        sidelog_add_entity(target);
    
    // kill the target
    if(target->hp <= 0)
    {
        sprintf(buffer, "%s killed %s", attacker->name, target->name);
        add_log_entry(buffer);
        
        // give exp
        if(attacker == player)
            player->exp += target->level * 5 + 3;

        // drop items
        for(int i = 0; i < target->inventory_size; i++)
            if(target->inventory[i] != NULL)
                item_drop(target, target->inventory[i]);

        // cleanup
        sidelog_remove_entity(target);

        for(int i = 0; i < MAX_MONSTERS; i++)
            if(target == monsters[i])
                monsters[i] = NULL;
        
        free(target);

        draw_items(); // for any drops to appear on screen immediately, which still doesn't work
    }

    free(buffer);
}

Entity* spawn(char* name, int y, int x)
{
    Entity* new = calloc(1, sizeof(Entity));

    if(strcmp(name, "Rat") == 0)
    {
        strcpy(new->name, "Rat");
        new->max_hp = 20;
        new->hp = 20;
        new->damage = 3;
        new->armor = 1;
        new->pos.y = y;
        new->pos.x = x;
        new->c = 'r';
        new->color = COLOR_PAIR(RED);
        new->visible = 0;
        new->state = WANDERING;
        new->speed = 1.2;
        new->poise = new->speed * speed_modifier;
        new->vision_range = 10;
        new->inventory = calloc(1, sizeof(Item*));
        new->inventory_size = 1;
        new->inventory[0] = create_item("Rat Meat");

        for(int i = 0; i < MAX_MONSTERS; i++)
        {
            if(monsters[i] == NULL) 
            {
                monsters[i] = new;
                break;
            }
        }

        return new;
    }

    else 
    {
        free(new);
        return NULL;
    }
}