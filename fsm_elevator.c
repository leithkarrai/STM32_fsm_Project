/*
 * fsm_elevator.c
 *
 *  Created on: Dec 2, 2024
 *      Author: mathi
 */

#include "fsm_elevator.h"


State_Enum actual_state = START_STATE;

// FSM inputs
int actual_floor = 0;
int target_floor = 0;
UserInput_Enum user_input = USER_INPUT_INVALID;
ButtonState_Enum emergency_stop_button = RELEASED;

uint32_t fsm_t_ref = 0;
ExecutionState_Enum execution_state = STATE_NOT_EXECUTED;

// Function pointer array storing states callbacks
// /!\ STORED IN THE SAME ORDER AS UserInput_Enum
void (*fsm_callbacks[])(void) = {
	start_state_callback,
	wfua_state_callback,
	up_state_callback,
	down_state_callback,
	emergency_stop_state_callback,
	stop_state_callback,
};


void start_state_callback(void) {
	__NOP();
}

void wfua_state_callback(void) {
	if (execution_state == STATE_NOT_EXECUTED) {
		// Reset input buffer
		// Do anything related to stat initialization
		user_input = USER_INPUT_INVALID;
	}
	__NOP();
}

void up_state_callback(void) {
	__NOP();
}

void down_state_callback(void) {
	__NOP();
}

void emergency_stop_state_callback(void) {
	__NOP();
}

void stop_state_callback(void) {
	__NOP();
}


/**
 * @brief Initializes FSM
 * @param void
 * @return void
 */
void fsm_init(void) {
	// Reset actual state
	actual_state = START_STATE;

	// Reset inputs
	actual_floor = 0;
	target_floor = 0;
	user_input = USER_INPUT_INVALID;

	fsm_t_ref = HAL_GetTick();
	execution_state = STATE_NOT_EXECUTED;
}

/**
 * @brief Sets new state
 * @param _new_state New state to be set
 * @return void
 */
void fsm_set_new_state(State_Enum _new_state) {
	fsm_t_ref = HAL_GetTick();
	execution_state = STATE_NOT_EXECUTED;
	actual_state = _new_state;
}

/**
 * @brief Executes a frame of the finite state machine and take care of transitions if needed
 * @param void
 * @return void
 */
void fsm_execute(void) {
	// 1 - State execution
	fsm_callbacks[actual_state]();

	// 2 - (optional) Inputs update
	execution_state = STATE_EXECUTED;

	// 3 - Check for transitions
	switch (actual_state) {
	case START_STATE:
		fsm_set_new_state(WFUA_STATE);
		break;

	case WFUA_STATE:
		if (emergency_stop_button == PRESSED) {
			fsm_set_new_state(EMERGENCY_STOP_STATE);
		}

		if (user_input == USER_INPUT_INVALID)
			break;

		if (target_floor > actual_floor) {
			fsm_set_new_state(UP_STATE);
		} else if (target_floor < actual_floor) {
			fsm_set_new_state(DOWN_STATE);
		}

		break;

	case UP_STATE:
		if (emergency_stop_button == PRESSED) {
			fsm_set_new_state(EMERGENCY_STOP_STATE);
		}

		if (target_floor == actual_floor) {
			fsm_set_new_state(STOP_STATE);
		}
		break;

	case DOWN_STATE:
		if (emergency_stop_button == PRESSED) {
			fsm_set_new_state(EMERGENCY_STOP_STATE);
		}

		if (target_floor == actual_floor) {
			fsm_set_new_state(STOP_STATE);
		}
		break;

	case EMERGENCY_STOP_STATE:
		break;

	case STOP_STATE:
		if (emergency_stop_button == PRESSED) {
			fsm_set_new_state(EMERGENCY_STOP_STATE);
		}

		fsm_set_new_state(WFUA_STATE);
		break;
	}
}

