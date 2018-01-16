/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: chrisrk192
 *
 * Created on September 2, 2016, 9:31 AM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        struct Creature inRoom[10];
        int name;
        int cleanState;
//!!!!!!not sure how to link the rooms, may link up with types of Room or maybe ints or pointers
        int north;
        int south;
        int east;
        int west;  
    } ;
    
    struct Room *roomPtr;
    struct Creature *creatPtr;

int main(int argc, char** argv) {
    //make sure you cast malloc to the type you want otherwise the compiler yells at you
    roomPtr = (struct Room *)malloc(100*sizeof(struct Room));
    roomPtr->cleanState = 2;
    roomPtr++;
    roomPtr->cleanState = 3;
    roomPtr--;
    
    printf("%d", (roomPtr+1)->cleanState);
    free(roomPtr);
    int t;
    int c;
    
    processInputTwo(&t, &c);
    printf("the target num is %d", t);
    
    
    
    return (0);
}
int processInputTwo(int *targetNum, int *command){
    int isTarget = 1; //0 is false 1 is true
    
    int tempTarget = -1;
    char str[100];
    char cmd[100];

    
    //gets(str);
    fgets(str, sizeof(str), stdin);
    if(sscanf(str, "%d:%s ", &tempTarget, cmd) != 2) {
        isTarget = false; 
        sscanf(str, "%s", cmd);
    }
    //if there is no target, player does it
    //if there is a target, the target does it
    printf("%d \n", tempTarget);
    printf("%s", cmd);
    
    *targetNum = tempTarget;
    //convert the cmd string into a command number
    //convert to lowercase
    //compare the string
    if(strcmp(cmd, "help") == 0)
        printf("equel");
    //set the command value
    
        
    
    return 0;
}



int processInput(){
    int isTarget = 0; //0 is false
    int targetNum = -1;
    char str[100];
    char first[100];
    char cmd[100];

    
    
    //gets(str);
    fgets(str, sizeof(str), stdin);
    if(sscanf(str, "%d:%s ", &targetNum, cmd) != 2) {
        sscanf(str, "%s", cmd);
    }
       
    printf("%d \n", targetNum);
    printf("%s", cmd);
    bool i = true;
    if(i)
        printf("true");
    
    return 0;
}

/*
 * 
 * (*ptr).a is same as ptr->a
 * 
 * swap(&a, &b)
 * PASSING BY REFERANCE NOT BY VALUE CAN CHANGE THE VALUE
 * void swap(int *a, int *b){
 *  int t = (*a);
 *  (*a) = (*b);
 *  (*b) = t;
 * }
 void main() 
{
struct Cricket *ptr = match;   // By default match[0]
for(i=0;i<4;i++) 
    {
    printf("\nMatch : %d",i+1);
    printf("\n%s Vs %s",ptr->team1,ptr->team2);
    printf("\nPlace : %s",ptr->ground);

    if(match[i].result == 1)
        printf("nWinner : %s",ptr->team1);
    else
        printf("nWinner : %s",ptr->team1);
    printf("\n");

    // Move Pointer to next structure element
    ptr++;
    }
 */
