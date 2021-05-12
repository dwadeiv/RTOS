#include "test_functions.h"
#include "fifo_queue.h"
#include <stdio.h>

void init(struct data_t *data, int *pwm_duty_cycle, int *current_gain, int *current_x_position, int *current_theta, int current_force, int max_force, int min_force, int max_x_position, int min_x_position, int size) {

    for(int i = 0; i < size; i ++) {

        data[i].pwm_duty_cycle = pwm_duty_cycle[i];
        data[i].current_gain = current_gain[i];
        data[i].current_theta = current_theta[i];
        data[i].current_x_position = current_x_position[i];
        data[i].current_force = current_force;
        data[i].max_force = max_force;
        data[i].max_x_position = max_x_position;
        data[i].min_force = min_force;
        data[i].min_x_position = min_x_position;

    }

    queueInit();

}

int fifo_queue_isEmpty() {

    return isEmpty();

}

int fifo_queue_push(int value) {

    push(value);
    return peek();

}

int fifo_queue_pop() {

    int popped_value = peek();
    pop();
    return isEmpty();

}
