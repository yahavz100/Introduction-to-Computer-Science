#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "stack.h"

/***********************
 * Name: Yahav Zarfati
 * ID: 
 * Group: 01 - CS
 * Assignment: ass05
 ***********************/

 /******************
  * Function Name:isLegalString
  * Input:char str[]
  * Output:int 0 or 1
  * Function Operation:The function check if given string is legal or not legal.
  ******************/
int isLegalString(char str[]) {
	Element elements1;
	int rightCounter = 0;
	int leftCounter = 0;
	Stack* stack = initStack();
	//Check if malloc succeed
	if (stack != NULL) {
		int length;
		length = strlen(str);
		//Run on whole string and check for correct letters to insert stack
		for (int i = 0; i < length; i++) {
			//Check if its the correct letter("opener")
			if (str[i] == '<' || str[i] == '{' || str[i] == '[' || str[i] == '(') {
				leftCounter++;
				elements1.c = str[i];
				push(stack, elements1);
			}
			//Check if its the correct letter("closer")
			if (str[i] == '>') {
				//Check if its the correct letter("opener")
				if (top(stack).c == '<') {
					rightCounter++;
					pop(stack);
				}
			}
			//Check if its the correct letter("closer")
			else if (str[i] == '}') {
				//Check if its the correct letter("opener")
				if (top(stack).c == '{') {
					rightCounter++;
					pop(stack);
				}
			}
			//Check if its the correct letter("closer")
			else if (str[i] == ']') {
				//Check if its the correct letter("opener")
				if (top(stack).c == '[') {
					rightCounter++;
					pop(stack);
				}
			}
			//Check if its the correct letter("closer")
			else if (str[i] == ')') {
				//Check if its the correct letter("opener")
				if (top(stack).c == '(') {
					rightCounter++;
					pop(stack);
				}
			}
		}
		//Check if stack is empty and all openers had closers
		if (isStackEmpty && rightCounter == leftCounter) {
			destroyStack(stack);
			return 1;
		}
		destroyStack(stack);
		return 0;
	}
	printf("Strings: isLegalString: Cant initialize");
	destroyStack(stack);
	return 1;
}
