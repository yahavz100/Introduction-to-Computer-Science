#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "queue.h"

/***********************
 * Name: Yahav Zarfati
 * ID: 
 * Group: 01 - CS
 * Assignment: ass05
 ***********************/

 /******************
 * Function Name:initQueue
 * Input:No input
 * Output:Pointer to queue or NULL in case of failure
 * Function Operation:The function allocate memory for queue,stack and elements,initialize stack to default values,
 in case of malfunction print error and return NULL, return a pointer to queue if succeed.
 ******************/
Queue* initQueue() {
	Queue* queue = malloc(sizeof(Queue));
	//Check if malloc succeed
	if (queue != NULL) {
		Stack* stack1 = initStack();
		Stack* stack2 = initStack();
		queue->s1 = stack1;
		queue->s2 = stack2;
		//Check if malloc succeed
		if (stack1 == NULL || stack2 == NULL) {
			printf("Queue: initQueue: Cant initialize stacks");
			return NULL;
		}
		return queue;
	}
	printf("Queue: initQueue: Cant initialize queue");
	queue = NULL;
	free(queue);
	return NULL;
}

/******************
 * Function Name:destroyQueue
 * Input:Queue* queue
 * Output:Void no output
 * Function Operation:The function free allocated memory.
 ******************/
void destroyQueue(Queue* queue) {
	destroyStack(queue->s1);
	destroyStack(queue->s2);
	//Check if queue is not NULL,free allocated memory
	if (queue != NULL)
		free(queue);
}

/******************
 * Function Name:isQueueEmpty
 * Input:Queue* queue
 * Output:int 1 or 0
 * Function Operation:The function check if queue is empty, if empty return 1 else 0.
 ******************/
int isQueueEmpty(Queue* queue) {
	//Check if both stacks are empty
	if ((isStackEmpty(queue->s1) && (isStackEmpty(queue->s2))))
		return 1;
	return 0;
}

/******************
 * Function Name:lenOfQueue
 * Input:Queue* queue
 * Output:int number of elements in queue
 * Function Operation:The function return the number of elements in queue.
 ******************/
int lenOfQueue(Queue* queue) {
	return(lenOfStack(queue->s1) + lenOfStack(queue->s2));
}

/******************
 * Function Name:capacityOfQueue
 * Input:Queue* queue
 * Output:int size of array in allocated memory
 * Function Operation:The function return the size of the current array that is allocated in memory.
 ******************/
int capacityOfQueue(Queue* queue) {
	return(capacityOfStack(queue->s1) + capacityOfStack(queue->s2));
}

/******************
 * Function Name:enqueue
 * Input:Queue* queue,Element element
 * Output:Void no output
 * Function Operation:The function insert given element into top of the queue.
 ******************/
void enqueue(Queue* queue, Element element) {
	push(queue->s1, element);
}

/******************
 * Function Name:dequeue
 * Input:Queue* queue
 * Output:Element temp in top of the queue
 * Function Operation:The function return top element in queue and change the queue.
 ******************/
Element dequeue(Queue* queue) {
	//Run on whole s1 and reverse elements order by using s2
	while (queue->s1->topIndex > -1)
		push(queue->s2, pop(queue->s1));
	 Element temp = (pop(queue->s2));
	 //Run on whole s2 and reverse elements order by using s1
	while (queue->s2->topIndex > -1)
		push(queue->s1, pop(queue->s2));
	return temp;
}

/******************
 * Function Name:peek
 * Input:Queue* queue
 * Output:Element temp in top of the queue
 * Function Operation:he function return top element in queue but dont change the queue.
 ******************/
Element peek(Queue* queue) {
	//Run on whole s1 and reverse elements order by using s2
	while (queue->s1->topIndex > -1)
		push(queue->s2, pop(queue->s1));
	Element temp = (top(queue->s2));
	//Run on whole s2 and reverse elements order by using s1
	while (queue->s2->topIndex > -1)
		push(queue->s1, pop(queue->s2));
	return temp;
}