
/* 
 * File:   main.c
 * Author: Christopher King
 * Created on August 31, 2016, 11:00 AM
 * Assignment: Lab 1
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * 
 */
    typedef int bool;
        #define true 1
        #define false 0

    struct Creature{
        int type;
        int roomIn;
        int cleanPref; 
        int name;
    } ;
    
    struct Room{
        
        int name;
        int cleanState;
        int north;
        int south;
        int east;
        int west;  
    } ;
    
    int getStartUpData(struct Room *, struct Creature *, int *, int *);
    int gameLoop(int , struct Room *, struct Creature *);
    int displayRoomInfo(int , struct Room *, struct Creature *);
    int processInput(int *, int *);
    int performUserAction(int , int , int , struct Room *, struct Creature *, int *);
    int north(int , int , struct Room *, struct Creature *, int , int *);
    int south(int , int , struct Room *, struct Creature *, int , int *);
    int east(int , int , struct Room *, struct Creature *, int , int *);
    int west(int , int , struct Room *, struct Creature *, int , int *);
    int help();
    int exitGame(int );
    int updateGameState(struct Room *, struct Creature *,int *);   

int main(int argc, char** argv) {
   
    
    struct Room *rooms;
    struct Creature *creatures;
    int playerIndex; //The players location in the array of creatures
    int numC,numR; //the number of creatures and rooms
    
    
    //just in case////////////////////////
    struct Room *roomsPtr;              //
    struct Creature *creaturesPtr;      //
    //////////////////////////////////////
    
    rooms = (struct Room *)malloc(100*sizeof(struct Room));
    creatures = (struct Creature *)malloc(100*sizeof(struct Creature));
    
    //just for the free()////////////////
    roomsPtr = rooms;                  //
    creaturesPtr = creatures;          //
    /////////////////////////////////////
    
    playerIndex = getStartUpData(rooms, creatures, &numC, &numR);
    
    //start the game
    if(playerIndex != -1)
        gameLoop(playerIndex, rooms, creatures);
    
    
    free(roomsPtr);
    free(creaturesPtr);
    
    
    return (0);   
}
/*Reads in start up game data from stdin*/
int getStartUpData(struct Room *rooms, struct Creature *creatures, int *numC, int *numR){
    int numRooms;
    int numCreatures;
    int playerIndex;
    
    
    playerIndex = -1;
    //Clear out the Arrays
    for (int i = 0; i < 100; i++){
        //clear rooms
        (rooms + i)->cleanState = -1;
        (rooms + i)->name = -1;
        (rooms + i)->north = -1;
        (rooms + i)->south = -1;
        (rooms + i)->east = -1;
        (rooms + i)->west = -1;
        //clear creatures
        (creatures + i)->roomIn = -2;
        (creatures + i)->cleanPref -2;
        (creatures + i)->type = -2;
        (creatures + i)->name = -2;
                
    }
    
    
    
    //get numRooms 
    do{
        printf("Enter the number of rooms\n");
        printf(">");
        scanf("%d", &numRooms);
    } while(numRooms < 1 || numRooms > 100);
    //loop get room info numRooms times
    for (int i = 0; i < numRooms; i++){
        int state, north, south, east, west;
        //get the room info
        printf("\nEnter the : State, North Room, South Room, East Room, West Room\n");
        scanf("%d %d %d %d %d", &state, &north, &south, &east, &west);

        (rooms + i)->name = i;
        (rooms + i)->cleanState = state;
        (rooms + i)->north = north;
        (rooms + i)->south = south;
        (rooms + i)->east = east;
        (rooms + i)->west = west;
    }
    
    //get num creatures + error check
    do{
        printf("Enter the number of Creatures\n");
        printf(">");
        scanf("%d", &numCreatures);
    } while(numCreatures < 1 || numCreatures > 100);
    //loop creatures
    for (int i = 0; i < numCreatures; i++){
        int type, room;
        //get the room info
        printf("\nEnter the : Type, Room\n");
        scanf("%d %d", &type, &room);
        //set values based on type
        switch(type){
            //player
            case 0: 
                playerIndex = i;
                (creatures + i)->name = i;
                (creatures + i)->type = type;
                (creatures + i)->roomIn = room;
                (creatures + i)->cleanPref = -1;
                break;
            //animal
            case 1:
                (creatures + i)->name = i;
                (creatures + i)->type = type;
                (creatures + i)->roomIn = room;
                (creatures + i)->cleanPref = 0;
                break;
            //NPC
            case 2:
                (creatures + i)->name = i;
                (creatures + i)->type = type;
                (creatures + i)->roomIn = room;
                (creatures + i)->cleanPref = 2;
                break;
            default: break;  
        }
      
    }
    //if player was not created, creates one now
    //array could be full, must check
    if(playerIndex == -1){
        printf("You didnt create a player, you kinda need that. ");
        if(numCreatures < 100){
            playerIndex = 100;
            (creatures + numCreatures)->name = 100;
            (creatures + numCreatures)->type = 0;
            (creatures + numCreatures)->roomIn = 0;
            (creatures + numCreatures)->cleanPref = -1;
        } else{
            printf("You also didnt leave enough room in the house to do so...\n now i have to quit");
        }
    }
    
    
    *numR = numRooms;
    *numC = numCreatures;
    return playerIndex;
}

int gameLoop(int playerIndex, struct Room *rooms, struct Creature *creatures){
    int rep = 40;
    int running = true;
    int target = -1;
    int command = -1;
    int direction;
    
    while(running == true){
        
        processInput(&target, &command);
        running = performUserAction(target, command, playerIndex, rooms, creatures, &rep);
        if(running)
            running = updateGameState(rooms, creatures, &rep);
    }
    
}

int displayRoomInfo(int playerIndex, struct Room *rooms, struct Creature *creatures){

    int activeRoom = (creatures + playerIndex)->roomIn;
    printf("\nYou are currently in room %d. ", activeRoom );
    //display message based on room state
    switch((rooms + activeRoom)->cleanState){
        //clean
        case 0: 
            printf("This Room is clean. ");
            break;
        //Arnold Palmer(half and half))
        case 1: 
            printf("This Room is kinda Clean, kinda Dirty. \n");
            break;
        //dirty
        case 2: 
            printf("This Room is dirty. ");
            break;
        default: break;
        
    }
    //say what it neighbors
    if((rooms + activeRoom)->north != -1)
        printf("Door to %d to the North. ", (rooms + activeRoom)->north);
    if((rooms + activeRoom)->south != -1)
        printf("Door to %d to the South. ", (rooms + activeRoom)->south);
    if((rooms + activeRoom)->east != -1)
        printf("Door to %d to the East. ", (rooms + activeRoom)->east);
    if((rooms + activeRoom)->west != -1)
        printf("Door to %d to the West. ", (rooms + activeRoom)->west);
    
    //check to see if there are monsters in the room
    printf("\nCreatures in this room: ");
    for(int i = 0; i < 100; i++ ){
        if ((creatures + i)->roomIn == activeRoom){
            //if monsters are found, display based on their type
            switch((creatures + i)->type){
                case 1:
                    //Animal
                    printf("Animal %d ", (creatures + i)->name);
                    break;
                    //NPC
                case 2: 
                    printf("NPC %d ", (creatures + i)->name);
                    break;
                
                default: break;
                
            }
        }
    }
    printf("\n");
}

int processInput(int *targetNum, int *command){
    (*targetNum) = -1;
    (*command) = -1;
    int tempTarget;
    char str[100];
    char cmd[100];

    do{
        //gets(str);
        fgets(str, sizeof(str), stdin);
        if(sscanf(str, "%d:%s ", &tempTarget, cmd) != 2) {

            sscanf(str, "%s", cmd);
            tempTarget = -1;
        }

        *targetNum = tempTarget;

        //convert the cmd string into a command number
            //convert to lowercase
        if(strcmp(cmd, "help") == 0)
            *command = 0;
        else if(strcmp(cmd, "look") == 0)
            *command = 1;
        else if(strcmp(cmd, "dirty") == 0)
            *command = 2;
        else if(strcmp(cmd, "clean") == 0)
            *command = 3;
        else if(strcmp(cmd, "north") == 0)
            *command = 4;
        else if(strcmp(cmd, "south") == 0)
            *command = 5;
        else if(strcmp(cmd, "east") == 0)
            *command = 6;
        else if(strcmp(cmd, "west") == 0)
            *command = 7;
        else if(strcmp(cmd, "exit") == 0)
            *command = 8;
    } while (*command == -1);
    //printf("Command: %d . Target: %d", *command, *targetNum);
     
    return 0;
}
/*Most game logic happens within*/
int performUserAction(int target, int action, int playerIndex, struct Room *rooms, struct Creature *creatures, int *respect){
    //handle the creature grumbling 
    int keepPlaying = true; //continue by default
    int activeRoom = (creatures + playerIndex)->roomIn;
    int firstRandDir, secondRandDir, thirdRandDir, fourthRandDir;
    
    
    switch(action){
        //help
        case 0: 
            help();
            break;
        //look
        case 1: 
            displayRoomInfo(playerIndex, rooms, creatures);
            break;
        //dirty (2)
        case 2:       
            if((rooms + activeRoom)->cleanState < 2){
                (rooms + activeRoom)->cleanState++;
                //if the player is making an creature do it
                if(target != -1){
                   
                    printf("%d dirties the room. ", target);
                    //if the creature type is NPC, then happy alot
                    if((creatures + target)->type == 2){
                        *respect += 3;
                        printf("%d smiles a lot. Respect is now %d.\n", target, *respect);
                    //if the creature is an animal, then unhappy alot
                    } else if((creatures + target)->type == 1){
                        *respect -=3;
                        printf("%d growls a lot. Respect is now %d.\n", target, *respect);
                    } 
                } else {
                    printf("You dirty the room. ");
                }
                //loop through the rest of the creatures and happy/mad + move accordingly
                for(int i = 0; i < 100; i++ ){
                    if ((creatures + i)->roomIn == activeRoom ){
                        //if monsters are found, act based on their type
                        switch((creatures + i)->type){
                            case 1:
                                //Animal. Only animals would ever move on a dirty command
                                if((creatures + i)->name != target){
                                    *respect -=1;
                                    printf("Animal %d growls at you. Respect is now %d\n", (creatures + i)->name, *respect);
                                }
                                //if the state is dirty(2) then try to leave
                                if((rooms + activeRoom)->cleanState == 2){
                            // attempt to leave in a random direction
                            //assign the random order of doors to check
                                        firstRandDir = rand() % 4;//creates a random num between  0 and 3
                                    do{
                                        secondRandDir = rand() % 4;
                                    }while(secondRandDir == firstRandDir);
                                    do{
                                        thirdRandDir = rand() % 4;
                                    }while(thirdRandDir == secondRandDir || thirdRandDir == firstRandDir);
                                    do{
                                        fourthRandDir = rand() % 4;
                                    }while(fourthRandDir == firstRandDir || fourthRandDir == secondRandDir || fourthRandDir == thirdRandDir);
                                    
                                    //check the first door
                                    
                                    int foundNewRoom = false;
                                    
                                    switch(firstRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                    }
                                    //if not moved then try again
                                    if(foundNewRoom == false){
                                        switch(secondRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        }
                                    }
                                    if(foundNewRoom == false){
                                        switch(thirdRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        }
                                    }
                                    if(foundNewRoom == false){
                                        switch(fourthRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        }
                                    }
                                    if(foundNewRoom == false){
                                        //if he cant find a place to go kill him
                                        printf("Animal %d had to leave the house.", (creatures + i)->name);
                                        (creatures + i)->name = -2;
                                        (creatures + i)->cleanPref = -2;
                                        (creatures + i)->roomIn = -2;
                                        (creatures + i)->type = -2;
                                        //loop through creatures in room and all grumble
                                        for(int m = 0; m < 100; m++){
                                            if((creatures + m)->roomIn == (rooms + activeRoom)->name){
                                                //display message based on type
                                                //animal
                                                if((creatures + m)->type == 1){
                                                    (*respect)--;
                                                    printf("Animal %d growls at you. Respect is now %d", (creatures + m)->name, *respect);
                                                }
                                                if((creatures + m)->type == 2){
                                                    (*respect)--;
                                                    printf("NPC %d grumbles at you. Respect is now %d", (creatures + m)->name, *respect);
                                                }
                                            }
                                                
                                        }
                                    }
                                }
                                break;
                                //NPC
                            case 2:
                                *respect +=1;
                                printf("NPC %d  smiles at you. Respect is now %d\n", (creatures + i)->name, *respect);
                                break;

                            default: break;

                        }
                    }
                    
                    
                
                }
                
            
            }    
            break;
        //clean (0)
        case 3:
            if((rooms + activeRoom)->cleanState > 0 ){
                (rooms + activeRoom)->cleanState--;
                //if the player is making an creature do it
                if(target != -1){
                   
                    printf("%d cleans the room. ", target);
                    //if the creature type is NPC, then happy alot
                    if((creatures + target)->type == 2){
                        *respect -= 3;
                        printf("%d grumbles a lot. Respect is now %d.\n", target, *respect);
                    //if the creature is an animal, then unhappy alot
                    } else if((creatures + target)->type == 1){
                        *respect +=3;
                        printf("%d licks you a lot. Respect is now %d.\n", target, *respect);
                    } 
                } else {
                    printf("You clean the room. ");
                }
                //loop through the rest of the creatures and happy/mad + move accordingly
                for(int i = 0; i < 100; i++ ){
                    if ((creatures + i)->roomIn == activeRoom ){
                        //if monsters are found, act based on their type
                        switch((creatures + i)->type){                   
                            case 1:
                                *respect +=1;
                                printf("Animal %d licks you. Respect is now %d\n", (creatures + i)->name, *respect);
                                //if the state is dirty(2) then try to leave
                                break;
                                //NPC
                            case 2:
                                //NPCs would be the only ones to move on a clean cmd
                                if((creatures + i)->name != target){
                                    *respect -=1;
                                    printf("NPC %d  grumbles at you. Respect is now %d\n", (creatures + i)->name, *respect);
                                }
                                if((rooms + activeRoom)->cleanState == 0){
                                // attempt to leave in a random direction
                                //assign the random order of doors to check
                                        firstRandDir = rand() % 4;//creates a random num between  0 and 3
                                        //printf("Random direction: %d\n",firstRandDir );
                                    do{
                                        secondRandDir = rand() % 4;
                                    }while(secondRandDir == firstRandDir);
                                        //printf("Random direction: %d\n",secondRandDir );
                                    do{
                                        //thirdRandDir = rand() % 4;
                                    }while(thirdRandDir == secondRandDir || thirdRandDir == firstRandDir);
                                        //printf("Random direction: %d\n",thirdRandDir );
                                    do{
                                        fourthRandDir = rand() % 4;
                                    }while(fourthRandDir == firstRandDir || fourthRandDir == secondRandDir || fourthRandDir == thirdRandDir);
                                        //printf("Random direction: %d\n",fourthRandDir );
                                    //check the first door
                                    
                                    int foundNewRoom = false;
                                    
                                    switch(firstRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                    }
                                    //if not moved then try again
                                    if(foundNewRoom == false){
                                        switch(secondRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        }
                                    }
                                    if(foundNewRoom == false){
                                        switch(thirdRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        }
                                    }
                                    if(foundNewRoom == false){
                                        switch(fourthRandDir){
                                        case 0://north
                                            foundNewRoom = north(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 1://south
                                            foundNewRoom = south(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 2://east
                                            foundNewRoom = east(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        case 3://west
                                            foundNewRoom = west(i, playerIndex, rooms, creatures, false, respect);
                                            break;
                                        }
                                    }
                                    if(foundNewRoom == false){
                                        //if he cant find a place to go kill him
                                        printf("NPC %d had to leave the house.", (creatures + i)->name);
                                        (creatures + i)->name = -2;
                                        (creatures + i)->cleanPref = -2;
                                        (creatures + i)->roomIn = -2;
                                        (creatures + i)->type = -2;
                                        //loop through creatures in room and all grumble
                                        for(int m = 0; m < 100; m++){
                                            if((creatures + m)->roomIn == (rooms + activeRoom)->name){
                                                //display message based on type
                                                //animal
                                                if((creatures + m)->type == 1){
                                                    (*respect)--;
                                                    printf("Animal %d growls at you. Respect is now %d", (creatures + m)->name, *respect);
                                                }
                                                if((creatures + m)->type == 2){
                                                    (*respect)--;
                                                    printf("NPC %d grumbles at you. Respect is now %d", (creatures + m)->name, *respect);
                                                }
                                            }
                                        }
                                    }
                                }
                                break;

                            default: break;

                        }
                    }
                }
            } 
            break;
        //north
        case 4:
            north(target, playerIndex, rooms, creatures, true, respect);
            break;
        //south
        case 5:
            south(target, playerIndex, rooms, creatures, true, respect);
            break;
        //east
        case 6: 
            east(target, playerIndex, rooms, creatures, true, respect);
            break;
        //west
        case 7: 
            west(target, playerIndex, rooms, creatures, true, respect);
            break;
        //exit
        case 8: 
            exitGame(1);
            keepPlaying = false;
            break;
        default: break;
        
    }
    return keepPlaying;
}

int north(int target, int playerIndex, struct Room *rooms, struct Creature *creatures, int playerIssued, int *respect){
    int activeRoom = (creatures + playerIndex)->roomIn;
    int numInRoom = 0;
    int wentToRoom = false;
    //if there is a room to the north
    if((rooms + activeRoom)->north != -1){
        if(target != -1){
            //(creatures + target)->roomIn = (rooms + activeRoom)->north;
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room north of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->north))
                    numInRoom++;
            }
            if(numInRoom < 10){
                (creatures + target)->roomIn = ((rooms + activeRoom)->north);
                printf("%d leaves toward the north", target);
                wentToRoom = true;
                //check to see if they need to clean or dirty the new room
                //NPC
                if((creatures + target)->type == 2){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 0){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState++;
                        
                    }
                }
                //Animal
                if((creatures + target)->type == 1){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 2){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState--;    
                    }
                }  
            } else {
                printf("Room is full");
            //if the player made the thing move, but it cant, get mad
                if(playerIssued == true){ 
                    *respect--; 
                    if((creatures + target)->type == 2){
                        printf("NPC %d grumbles at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                    //Animal
                    if((creatures + target)->type == 1){
                        printf("Animal %d growls at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                }
            }
            
        } else {
            
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room north of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->north))
                    numInRoom++;
            }
            if(numInRoom < 10){
                (creatures + playerIndex)->roomIn = ((rooms + activeRoom)->north);
                printf("You leave toward the north");
                wentToRoom = true;
            } else {
                printf("Room is full");
                
                
            }  
            
        }
    } else {
        printf("There is no room to the north");
        wentToRoom = false;
        //went to room is false
    }
    return wentToRoom;
}

int south(int target, int playerIndex, struct Room *rooms, struct Creature *creatures, int playerIssued, int *respect){
    int activeRoom = (creatures + playerIndex)->roomIn;
    int numInRoom = 0;
    int wentToRoom = false;
    //if there is a room to the south
    if((rooms + activeRoom)->south != -1){
        if(target != -1){
            //(creatures + target)->roomIn = (rooms + activeRoom)->south;
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room south of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->south)){
                    numInRoom++;
                }
            }
            if(numInRoom < 10){
                (creatures + target)->roomIn = ((rooms + activeRoom)->south);
                printf("%d leaves toward the south", target);
                wentToRoom = true;
                //check to see if they need to clean or dirty the new room
                //NPC
                if((creatures + target)->type == 2){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 0){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState++;
                        
                    }
                }
                //Animal
                if((creatures + target)->type == 1){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 2){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState--;
                        
                    }
                }
                
            } else {
                printf("Room is full");
            //if the player made the thing move, but it cant, get mad
                if(playerIssued == true){ 
                    *respect--;
                    if((creatures + target)->type == 2){
                        printf("NPC %d grumbles at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                    //Animal
                    if((creatures + target)->type == 1){
                        printf("Animal %d growls at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                }
            }                             
        } else {
            
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room south of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->south))
                    numInRoom++;
            }
            if(numInRoom < 10){
                (creatures + playerIndex)->roomIn = ((rooms + activeRoom)->south);
                printf("You leave toward the south");
                wentToRoom = true;
            } else {printf("Room is full");}  
            
        }
    } else {
        printf("There is no room to the south");
        //went to room is false
         wentToRoom = false;
    }
    return wentToRoom;
}

int east(int target, int playerIndex, struct Room *rooms, struct Creature *creatures, int playerIssued, int *respect){
    int activeRoom = (creatures + playerIndex)->roomIn;
    int numInRoom = 0;
    int wentToRoom = false;
    //if there is a room to the east
    if((rooms + activeRoom)->east != -1){
        if(target != -1){
            //(creatures + target)->roomIn = (rooms + activeRoom)->east;
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room east of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->east))
                    numInRoom++;
            }
            if(numInRoom < 10){
                (creatures + target)->roomIn = ((rooms + activeRoom)->east);
                printf("%d leaves toward the east", target);
                wentToRoom = true;
                //check to see if they need to clean or dirty the new room
                //NPC
                if((creatures + target)->type == 2){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 0){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState++;
                        
                    }
                }
                //Animal
                if((creatures + target)->type == 1){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 2){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState--;
                        
                    }
                }
                
            } else {
                printf("Room is full");
            //if the player made the thing move, but it cant, get mad
                if(playerIssued == true){ 
                    *respect--; 
                    if((creatures + target)->type == 2){
                        printf("NPC %d grumbles at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                    //Animal
                    if((creatures + target)->type == 1){
                        printf("Animal %d growls at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                }
            }                             
        } else {
            
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room east of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->east))
                    numInRoom++;
            }
            if(numInRoom < 10){
                (creatures + playerIndex)->roomIn = ((rooms + activeRoom)->east);
                printf("You leave toward the east");
                wentToRoom = true;
            } else {printf("Room is full");}  
            
        }
    } else {
        printf("There is no room to the east");
        //went to room is false
         wentToRoom = false;
    }
    return wentToRoom;
}

int west(int target, int playerIndex, struct Room *rooms, struct Creature *creatures, int playerIssued, int *respect){
    int activeRoom = (creatures + playerIndex)->roomIn;
    int numInRoom = 0;
    int wentToRoom = false;
    //if there is a room to the west
    if((rooms + activeRoom)->west != -1){
        if(target != -1){
            //(creatures + target)->roomIn = (rooms + activeRoom)->west;
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room west of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->west))
                    numInRoom++;
            }
            if(numInRoom < 10){
                (creatures + target)->roomIn = ((rooms + activeRoom)->west);
                printf("%d leaves toward the west", target);
                wentToRoom = true;
                //check to see if they need to clean or dirty the new room
                //NPC
                if((creatures + target)->type == 2){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 0){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState++;
                        
                    }
                }
                //Animal
                if((creatures + target)->type == 1){
                    if((rooms + (creatures + target)->roomIn)->cleanState == 2){
                        //unacceptable conditions, must dirty
                        (rooms + (creatures + target)->roomIn)->cleanState--;
                        
                    }
                }
                
            } else {
                printf("Room is full");
            //if the player made the thing move, but it cant, get mad
                if(playerIssued == true){ 
                    *respect--;
                    if((creatures + target)->type == 2){
                        printf("NPC %d grumbles at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                    //Animal
                    if((creatures + target)->type == 1){
                        printf("Animal %d growls at you. Respect is now %d. ", (creatures + target)->name, *respect);
                    }
                }
            }                              
        } else {
            
            //if the room has less than 10 creatures inside it
            for(int c = 0; c < 100; c++){       //get the name of the room west of here
                if((creatures + c)->roomIn == ((rooms + activeRoom)->west))
                    numInRoom++;
            }
            if(numInRoom < 10){
                (creatures + playerIndex)->roomIn = ((rooms + activeRoom)->west);
                printf("You leave toward the west");
                wentToRoom = true;
            } else {printf("Room is full");}  
                    
            
        }
    } else {
        printf("There is no room to the west");
        //went to room is false
         wentToRoom = false;
    }
    return wentToRoom;
}

int help(){
    printf("------------------Help Menu--------------------------------\n");
    printf("Input without a target will be assumed as a player action\n");
    printf("Input should be one of the following formats:\n");
    printf("    Target:Command\n");
    printf("    Command \n");
    printf("Valid Commands are limited to the following:\n");
    printf("    look\n");
    printf("    clean/dirty\n");
    printf("    north/south/east/west\n");
    printf("    help\n");
    printf("    quit\n");
    printf("Example Input:    0:north\n");
    printf("Example Input:    look\n");
    printf("--------------------------------------------------------------\n");
   
    
    
}

int exitGame(int exitCondition){
    //exitCondition : 0 = fail, 1 = not complete, 2 = win
    if(exitCondition == 0)
        printf("You Lost the Game. ");
    else if(exitCondition == 1)
        printf("You Quit the Game. ");
     else if(exitCondition == 2)
        printf("You Won the Game. ");
    printf("Thank you For Playing. GoodBye");
    //free all malloc
    return false;
}

int updateGameState(struct Room *rooms, struct Creature *creatures,int *respect){
    /*Initially thought this was going to have to contain more
     * but due to the monsters entering rooms and changing them to a neutral state
     * there will be no chain reactions, so this is much smaller than initially
     * planned
     */
    int runAgain = true;
    int checkAgain = false;
    int firstRandDir, secondRandDir, thirdRandDir, fourthRandDir;
    int playerIndex = -1; //could pass  a literal to n/s/e/w instead
    
    //Check to see if they won or lost
    if(*respect >= 80){
        runAgain = false;
        exitGame(2);
    }    
    if(*respect <= 0){
        runAgain = false;
        exitGame(0);
    }
    return runAgain;    
}





