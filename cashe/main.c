/* 
 * File:   main.c
 * Author: cking20
 * Created on November 4, 2016, 8:15 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#define DEBUG = true;

void updatePolicyVals(int, int, unsigned int, int**, time_t**);
int findIndextoReplace(int, unsigned int, unsigned int, 
        unsigned int, int**, time_t **);
int countBitsRequired(unsigned int);
int getMemAddrFromConsole(unsigned int );
void getStartingInput(unsigned int*,unsigned int*, unsigned int*,
        unsigned int*,int*,unsigned int*, unsigned int*,
        unsigned int*,unsigned int*, unsigned int *);


int main(int argc, char** argv) {
    unsigned int 
    S,//number of sets
    E,//number of lines per set)
    B,//number of BYYYTES in a data block
    m,//number of bits in mem address
    h, //the hit time
    p, //the miss penalty //miss is +(h+p)//hit is +(h)
    C,//Capacity C of a cashe is S x E x B
    b,//number of bits to denote block id        
    t,//num tag bits = m - (b + s) NOTE LOWERE CASE DENOTES num bits
    s;//number of bits to denote set        
    int mem,rp, numMiss, numHit, numRef, index; 
    int time, hitFlag, aNonValid;
    double rate;
    unsigned int tag,set;
    time = 0; numMiss = 0; numHit = 0, numRef = 0;
    
    getStartingInput(&S, &E,&B, &m, &rp, &h, &p, &s, &t, &b);
    int **lfu;    //lfu[S][E];      //array for lfu pollicy
    time_t **lru; // lru[S][E];   //array for lru pollicy  
    
//BUILD THE CASHE; could be malloced   
    unsigned int cashe[S * E * (B+2)]; //Need the plus 2 to hold the value and tag bits in the array
    
    lfu = malloc(S * sizeof *lfu);
    lru = malloc(S * sizeof *lru);
    for (int i=0; i<S; i++){
        lfu[i] = malloc(E * sizeof *lfu[i]);
        lru[i] = malloc(E * sizeof *lru[i]);
    }
    
    //initialize valid slots to 0;
    for(int n = 0; n < S; n++){//was for(int n = 0; n < S*E; n++){
        for(int e = 0; e < E; e++){
            //(the set number times the number of lines per set times the size
            //  of a line) plus (the line number times size of line) plus index
            cashe[(n * E * (B+2)) + e * (B+2) + 0] = 0;
            lfu[n][e] = 0;
            lru[n][e] = clock();//may not need
        }
    }
       
    mem = getMemAddrFromConsole(m);
    while(mem != -1){
        hitFlag = 0;
        aNonValid = 1;
        numRef++;
        //break apart mem into its chunks
        //isolate the tag
        tag = mem >> (s+b);
        tag = tag << (s+b);
        //isolate set
        set = mem ^ tag; //xor
        set = set >> b;       
#ifdef DEBUG
        printf("tag  = %u ",tag);printf("set  = %u\n",set);      
#endif 
        //(loop)check to see if there are any valids and tags match
        for (int e = 0; e < E; e++){
            if(cashe[(set * E * (B+2)) + e * (B+2) + 0] == 1){//valid
                if(cashe[(set * E * (B+2)) + e * (B+2) + 1] == tag){//its a hit                 
                    printf("H\n");
                    numHit;
                    time = time + h;
                    updatePolicyVals(rp, e, set,lfu,lru);
                    hitFlag = 1;
                } 
            }
        }
        if(hitFlag != 1){//no hit
            printf("M\n");
            numMiss++;
            time = time + h + p;
            //CHECK TO SEE IF ANY ARE NOT VALID
            for (int e = 0; e < E; e++){
                if(cashe[(set * E * (B+2)) + e * (B+2) + 0] == 0){//there is a spot thats not valid
                    //replace at e, dont check rep pol
                    index = e;
                    aNonValid = 1;//set the flag
                }
            }
            //IF NONE VALID THEN DO REP POL
            if(aNonValid == 0){ //all the spots were valid
                int e = findIndextoReplace(rp, tag, set, E, lfu,lru);
                cashe[(set * E * (B+2)) + e * (B+2) + 1] = tag;
                cashe[(set * E * (B+2)) + e * (B+2) + 0] = 1;
                updatePolicyVals(rp, e, set,lfu, lru);
            } else {
                cashe[(set * E * (B+2)) + index * (B+2) + 1] = tag;
                cashe[(set * E * (B+2)) + index * (B+2) + 0] = 1;
                updatePolicyVals(rp, index, set, lfu, lru);
            }  
        }
        mem = getMemAddrFromConsole(m);
    }
    
    //Calculate statistics
    if(numRef > 0)
        rate = (((double) numMiss / numRef) * 100);
    else
        rate = 0;
    printf("%d %d", (int)rate, time);
    //Free Memory
    for (int i=0; i<S; i++){
        free(lfu[i]);
        free(lru[i]);
    }
    free(lfu);
    free(lru);

    return (EXIT_SUCCESS);
}

void getStartingInput(unsigned int* S,unsigned int* E, unsigned int* B,
        unsigned int* m,int* rp,unsigned int* h, unsigned int* p,
        unsigned int* s,unsigned int* t, unsigned int *b){
    
    char replacePolicy[4];
    printf("S,E,B,m,RepPol,h,p\n");
    scanf("%d %d %d %d %s %d %d", S,E,B,m, replacePolicy, h, p);//Get starting input
    
#ifdef DEBUG
    printf("S = %u ",*S); printf("E = %u ",*E);printf("B = %u ",*B);printf("m = %u\n",*m);
    printf("Rep Pol = %s \n",replacePolicy);
    printf("h = %u ",*h);printf("p = %u\n",*p);    
#endif      
    *b = countBitsRequired((*B));
    *s = countBitsRequired((*S));
    *t = (*m) -((*s) + (*b));
    //verify replacement policy  
    if(replacePolicy[0] == 'L' && replacePolicy[1] == 'F' && replacePolicy[2] == 'U')
        *rp = 1;
    else if(replacePolicy[0] == 'L' && replacePolicy[1] == 'R' && replacePolicy[2] == 'U')
        *rp = 0;
    else
        *rp = -1;
    
#ifdef DEBUG 
    printf("b = %u ",*b);printf("s = %u ",*s); printf("t = %u \n",*t);
    printf("rp = %d", *rp);   
#endif    
    
    
    return;
}

int getMemAddrFromConsole(unsigned int m){
    char mem[(m/4)+1];
    int addr = 0;
    scanf("%s",mem);
    if(mem[0] != '-')
        addr = (int)strtol(mem, NULL, 16);
    else
        addr = -1;
    printf("%s ",mem);
#ifdef DEBUG
    //printf("%s ",mem);
    //printf("%d\n", addr);
#endif   
    return addr;
}

int countBitsRequired(unsigned int num){
     //ex: 8 sets will hold values 0 to 7 so 7 is max num
     num--;
    int i = 0;
    if(num == 0) return i;
    while ((num) >>= 1){
        i++;
    }
    i++;
#ifdef DEBUG
    printf("bits required: %d\n", i);
#endif
    return i;
}

int findIndextoReplace(int rp, unsigned int tag, unsigned int set, unsigned int E, int **lfu, time_t **lru){
    int least,index;
    time_t oldest_t;
    index = 0;
    switch(rp){
        case 1://LFU
            if(E > 1){ //if E is 1, there is no need to check
                least = lfu[set][0]; //priming read
                for(int e = 0; e < E; e++){
                    if(least > lfu[set][e]){
                        least = lfu[set][e]; 
                        index = e;
                    }  
                }
                updatePolicyVals(1, index, set, lfu, lru);
            } 
            break;
        case 0://LRU
            if(E > 1){ //if E is 1, there is no need to check
                oldest_t = lru[set][0]; //priming read
                for(int e = 0; e < E; e++){
                    if(oldest_t > lru[set][e]){
                        oldest_t = lru[set][e]; 
                        index = e;
                    }  
                }
                updatePolicyVals(0, index, set, lfu, lru);
            } 
            break;     
        case -1://None Specified
            break;
    }
    return index;
}

void updatePolicyVals(int rp, int e, unsigned int set, int **lfu, time_t **lru){
    switch(rp){
        case 1://LFU
            lfu[set][e] +=1;
            break;
        case 0://LRU
            lru[set][e] = clock();
            break;     
        case -1://None Specified
            break;
    }
    return;
}
