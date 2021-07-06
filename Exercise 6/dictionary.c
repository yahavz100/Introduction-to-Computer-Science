#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dictionary.h"

/***********************
 * Name: Yahav Zarfati
 * ID: 313163255
 * Group: 01 - CS
 * Assignment: ass06
 ***********************/

struct Dictionary {
	int value;
	int key;
	Dictionary* next;
};

/******************
  * Function Name:initDictionary
  * Input:No input
  * Output:Pointer to dictionary or MEM_ERROR(2) in case of failure
  * Function Operation:The function allocate memory for dictionary,initialize dictionary to default values,
 in case of malfunction print error and return 2, return a pointer to dictionary if succeed.
  ******************/
Dictionary* initDictionary() {
	Dictionary* diction = malloc(sizeof(Dictionary));
	//Check if malloc succeed
	if (diction == NULL) {
		printf("Cant initialize dictionary");
		//Did casting to MEM_ERROR becuase of warning to return Dictuinary*
		return (Dictionary*)MEM_ERROR;
	}
	diction->next = NULL;
	return diction;
}

/******************
 * Function Name:destroyDictionary
 * Input:Dictionary* d
 * Output:Void no output
 * Function Operation:The function free allocated memory.
 ******************/
void destroyDictionary(Dictionary* d) {
	Dictionary* temp;
	//Free each node in list
	while (d->next != NULL) {
		temp = d;
		d = d->next;
		free(temp);
	}
	//Check if given input is NULL
	if (d == NULL)
		return;
	//Check if there is memory to free
	if (d->next == NULL) {
		free(d);
		d = NULL;
		return;
	}
	destroyDictionary(d->next);
}

/******************
 * Function Name:sizeOfDictionary
 * Input:Dictionary* d
 * Output:int size of
 * Function Operation:The function return number of keys/values in dictionary.
 ******************/
int sizeOfDictionary(Dictionary* d) {
	int counter = 0;
	//Run on dictionary
	while (d->next != NULL) {
		counter++;
		d = d->next;
	}
	return counter;
}

/******************
 * Function Name:putInDictionary
 * Input:Dictionary* d,int key, int value
 * Output:Result SUCCESS/MEM_ERROR - 0/2(enum)
 * Function Operation:The function insert into dictionary new key and value.
 ******************/
Result putInDictionary(Dictionary* d, int key, int value) {
	//Run on dictionary
	while(d->next!=NULL){
		//Check if given key same as key in dictionary
		if (key == d->next->key) {
			d->next->value = value;
			return SUCCESS;
		}
		d = d->next;
	}
	Dictionary* newDiction = initDictionary();
	d->next = newDiction;
	newDiction->key = key;
	newDiction->value = value;
	newDiction->next = NULL;
	return SUCCESS;
}

/******************
 * Function Name:getFromDictionary
 * Input:Dictionary* d,int key
 * Output:int value or 0 if not found
 * Function Operation:The function find according to given key the right vaue and return it,
 in case didn't find return 0.
 ******************/
int getFromDictionary(Dictionary* d, int key) {
	//Run on dictionary
	while (d != NULL) {
		//Check if given key same as dictionary
		if (d->key == key)
			return d->value;
		d = d->next;
	}
	return 0;
}

/******************
 * Function Name:removeFromDictionary
 * Input:Dictionary* d,int key
 * Output:Result SUCCESS - 0 (enum) or nothing
 * Function Operation:The function remove given key from dictionary, if key not exist doesnt do anything.
 ******************/
Result removeFromDictionary(Dictionary* d, int key) {
	Dictionary* temp;
	//Check if dictionary is null or if its on first spine in dicitonary
	if (d == NULL)
		return SUCCESS;
	else if (d->key == key) {
		temp = d;
		d = d->next;
		free(temp);
		return SUCCESS;
	}
	//Run on dictionary
	while (d->next != NULL) {
		//Check if given key is same as key in next dictionary key
		if (d->next->key == key) {
			temp = d->next;
			d->next = d->next->next;
			free(temp);
			return SUCCESS;
		}
		d = d->next;
	}
}

/******************
 * Function Name:swap
 * Input:Dictionary* d1,Dictionary* d2
 * Output:Void - no output
 * Function Operation:The function swap data between given dictionaries.
 ******************/
void swap(Dictionary* d1, Dictionary* d2)
{
	int tempV = d1->value;
	d1->value = d2->value;
	d2->value = tempV;
	int tempK = d1->key;
	d1->key = d2->key;
	d2->key = tempK;
}

/******************
 * Function Name:bubbleSort
 * Input:Dictionary* d
 * Output:Void - no output
 * Function Operation:The function bubble sort given dictionary.
 ******************/
void bubbleSort(Dictionary* d){
	int isSwapped;
	Dictionary* d1;
	Dictionary* dptr = NULL;
	//Check if dictionary is null
	if (d == NULL)
		return;
	//Run swap on whole dictionary
	do{
		isSwapped = 0;
		d1 = d;
		//Run swap func on dictionary nodes
		while (d1->next != dptr){
			//Check if key is bigger from next node key
			if (d1->key > d1->next->key){
				swap(d1, d1->next);
				isSwapped = 1;
			}
			d1 = d1->next;
		}
		dptr = d1;
	} while (isSwapped);
}

/******************
 * Function Name:printDictionary
 * Input:Dictionary *d
 * Output:Void no output
 * Function Operation:The function print all values in dictionary sorted by keys.
 ******************/
void printDictionary(Dictionary* d) {
    //Check unique case of dictionary size 0
	if (sizeOfDictionary(d) == 0) {
		printf("{}");
		return;
	}
	d = d->next;
	bubbleSort(d);
	printf("{");
	//Run print on whole dictionary
	while (d != NULL) {
		printf("[%d:%d]", d->key, d->value);
		d = d->next;
	}
	printf("}");
}

/******************
 * Function Name:createDictionaryFromArrays
 * Input:int keys[], int values[], int size
 * Output:Pointer to dictionary
 * Function Operation:The function create dictionary with the keys and values given from input.
 ******************/
Dictionary* createDictionaryFromArrays(int keys[], int values[], int size) {
	Dictionary* diction = initDictionary();
	//Insert new values and keys into dictionary *size times
	for (int i = 0; i < size; i++) {
		putInDictionary(diction, keys[i], values[i]);
	}
	return diction;
}
