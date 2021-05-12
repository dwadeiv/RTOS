/*
 * fifo_queue.h
 *
 *  Created on: Mar 28, 2021
 *      Author: David
 */

#ifndef SRC_HEADER_FILES_FIFO_QUEUE_H_
#define SRC_HEADER_FILES_FIFO_QUEUE_H_

//***********************************************************************************
// Include Files
//***********************************************************************************
#include <stdio.h>
#include <stdlib.h>

//***********************************************************************************
// global variables
//***********************************************************************************
struct Node{

	int value;
	struct Node* next;

};

//***********************************************************************************
// function prototypes
//***********************************************************************************
void queueInit();
void push(int value);
void pop();
int peek();
int isEmpty();

#endif /* SRC_HEADER_FILES_FIFO_QUEUE_H_ */
