#ifndef ROGUE_H
#define ROGUE_H

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// color pairs
#define VISIBLE_COLOR 1
#define SEEN_COLOR 2
#define RED 3
#define BLUE 2
#define WHITE 1

// directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// states
#define NEUTRAL 0 // used for idle entities and the player
#define WANDERING 1 // random movement
#define HUNTING 2 // moving toward the player

// nigger
#define MAX_LOG_ENTRIES 128
#define MAX_MONSTERS 128
#define MAX_ITEMS 128

typedef struct
{
    int y;
    int x;
} Position;

typedef struct
{
    Position pos;
    char c;
    int color;
    int visible;
    int level;
    char name[16];
} Item;

typedef struct
{
    Position pos;
    char c;
    int color;
    int hp;
    int max_hp;
    int armor;
    int damage;
    int level;
    int exp; // only used for player
    int visible; // only used for monsters
    int state; // 0 for player and idle npcs, 1 or 2 for monsters
    float speed; // actions per turn
    float poise; // tied to speed; at >=1 you can perform a turn and you lose 1; gain speed amount of poise every turn
    int vision_range; // for player this determines LOS, for monsters how near the player needs to be to draw aggro
    Item** inventory;
    int inventory_size;
    char name[16];

} Entity;

typedef struct
{
    char c;
    int color;
    int walkable;
    int transparent;
    int visible;
    int seen;
} Tile;

typedef struct Node
{
    Position pos;
    struct Node* parent;
    int heuristic; // H cost
} Node;

typedef struct
{
    int height;
    int width;
    Position pos; // upper left corner
    Position center;
} Room;

typedef struct
{
    char text[64];
    int duration;
} Log_entry;

typedef struct
{
    Entity* player;
    Entity** monsters;
    Tile** map;
    Log_entry* game_log;
    char** sidelog;
    char** inventory_log;
    Item** items;
} Save;

// engine.c functions
int setup(void);
void loop(void);
void close(void);

// draw.c functions
void draw_map(void);
void draw_grid(void);
void draw_item(Item* item);
void draw_items(void);
void draw_entity(Entity* entity);
void draw_entities(Entity** entities);
void draw_log(void);
void draw_sidelog(void);
void draw_inventory_log(void);
void draw(void);

// map.c functions
Tile** create_map_tiles(void);
Position setup_map(void);
void free_map(void);

// room.c functions
Room create_room(int y, int x, int height, int width);
void add_room_to_map(Room room);
void connect_room_centers(Position center1, Position center2);

// item.c functions
void init_items(void);
Item* create_item(char* name);
Item* create_item_on_map(char* name, int y, int x);
void get_item_description(char* name, char* buffer);
void item_pickup(Entity* entity, Item* item);
void item_pickup_all(Entity* entity, Position position);
void item_drop(Entity* owner, Item* item);

// fov.c functions
void make_fov(Entity* player);
void clear_fov(Entity* player);
int get_distance(Position origin, Position target);
int is_in_map(int y, int x);
int line_of_sight(Position origin, Position target);
int sign(int a);

// log.c functions
void init_log(void);
void init_sidelog(void);
void init_file_log(void);
void init_inventory_log(void);
void file_log(char* string);
void add_log_entry(char* text);
void add_sidelog_entry(char* text);
void sidelog_add_entity(Entity* entity);
void sidelog_remove_entity(Entity* entity);
void sidelog_print_entity(Entity* entity);
void sidelog_update(void);
void sidelog_clear(void);
void inventory_log_add(Item* item);
void inventory_log_remove(Item* item);

// level.c functions
void init_monsters(void);
void process_ai(void);
void move_monster(Entity* monster, Position new_pos);
void attack(Entity* attacker, Entity* target);
Entity* spawn(char* name, int y, int x);

// pathfinding.c functions
void init_nodes(void);
int calculate_heuristic(Position current, Position target);
int find_path(Position start, Position target);
int determine_direction(int current_y, int current_x, int next_y, int next_x);

// player.c functions
Entity* create_player(Position pos);
void change_player_speed(float new_speed);
void handle_input(int input);
void move_player(Position new_pos);
int is_entity_on_pos(Position pos);
int is_item_on_pos(Position pos);
Entity* get_entity_from_pos(Position pos);
Item* get_item_from_pos(Position pos);

// terminal.c functions
void terminal_input(char* buffer);
void terminal_execute(char* buffer);

// save.c functions
void init_save(void);
void save(void);
void load(void);


// externs
extern const int MAP_HEIGHT;
extern const int MAP_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int WINDOW_WIDTH;
extern float speed_modifier; // initialized in create_player()
extern Tile** map;
extern Node** nodes;
extern Entity* player;
extern Entity** monsters;
extern Log_entry* game_log;
extern char** sidelog;
extern Item** items; // items currently on the map
extern char** inventory_log;
extern Position start_pos;

#endif