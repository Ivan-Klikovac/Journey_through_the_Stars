#include <jtts.h>

Node** nodes;

void init_nodes(void)
{
    nodes = calloc(MAP_HEIGHT, sizeof(Node*));

    for(int y = 0; y < MAP_HEIGHT; y++)
    {
        nodes[y] = calloc(MAP_WIDTH, sizeof(Node));
        for(int x = 0; x < MAP_WIDTH; x++)
        {
            if(map[y][x].walkable)
            {
                nodes[y][x].pos.y = y;
                nodes[y][x].pos.x = x;
                nodes[y][x].parent = NULL;
                nodes[y][x].heuristic = calculate_heuristic(nodes[y][x].pos, player->pos);
            }
        }
    }
}

int calculate_heuristic(Position current, Position target)
{
    return abs(target.y - current.y) + abs(target.x - current.x);
}

int find_path(Position start, Position target) // greedy best-first search
{
    int direction = -1; // init to an invalid direction

    if(start.y >= 0 && start.y < MAP_HEIGHT && start.x >= 0 && start.x < MAP_WIDTH)
    {
        //FILE* log = fopen("log.txt", "w");
        //fprintf(log, "current coords: [%d][%d]\n", start.y, start.x);
        //fclose(log);

        Node* current = &nodes[start.y][start.x];
        current->heuristic = calculate_heuristic(start, target);

        for(int dy = -1; dy <= 1; dy++)
        {
            for(int dx = -1; dx <= 1; dx++)
            {
                int ny = current->pos.y + dy;
                int nx = current->pos.x + dx;

                if(ny >= 0 && ny < MAP_HEIGHT && nx >= 0 && nx < MAP_WIDTH)
                {
                    nodes[ny][nx].heuristic = calculate_heuristic(nodes[ny][nx].pos, target);
                    if(direction == -1 || nodes[ny][nx].heuristic < nodes[current->pos.y][current->pos.x].heuristic)
                    {
                        direction = determine_direction(current->pos.y, current->pos.x, ny, nx);
                    }
                }
            }
        }
    }

    return direction;
}

int determine_direction(int current_y, int current_x, int next_y, int next_x)
{
    if(next_y < current_y) return DOWN;
    else if(next_y > current_y) return UP;
    else if(next_x < current_x) return LEFT;
    else if(next_x > current_x) return RIGHT;

    else return -1; // invalid direction
}