//
// Created by root on 11/24/22.
//

#include "game_logic.h"
const char* getEnumName(enum keyboard_option option)
{
    switch (option)
    {
        case ADD_LARGE_TREASURE: return "ADD_LARGE_TREASURE";
        case ADD_TREASURE: return "ADD_TREASURE";
        case ADD_COIN: return "ADD_COIN";
        case ADD_BEAST: return "ADD_BEAST";
        case DO_NOTHING: return "DO_NOTHING";
        case QUIT_GAME: return "QUIT_GAME";

    }
}
void load_map( char **map,  int number_of_cols, int number_of_rows)
{
    FILE *ptr=fopen("map.txt", "r");
    if(ptr==NULL) return;
    int r=0,c=0;

    while (!feof(ptr))
    {
        char ch;
        int x= fscanf(ptr,"%c",&ch);
        if(x!=1)
        {
            fclose(ptr);
            return;
        }
        if(ch!='\n' && ch!='\0')
        {
            if(r==number_of_rows)
            {
                fclose(ptr);
                return;
            }
            map[r][c] = ch;
            c++;
            if(c==number_of_cols)
            {
                c=0;
                r++;
            }

        }
    }
    fclose(ptr);
}
void map_copy(char **map_dest, char **map_src,int width, int height )
{
    if(map_src==NULL || map_dest==NULL) return;
    if(width<1 || height<1) return;
    for(int i=0; i<height;i++)
    {
        for(int j=0; j<width; j++)
        {
            map_dest[i][j] = map_src[i][j];
        }
    }

}
void game_init( struct player *player)
{
    player->connectionStatus = NotConnected;
    player->PID= NotConnected;
    player->brought_coins=0;
    player->carried_coins=0;
    player->deaths=0;
    player->player_moved = NotMoved;
}
void send_map_to_player(struct player *player, char **map)
{
    int x,y;
    x= player->coordinates.x;
    y= player->coordinates.y;

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
            player->map[i][j] = '|';
    }
    int y1= -2;
    for(int i=0; i<5; i++)
    {   int x1=-2;
        for(int j=0; j<5; j++)
        {
            if(y+y1>=0 && y+y1<NUMBER_OF_ROWS && x+x1>=0 && x+x1<NUMBER_OF_COLS)
                player->map[i][j] = map[y+y1][x+x1];
            x1++;
        }
        y1++;
    }
    player->map[2][2] = 'p';
}
void take_coins(struct player *player,char **map_base)
{
    int x = player->coordinates.x;
    int y = player->coordinates.y;
    if(map_base[y][x] == 'c')
    {
        map_base[y][x] = ' ';
        player->carried_coins +=COIN;
    }
    if(map_base[y][x] == 't')
    {
        map_base[y][x] = ' ';
        player->carried_coins +=TREASURE;
    }
    if(map_base[y][x] == 'T')
    {
        map_base[y][x] = ' ';
        player->carried_coins +=LARGE_TREASURE;
    }
}
void slowdown_player(struct player *player, char **map_base)
{
    int x = player->coordinates.x;
    int y = player->coordinates.y;
    if(map_base[y][x] == '#' && player->effect==Neutral)
    {
        player->effect = SlowDown;
    }
}
void spawn_player(struct player *player)
{
    if(player==NULL) return;
    switch (player->playerID) {
        case 0:
        {
            player->coordinates.x = 9;
            player->coordinates.y =1;
            break;
        }
        case 1:
        {
            player->coordinates.x = 4;
            player->coordinates.y =3;
            break;
        }
        case 2:
        {
            player->coordinates.x = 17;
            player->coordinates.y =21;
            break;
        }
        case 3:
        {
            player->coordinates.x =27;
            player->coordinates.y =17;
            break;
        }
        default: return;
    }
}
void kill_player(struct player *player)
{
    player->carried_coins =0;
    player->effect = Neutral;
    player->deaths +=1;
    spawn_player(player);
}
void players_collision(struct player **players, int size, struct buffer *death_points)
{

    for(int i=0; i<size; i++)
    {
        int has_collision =0;
        int x = players[i]->coordinates.x;
        int y = players[i]->coordinates.y;

        for (int j = 0; j < size; ++j) {
            if(i!=j)
            {
                if(players[j]->connectionStatus == Connected)
                {
                    if(players[j]->coordinates.x == x && players[j]->coordinates.y == y)
                    {
                        add_point(death_points,x,y,players[j]->carried_coins);
                        kill_player(players[j]);
                        has_collision++;
                    }
                }
            }
        }
        if(has_collision)
        {
            add_point(death_points,x,y,players[i]->carried_coins);
            kill_player(players[i]);
        }
    }


}
void add_death_points_to_map(struct buffer *death_points, char **map)
{
    for (int i = 0; i < death_points->size; ++i) {
        if(death_points->array[i]==NULL) break;

        int x = death_points->array[i]->x;
        int y = death_points->array[i]->y;
        map[y][x] = 'D';
    }
}
void take_death_point(struct player *player,struct buffer *death_points)
{
    if(player==NULL || death_points==NULL) return;
    int x = player->coordinates.x;
    int y = player->coordinates.y;
    struct death_point* point = find_obj(death_points, x,y);
    if(point!=NULL)
    {
        player->carried_coins += point->stored_value;
        remove_point(death_points,x,y);
    }

}
void add_beasts_to_map(struct beast_buffer *beasts, char **map)
{
    for (int i = 0; i < beasts->size; ++i) {
        if(beasts->array[i]==NULL) break;

        int x = beasts->array[i]->x;
        int y = beasts->array[i]->y;
        map[y][x] = '*';
    }
}
void update_beast_view(struct beast *beast, int end_c, int end_row, int minus)
{
    for(int i=end_row-minus; i<end_row; i++)
    {
        for(int j=end_c- minus; j<end_c; j++)
            beast->map[i][j] = '|';
    }
}
void update_beast_map(struct beast *beast, char **map)
{
    if(beast==NULL || map ==NULL) return;
    int x,y;
    x= beast->x;
    y= beast->y;

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
            beast->map[i][j] = '|';
    }
    int y1= -2;
    for(int i=0; i<5; i++)
    {   int x1=-2;
        for(int j=0; j<5; j++)
        {
            if(y+y1>=0 && y+y1<NUMBER_OF_ROWS && x+x1>=0 && x+x1<NUMBER_OF_COLS)
                beast->map[i][j] = map[y+y1][x+x1];
            x1++;
        }
        y1++;
    }

    if(beast->map[2][2] == ' ')
    {
        beast->map[2][2] = '*';
    }


    // todo możliwe błędy

    if(beast->map[1][1]!=' ')
        update_beast_view(beast,2,2,2);
    if(beast->map[1][3]!=' ')
        update_beast_view(beast,5,2,2);

    if(beast->map[3][1]!=' ')
        update_beast_view(beast,3,5,2);
    if(beast->map[3][3]!=' ')
        update_beast_view(beast,5,5,2);

}
void add_beast_to_buffer(struct beast_buffer *beasts, char **map)
{
    if(beasts == NULL || map==NULL) return;


    int x=0,y=0;
    while (map[y][x]!=' ')
    {
        x= rand() % (NUMBER_OF_COLS-1) + 1;
        y= rand() % (NUMBER_OF_ROWS-1) + 1;
    }
    add_beast(beasts,x,y);

    struct beast *beast = find_beast(beasts,x,y);
    if(beast!=NULL)
    {
        update_beast_map(beast,map);
    }
}
int beast_can_see_player(struct beast *beast)
{

    if(beast == NULL) return -1;
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if(beast->map[i][j] >= '1' && beast->map[i][j] <= '9') return 1;
        }
    }
    return 0;

}
void slowdown_beast(struct beast *beast, char **map_base)
{
    int x = beast->x;
    int y = beast->y;
    if(map_base[y][x] == '#' && beast->effect==Neutral)
    {
        beast->effect = SlowDown;
    }
}
enum MoveOptions where_is_player(struct beast *beast)
{

    int x=-1;
    int y= -1;
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if(beast->map[i][j] >= '1' && beast->map[i][j] <= '9')
            {
                x= j;
                y= i;
                break;
            }
        }
    }

    if(x== -1 || y==-1) return STAY;

    // tutaj 2 jest tylko dlatego ze bestia na swojej mapie znajduje sie na srodku w pozycji x=2 y=2
    // rozpatrywany jest lokalny układ współrzędnych
    if(2>x) return LEFT;
    if(2<y) return DOWN;
    if(2<x) return RIGHT;
    if(2>y) return UP;

    return STAY;
}
void move_beast(struct beast *beast, char **map)
{
    if(beast==NULL || map==NULL) return;
    if(beast->effect == SlowDown)
    {
        beast->effect = CanNotBeSlowDown;
        return;
    }

    beast->moveOption = RANDOM;
    //todo change beast_mode
    if(beast_can_see_player(beast) == 1)
    {
        beast->moveOption = CHASE_PLAYER;
    }
    int x,y;
    x=beast->x;
    y=beast->y;
    srand(time(NULL));
    enum MoveOptions dir = rand() %4;

    if(beast->moveOption == CHASE_PLAYER)
        dir = where_is_player(beast);
    switch (dir) {
        case UP:
        {
            if(map[y-1][x] != '|')
                beast->y--;
            break;
        }
        case DOWN:
        {
            if(map[y+1][x] != '|')
                beast->y++;
            break;
        }
        case RIGHT:
        {
            if(map[y][x+1] != '|')
                beast->x++;
            break;
        }
        case LEFT:
        {
            if(map[y][x-1] != '|')
                beast->x--;
            break;
        }
        default:break;
    }
    beast->effect = Neutral;
    slowdown_beast(beast,map);
}
void add_object(char **tab,enum options option)
{
    int x=0,y=0;
    while (tab[y][x]!=' ')
    {
        x=rand() % (NUMBER_OF_COLS-1) + 1;
        y=rand() % (NUMBER_OF_ROWS-1) + 1;
    }
    char object;
    switch (option) {
        case COIN: {
            object='c';
            break;
        }
        case TREASURE:
        {
            object='t';
            break;
        }
        case LARGE_TREASURE:
        {
            object='T';
            break;
        }
        default: object=' ';
    }
    tab[y][x] = object;
}
void beast_kill_player(struct player **players,int size, struct beast *beast,struct buffer *death_points)
{
    for(int i=0; i<size; i++)
    {
        int x = players[i]->coordinates.x;
        int y = players[i]->coordinates.y;
        if(beast->x == x && beast->y == y)
        {
            add_point(death_points,x,y,players[i]->carried_coins);
            kill_player(players[i]);
        }
    }
}
void player_visit_camp(struct player *player, char ** basic_map)
{
    if(player == NULL || basic_map ==NULL) return;
    int x = player->coordinates.x;
    int y = player->coordinates.y;

    if(basic_map[y][x] == 'A')
    {
        player->brought_coins += player->carried_coins;
        player->carried_coins = 0;
    }
}
void move_player(struct player *player)
{
    if(player==NULL) return;
    if(player->player_moved == Moved) return;
    if(player->player_move== STAY) return;
    if(player->effect == SlowDown)
    {
        player->effect = CanNotBeSlowDown;
        return;
    }
    int x = 2;
    int y = 2;
    if(player->player_move == UP && player->map[y-1][x] !='|')
    {
        player->coordinates.y--;
        player->effect = Neutral;
    }
    else if(player->player_move == DOWN && player->map[y+1][x] !='|')
    {
        player->coordinates.y++;
        player->effect = Neutral;
    }
    else if (player->player_move == RIGHT && player->map[y][x+1] !='|')
    {
        player->coordinates.x++;
        player->effect = Neutral;
    }
    else if(player->player_move == LEFT && player->map[y][x-1] !='|')
    {
        player->coordinates.x--;
        player->effect = Neutral;
    }
    player->player_move = STAY;
    player->player_moved = Moved;
}
struct coordinates find_camp(char **map)
{
    struct coordinates c;
    c.x =0;
    c.y =0;
    for(int i=0; i<NUMBER_OF_ROWS; i++)
    {
        for(int j=0; j<NUMBER_OF_COLS; j++)
        {
            if(map[i][j] =='A')
            {
                c.y= i;
                c.x = j;
                return c;
            }
        }
    }
    return c;
}
void *move_beast_thread(void *arg)
{
    struct thread_task *ptr = (struct thread_task *) arg;
    if(ptr==NULL) return NULL;
    while (*ptr->option != QUIT_GAME)
    {
        if(ptr->beast->beast_moved ==0)
        {
            move_beast(ptr->beast, ptr->map);
            ptr->beast->beast_moved = 1;
        }
    }
    return NULL;
}
