#include "priority.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int *priority, int size) {

    for(int i = 0; i < size; i ++) {

        task[i].process_id = i;
        task[i].execution_time = execution[i];
        task[i].left_to_execute = execution[i];
        task[i].priority = priority[i];

    }

}

static struct node_t* swap(struct node_t* ptr1, struct node_t* ptr2) {

    struct node_t* tmp = ptr2->next; 
    ptr2->next = ptr1; 
    ptr1->next = tmp; 
    return ptr2; 

} 

void priority_schedule(struct task_t *task, int size) {
    // Hints:
    // 1. Create Queue based on the task array in the correct order
    // 2. Each task can be processed for a time interval of 1 (i.e quantum time of 1)
    // 3. You can process by pushing and popping items from the queue
    // 4. You must recalculate the priorities after every turn

    /* First need to sort the task array based on task priority */
    for(int i = 0; i < size - 1; i ++) {

        for(int j = 0; j < size - i - 1; j ++) {

            if(task[j].priority < task[j+1].priority) {

                struct task_t temp = task[j];
                task[j] = task[j+1];
                task[j+1] = temp;

            }

        }

    }

    struct node_t* head = create_queue(task, size);
    int total_count = 0;
    struct task_t* current_task;
    int quantum = 1;
    int tasks_remaining = size;

    // Keep running until queue is empty
    while(!is_empty(&head)) {

        // Current task to be executed
        current_task = peek(&head);
        // printf("\nTask %d is executing with priority %d", current_task->process_id, current_task->priority);

        // Task is able to finish executing
        if(current_task->left_to_execute <= quantum) {

            total_count += quantum;
            // Update turnaround and wait time for the task
            current_task->waiting_time = total_count - current_task->execution_time;
            current_task->turnaround_time = total_count;
            pop(&head);
            // Decrement tasks remaining for the sorting algorithm
            tasks_remaining --;

        }

        // Task is unable to finish
        else {

            total_count += quantum;
            current_task->left_to_execute -= quantum;
            push(&head, current_task);
            pop(&head);
            
        }

        update_priority(&head, total_count);

        /* Bubble Sort the linked list based off of the new priorities */
        struct node_t** h;
        int swapped;

        for(int i = 0; i <= tasks_remaining; i++) {

            h = &head;
            swapped = 0;

            for(int j = 0; j < tasks_remaining - i - 1; j++) {

                struct node_t* p1 = *h;
                struct node_t* p2 = p1->next;

                // Order the linked list from highest priority to lowest priority
                if(p1->task->priority < p2->task->priority) {

                    *h = swap(p1, p2);
                    swapped = 1;

                }

                h = &(*h)->next;

            }

            // Break if the loop ended without any swap 
            if(swapped == 0) {

                break;

            }

        }

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