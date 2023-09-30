#include <jtts.h>

Entity* player;
Tile** map;
Position start_pos;

const int WINDOW_HEIGHT = 36;
const int WINDOW_WIDTH = 130;

int setup(void)
{
    initscr();
    noecho();
    curs_set(0);
    resize_term(WINDOW_HEIGHT, WINDOW_WIDTH);

    map = create_map_tiles();
    start_pos = setup_map();
    player = create_player(start_pos);
    
    init_nodes();
    init_monsters();
    init_log();
    init_sidelog();
    sidelog_add_entity(player);
    init_file_log();
    init_inventory_log();
    init_items();
    init_save();

    srand(time(NULL));

    if(has_colors())
    {
        start_color();

        init_pair(VISIBLE_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(SEEN_COLOR, COLOR_BLUE, COLOR_BLACK);
        init_pair(RED, COLOR_RED, COLOR_BLACK);

        return 1;
    }

    else
    {
        mvprintw(20, 50, "This terminal does not support colors. Press any key to exit...");
        getch();
        close();
        return 0;
    }
}

void loop(void)
{
    make_fov(player);
    draw();

    int c; // input

    while((c = getch()))
    {
        if(c == 27) // esc
            break;

        if(c == '/')
        {
            char* buffer = malloc(64);
            terminal_input(buffer);
            //add_log_entry(buffer);
            terminal_execute(buffer);
            draw();
            continue;
        }

        handle_input(c);
        draw();

        process_ai(); // process_ai runs draw() after each AI turn
        //draw();
    }
}

void close(void)
{
    endwin();
    free(player);
}