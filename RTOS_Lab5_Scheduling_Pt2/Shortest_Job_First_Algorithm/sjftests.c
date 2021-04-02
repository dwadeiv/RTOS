#include <stdlib.h>
#include "ctest.h"
#include "sjf.h"
#include <time.h>

/* ------------------------------------------------------------------------------------

                                TEST SUITE 1 
                                
------------------------------------------------------------------------------------*/
CTEST_DATA(shortestjobfirst_1) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(shortestjobfirst_1) {
    int execution[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

// Test Process ID
CTEST2(shortestjobfirst_1, test_process_1) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// Tests execution time
CTEST2(shortestjobfirst_1, test_executuion_time_1) {

    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i+1, (int)data->task[i].execution_time);
    }
    
}

// Tests Wait Time
CTEST2(shortestjobfirst_1, test_wait_time_1) {

    float expected = (1 + 3) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(shortestjobfirst_1, test_turnaround_time_1) {

    float expected = (1 + 3 + 6) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);
}

/* ------------------------------------------------------------------------------------

                                TEST SUITE 2 
                                
------------------------------------------------------------------------------------*/
CTEST_DATA(shortestjobfirst_2) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(shortestjobfirst_2) {
    int execution[] = {5, 10, 1};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

// Test Process ID
CTEST2(shortestjobfirst_2, test_process_2) {
    
    ASSERT_EQUAL(2, (int)data->task[0].process_id);
    ASSERT_EQUAL(0, (int)data->task[1].process_id);
    ASSERT_EQUAL(1, (int)data->task[2].process_id);

}

// Tests execution time
CTEST2(shortestjobfirst_2, test_executuion_time_2) {

    ASSERT_EQUAL(1, (int)data->task[0].execution_time);
    ASSERT_EQUAL(5, (int)data->task[1].execution_time);
    ASSERT_EQUAL(10, (int)data->task[2].execution_time);
    
}

// Tests Wait Time
CTEST2(shortestjobfirst_2, test_wait_time_2) {

    float expected = (1 + 6) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(shortestjobfirst_2, test_turnaround_time_2) {

    float expected = (1 + 6 + 16) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);
}