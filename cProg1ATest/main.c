#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
/*
Christoper King
Homework 1A 
Due Sept 23 2016
Description: 
	This program displays all the permutations of a user specified string.
Input: 
	the string to permute should be specified after the name of the program.
Output:
	Each permutation of the string is displayed on its own line.
Error Handleing:
	If the user inputs multiple strings, only the first is permutated
 */


static void usage();
int permutate(int , char *, int, char **);
void swap(char *, char *);
char **getPermutations(char *);

int main(int argc, char *argv[]) {
	
	char *theString = 0;
	char *theOrigString = 0;
   char *prgn = 0;
   char *options = "";//":a:b:c:d:e";
   prgn = (char *)malloc(sizeof(char)*(strlen(argv[0])+1));
   strcpy(prgn,argv[0]);
   
   if(argc >= 2){
		//get the lenght of the string
		int rows = 1;
		int n = (strlen(argv[1]));
		//create an array with that argument
		theString = (char *)malloc(sizeof(char)*(strlen(argv[1])+1));
		theOrigString = theString;
		strcpy(theString, argv[1]);
		//add a terminator
		*(theString + n) = '\0';
		char** perms = 0;
		perms = getPermutations(theString);
                
		
		//loop through and free perms
			//get the num of rows
		for(int i = 0; i < n; i++)
			rows = rows * (i+1);
                
                for(int i = 0; i < rows; i++)
                    printf("%s\n", perms[i]);
                
                
		for(int i = 0; i < rows; i++)	
			free(perms[i]);
                free(perms);
             
   
   } else if(argc == 1){
	   usage(prgn);
   }
   
   //Free memory
	free(theOrigString);
	free(prgn);
	exit(0);
        return 0;
}

void usage(char *prgn) {
	//This should be called if no string is supplied at runtime.
	//This displays the propper usage of this program. 
   fprintf(stderr,"usage: %s string_to_permute \n", prgn);
   
}
char **getPermutations(char *theString){
	/*
	PreConditions:
		there must be a string with a terminator at the end
	PostConditions: There will be a 2D array of size 
		n-1! by n where n = lenght of theString
		THIS WILL NEED TO BE FREED!
	*/
	
	char** perms = 0;
	int aryIndx = 0;
	int rows = 1;
	int n = (strlen(theString)); // exclude the null terminator
	//malloc a 2D array of n-1! by n
	//get n!
	for(int i = 0; i < n; i++)
		rows = rows * (i+1);
	perms = malloc(rows * sizeof(char *));
	for (int i = 0; i < rows; i++)
		*(perms + i)= malloc(n+1 * sizeof(char*)); 
	permutate(n, theString, aryIndx, perms);
	return perms;
}


int permutate(int n, char *theString, int aryIndx, char **perms){
	//printf("Run : %d\n", aryIndx);
	/*
	This function recursively displays all permutations of 
		a string (theString) with specified lenght (n).
	Heaps algorithm is utilized to provide an efficient solution. 
	*/
	//using Heaps Algorithm
	if(n == 1){
		aryIndx++;
		//print the final permutation AKA assign to array
		//printf("%s %d\n", theString, aryIndx);
//ISSUE HERE
		strcpy(*(perms+(aryIndx -1)),theString);
	} else {
		for(int i = 0; i < n - 1; i++){
			//print the next permutation
			aryIndx = permutate(n -1, theString, aryIndx, perms);
			//perform a swap of the chars in the string
			if(n%2 == 0){
				swap(theString+i, theString + (n-1));
			} else{
				swap(theString, theString + (n-1));
			}
		}
		aryIndx = permutate(n-1, theString, aryIndx, perms);
	}
	return aryIndx;
}

void swap(char *firstArrayLoc, char *secondArrayLoc){
	/*
	This function swaps 2 chars in a char array at 
		the firstArrayLoc and the secondArrayLoc
	*/
	char temp;
	temp = *firstArrayLoc;
	*firstArrayLoc = *secondArrayLoc;
	*secondArrayLoc = temp;
}
