#include <jtts.h>

void terminal_input(char* buffer)
{
    char c;
    int i = 1;

    buffer[0] = '/';
    mvaddch(WINDOW_HEIGHT - 1, i, '/');

    while((c = getch()) != '\n')
    {
        if(i < 63) // AL ME ZAPLIVAVA
        {
            if(c == '\b')
            {
                mvaddch(WINDOW_HEIGHT - 1, i, ' ');
                i--;
                continue;
            }

            buffer[i++] = c;
            mvaddch(WINDOW_HEIGHT - 1, i, c);
        }

        else
        {
            add_log_entry("overflow");
            break;
        }
    }

    buffer[i] = '\0';
}

void terminal_execute(char* buffer)
{
    if(strcmp(buffer, "/help") == 0)
    {
        add_log_entry("uhhhhh...");
    }

    if(strcmp(buffer, "/todo") == 0)
    {
        add_log_entry("dropped items are drawn after 1 turn instead of instantly");
    }

    if(strcmp(buffer, "/save") == 0)
    {
        save();
    }

    if(strcmp(buffer, "/load") == 0)
    {
        load();
    }
}