#include "create_array.h"
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>
#include  <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <ncurses.h>
#include "buffer.h"
#include "beast.h"
#include "game_enums.h"
#include "game_logic.h"
#define MAX_NUMBER_OF_BEASTS 25
#define MAX_NUMBER_OF_DEATH_POINTS 25

int PLAYERS_LIMIT_TOTAL = 4;
int Current_Number_of_beasts  = 5;

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
void print_player_info(struct player *player, int x, int y)
{

    start_color();
    init_pair(6,COLOR_BLACK,COLOR_WHITE);
    attron(COLOR_PAIR(6));
    if(player->connectionStatus == Connected)
    {

        mvprintw(y+1,x,"%d",player->PID);
        mvprintw(y+2,x,"%s",player->player_type);
        mvprintw(y+3,x,"%02d/%02d",player->coordinates.x,player->coordinates.y);
        mvprintw(y+4,x,"%d",player->deaths);

        mvprintw(y+8,x,"%d",player->carried_coins);
        mvprintw(y+9,x,"%d",player->brought_coins);
    }
    else
    {
        mvprintw(y+1,x,"-");
        mvprintw(y+2,x,"-");
        mvprintw(y+3,x,"--/--");
        mvprintw(y+4,x,"-");
    }
    attroff(COLOR_PAIR(6));
}
void print_players_info(struct player **players, int number_of_players, int start_x, int start_y)
{
    int x = 0;
    start_color();
    init_pair(8,COLOR_BLACK,COLOR_WHITE);

    char number = '1';
    for(int i=0; i<number_of_players; i++)
    {
        if(players[i]->connectionStatus == Connected)
        {
            attron(COLOR_PAIR(8));
            mvprintw(start_y,start_x+x, "%s%c", "Player",number);
            attroff(COLOR_PAIR(8));
            print_player_info(players[i],start_x +x , start_y);
        }
        x += 10;
        number++;
    }

}
void print_server_info(pid_t PID, struct coordinates camp,int start_x, int start_y)
{
    start_color();
    init_pair(8,COLOR_BLACK,COLOR_WHITE);
    attron(COLOR_PAIR(8));
    for(int i=0; i<2; i++)
    {
        for(int j=-2; j<50; j++)
        {
            mvprintw(start_y+1+i,start_x+1+j," ");
        }
    }
    mvprintw(start_y+1,start_x+1,"Server's PID: %d", PID);
    mvprintw(start_y+2,start_x+2,"Campsite X/Y: %02d/%02d", camp.x, camp.y );
    attroff(COLOR_PAIR(8));
}
void print_stats(struct player **players, int number_of_players, unsigned long long round, int start_x, int start_y)
{
    start_color();
    init_pair(8,COLOR_BLACK,COLOR_WHITE);
    attron(COLOR_PAIR(8));
    for(int i=0; i<23; i++)
    {
        for(int j=-2; j<50; j++)
        {
            mvprintw(start_y+3+i,start_x+1+j," ");
        }
    }


    mvprintw(start_y+3,start_x+2,"Round number: %llu",round );


    mvprintw(start_y+6,start_x+1,"Parameter:");
    mvprintw(start_y+7,start_x+2,"PID");
    mvprintw(start_y+8,start_x+2,"Type");
    mvprintw(start_y+9,start_x+2,"CURR X/Y");
    mvprintw(start_y+10,start_x+2,"Deaths");

    mvprintw(start_y+13,start_x+1,"Coins");
    mvprintw(start_y+14,start_x+5,"carried");
    mvprintw(start_y+15,start_x+5,"brought");


    mvprintw(start_y+18,start_x+1,"Legend:");
    mvprintw(start_y+19,start_x+7,"- players");
    mvprintw(start_y+20,start_x+7,"- wall");
    mvprintw(start_y+21,start_x+7,"- bushes (slow down)");
    mvprintw(start_y+22,start_x+7,"- enemy");
    mvprintw(start_y+23,start_x+7,"- one coin");
    mvprintw(start_y+23,start_x+31,"- dropped treasure");
    mvprintw(start_y+24,start_x+7,"- treasure (10 coin)");
    mvprintw(start_y+25,start_x+7,"- large treasure (50 coin)");
    mvprintw(start_y+22,start_x+31,"- campsite ");
    attroff(COLOR_PAIR(8));

    init_pair(9,COLOR_WHITE,COLOR_MAGENTA);
    attron(COLOR_PAIR(9));
    mvprintw(start_y+19,start_x+2,"1234");
    attroff(COLOR_PAIR(9));

    init_pair(10,COLOR_BLACK,COLOR_BLACK);
    attron(COLOR_PAIR(10));
    mvprintw(start_y+20,start_x+2,"|");
    attroff(COLOR_PAIR(10));

    init_pair(11,COLOR_BLACK,COLOR_WHITE);
    attron(COLOR_PAIR(11));
    mvprintw(start_y+21,start_x+2,"#");
    mvprintw(start_y+22,start_x+2,"*");
    attroff(COLOR_PAIR(11));

    init_pair(12,COLOR_BLACK,COLOR_YELLOW);
    attron(COLOR_PAIR(12));
    mvprintw(start_y+23,start_x+2,"c");
    mvprintw(start_y+24,start_x+2,"t");
    mvprintw(start_y+25,start_x+2,"T");
    mvprintw(start_y+23,start_x+29, "D");
    attroff(COLOR_PAIR(12));

    init_pair(13,COLOR_YELLOW,COLOR_GREEN);
    attron(COLOR_PAIR(13));
    mvprintw(start_y+22,start_x+29,"A");
    attroff(COLOR_PAIR(13));
    print_players_info(players,number_of_players,start_x+14,start_y+6);
    move(0,0);
}
void *keyborad_listener(void *arg)
{
    enum keyboard_option *selected_option = (enum keyboard_option *) arg;
    while (1)
    {
        int option = getch();
        if(*selected_option==DO_NOTHING)
        {
            if(option == 'q')
            {
                *selected_option = QUIT_GAME;
                break;
            }
            switch (option) {
                case  't':
                {
                    *selected_option = ADD_TREASURE;
                    break;
                }
                case 'T':
                {
                    *selected_option = ADD_LARGE_TREASURE;
                    break;
                }
                case 'c':
                {
                    *selected_option = ADD_COIN;
                    break;
                }
                case 'b':
                {
                    *selected_option = ADD_BEAST;
                    break;
                }
                default:
                {
                    *selected_option = DO_NOTHING;
                }
            }
        }

    }
    return NULL;
}
void unlock_player_move(struct player *player)
{
        player->player_moved = NotMoved;
}
void try_to_unlink_everything(char **sem_names, int size_sem, char **shm_names_read, char **shm_names_write, int size_sh)
{
    for(int i=0; i<size_sh; i++)
    {
        shm_unlink(shm_names_read[i]);
        shm_unlink(shm_names_write[i]);
    }
    for(int i=0; i<size_sem; i++)
    {
        sem_unlink(sem_names[i]);
    }

    shm_unlink("players_list");
    sem_unlink("my_sem");

}
void init_player(struct player *player)
{
    if(player == NULL) return;
    player->connectionStatus = NotConnected;
    player->PID = 0;
    player->coordinates.x =0;
    player->coordinates.y =0;
    player->deaths =0;
    player->carried_coins =0;
    player->brought_coins =0;
    player->effect = Neutral;
    player->player_moved = NotMoved;
    player->player_move = STAY;
    strcpy(player->player_type, "HUMAN");
}
void kill_zombie(struct shared_memory_names *names,struct player **players,struct server_read_data **player_read,struct server_write_data **player_write,  int index)
{
    names->slots[index].status = NotConnected;
    init_player(players[index]);
    player_read[index]->serverPID = 0;
    player_read[index]->connectionStatus = NotConnected;
    player_read[index]->round_number = 0;
    player_read[index]->playerID = 0;
    player_read[index]->coordinates.x = 0;
    player_read[index]->coordinates.y = 0;
    player_read[index]->deaths = 0;
    player_read[index]->carried_coins = 0;
    player_read[index]->brought_coins = 0;

    player_write[index]->playerPID = 0;
    player_write[index]->move = STAY;
    player_write[index]->status = NotConnected;
    player_write[index]->rounds_to_exit = 0;

}

int main()
{
    char *sem_names[4];
    sem_names[0] = "sem1";
    sem_names[1] = "sem2";
    sem_names[2] = "sem3";
    sem_names[3] = "sem4";


    char *shm_names_read[4];
    shm_names_read[0] = "player1_read";
    shm_names_read[1] = "player2_read";
    shm_names_read[2] = "player3_read";
    shm_names_read[3] = "player4_read";


    char *shm_names_write[4];
    shm_names_write[0] = "player1_write";
    shm_names_write[1] = "player2_write";
    shm_names_write[2] = "player3_write";
    shm_names_write[3] = "player4_write";


    //todo poprawic
    try_to_unlink_everything(sem_names,4,shm_names_read,shm_names_write,4);


    // new named sem
    sem_t *sem = sem_open("my_sem", O_CREAT | O_EXCL, 0777,1 );
    if(sem == SEM_FAILED)
    {
        printf("Failed to open sem\n");
        return -1;
    }

    sem_wait(sem);
    int shm_all_process = shm_open("players_list", O_CREAT | O_EXCL | O_RDWR,0777);
    sem_post(sem);
    if(shm_all_process == -1 )
    {
        printf("Failed to open shared memory\n");
        shm_unlink("players_list");
        sem_close(sem);
        sem_unlink("my_sem");
        return -2;
    }

    if(ftruncate(shm_all_process, sizeof(struct shared_memory_names))!=0) // przypisanie pamięci
    {
        printf("truncate failed\n");
        close(shm_all_process);
        shm_unlink("players_list");
        sem_close(sem);
        sem_unlink("my_sem");
        return -3;
    }

    sem_wait(sem);
    struct shared_memory_names *names =mmap(NULL, sizeof(struct shared_memory_names), PROT_READ | PROT_WRITE, MAP_SHARED,shm_all_process,0);
    if(names == MAP_FAILED)
    {
        sem_post(sem);
        printf("Map failed");
        close(shm_all_process);
        shm_unlink("players_list");
        sem_close(sem);
        sem_unlink("my_sem");
        return -4;
    }
    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
        names[i].slots->status = NotConnected;
        strcpy( names->slots[i].read_data, shm_names_read[i]);
        strcpy( names->slots[i].write_data, shm_names_write[i]);
        strcpy( names->slots[i].sem_name, sem_names[i]);
    }
    sem_post(sem);


    //todo pamieci wspoldzelone
    struct player *players[PLAYERS_LIMIT_TOTAL];
    struct server_read_data *player_read[PLAYERS_LIMIT_TOTAL]; // pamiec ktora server wysyla
    struct server_write_data *player_write[PLAYERS_LIMIT_TOTAL]; // pamiec ktora server odbiera

    int shm_player_read[PLAYERS_LIMIT_TOTAL];
    int shm_player_write[PLAYERS_LIMIT_TOTAL];
    sem_wait(sem);
    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
        shm_player_read[i] = shm_open(shm_names_read[i], O_CREAT | O_EXCL | O_RDWR,0777);
        // todo check if fail
        if(shm_player_read[i] == -1)
        {
            printf("Failed to create players shared memory\n");
            for (int j = 0; j < i; ++j) {

                close(shm_player_read[j]);
                close(shm_player_write[j]);
                shm_unlink(shm_names_read[j]);
                shm_unlink(shm_names_write[j]);
            }
            munmap(names,sizeof(struct shared_memory_names));
            close(shm_all_process);
            shm_unlink("players_list");
            sem_close(sem);
            sem_unlink("my_sem");
            return -5;
        }

        shm_player_write[i] = shm_open(shm_names_write[i], O_CREAT | O_EXCL | O_RDWR,0777);
        if(shm_player_write[i] == -1)
        {
            printf("Failed to create players shared memory\n");
            for (int j = 0; j < i; ++j) {
                close(shm_player_read[j]);
                close(shm_player_write[j]);
                shm_unlink(shm_names_read[j]);
                shm_unlink(shm_names_write[j]);
            }
            munmap(names,sizeof(struct shared_memory_names));
            close(shm_player_read[i]);
            shm_unlink(shm_names_read[i]);
            close(shm_all_process);
            shm_unlink("players_list");
            sem_close(sem);
            sem_unlink("my_sem");
            return -5;
        }
    }

    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
        if(ftruncate(shm_player_read[i], sizeof(struct server_read_data))!=0) // przypisanie pamięci
        {
            printf("truncate struct players failed \n");
            for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
                    close(shm_player_read[j]);
                    close(shm_player_write[j]);
                    shm_unlink(shm_names_read[j]);
                    shm_unlink(shm_names_write[j]);
            }
            munmap(names,sizeof(struct shared_memory_names));
            close(shm_all_process);
            shm_unlink("players_list");
            sem_close(sem);
            sem_unlink("my_sem");
            return -6;
        }

        if(ftruncate(shm_player_write[i], sizeof(struct server_write_data))!=0)
        {
            printf("truncate struct players failed \n");
            for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
                close(shm_player_read[j]);
                close(shm_player_write[j]);
                shm_unlink(shm_names_read[j]);
                shm_unlink(shm_names_write[j]);
            }
            munmap(names,sizeof(struct shared_memory_names));
            close(shm_all_process);
            shm_unlink("players_list");
            sem_close(sem);
            sem_unlink("my_sem");
            return -6;
        }
    }

    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
         player_read[i] =mmap(NULL, sizeof(struct server_read_data), PROT_READ | PROT_WRITE, MAP_SHARED,shm_player_read[i],0);
         if(player_read[i] == MAP_FAILED)
         {
             printf("mmpa players failed\n");
             for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
                 close(shm_player_read[j]);
                 close(shm_player_write[j]);
                 shm_unlink(shm_names_read[j]);
                 shm_unlink(shm_names_write[j]);
                 if(j<i)
                 {
                     munmap(player_read[j],sizeof(struct server_read_data));
                 }
             }
             munmap(names,sizeof(struct shared_memory_names));
             close(shm_all_process);
             shm_unlink("players_list");
             sem_close(sem);
             sem_unlink("my_sem");
             return -7;
         }
    }


    for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
    {
        player_write[i] =mmap(NULL, sizeof(struct server_write_data ), PROT_READ | PROT_WRITE, MAP_SHARED,shm_player_write[i],0);
        if(player_write[i] == MAP_FAILED)
        {
            printf("mmpa players failed\n");
            for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
                close(shm_player_read[j]);
                close(shm_player_write[j]);
                shm_unlink(shm_names_read[j]);
                shm_unlink(shm_names_write[j]);
                if(j<i) {
                    munmap(player_write[j],sizeof(struct server_write_data));
                }
            }

            for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j)
            {
                munmap(player_read[j],sizeof(struct server_read_data));
            }
            munmap(names,sizeof(struct shared_memory_names));
            close(shm_all_process);
            shm_unlink("players_list");
            sem_close(sem);
            sem_unlink("my_sem");
            return -7;
        }
    }
    sem_post(sem);


    // todo make 4 sems for players - unused
    // if used check if sems are closed and unlinked
    // by default psems are free and unlkend so we must delete it when we decide to delete next line
    sem_t *psems[PLAYERS_LIMIT_TOTAL];
    for (int i = 0; i < PLAYERS_LIMIT_TOTAL; ++i) {
        psems[i]=  sem_open(sem_names[i], O_CREAT | O_EXCL, 0777,1 );
        if(psems[i] == SEM_FAILED)
        {
            printf("open sems for players failed\n");
            for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
                close(shm_player_read[j]);
                close(shm_player_write[j]);
                shm_unlink(shm_names_read[j]);
                shm_unlink(shm_names_write[j]);

                munmap(player_write[j],sizeof(struct server_write_data));
                munmap(player_read[j],sizeof(struct server_read_data));
                if(j<i)
                {
                    sem_close(psems[j]);
                    sem_unlink(sem_names[j]);
                }
            }
            munmap(names,sizeof(struct shared_memory_names));
            close(shm_all_process);
            shm_unlink("players_list");
            sem_close(sem);
            sem_unlink("my_sem");
            return -8;
        }
    }


    char **map_base;
    char **map_interactive;

    //todo free memory
    struct buffer *death_points = create_buffer(MAX_NUMBER_OF_DEATH_POINTS);
    if(death_points == NULL)
    {
        printf("create death_points failed\n");
        for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
            close(shm_player_read[j]);
            close(shm_player_write[j]);
            shm_unlink(shm_names_read[j]);
            shm_unlink(shm_names_write[j]);

            munmap(player_write[j],sizeof(struct server_write_data));
            munmap(player_read[j],sizeof(struct server_read_data));
            sem_close(psems[j]);
            sem_unlink(sem_names[j]);

        }
        munmap(names,sizeof(struct shared_memory_names));
        close(shm_all_process);
        shm_unlink("players_list");
        sem_close(sem);
        sem_unlink("my_sem");
        return -9;
    }

    struct beast_buffer *beasts = create_beast_buffer(MAX_NUMBER_OF_BEASTS);
    if(beasts == NULL)
    {

        printf("create beasts_buffer failed\n");
        free_buffer(&death_points);
        for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
            close(shm_player_read[j]);
            close(shm_player_write[j]);
            shm_unlink(shm_names_read[j]);
            shm_unlink(shm_names_write[j]);

            munmap(player_write[j],sizeof(struct server_write_data));
            munmap(player_read[j],sizeof(struct server_read_data));

            sem_close(psems[j]);
            sem_unlink(sem_names[j]);

        }
        munmap(names,sizeof(struct shared_memory_names));
        close(shm_all_process);
        shm_unlink("players_list");
        sem_close(sem);
        sem_unlink("my_sem");
        return -10;
    }
    //game init


    //todo inicjalizacja bestii jako watek
    sem_wait(sem);
    if(create_array_2d_2(&map_base,NUMBER_OF_COLS,NUMBER_OF_ROWS)!=0)
    {
        printf("create map failed\n");
        free_beast_buffer(&beasts);
        free_buffer(&death_points);
        for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
            close(shm_player_read[j]);
            close(shm_player_write[j]);
            shm_unlink(shm_names_read[j]);
            shm_unlink(shm_names_write[j]);

            munmap(player_write[j],sizeof(struct server_write_data));
            munmap(player_read[j],sizeof(struct server_read_data));
            sem_close(psems[j]);
            sem_unlink(sem_names[j]);

        }
        munmap(names,sizeof(struct shared_memory_names));
        close(shm_all_process);
        shm_unlink("players_list");
        sem_close(sem);
        sem_unlink("my_sem");
        return -11;
    }// create map
    if(create_array_2d_2(&map_interactive, NUMBER_OF_COLS,NUMBER_OF_ROWS)!=0)
    {
        printf("create map failed\n");
        destroy_array_2d(&map_base,NUMBER_OF_ROWS);
        free_beast_buffer(&beasts);
        free_buffer(&death_points);
        for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
            close(shm_player_read[j]);
            close(shm_player_write[j]);
            shm_unlink(shm_names_read[j]);
            shm_unlink(shm_names_write[j]);

            munmap(player_write[j],sizeof(struct server_write_data));
            munmap(player_read[j],sizeof(struct server_read_data));
            sem_close(psems[j]);
            sem_unlink(sem_names[j]);

        }
        munmap(names,sizeof(struct shared_memory_names));
        close(shm_all_process);
        shm_unlink("players_list");
        sem_close(sem);
        sem_unlink("my_sem");
        return -12;
    }

    load_map(map_base,NUMBER_OF_COLS,NUMBER_OF_ROWS);          // load map
    load_map(map_interactive,NUMBER_OF_COLS,NUMBER_OF_ROWS);


    for(int i=0; i<PLAYERS_LIMIT_TOTAL ;i++)
    {
        players[i] = calloc(1, sizeof(struct player));
        if(players[i]==NULL)
        {
            printf("can't allocate player memory\n");
            for(int j=0; j<i; j++)
            {
                free(players[j]);
            }
            destroy_array_2d(&map_interactive, NUMBER_OF_ROWS);
            destroy_array_2d(&map_base,NUMBER_OF_ROWS);
            free_beast_buffer(&beasts);
            free_buffer(&death_points);
            for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
                close(shm_player_read[j]);
                close(shm_player_write[j]);
                shm_unlink(shm_names_read[j]);
                shm_unlink(shm_names_write[j]);

                munmap(player_write[j],sizeof(struct server_write_data));
                munmap(player_read[j],sizeof(struct server_read_data));
                sem_close(psems[j]);
                sem_unlink(sem_names[j]);
            }
            munmap(names,sizeof(struct shared_memory_names));
            close(shm_all_process);
            shm_unlink("players_list");
            sem_close(sem);
            sem_unlink("my_sem");

            return 15;



        }
    }
    for (int i = 0; i < PLAYERS_LIMIT_TOTAL; ++i) {
        game_init(players[i]);
        players[i]->playerID = i;
    }

    pthread_t beast_thread[MAX_NUMBER_OF_BEASTS];
    struct thread_task task[MAX_NUMBER_OF_BEASTS];
    for(int i=0; i<MAX_NUMBER_OF_BEASTS; i++)
    {
        task[i].map = map_interactive;
        task[i].beast = beasts->array[i];
    }
    sem_post(sem);




 // todo init screen
    initscr();
    cbreak();
    noecho();

    int HEIGHT = 5;
    int WIDTH = 15;
    WINDOW  *game_window  = newwin(HEIGHT,WIDTH,0,0);
    keypad(game_window,true);
    box(game_window, 0, 0);

    int Number_of_players;


    pthread_t listener;
    enum keyboard_option selected_option = DO_NOTHING;
    pthread_create(&listener,NULL,keyborad_listener, (void *) &selected_option);

    for(int i=0; i<Current_Number_of_beasts; i++)
    {
        add_beast_to_buffer(beasts,map_interactive);
        task[beasts->current_size-1].beast = beasts->array[beasts->current_size-1];
        task[beasts->current_size-1].option = &selected_option;
    }
    printf("ilosc beastii: %d\n", beasts->current_size);


    Current_Number_of_beasts = beasts->current_size;
    for(int i=0; i<Current_Number_of_beasts; i++)
    {
        if(pthread_create(&beast_thread[i],NULL,move_beast_thread, (void *) &task[i])!=0)
            printf("Error\n");
    }


    unsigned  long long round = 0;
    sem_t sem_beast;
    sem_init(&sem_beast,0,1);

    while ( selected_option!=QUIT_GAME) {

        switch (selected_option) {
            case ADD_COIN:
            {
                add_object(map_base,COIN);
                selected_option = DO_NOTHING;
                break;
            }
            case ADD_TREASURE:
            {
                add_object(map_base,TREASURE);
                selected_option = DO_NOTHING;
                break;
            }
            case ADD_LARGE_TREASURE:
            {
                add_object(map_base,LARGE_TREASURE);
                selected_option = DO_NOTHING;
                break;
            }
            case ADD_BEAST:
            {
                //todo dodawac nowy watek
                add_beast_to_buffer(beasts,map_interactive);
                task[beasts->current_size-1].beast = beasts->array[beasts->current_size-1];
                task[beasts->current_size-1].option = &selected_option;
                selected_option = DO_NOTHING;
                if(pthread_create(&beast_thread[beasts->current_size-1],NULL,move_beast_thread, (void *) &task[beasts->current_size-1])!=0)
                    printf("Error\n");

                Current_Number_of_beasts = beasts->current_size;
                break;
            }
            default:
                selected_option = DO_NOTHING;
        }
        Number_of_players = 0;
        sem_wait(sem);
        map_copy(map_interactive,map_base,NUMBER_OF_COLS,NUMBER_OF_ROWS);
        add_beasts_to_map(beasts,map_interactive);
        sem_post(sem);

        sem_wait(sem);
        for(int i=0; i<PLAYERS_LIMIT_TOTAL; i++)
        {
            if(player_write[i]->rounds_to_exit ==0)
                kill_zombie(names,players,player_read,player_write,i);
        }
        sem_post(sem);
        for (int i = 0; i <4 ; ++i)
            sem_wait(psems[i]);

        for (int i = 0; i <4 ; ++i) {
            if(player_write[i]->status == Connected)
            {
                players[i]->player_move = player_write[i]->move;
                player_write[i]->move = STAY;
                move_player(players[i]);
            }
        }
        for (int i = 0; i <4 ; ++i) {
            if(player_write[i]->status == Connected)
            {
                Number_of_players++;
                int x,y;
                for(int j=0; j<Current_Number_of_beasts; j++)
                    beast_kill_player(players,Number_of_players,beasts->array[j],death_points);

                player_visit_camp(players[i],map_base);
                take_coins(players[i],map_base);
                take_death_point(players[i],death_points);
                slowdown_player(players[i],map_base);

                players_collision(players,PLAYERS_LIMIT_TOTAL,death_points);
                x= players[i]->coordinates.x;
                y= players[i]->coordinates.y;
                add_death_points_to_map(death_points,map_interactive);

                if(player_write[i]->rounds_to_exit>0)
                    player_write[i]->rounds_to_exit--;

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
                    default: break;
                }
            }
            sem_wait(sem);
            if(players[i]->connectionStatus == NotConnected && player_write[i]->status== WaitingForData)
            {

                player_write[i]->status = Connected;
                init_player(players[i]);
                spawn_player(players[i]);
                strcpy(players[i]->player_type, "HUMAN");
                player_read[i]->connectionStatus = Connected;
                player_read[i]->coordinates = players[i]->coordinates;
                player_read[i]->brought_coins = 0;
                player_read[i]->carried_coins = 0;
                player_read[i]->deaths =0;
                player_read[i]->playerID = players[i]->playerID;
                player_read[i]->round_number = round;
                player_read[i]->serverPID =getpid();
                player_write[i]->rounds_to_exit = AFK_TIME;
                memcpy(player_read[i]->map, players[i]->map, sizeof(players[i]->map));
                players[i]->PID = player_write[i]->playerPID;
                players[i]->connectionStatus = Connected;

            }

            if(players[i]->connectionStatus == Connected && player_write[i]->status == NotConnected)
            {
                players[i]->connectionStatus = NotConnected;
            }

            sem_post(sem);
        }


        for (int i = 0; i <4 ; ++i)
        {
            send_map_to_player(players[i],map_interactive);
            sem_post(psems[i]);
        }


        sem_wait(&sem_beast);
        for(int i=0; i<beasts->current_size; i++)
            update_beast_map(beasts->array[i],map_interactive);



        sem_wait(sem);
        print_window(map_interactive,NUMBER_OF_COLS,NUMBER_OF_ROWS);
        print_server_info(getpid(), find_camp(map_base),NUMBER_OF_COLS + 7, 4);
        print_stats(players,4,round,NUMBER_OF_COLS + 7, 4);
        refresh();
        for (int i = 0; i < Current_Number_of_beasts; ++i) {
            task[i].beast->beast_moved =0;
        }
        for(int i=0; i<4; i++)
        {
            if(players[i]->connectionStatus == Connected)
            {
                unlock_player_move(players[i]);
                player_read[i]->round_number = round;
                memcpy(player_read[i]->map, players[i]->map, sizeof(players[i]->map));
                player_read[i]->brought_coins = players[i]->brought_coins;
                player_read[i]->carried_coins = players[i]->carried_coins;
                player_read[i]->deaths = players[i]->deaths;
                player_read[i]->coordinates = players[i]->coordinates;
            }
        }
        sem_post(&sem_beast);
        round++;

        sem_post(sem);
        sleep(1);
    }
    endwin();


    printf("clearing data\n");

    for(int j=0; j<PLAYERS_LIMIT_TOTAL; j++)
    {
        if(player_read[j]->connectionStatus !=NotConnected)
        {
            player_read[j]->connectionStatus = ServerQuit;
        }
        free(players[j]);
    }
    destroy_array_2d(&map_interactive, NUMBER_OF_ROWS);
    destroy_array_2d(&map_base,NUMBER_OF_ROWS);
    free_beast_buffer(&beasts);
    free_buffer(&death_points);
    for (int j = 0; j < PLAYERS_LIMIT_TOTAL; ++j) {
        close(shm_player_read[j]);
        close(shm_player_write[j]);
        shm_unlink(shm_names_read[j]);
        shm_unlink(shm_names_write[j]);

        munmap(player_write[j],sizeof(struct server_write_data));
        munmap(player_read[j],sizeof(struct server_read_data));
        sem_close(psems[j]);
        sem_unlink(sem_names[j]);
    }
    munmap(names,sizeof(struct shared_memory_names));
    close(shm_all_process);
    shm_unlink("players_list");
    sem_close(sem);
    sem_unlink("my_sem");

    printf("Server finished \n");
    return 0;

}