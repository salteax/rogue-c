#include <stdlib.h>
#include <ncurses.h>

/*  #IDEEN#
    1. Player customization     => Farbe, Name, Symbol (aka Skin)
    2. Klasse                   => versch. Startwerte
*/

/*  #HOW TO COMPILE#
    gcc main.c -lncurses
*/

//test

typedef struct Room {
    int yPos;
    int xPos;
    int height;
    int width;
} Room;

typedef struct Player {
    int yPos;
    int xPos;
    int health;
    char* symbol;
    //Room* room;
} Player;

int screenSetup();
Room** mapSetup();
Room* createRoom(int y, int x, int height, int width);
int drawRoom(Room* room);
Player* playerSetup();
int handleInput(int input, Player* player);
int checkPostion(int newY, int newX, Player* player);
int playerMove(int y, int x, Player* player);

int main() {
    //initialisation
    Player* player;
    int ch;

    //Game setup
    screenSetup();
    mapSetup();
    player = playerSetup();

    //Game loop
    while((ch = getch())!= 'q') {
        handleInput(ch, player);
    }
    endwin();   //ends ncurses sesion after exiting

    return 0;
}

int screenSetup() {
    initscr();
    printw("Hello World!");
    noecho();
    refresh();

    return 0;
}

Room** mapSetup() {
    Room ** rooms;
    rooms = malloc(sizeof(Room)*6);

    rooms[0] = createRoom(13, 13, 6, 8);
    rooms[1] = createRoom(2, 40, 6, 8);
    rooms[2] = createRoom(20, 40, 6, 12);

    drawRoom(rooms[0]);
    drawRoom(rooms[1]);
    drawRoom(rooms[2]);

    return 0;
}

Room * createRoom(int y, int x, int height, int width) {
    Room* newRoom;
    newRoom = malloc(sizeof(Room));

    newRoom->yPos = y;
    newRoom->xPos = x;
    newRoom->height = height;
    newRoom->width = width;

    return newRoom;
}

int drawRoom(Room* room) {

    for(int i = 0; i < room->height; i++) {
        for(int j = 0; j < room->width; j++) {
            if((i == 0) || (i == room->height-1)) {
                mvprintw(room->yPos+i, room->xPos+j, "-"); 
            } else {
                if((j == 0) || (j == room->width-1)) {
                    mvprintw(room->yPos+i, room->xPos+j, "|");
                } else {
                    mvprintw(room->yPos+i, room->xPos+j, ".");
                }
            }
        }
    }

    return 0;
}

Player* playerSetup() {
    Player* newPlayer;
    newPlayer = malloc(sizeof(Player));

    newPlayer->yPos = 14;
    newPlayer->xPos = 14;
    newPlayer->health = 20;
    newPlayer->symbol = "@";

    playerMove(newPlayer->yPos, newPlayer->xPos, newPlayer);

    return newPlayer; 
}

int handleInput(int input, Player* player) {
    int newY;
    int newX;
    
    switch(input) {
        //move up
        case 'w':
        case 'W':
            newY = player->yPos-1;
            newX = player->xPos;
            break;
        //move down
        case 's':
        case 'S':
            newY = player->yPos+1;
            newX = player->xPos;
            break;
        //move left
        case 'a':
        case 'A':
            newY = player->yPos;
            newX = player->xPos-1;
            break;
        //move right
        case 'd':
        case 'D':
            newY = player->yPos;
            newX = player->xPos+1;
            break;
        default:
            break;
    }

    checkPostion(newY, newX, player);

    return 0;
}

//check what is at next position
int checkPostion(int newY, int newX, Player* player) {
    int space;
    switch(mvinch(newY, newX)) {
        case '.':
            playerMove(newY, newX, player);
            break;
        default:
            move(player->yPos, player->xPos);
            break;
    }

    return 0;
}

int playerMove(int y, int x, Player* player) {
    mvprintw(player->yPos, player->xPos, ".");
    
    player->yPos = y;
    player->xPos = x;
    
    mvprintw(player->yPos, player->xPos, player->symbol);
    move(player->yPos, player->xPos);

    return 0;
}