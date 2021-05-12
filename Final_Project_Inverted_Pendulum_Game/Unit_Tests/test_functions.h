
#ifndef __UNIT_TEST__
#define __UNIT_TEST__

//----------------------------------------------------------------------------------------------------------------------------------
/// @brief Structure which holds the Unit Test Data
//----------------------------------------------------------------------------------------------------------------------------------
struct data_t {

    /// PWM Duty Cycle
    int pwm_duty_cycle;

    /// Current_Force
    int current_force;

    // Max Force Value
    int max_force;

    // Min Force Value
    int min_force;

    // Current Gain Value
    int current_gain;

    // Current X position Value
    int current_x_position;

    // Max X position Value
    int max_x_position;

    // Min X position Value
    int min_x_position;

    // Current Pole Theta Value
    int current_theta;
};

//----------------------------------------------------------------------------------------------------------------------------------
/// @brief Structure which holds the different force states
//----------------------------------------------------------------------------------------------------------------------------------
typedef enum {

    MAX_LEFT_FORCE,
    LEFT_FORCE,
    RIGHT_FORCE,
    MAX_RIGHT_FORCE

}e_Force_Types;

//----------------------------------------------------------------------------------------------------------------------------------
/// @brief Intialize the data array
///
/// @param[in] data The buffer containing data sets
/// @param[in] pwm_duty_cycle The pwm_duty_cycle for each data set
/// @param[in] current_gain The current gain for each data set
/// @param[in] current_x_position The current X position for each data set
/// @param[in] current_theta The current theta for each data set
/// @param[in] max_force The max force
/// @param[in] min_force The min force
/// @param[in] max_x_position The max x position
/// @param[in] min_x_position The min x position
/// @param[in] size The size of the buffer
//----------------------------------------------------------------------------------------------------------------------------------
void init(struct data_t *data, int *pwm_duty_cycle, int *current_gain, int *current_x_position, int *current_theta, int current_force, int max_force, int min_force, int max_x_position, int min_x_position, int size);

//----------------------------------------------------------------------------------------------------------------------------------
/// @brief Check to see if queue is empty
/// @return 1 if queue is empty, 0 if it is not empty
//----------------------------------------------------------------------------------------------------------------------------------
int fifo_queue_isEmpty();

//----------------------------------------------------------------------------------------------------------------------------------
/// @brief Push to FIFO queue
/// @return Value that was pushed
//----------------------------------------------------------------------------------------------------------------------------------
int fifo_queue_push(int value);

//----------------------------------------------------------------------------------------------------------------------------------
/// @brief Pop FIFO queue
/// @return Return isEmpty() to ensure value was popped from queue
//----------------------------------------------------------------------------------------------------------------------------------
int fifo_queue_pop();


#endif // __FIRST_COME_FIRST_SERVED__  