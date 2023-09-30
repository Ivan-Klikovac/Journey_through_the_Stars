#include <jtts.h>

Item** items;

void init_items(void)
{
    items = calloc(MAX_ITEMS, sizeof(Item*));
}

Item* create_item(char* name)
{
    Item* new = calloc(1, sizeof(Item));

    if(strcmp(name, "Rat Meat") == 0)
    {
        //new->c = 158; // ×
        new->c = '*';
        new->color = COLOR_PAIR(RED);
        new->level = 0; // default level for items like this. if item level = 0, don't print it
        strcpy(new->name, "Rat Meat");

        return new;
    }

    return NULL;
}

Item* create_item_on_map(char* name, int y, int x)
{
    Item* new = calloc(1, sizeof(Item));

    new->pos.y = y;
    new->pos.x = x;

    if(strcmp(name, "Rat Meat") == 0)
    {
        new->c = '*';
        new->color = COLOR_PAIR(RED);
        strcpy(new->name, "Rat Meat");
    }

    else return NULL;

    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(items[i] == NULL)
        {
            items[i] = new;
            break;
        }
    }
}

void get_item_description(char* name, char* buffer)
{
    if(strcmp(name, "Rat Meat") == 0)
        sprintf(buffer, "You'll want to cook this. Thoroughly.");
}

void item_pickup(Entity* entity, Item* item)
{
    // remove the item from the list of items on the map
    for(int i = 0; i < MAX_ITEMS; i++)
        if(items[i] == item)
            items[i] = NULL;

    for(int i = 0; i < entity->inventory_size; i++)
    {
        if(entity->inventory[i] == NULL)
        {
            entity->inventory[i] = item;
            break;
        }
    }

    inventory_log_add(item);

    char* buffer = malloc(128);
    sprintf(buffer, "%s picked up %s", entity->name, item->name);
    add_log_entry(buffer);
    free(buffer);
}

void item_pickup_all(Entity* entity, Position pos)
{
    for(int i = 0; i < MAX_ITEMS; i++)
        if(items[i] != NULL)
            if((items[i]->pos.y == pos.y) && (items[i]->pos.x == pos.x))
                item_pickup(entity, items[i]);
}

void item_drop(Entity* owner, Item* item)
{
    char* buffer = malloc(128);
    sprintf(buffer, "%s dropped %s", owner->name, item->name);
    add_log_entry(buffer);
    free(buffer);

    for(int i = 0; i < owner->inventory_size; i++)
    {
        if(owner->inventory[i] == item)
            owner->inventory[i] = NULL;
    }

    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(items[i] == NULL)
        {
            items[i] = item;
            break;
        }
    }

    item->pos.y = owner->pos.y;
    item->pos.x = owner->pos.x;
}