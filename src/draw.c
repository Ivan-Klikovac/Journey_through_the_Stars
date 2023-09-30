#include <jtts.h>

void draw_map(void)
{
    for(int y = 0; y < MAP_HEIGHT; y++)
    {
        for(int x = 0; x < MAP_WIDTH; x++)
        {
            if(map[y][x].visible)
                mvaddch(y, x, map[y][x].c | map[y][x].color);
            else if(map[y][x].seen)
                mvaddch(y, x, map[y][x].c | COLOR_PAIR(SEEN_COLOR));
            else mvaddch(y, x, ' ');
        }
    }
}

void draw_grid(void) // draws the lines separating the map, inventory, log and sidelog
{
    // horizontal for log
    for(int i = 0; i < WINDOW_WIDTH; i++)
        mvprintw(MAP_HEIGHT + 2, i, "%c", 205);
    
    // corner between log and sidelog
    mvprintw(MAP_HEIGHT + 2, MAP_WIDTH + 2, "%c", 202);

    // vertical for sidelog
    for(int i = 0; i < MAP_HEIGHT + 2; i++)
        mvprintw(i, MAP_WIDTH + 2, "%c", 186);

    // corner between log and inventory log
    mvprintw(MAP_HEIGHT + 2, WINDOW_WIDTH / 2, "%c", 203);

    // vertical for inventory log
    for(int i = 3; i < WINDOW_HEIGHT - MAP_HEIGHT; i++)
        mvprintw(MAP_HEIGHT + i, WINDOW_WIDTH / 2, "%c", 186);

    // todo: change mvprintw to mvaddch, probably more efficient
}

void draw_item(Item* item)
{
    mvaddch(item->pos.y, item->pos.x, item->c | item->color);
}

void draw_items(void)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(items[i] != NULL)
        {
            if(items[i]->visible)
                draw_item(items[i]);
        }
    }
}

void draw_entity(Entity* entity)
{
    mvaddch(entity->pos.y, entity->pos.x, entity->c | entity->color);
}

void draw_entities(Entity** entities) // only really used for monsters rn
{
    for(int i = 0; i < MAX_MONSTERS; i++)
    {
        if(entities[i] != NULL)
        {
            if(entities[i]->visible)
                draw_entity(entities[i]);
        }
    }
}

void draw_log(void) // shits itself if it runs out of screen space
{
    int pos = 0;
    for(int i = 0; i < MAX_LOG_ENTRIES; i++)
    {
        if(game_log[i].duration > 0)
        {
            mvprintw(MAP_HEIGHT + 3 + pos, 1, game_log[i].text);
            pos++; // when something gets printed, go to the next line
            game_log[i].duration--; // decrease the duration by 1 every time it gets printed
        }
    }
}

void draw_sidelog(void)
{
    sidelog_update(); // todo: instead of running it every frame, only run it when needed
    int pos = 0;

    for(int i = 0; i < 16; i++)
    {
        if(sidelog[i] != NULL)
        {
            //if(strcmp(sidelog[i], "") != 0) // unironically this might be more costly to run every time than sometimes running print once more
            mvprintw(pos + 1, MAP_WIDTH + 2, sidelog[i]); // print if it's not empty, skip line either way
            pos++;
        }
    }
}

void draw_inventory_log(void)
{
    int pos = 0;

    for(int i = 0; i < player->inventory_size; i++)
    {
        if(inventory_log[i] != NULL)
        {
            int character = atoi(inventory_log[i]); // the beginning of this string will store the colored character
            int num_digits = abs(character); // to know how many characters of the initial string to skip when printing
            num_digits = (int) log10(num_digits) + 1; // abs -> log10 + 1 is the number of digits
            mvprintw(MAP_HEIGHT + pos + 3, WINDOW_WIDTH / 2 + 2, inventory_log[i] + num_digits - 1); // first print the string
            mvaddch(MAP_HEIGHT + pos + 3, WINDOW_WIDTH / 2 + 2, character); // and then print the colored character on top
            pos++;
        }
    }
}

void draw(void)
{
    clear();
    draw_map();
    draw_grid();
    draw_items();
    draw_entities(monsters);
    draw_entity(player); // player should be drawn last so that he's always above other stuff on the map
    draw_log();
    draw_sidelog();
    draw_inventory_log();
}