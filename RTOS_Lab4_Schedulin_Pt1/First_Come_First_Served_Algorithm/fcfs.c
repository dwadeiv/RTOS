#include "fcfs.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int size) {

    for(int i = 0; i < size; i ++) {

        task[i].process_id = i;
        task[i].execution_time = execution[i];

    }

}

void first_come_first_served(struct task_t *task, int size) {
    // Hints:
    // 1. Create Queue based on the task array
    // 2. Pop off the front item until there are no more tasks
    struct node_t* head = create_queue(task, size);

    int task_index = 0;
    int count = 0;
    struct task_t* previous_task = NULL;

    while(!is_empty(&head)) {

        struct task_t* current_task = peek(&head);

        // Perform for every task but first
        if(task_index != 0) {

            // Calculate wait time

            int old_count = count;

            while(count!= old_count + previous_task->execution_time) {

                count ++;

            }

        }

        // Making copy of previous task for timing
        previous_task = current_task;

        // Pop finished task
        pop(&head);

        // Update wait and turnaround time
        current_task->waiting_time = count;
        current_task->turnaround_time = count + current_task->execution_time;

        task_index++;

    }


    empty_queue(&head);

    calculate_average_wait_time(task, size);
    calculate_average_turn_around_time(task, size);

}

float calculate_average_wait_time(struct task_t *task, int size) {
    
    int sum = 0;

    for(int i = 0; i < size; i++) {

        sum += task[i].waiting_time;

    }

    float average = (float)sum / (float)size;

    return average;

}

float calculate_average_turn_around_time(struct task_t *task, int size) {
   
    int sum = 0;

    for(int i = 0; i < size; i++) {

        sum += task[i].turnaround_time;

    }

    float average = (float)sum / (float)size;

    return average;
}