#include "rr.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int size) {

    for(int i = 0; i < size; i ++) {

        task[i].process_id = i;
        task[i].execution_time = execution[i];
        task[i].left_to_execute = execution[i];

    }

}

void round_robin(struct task_t *task, int quantum, int size) {
    // Hints:
    // 1. Create Queue based on the task array
    // 2. Process each "task" in round robin fashion
    // 3. You can process by pushing and popping items from the queue
    struct node_t* head = create_queue(task, size);
    int total_count = 0;
    struct task_t* current_task;

    // Keep running until queue is empty
    while(!is_empty(&head)) {

        // Current task to be executed
        current_task = peek(&head);

        // Time left to execute is greater than allocated quantum
        if(current_task->left_to_execute > quantum) {

            total_count += quantum;
            current_task->left_to_execute -= quantum;
            push(&head, current_task);
            pop(&head);

        }

        // Task is able to finish executing
        else if(current_task->left_to_execute <= quantum) {

            total_count += current_task->left_to_execute;
            current_task->waiting_time = total_count - current_task->execution_time;
            current_task->turnaround_time = total_count;
            pop(&head);

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