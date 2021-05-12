#include <stdlib.h>
#include "ctest.h"
#include "test_functions.h"


/* ------------------------------------------------------------------------------------

                                UNIT TESTS
                                
------------------------------------------------------------------------------------*/
CTEST_DATA(unittests) {
    struct data_t data[3];
    int size;
};

CTEST_SETUP(unittests) {
    int pwm_duty_cycle[] = {20, 40, 100};
    int current_gain[] = {2, 5, 10};
    int current_x_position[] = {0, 5, 10};
    int current_theta[] = {10, -25, 90};
    int current_force = 5;
    int max_force = 10;
    int min_force = 1;
    int max_x_position = 10;
    int min_x_position = 0;
    data->size = sizeof(data) / sizeof(data[0]);
    init(data->data, pwm_duty_cycle, current_gain, current_x_position, current_theta, current_force, max_force, min_force, max_x_position, min_x_position, data->size);

}

// Tests FIFO Queue isEmpty function
CTEST2(unittests, fifo_isEmpty) {

    int value = fifo_queue_isEmpty();
    ASSERT_EQUAL(1, value);
    // ASSERT_EQUAL(1, (int)data->task[i].execution_time);

}

// Tests FIFO Queue push function
CTEST2(unittests, fifo_push) {

    int expected = 2;
    int value = fifo_queue_push(expected);
    ASSERT_EQUAL(expected, value);

}

// Tests FIFO Queue pop function
CTEST2(unittests, fifo_pop) {

    int expected = 1;
    int value = fifo_queue_pop();
    ASSERT_EQUAL(expected, value);

}

// Tests PWM Duty Cycle
CTEST2(unittests, pwm) {

    int expected_max = 100;
    int expected = 1;

    for(int i = 0; i < data->size; i++) {

        int value = ((int)data->data[i].pwm_duty_cycle <= expected_max);
        ASSERT_EQUAL(expected, value);

    }

}

// Tests Force Setpoint value
CTEST2(unittests, force_setpoint) {

    int expected_max = 100;
    int expected = 1;

    for(int i = 0; i < data->size; i++) {

        int value = ((int)data->data[i].current_force == MAX_LEFT_FORCE || (int)data->data[i].current_force == LEFT_FORCE || (int)data->data[i].current_force == RIGHT_FORCE || (int)data->data[i].current_force == MAX_RIGHT_FORCE);
        ASSERT_EQUAL(expected, value);

    }

}

// Tests Max Gain
CTEST2(unittests, max_gain) {

    int expected = 1;

    for(int i = 0; i < data->size; i++) {

        int value = ((int)data->data[i].current_gain <= (int)data->data[i].max_force);
        ASSERT_EQUAL(expected, value);

    }

}

// Tests Min Gain
CTEST2(unittests, min_gain) {

    int expected = 1;

    for(int i = 0; i < data->size; i++) {

        int value = ((int)data->data[i].current_gain >= (int)data->data[i].min_force);
        ASSERT_EQUAL(expected, value);

    }
}

// Tests Max X position
CTEST2(unittests, max_x) {

    int expected = 1;

    for(int i = 0; i < data->size; i++) {

        int value = ((int)data->data[i].current_x_position <= (int)data->data[i].max_x_position);
        ASSERT_EQUAL(expected, value);

    }

}

// Tests Min X position
CTEST2(unittests, min_x) {

    int expected = 1;

    for(int i = 0; i < data->size; i++) {

        int value = ((int)data->data[i].current_x_position >= (int)data->data[i].min_x_position);
        ASSERT_EQUAL(expected, value);

    }

}

// Tests Theta Bounds
CTEST2(unittests, theta) {

    int expected_max = 90;
    int expected = 1;

    for(int i = 0; i < data->size; i++) {

        int value = ((int)data->data[i].current_theta <= expected_max || (int)data->data[i].current_theta >= -expected_max);
        ASSERT_EQUAL(expected, value);

    }

}







