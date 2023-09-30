#include <jtts.h>

Save save_data;

void init_save(void)
{
    save_data.player = player;
    save_data.monsters = monsters;
    save_data.map = map;
    save_data.game_log = game_log;
    save_data.sidelog = sidelog;
    save_data.inventory_log = inventory_log;
    save_data.items = items;
}

void serialize_entity(FILE* savefile, Entity* entity)
{
    fwrite(entity, sizeof(Entity), 1, savefile);
}

void deserialize_entity(FILE* savefile, Entity* entity)
{
    fread(entity, sizeof(Entity), 1, savefile);
}

void save(void)
{
    add_log_entry("saving...");
    FILE* savefile = fopen("save.txt", "w");
    if(!savefile) 
        add_log_entry("NULL");

    //player serialization
    serialize_entity(savefile, player);
    
    // monster serialization
    int n = 0; // how many monsters are to be serialized
    for(int i = 0; i < MAX_MONSTERS; i++)
        if(monsters[i] != NULL) n++;
    
    fprintf(savefile, "%d\n", n); // save the number and then a \n for fgets to stop there
    
    for(int i = 0; i < MAX_MONSTERS; i++)
        if(monsters[i] != NULL)
            serialize_entity(savefile, monsters[i]);

    // map serialization





    fclose(savefile);
}

void load(void)
{
    add_log_entry("loading...");
    FILE* savefile = fopen("save.txt", "r");
    if(!savefile) 
        add_log_entry("NULL");

    /// player deserialization
    deserialize_entity(savefile, player);

    // monster deserialization
    char buffer[128];
    fgets(buffer, 128, savefile); // picks up just the number of monsters after the player data and the \n

    int n = atoi(buffer);

    for(int i = 0; i < MAX_MONSTERS; i++)
    {
        free(monsters[i]);
        memset(monsters[i], 0, sizeof(Entity));
    }
    
    memset(monsters, 0, MAX_MONSTERS * sizeof(Entity*));

    for(int i = 0; i < n; i++)
        if(monsters[i] != NULL)
        {
            deserialize_entity(savefile, monsters[i]);
        }

    // map deserialization




    fclose(savefile);
}