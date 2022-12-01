

#ifndef SERVER_2_GAME_ENUMS_H
#define SERVER_2_GAME_ENUMS_H
enum ConnectionStatus
{
    NotConnected,Connected, WaitingForData,ServerQuit
};
enum effect {SlowDown, Neutral,CanNotBeSlowDown};
enum options {COIN=1,TREASURE=10,LARGE_TREASURE=50};
enum MoveOptions {UP=0,DOWN=1,RIGHT=2,LEFT=3, STAY=4};
enum move_status {Moved, NotMoved};
enum keyboard_option{ADD_LARGE_TREASURE, ADD_TREASURE, ADD_COIN, ADD_BEAST, DO_NOTHING, QUIT_GAME};
#endif //SERVER_2_GAME_ENUMS_H
