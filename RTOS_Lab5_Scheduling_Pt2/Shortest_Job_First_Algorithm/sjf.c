#include "sjf.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int size) {

    for(int i = 0; i < size; i ++) {

        task[i].process_id = i;
        task[i].execution_time = execution[i];

    }

}

void shortest_job_first(struct task_t *task, int size) {
    // Hints:
    // 1. Create Queue based on the task array in the correct order
    // 2. Process each "task" until completion
    // 3. You can process by popping items from the queue

    /* First need to sort the tasks based on their execution time */
    for(int i = 0; i < size - 1; i ++) {

        for(int j = 0; j < size - i - 1; j ++) {

            if(task[j].execution_time > task[j+1].execution_time) {

                struct task_t temp = task[j];
                task[j] = task[j+1];
                task[j+1] = temp;

            }

        }

    }

    struct node_t* head = create_queue(task, size);
    int total_count = 0;
    struct task_t* current_task;

    // Keep running until queue is empty
    while(!is_empty(&head)) {

        // Current task to be executed
        current_task = peek(&head);

        total_count += current_task->execution_time;
        current_task->waiting_time = total_count - current_task->execution_time;
        current_task->turnaround_time = total_count;

        pop(&head);

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