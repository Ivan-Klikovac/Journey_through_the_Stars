#include <jtts.h>

Log_entry* game_log;
char** sidelog;
Entity** logged_entities;
int num_sidelog_entities = 2;
char** inventory_log;

void init_log(void)
{
    game_log = calloc(MAX_LOG_ENTRIES, sizeof(Log_entry));
}

void init_sidelog(void)
{
    sidelog = calloc(20, sizeof(char*)); // add more space if needed
    logged_entities = calloc(num_sidelog_entities, sizeof(Entity*));
}

void init_inventory_log(void)
{
    inventory_log = calloc(player->inventory_size, sizeof(char*));
}

void init_file_log(void)
{
    FILE* log = fopen("log.txt", "w");
    fclose(log); // file will be empty now
}

void file_log(char* string)
{
    FILE* log = fopen("log.txt", "a");
    fprintf(log, "%s", string);
    fclose(log);
}

void add_log_entry(char* text)
{
    int i;

    for(i = 0; game_log[i].duration > 0; i++);

    strcpy(game_log[i].text, text);
    game_log[i].duration = 3;
}

void add_sidelog_entry(char* text)
{
    int i;
    for(i = 0; sidelog[i] != 0; i++);

    sidelog[i] = malloc(strlen(text) * sizeof(char) + 1);
    strcpy(sidelog[i], text);
}

void sidelog_add_entity(Entity* entity)
{
    // first check if it's on sidelog already
    for(int i = 0; i < num_sidelog_entities; i++)
        if(logged_entities[i] == entity)
            return;

    for(int i = 0; i < num_sidelog_entities; i++)
    {
        if(logged_entities[i] == NULL)
        {
            logged_entities[i] = entity;
            break;
        }
    }
}

void sidelog_remove_entity(Entity* entity)
{
    for(int i = 0; i < num_sidelog_entities; i++)
    {
        if(logged_entities[i] == entity)
            logged_entities[i] = NULL;
    }
}

void sidelog_print_entity(Entity* entity)
{
    char* buffer = malloc(64 * sizeof(char));
    
    if(entity == player)
    {
        sprintf(buffer, "%c %s", 186, entity->name);
        add_sidelog_entry(buffer);
        add_sidelog_entry("");
        sprintf(buffer, "%c %d/%d HP", 186, entity->hp, entity->max_hp);
        add_sidelog_entry(buffer);
        sprintf(buffer, "%c Level %d  | %d/%d EXP", 186, entity->level, entity->exp, entity->level * 10);
        add_sidelog_entry(buffer);
        sprintf(buffer, "%c Armor: %d | Damage: %d", 186, entity->armor, entity->damage);
        add_sidelog_entry(buffer);
        //add_sidelog_entry("");
        sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 204, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205);
        add_sidelog_entry(buffer);
        //add_sidelog_entry("");
    }

    else
    {
        sprintf(buffer, "%c %s [level %d]", 186, entity->name, entity->level);
        add_sidelog_entry(buffer);
        add_sidelog_entry("");
        sprintf(buffer, "%c %d/%d HP", 186, entity->hp, entity->max_hp);
        add_sidelog_entry(buffer);
        sprintf(buffer, "%c Armor: %d | Damage: %d", 186, entity->armor, entity->damage);
        add_sidelog_entry(buffer);
        //add_sidelog_entry("");
        sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 204, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205);
        add_sidelog_entry(buffer);
        //add_sidelog_entry("");
    }

    free(buffer);
}

void sidelog_update(void)
{
    sidelog_clear();

    for(int i = 0; i < num_sidelog_entities; i++)
    {
        if(logged_entities[i] != NULL)
        {
            sidelog_print_entity(logged_entities[i]);
        }
    }
}

void sidelog_clear(void)
{
    for(int i = 0; i < 16; i++)
    {
        if(sidelog[i] != NULL)
        {
            free(sidelog[i]);
            sidelog[i] = NULL;
        }
    }
}

void inventory_log_add(Item* item) // todo: add columns
{
    for(int i = 0; i < player->inventory_size; i++)
    {
        if(inventory_log[i] != NULL && !item->level && (strncmp(strchr(inventory_log[i], ' ') + 1, item->name, strlen(item->name)) == 0))
        {   // (only group level 0 items)
            // group these two items together
            // ok so the structure of the string is
            // c+colorcode name (number)
            // the last part might not be there
            // now I need to check if it already has the number in it, that is, if it already shows >1 item
            char* ptr = strchr(inventory_log[i], '\0');
            if(*(ptr - 1) == ')') // stack with >1 item
            {
                ptr = strrchr(inventory_log[i], '('); // last occurrence of (
                int n = atoi(ptr + 1); // *ptr is (, *(ptr+1) is the start of the number
                *ptr = '\0'; // null term the bracket and just run strcat
                char* buffer = malloc(16);
                sprintf(buffer, "(%d)", n + 1); // add 1 more to the stack
                strcat(inventory_log[i], buffer);
                free(buffer);
                break; // job done
            }

            else // the stack had just 1 item, just add (2)
            {
                strcat(inventory_log[i], " (2)");
                break; // these are necessary
            }
        }

        else if(inventory_log[i] == NULL)
        {
            char* entry = malloc(128);
            sprintf(entry, "%d %s", item->c | item->color, item->name);
            
            if(item->level) // if it's not level 0
            {
                char* buffer = malloc(32); // need it to sprintf safely
                sprintf(buffer, " [Level %d]", item->level);
                strcat(entry, buffer);
                free(buffer);
            }

            inventory_log[i] = entry;
            break;
        }
    }
}

void inventory_log_remove(Item* item)
{
    for(int i = 0; i < player->inventory_size; i++)
    {
        if(strcmp(inventory_log[i] + 2, item->name) == 0) // first 2 chars are item->c and a whitespace
        {
            free(inventory_log[i]);
            inventory_log[i] = NULL;
        }
    }
}