#include <jtts.h>

const int MAP_HEIGHT = 25;
const int MAP_WIDTH = 100;


int main(void)
{
    int compatible_terminal = setup();
    if(!compatible_terminal) { close(); return 0; }

    spawn("Rat", start_pos.y + 3, start_pos.x + 2);
    spawn("Rat", start_pos.y + 2, start_pos.x + 3);

    
    loop();

    close();

    return 0;
}