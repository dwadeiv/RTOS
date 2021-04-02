#include <stdlib.h>
#include "ctest.h"
#include "fcfs.h"

// Note: the name in the first parameter slot must match all tests in that group

/* ------------------------------------------------------------------------------------

                                TEST SUITE 1 
                                
------------------------------------------------------------------------------------*/
CTEST_DATA(firstcomefirstserved_1) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(firstcomefirstserved_1) {
    int execution[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, data->size);
    first_come_first_served(data->task, data->size);

}

// Test Process ID
CTEST2(firstcomefirstserved_1, test_process_1) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// Tests execution time
CTEST2(firstcomefirstserved_1, test_executuion_time_1) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i+1, (int)data->task[i].execution_time);
    }
}

// Tests Wait Time
CTEST2(firstcomefirstserved_1, test_wait_time_1) {

    float expected = (1 + 3) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(firstcomefirstserved_1, test_turnaround_time_1) {

    float expected = (1 + 3 + 6) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);
}

/* ------------------------------------------------------------------------------------

                                TEST SUITE 2 

------------------------------------------------------------------------------------*/
CTEST_DATA(firstcomefirstserved_2) {
    struct task_t task[4];
    int size;
};

CTEST_SETUP(firstcomefirstserved_2) {
    int execution[] = {5, 32, 19, 5};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, data->size);
    first_come_first_served(data->task, data->size);

}

// Test Process ID
CTEST2(firstcomefirstserved_2, test_process_2) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// Tests execution time
CTEST2(firstcomefirstserved_2, test_executuion_time_2) {

    ASSERT_EQUAL(5, (int)data->task[0].execution_time);
    ASSERT_EQUAL(32, (int)data->task[1].execution_time);
    ASSERT_EQUAL(19, (int)data->task[2].execution_time);
    ASSERT_EQUAL(5, (int)data->task[3].execution_time);

}

// Tests Wait Time
CTEST2(firstcomefirstserved_2, test_wait_time_2) {

    float expected = (5 + 37 + 56) / 4.0;
    float value = calculate_average_wait_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(firstcomefirstserved_2, test_turnaround_time_2) {

    float expected = (5 + 37 + 56 + 61) / 4.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    printf("\nvalue is %f", value);
    printf("\nexpected is %f", expected);
    ASSERT_EQUAL(expected, value);
}

/* ------------------------------------------------------------------------------------

                                TEST SUITE 3 

------------------------------------------------------------------------------------*/
CTEST_DATA(firstcomefirstserved_3) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(firstcomefirstserved_3) {
    int execution[] = {5, 5, 5};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, data->size);
    first_come_first_served(data->task, data->size);

}

// Test Process ID
CTEST2(firstcomefirstserved_3, test_process_3) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// Tests execution time
CTEST2(firstcomefirstserved_3, test_executuion_time_3) {

    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(5, (int)data->task[i].execution_time);
    }
}

// Tests Wait Time
CTEST2(firstcomefirstserved_3, test_wait_time_3) {

    float expected = (5 + 10) / 3.0;
    float value = calculate_average_wait_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);

}

// Tests Turnaround Time
CTEST2(firstcomefirstserved_3, test_turnaround_time_3) {

    float expected = (5 + 10 + 15) / 3.0;
    float value = calculate_average_turn_around_time(data->task, data->size);
    ASSERT_EQUAL(expected, value);
}



