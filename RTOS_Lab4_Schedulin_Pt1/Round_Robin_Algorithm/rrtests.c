#include <stdlib.h>
#include "ctest.h"
#include "rr.h"

CTEST_DATA(roundrobin_1) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(roundrobin_1) {
    int execution[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 2;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

// Test Process ID
CTEST2(roundrobin_1, test_process_1) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// Tests execution time
CTEST2(roundrobin_1, test_executuion_time_1) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i+1, (int)data->task[i].execution_time);
    }
}

// Tests Wait Time
CTEST2(roundrobin_1, test_wait_time_1) {

    float expected = (1 + 3) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(roundrobin_1, test_turnaround_time_1) {

    float expected = (1 + 3 + 6) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);
}

/* ------------------------------------------------------------------------------------

                                TEST SUITE 2 

------------------------------------------------------------------------------------*/
CTEST_DATA(roundrobin_2) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(roundrobin_2) {
    int execution[] = {5, 5, 5};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 2;
    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);

}

// Test Process ID
CTEST2(roundrobin_2, test_process_2) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// Tests execution time
CTEST2(roundrobin_2, test_executuion_time_2) {

    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(5, (int)data->task[i].execution_time);
    }

}

// Tests Wait Time
CTEST2(roundrobin_2, test_wait_time_2) {

    float expected = (8 + 9 + 10) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(roundrobin_2, test_turnaround_time_2) {

    float expected = (13 + 14 + 15) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);
}

/* ------------------------------------------------------------------------------------

                                TEST SUITE 3 

------------------------------------------------------------------------------------*/
CTEST_DATA(roundrobin_3) {
    struct task_t task[4];
    int size;
};

CTEST_SETUP(roundrobin_3) {
    int execution[] = {4, 3, 2, 1};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 3;
    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);

}

// Test Process ID
CTEST2(roundrobin_3, test_process_3) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// Tests execution time
CTEST2(roundrobin_3, test_executuion_time_3) {

    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(4-i, (int)data->task[i].execution_time);
    }

}

// Tests Wait Time
CTEST2(roundrobin_3, test_wait_time_3) {

    float expected = (6 + 3 + 6 + 8) / 4.0;
    float value = calculate_average_wait_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(roundrobin_3, test_turnaround_time_3) {

    float expected = (10 + 6 + 8 + 9)/4.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);
}