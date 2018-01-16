/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: chrisrk192
 *
 * Created on September 13, 2016, 2:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 
 */
//DEFINITIONS
    //SIZE
#define height 10
#define width 30
    //OUTPUT
#define grass '_'


typedef int bool;
        #define true 1
        #define false 0

typedef struct{
    int id;
    int quant;
    char tile;
}item;
typedef struct{
    int x;
    int y;
    char tile;
    item itemHere;
}cell;
typedef struct{
    int id;
    cell map[height][width];
    
}level;

//global pointer to level
level *currentLevel;


int main(int argc, char** argv) {
    currentLevel = (level *)malloc(sizeof(level));
    bool quit = false;
    item inventory[10]; 
    cell player;
    int target;
    int action;
    
    //set player start up vals
    //player.itemHere = 1;
    player.tile = '@';
    //need a better way of creating a starting position
    player.x = 5;
    player.y = 5;
     populateMap(currentLevel);
     currentLevel->map[player.x][player.y] = player;
     //currentLevel->map[player.x][player.y].tile = player.tile;
    //GameLoop
    while(quit == false){
        displayMap(currentLevel);
        processInput(&target, &action);
        quit = performUserAction(target, action, player);
    }
    
    //printChars();
    free(currentLevel);
    return (0);
}

int populateMap(level *l){
    //may want to malloc size of height*width cells and make pointers
    for(int h= 0; h < height; h++){
        for(int w = 0; w < width; w++){
            item none;
            cell blank;
            
            //assign random values to the cell
            int randNum = rand() % 10;
            switch(randNum){
                case 0:
                    none.tile = '.';
                    break;
                case 1:
                    none.tile = 'T';
                    break;
                case 2:
                    none.tile = '_';
                    break;
                default:
                    none.tile = ' ';
                    break;
            }
            
            none.id = 0;
            none.quant = 0;
            
            blank.tile = none.tile;
            blank.itemHere = none;
            blank.x = w;
            blank.y = w;
            
            //assign the block
            l->map[h][w] = blank;
            //printf("#");
        }
        //printf("\n");
    }
    return 0;
}

int displayMap(level *l){
    /*
     * This function displays the 2D array in the form of text
     * Loops through the array for each line then for each row
     */
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            printf("%c",l->map[h][w].tile);
        }
        printf("\n");
    }
    return 0;
}

int processInput(int *targetNum, int *command){
    //i need to transform target to a direction for non inventory cmds
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
        
        //only set the cmd if the target is a direction
        // 0 north 1 east 2 south 3 west
        if(tempTarget > -1 || tempTarget < 4)
            *targetNum = tempTarget;
        
        //convert the cmd string into a command number
            //convert to lowercase
        if(strcmp(cmd, "help") == 0)
            *command = 0;
        else if(strcmp(cmd, "attack") == 0)
            *command = 1;
        else if(strcmp(cmd, "activate") == 0)
            *command = 2;
        else if(strcmp(cmd, "inventory") == 0)
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
     
    return 0;
}

int performUserAction(int target, int action, cell player){
    int quit = false; //continue by default
    
    switch(action){
        //help
        case 0: 
           
            break;
        //look
        case 1: 
            
            break;
        //dirty (2)
        case 2:       
            
            break;
        //clean (0)
        case 3:
           
            break;
        //north
        case 4:
            
            break;
        //south
        case 5:
            
            break;
        //east
        case 6: 
           printf("east");
            break;
            break;
        //west
        case 7: 
            printf("west");
            break;
        //exit
        case 8: 
            quit = true; 
            break;
        default: 
            printf("Shouldnt happen");
            break;
        
    }
    return quit;
}
