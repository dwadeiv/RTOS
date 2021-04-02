#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct node_t* create_queue(struct task_t* task, int size) {
    
    // Have to double allocate for a pointer ///////////////////////////////////////////////////////
    struct node_t** head = malloc(sizeof (*head)); 
    head = malloc(sizeof(**head)); 
    *head = NULL;

    int test_counter_1 = 0; 
    int test_counter_2 = 0;

    for(int i = 0; i < size; i++) {

        push(head, &task[i]);

    }

    struct node_t* temp = *head;

    return temp;
}

struct node_t* create_new_node(struct task_t* task) {
    // return null so it compiles
    struct node_t* new_node;
    new_node = malloc(sizeof(struct node_t));

    new_node->task = task;
    new_node->next = NULL;

    return new_node;
}

struct task_t* peek(struct node_t** head) {

    struct node_t* temp = *head;

    if(!is_empty(head)) {

        return temp->task;

    }

    return 0;

}

void pop(struct node_t** head) {

    struct node_t* temp = *head;

    if(!is_empty(head)) {

        *head = temp->next;
        free(temp);

    }

    return;
}

void push(struct node_t** head, struct task_t* task) {

    struct node_t* new_node = create_new_node(task);

    if(is_empty(head)) {

        *head = new_node;

        return;

    }

    struct node_t* temp = *head;

    while(temp->next) {

        temp = temp->next;

    }

    temp->next = new_node;

}

int is_empty(struct node_t** head) {

    if(*head) {

        return 0;

    }

    return 1;
}

void empty_queue(struct node_t** head) {

    while(!is_empty(head)) {

        pop(head);
    
    }

}
