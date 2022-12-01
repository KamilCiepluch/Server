//
// Created by root on 11/24/22.
//

#ifndef SERVER_2_GAME_LOGIC_H
#define SERVER_2_GAME_LOGIC_H
#include <pthread.h>
#include "game_enums.h"
#include <stdlib.h>
#include <stdio.h>
#include "beast.h"
#include "create_array.h"
#include "buffer.h"
#define AFK_TIME 50
struct coordinates
{
    int x;
    int y;
};
struct player
{
    char player_type[6];
    struct coordinates coordinates;
    pid_t PID;
    int deaths;
    int carried_coins;
    int brought_coins;
    int playerID;
    enum effect effect;
    enum ConnectionStatus connectionStatus;
    char map[5][5];
    enum move_status  player_moved;
    enum MoveOptions player_move;

};
struct server_read_data
{
    pid_t serverPID;
    enum ConnectionStatus connectionStatus;
    unsigned  long long round_number;
    int playerID;
    struct coordinates coordinates;
    int deaths;
    int carried_coins;
    int brought_coins;
    char map[5][5];
};
struct server_write_data
{
    pid_t playerPID;
    enum MoveOptions move;
    enum ConnectionStatus status;
    unsigned long long rounds_to_exit;
};
struct shared_memory_slot
{
    char read_data[20];
    char write_data[20];
    char sem_name[20];
    enum ConnectionStatus status;
};
struct shared_memory_names
{
    struct shared_memory_slot slots[4];// Number of players
};
struct thread_task
{
    enum keyboard_option *option;
    struct beast *beast;
    char **map;
};
#define NUMBER_OF_COLS  51
#define NUMBER_OF_ROWS 25


const char* getEnumName(enum keyboard_option option);
void load_map( char **map,  int number_of_cols, int number_of_rows);
void map_copy(char **map_dest, char **map_src,int width, int height );
void game_init( struct player *player);
void send_map_to_player(struct player *player, char **map);
void take_coins(struct player *player,char **map_base);
void slowdown_player(struct player *player, char **map_base);
void spawn_player(struct player *player);
void kill_player(struct player *player);
void players_collision(struct player **players, int size, struct buffer *death_points);
void add_death_points_to_map(struct buffer *death_points, char **map);
void take_death_point(struct player *player,struct buffer *death_points);
void add_beasts_to_map(struct beast_buffer *beasts, char **map);
void update_beast_view(struct beast *beast, int end_c, int end_row, int minus);
void update_beast_map(struct beast *beast, char **map);
void add_beast_to_buffer(struct beast_buffer *beasts, char **map);
int beast_can_see_player(struct beast *beast);
void slowdown_beast(struct beast *beast, char **map_base);
enum MoveOptions where_is_player(struct beast *beast);
void move_beast(struct beast *beast, char **map);
void add_object(char **tab,enum options option);
void beast_kill_player(struct player **players,int size, struct beast *beast,struct buffer *death_points);
void player_visit_camp(struct player *player, char ** basic_map);
void move_player(struct player *player);
struct coordinates find_camp(char **map);
void *move_beast_thread(void *arg);


#endif //SERVER_2_GAME_LOGIC_H
