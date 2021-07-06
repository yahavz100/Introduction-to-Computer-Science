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
 * Function Name:initStack
 * Input:No input
 * Output:Pointer to stack or NULL in case of failure
 * Function Operation:The function allocate memory for stack and elements,initialize stack to default values,
 in case of malfunction print error and return NULL, return a pointer to stack if succeed.
 ******************/
Stack* initStack() {
	Stack* stack = malloc(sizeof(Stack));
	//Check if malloc succeed
	if (stack != NULL) {
		Element* element = malloc(sizeof(Element));
		stack->content = element;
		//Check if malloc succeed
		if (element != NULL) {
			stack->size = 1;
			stack->topIndex = -1;
			return stack;
		}
		printf("Stack: initStack: Cant initialize elements");
		free(stack);
		return NULL;
	}
	printf("Stack: initStack: Cant initialize stack");
	return NULL;
}

/******************
 * Function Name:destroyStack
 * Input:Stack* stack
 * Output:Void no output
 * Function Operation:The function free allocated memory.
 ******************/
void destroyStack(Stack* stack) {
	//Check if there is memory to free
	if (stack->content != NULL)
		free(stack->content);
	//Check if there is memory to free
	if (stack != NULL)
		free(stack);
}

/******************
 * Function Name:isStackEmpty
 * Input:Stack* stack
 * Output:int 1 or 0
 * Function Operation:The function check if stack is empty, if empty return 1 else 0.
 ******************/
int isStackEmpty(Stack* stack) {
	//Check if stack is empty by topIndex
	if (stack->topIndex == -1)
		return 1;
	return 0;
}

/******************
 * Function Name:lenOfStack
 * Input:Stack* stack
 * Output:int number of elements in stack
 * Function Operation:The function return the number of elements in stack.
 ******************/
int lenOfStack(Stack* stack) {
	return(stack->topIndex + 1);
}

/******************
 * Function Name:capacityOfStack
 * Input:Stack* stack
 * Output:int size of array in allocated memory
 * Function Operation:The function return the size of the current array that is allocated in memory.
 ******************/
int capacityOfStack(Stack* stack) {
	return(stack->size);
}

/******************
 * Function Name:push
 * Input:Stack* stack,Element element
 * Output:Void no output
 * Function Operation:The function push(last in first out) given element into top of the stack.
 ******************/
void push(Stack* stack, Element element) {
	int i = 0;
	stack->topIndex = stack->topIndex++;
	//Check if stack running out of memory
	if (stack->size == stack->topIndex+1) {
		stack->size = stack->size * 2;
		Element* tmp = realloc(stack->content, stack->size * sizeof(Element));
		//Check if malloc succeed
		if (tmp == NULL) {
			destroyStack(stack);
			printf("Stack: push: Cant allocate memory");
		}
		stack->content = tmp;
	}
	stack->content[stack->topIndex+i] = element;
}

/******************
 * Function Name:pop
 * Input:Stack* stack
 * Output:Element in top of the stack
 * Function Operation:The function return top element in stack and change the stack.
 ******************/
Element pop(Stack* stack) {
	//Check if stack has spare memory
	if ((stack->topIndex + 1)*2 < stack->size) {
		Element* tmp = realloc(stack->content,(stack->size/2) * sizeof(Element));
		//Check if malloc succeed
		if (tmp == NULL) {
			destroyStack(stack);
			printf("Stack: pop: Cant allocate memory");
		}
		stack->content = tmp;
		stack->size = stack->size/2;
	}
	return(stack->content[stack->topIndex--]);
}

/******************
 * Function Name:top
 * Input:Stack* stack
 * Output:Element in top of the stack
 * Function Operation:The function return top element in stack but dont change the stack.
 ******************/
Element top(Stack* stack) {
	return(stack->content[stack->topIndex]);
}

/******************
 * Function Name:printStack
 * Input:Stack* stack
 * Output:Void no output
 * Function Operation:The function print all elements in stack.
 ******************/
void printStack(Stack* stack) {
	//Check if there is stack or empty else print all elements
	if (stack == NULL)
		printf("Stack: printStack: Error!");
	else if (stack->topIndex==-1)
		printf("Stack: printStack: Stack is empty!\n");
	else {
		//Run print for all elements in stack
		for (int i = stack->topIndex; i > -1; i--) {
			printf("%d: ", i+1);
			printElement(stack->content[i]);
			printf("\n");
		}
	}
}