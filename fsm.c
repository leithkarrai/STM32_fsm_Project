/*
 * fsm_elevator.c
 *
 *  Created on: Dec 2, 2024
 *      Author: mathi
 */

#include "fsm.h"

#define next 1
#define prev 2
int test=0;
int BTNFlag=0;
uint32_t fsm_t_ref = 0;


State_Enum actual_state = START_STATE;
// FSM inputs

//ExecutionState_Enum execution_state = STATE_NOT_EXECUTED;

// Function pointer array storing states callbacks
// /!\ STORED IN THE SAME ORDER AS UserInput_Enum
void (*fsm_callbacks[])(void) = {
	start_state_callback,
	chaser_state_callback,
	buzzer_state_callback,
	jukebox_state_callback,
	display_state_callback,
};

void start_state_callback(void) {

	__NOP();
}

void chaser_state_callback(void) {
	test=1;

	__NOP();
}

void buzzer_state_callback(void){
	test=2;
	__NOP();
}
void jukebox_state_callback(void){
	test=3;
	__NOP();
}
void display_state_callback(void){
	test=4;
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

	fsm_t_ref = HAL_GetTick();
	//execution_state = STATE_NOT_EXECUTED;
}

/**
 * @brief Sets new state
 * @param _new_state New state to be set
 * @return void
 */
void fsm_set_new_state(State_Enum _new_state) {
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
	//execution_state = STATE_EXECUTED;

	// 3 - Check for transitions
	switch (actual_state) {
	case START_STATE:
		fsm_set_new_state(CHASER_STATE);

		break;
	case CHASER_STATE:
		if (BTNFlag == next) {
			BTNFlag-=next;
			fsm_set_new_state(BUZZER_STATE);
		}
		if (BTNFlag == prev){
			BTNFlag-=prev;
			fsm_set_new_state(DISPLAY_STATE);
		}
		break;
	case BUZZER_STATE:
		if (BTNFlag == next) {
			BTNFlag-=next;
			fsm_set_new_state(JUKEBOX_STATE);
		}
		if (BTNFlag == prev){
			BTNFlag-=prev;
			fsm_set_new_state(CHASER_STATE);
		}
		break;
	case JUKEBOX_STATE:
		if (BTNFlag == next) {
			BTNFlag-=next;
			fsm_set_new_state(DISPLAY_STATE);
		}
		if (BTNFlag == prev){
			BTNFlag-=prev;
			fsm_set_new_state(BUZZER_STATE);
		}
	case DISPLAY_STATE:
		if (BTNFlag == next) {
			BTNFlag-=next;
			fsm_set_new_state(CHASER_STATE);
		}
		if (BTNFlag == prev){
			BTNFlag-=prev;
			fsm_set_new_state(JUKEBOX_STATE);
		}
	}
}

