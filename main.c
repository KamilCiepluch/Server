#include "create_array.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include  <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <ncurses.h>
#include <math.h>
#include "buffer.h"
#include "beast.h"
int PLAYERS_LIMIT_TOTAL = 4;
int NUMBER_OF_COLS= 51;
int NUMBER_OF_ROWS=25;



enum ConnectionStatus
{
    NotConnected,Connected
};
enum effect {SlowDown, Neutral,CanNotBeSlowDown};
enum options {COIN=1,TREASURE=10,LARGE_TREASURE=50};
enum MoveOptions {UP=0,DOWN=1,RIGHT=2,LEFT=3, STAY=4};
struct coordinates
{
    int x;
    int y;
};
struct player
{
    struct coordinates coordinates;
    pid_t PID;
    int deaths;
    int carried_coins;
    int brought_coins;
    int playerID;
    enum effect effect;
    enum ConnectionStatus connectionStatus;
    char map[5][5];
};
struct shared_memory_slot
{
    char player_name[20];
    char sem_name[20];
    enum ConnectionStatus status;
};
struct shared_memory_names
{
    struct shared_memory_slot slots[4];// Number of players
};


void print_window(char **tab, int number_of_cols, int number_of_rows)
{
    int x=5,y=5;

    start_color();
    init_pair(1,COLOR_RED,COLOR_RED);
    init_pair(2,COLOR_BLACK,COLOR_YELLOW);
    init_pair(3,COLOR_BLACK,COLOR_GREEN);
    init_pair(4,COLOR_BLACK,COLOR_MAGENTA);
    init_pair(5,COLOR_BLACK,COLOR_WHITE);
    for(int i=0; i<number_of_rows; i++)
    {
        for(int j=0; j<number_of_cols; j++)
        {
            if(tab[i][j] == '|')
            {
                attron(COLOR_PAIR(1));
                mvprintw(x+i,y+j,"%c",tab[i][j]);
                attroff(COLOR_PAIR(1));
            }
            else if(tab[i][j] == 'c' || tab[i][j] == 't' || tab[i][j] == 'T' || tab[i][j] == 'D')
            {
                attron(COLOR_PAIR(2));
                mvprintw(x+i,y+j,"%c",tab[i][j]);
                attroff(COLOR_PAIR(2));
            }
            else if(tab[i][j] == 'A' )
            {
                attron(COLOR_PAIR(3));
                mvprintw(x+i,y+j,"%c",tab[i][j]);
                attroff(COLOR_PAIR(3));
            }
            else if(tab[i][j] >= '1' && tab[i][j] <= '9')
            {
                attron(COLOR_PAIR(4));
                mvprintw(x+i,y+j,"%c",tab[i][j]);
                attroff(COLOR_PAIR(4));
            }
            else
            {
                attron(COLOR_PAIR(5));
                mvprintw(x+i,y+j,"%c",tab[i][j]);
                attroff(COLOR_PAIR(5));
            }
        }
    }
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
void load_map( char **map,  int number_of_cols, int number_of_rows)
{
    FILE *ptr=fopen("map.txt", "r");
    if(ptr==NULL) return;
    int r=0,c=0;

    while (!feof(ptr))
    {
        char ch;
        int x= fscanf(ptr,"%c",&ch);
        if(x!=1) return;
        if(ch!='\n' && ch!='\0')
        {
            if(r==number_of_rows) return;
            map[r][c] = ch;
            c++;
            if(c==number_of_cols)
            {
                c=0;
                r++;
            }

        }
    }
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
void add_beast_to_map(struct beast_buffer *beasts, char **map)
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
    if(beasts==NULL || map==NULL) return;

    int x=7,y=1;
    /*
    while (map[y][x]!=' ')
    {
        x=rand() % (NUMBER_OF_COLS-1) + 1;
        y=rand() % (NUMBER_OF_ROWS-1) + 1;
    }
     */
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

    beast->moveOption = RANDOM;
    //todo change beast_mode
    if(beast_can_see_player(beast) == 1)
    {
        beast->moveOption = CHASE_PLAYER;
    }
    int x,y;
    x=beast->x;
    y=beast->y;
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




int main()
{


    char *sem_names[4];
    sem_names[0] = "sem1";
    sem_names[1] = "sem2";
    sem_names[2] = "sem3";
    sem_names[3] = "sem4";

    char *shm_names[4];
    shm_names[0] = "player1";
    shm_names[1] = "player2";
    shm_names[2] = "player3";
    shm_names[3] = "player4";




    int shm_all_process = shm_open("players_list", O_CREAT | O_EXCL | O_RDWR,0777);

    sem_t *sem = sem_open("my_sem", O_CREAT | O_EXCL, 0777,1 );

    if(shm_all_process==-1)
    {
        printf("Failed to open shared memory\n");
        shm_unlink("players_list");
        return -1;
    }
    if(ftruncate(shm_all_process, sizeof(struct shared_memory_names))!=0) // przypisanie pamięci
    {
        printf("truncate failed\n");
        shm_unlink("players_list");
        return -2;
    }
    sem_wait(sem);
    struct shared_memory_names *names =mmap(NULL, sizeof(struct shared_memory_names), PROT_READ | PROT_WRITE, MAP_SHARED,shm_all_process,0);

    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
        names[i].slots->status = NotConnected;
        strcpy( names->slots[i].player_name, shm_names[i]);
        strcpy( names->slots[i].sem_name, sem_names[i]);
    }
    sem_post(sem);

    // new named samaphore
    //todo check if correct





    // Todo now we make shm for each player + sem

    sem_wait(sem);

    int shm_player[PLAYERS_LIMIT_TOTAL];
    struct player *players[PLAYERS_LIMIT_TOTAL];

    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
        shm_player[i] = shm_open(shm_names[i], O_CREAT | O_EXCL | O_RDWR,0777);
        // todo check if fail
    }

    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
        //todo zwalnianie zasobow
        if(ftruncate(shm_player[i], sizeof(struct player))!=0) // przypisanie pamięci
        {
            printf("truncate struct player failed \n");
            shm_unlink("players_list");
            return -2;
        }
    }

    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
         players[i] =mmap(NULL, sizeof(struct player), PROT_READ | PROT_WRITE, MAP_SHARED,shm_player[i],0);
         //todo check if fail
    }
    sem_post(sem);

    // todo make 4 sems

    sem_t *psems[PLAYERS_LIMIT_TOTAL];

    //todo check if correct
    for (int i = 0; i < PLAYERS_LIMIT_TOTAL; ++i) {
        psems[i]=  sem_open(sem_names[i], O_CREAT | O_EXCL, 0777,1 );
    }
    ///




    char **map_base;
    char **map_interactive;

    //todo free memory
    struct buffer *death_points = create_buffer(25);
    struct beast_buffer *beasts = create_beast_buffer(25);

    //game init
    sem_wait(sem);
    create_array_2d_2(&map_base,NUMBER_OF_COLS,NUMBER_OF_ROWS);// create map
    create_array_2d_2(&map_interactive, NUMBER_OF_COLS,NUMBER_OF_ROWS);

    load_map(map_base,NUMBER_OF_COLS,NUMBER_OF_ROWS);          // load map
    load_map(map_interactive,NUMBER_OF_COLS,NUMBER_OF_ROWS);
    for (int i = 0; i < PLAYERS_LIMIT_TOTAL; ++i) {
        game_init(players[i]);
    }
    sem_post(sem);


    //todo print map
    // display_array_2d(map_base,NUMBER_OF_COLS,NUMBER_OF_ROWS);
    initscr();
    int counter = 10;
    int Number_of_players = 0;

    for(int i=0; i<5; i++)
    add_beast_to_buffer(beasts,map_interactive);
    while ( counter>0 || Number_of_players!=0)
    {



        Number_of_players = 0;
        sem_wait(sem);
        map_copy(map_interactive,map_base,NUMBER_OF_COLS,NUMBER_OF_ROWS);
        for(int i=0; i<1; i++)
            move_beast(beasts->array[i],map_interactive);
        sem_post(sem);



        for (int i = 0; i <4 ; ++i) {
            sem_wait(psems[i]);
            if(players[i]->connectionStatus == Connected)
            {
                Number_of_players++;
                int x,y;

                beast_kill_player(players,Number_of_players,beasts->array[0],death_points);
                player_visit_camp(players[i],map_base);
                take_coins(players[i],map_base);
                take_death_point(players[i],death_points);
                slowdown_player(players[i],map_base);
                players_collision(players,PLAYERS_LIMIT_TOTAL,death_points);
                x= players[i]->coordinates.x;
                y= players[i]->coordinates.y;

                add_death_points_to_map(death_points,map_interactive);
                add_beast_to_map(beasts,map_interactive);
                send_map_to_player(players[i],map_interactive);


                switch (i) {
                    case 0:
                    {
                        map_interactive[y][x] ='1';
                        break;
                    }

                    case 1:
                    {
                        map_interactive[y][x] ='2';
                        break;
                    }
                    case 2:
                    {
                        map_interactive[y][x] ='3';
                        break;
                    }
                    case 3:
                    {
                        map_interactive[y][x] ='4';
                        break;
                    }
                }
            }

            sem_post(psems[i]);
        }
        for(int i=0; i<1; i++)
            update_beast_map(beasts->array[i],map_interactive);

        print_window(map_interactive,NUMBER_OF_COLS,NUMBER_OF_ROWS);
        refresh();


        counter--;
        sleep(1);
    }
    endwin();
    //detach from shared memory
    shmdt(names);
    shm_unlink("players_list");
    sem_close(sem);
    sem_unlink("my_sem");
    destroy_array_2d(&map_base,NUMBER_OF_ROWS);
    destroy_array_2d(&map_interactive,NUMBER_OF_ROWS);

    for (int i = 0; i <PLAYERS_LIMIT_TOTAL ; ++i) {

        sem_close(psems[i]);
        sem_unlink(sem_names[i]);
        shmdt(players[i]);
        shm_unlink(shm_names[i]);
    }


    return 0;

}