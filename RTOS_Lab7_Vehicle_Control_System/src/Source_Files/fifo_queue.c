/*
 * fifo_queue.c
 *
 *  Created on: Mar 28, 2021
 *      Author: David
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "fifo_queue.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************
static struct Node* head;

//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************

void queueInit() {

	head = NULL;
	return;

}

void push(int value) {

	struct Node* new_node;
	new_node = malloc(sizeof(struct Node));
	new_node->value = value;
	new_node->next = NULL;

	if(isEmpty()) {

		head = new_node;

		return;

	}

	struct Node* temp = head;

	while(temp->next) {

		temp = temp->next;

	}

	temp->next = new_node;

}

void pop() {

	struct Node* temp = head;

	if(!isEmpty()) {

		head = temp->next;
		free(temp);

	}

	return;


}

int peek() {

	struct Node* temp = head;

	if(!isEmpty()) {

		return temp->value;

	}

	return 0;

}

int isEmpty() {

	if(head) {

		return 0;

	}

	return 1;


}
