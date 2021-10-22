#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

/*  #IDEEN#
    1. Player customization     => Farbe, Name, Symbol (aka Skin)
    2. Klasse                   => versch. Startwerte
*/

/*  #HOW TO COMPILE#
    gcc main.c -lncurses
*/

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Room {
    Position position;
    int height;
    int width;

    Position doors[4];
} Room;

typedef struct Player {
    Position position;
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
    while((ch = getch()) != 'q') {
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

    srand(time(NULL));

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

    newRoom->position.y = y;
    newRoom->position.x = x;
    newRoom->height = height;
    newRoom->width = width;

    //TODO: Türen variabel (1-4)
    //top
    newRoom->doors[0].y = newRoom->position.y;
    newRoom->doors[0].x = rand() % (width - 2) + newRoom->position.x + 1;
    //bottom
    newRoom->doors[1].y = newRoom->position.y + newRoom->height - 1;
    newRoom->doors[1].x = rand() % (width - 2) + newRoom->position.x + 1;
    //left
    newRoom->doors[2].y = rand() % (height - 2) + newRoom->position.y + 1;
    newRoom->doors[2].x = newRoom->position.x;
    //right
    newRoom->doors[3].y = rand() % (height - 2) + newRoom->position.y + 1;
    newRoom->doors[3].x = newRoom->position.x + width - 1;

    return newRoom;
}

int drawRoom(Room* room) {

    for(int i = 0; i < room->height; i++) {
        for(int j = 0; j < room->width; j++) {
            if((i == 0) || (i == room->height-1)) {
                mvprintw(room->position.y+i, room->position.x+j, "-"); 
            } else {
                if((j == 0) || (j == room->width-1)) {
                    mvprintw(room->position.y+i, room->position.x+j, "|");
                } else {
                    mvprintw(room->position.y+i, room->position.x+j, ".");
                }
            }
        }
    }

    //TODO: das muss später variabel sein mit 1-4 türen
    for(int i = 0; i < 4; i++) { 
        mvprintw(room->doors[i].y, room->doors[i].x, "+");
    }

    return 0;
}

Player* playerSetup() {
    Player* newPlayer;
    newPlayer = malloc(sizeof(Player));

    newPlayer->position.y = 14;
    newPlayer->position.x = 14;
    newPlayer->health = 20;
    newPlayer->symbol = "@";

    playerMove(newPlayer->position.y, newPlayer->position.x, newPlayer);

    return newPlayer; 
}

int handleInput(int input, Player* player) {
    int newY;
    int newX;
    
    switch(input) {
        //move up
        case 'w':
        case 'W':
            newY = player->position.y-1;
            newX = player->position.x;
            break;
        //move down
        case 's':
        case 'S':
            newY = player->position.y+1;
            newX = player->position.x;
            break;
        //move left
        case 'a':
        case 'A':
            newY = player->position.y;
            newX = player->position.x-1;
            break;
        //move right
        case 'd':
        case 'D':
            newY = player->position.y;
            newX = player->position.x+1;
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
            move(player->position.y, player->position.x);
            break;
    }

    return 0;
}

int playerMove(int y, int x, Player* player) {
    mvprintw(player->position.y, player->position.x, ".");
    
    player->position.y = y;
    player->position.x = x;
    
    mvprintw(player->position.y, player->position.x, player->symbol);
    move(player->position.y, player->position.x);

    return 0;
}