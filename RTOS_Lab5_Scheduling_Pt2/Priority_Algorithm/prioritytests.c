#include <stdlib.h>
#include "ctest.h"
#include "priority.h"

/* ------------------------------------------------------------------------------------

                                TEST SUITE 1 
                                
------------------------------------------------------------------------------------*/
CTEST_DATA(priority_1) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(priority_1) {
    int execution[] = {1, 2, 3};
    int priority[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    priority_schedule(data->task, data->size);
}

// Test Process ID
CTEST2(priority_1, test_process_1) {

    ASSERT_EQUAL(2, (int)data->task[0].process_id);
    ASSERT_EQUAL(1, (int)data->task[1].process_id);
    ASSERT_EQUAL(0, (int)data->task[2].process_id);

}

// Tests execution time
CTEST2(priority_1, test_executuion_time_1) {

    ASSERT_EQUAL(3, (int)data->task[0].execution_time);
    ASSERT_EQUAL(2, (int)data->task[1].execution_time);
    ASSERT_EQUAL(1, (int)data->task[2].execution_time);

}

// Tests Wait Time
CTEST2(priority_1, test_wait_time_1) {

    float expected = (1 + 4 + 2) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(priority_1, test_turnaround_time_1) {

    float expected = (2 + 6 + 5) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);
}

// Tests Priority
CTEST2(priority_1, test_priority_1) {

    ASSERT_EQUAL(24, (int)data->task[0].priority);
    ASSERT_EQUAL(16, (int)data->task[1].priority);
    ASSERT_EQUAL(8, (int)data->task[2].priority);
}

/* ------------------------------------------------------------------------------------

                                TEST SUITE 2 
                                
------------------------------------------------------------------------------------*/
CTEST_DATA(priority_2) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(priority_2) {
    int execution[] = {5, 2, 3};
    int priority[] = {5, 10, 15};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    priority_schedule(data->task, data->size);
}

// Test Process ID
CTEST2(priority_2, test_process_2) {

    ASSERT_EQUAL(2, (int)data->task[0].process_id);
    ASSERT_EQUAL(1, (int)data->task[1].process_id);
    ASSERT_EQUAL(0, (int)data->task[2].process_id);

}

// Tests execution time
CTEST2(priority_2, test_executuion_time_2) {

    ASSERT_EQUAL(3, (int)data->task[0].execution_time);
    ASSERT_EQUAL(2, (int)data->task[1].execution_time);
    ASSERT_EQUAL(5, (int)data->task[2].execution_time);

}

// Tests Wait Time
CTEST2(priority_2, test_wait_time_2) {

    float expected = (2 + 2 + 5) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);

} 

// Tests Turnaround Time
CTEST2(priority_2, test_turnaround_time_2) {

    float expected = (5 + 4 + 10) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);
}

// Tests Priority
CTEST2(priority_2, test_priority_2) {

    ASSERT_EQUAL(60, (int)data->task[0].priority);
    ASSERT_EQUAL(80, (int)data->task[1].priority);
    ASSERT_EQUAL(40, (int)data->task[2].priority);
}
