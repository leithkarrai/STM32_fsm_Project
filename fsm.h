/*
 * fsm_elevator.h
 *
 *  Created on: Dec 2, 2024
 *      Author: mathi
 */

#ifndef FSM_ELEVATOR_FSM_ELEVATOR_H_
#define FSM_ELEVATOR_FSM_ELEVATOR_H_

#include "stm32l1xx_hal.h"
#include "main.h"


typedef enum {
	START_STATE,  			// = 0
	CHASER_STATE,   		// = 1
	BUZZER_STATE,    		// = 2
	JUKEBOX_STATE,			// = 3
	DISPLAY_STATE,
} State_Enum;


/*
typedef enum {
	STATE_NOT_EXECUTED,
	STATE_EXECUTED,
} ExecutionState_Enum;
*/

void start_state_callback(void);
void chaser_state_callback(void);
void buzzer_state_callback(void);
void jukebox_state_callback(void);
void display_state_callback(void);

void fsm_init(void);
void fsm_set_new_state(State_Enum _new_state);
void fsm_execute(void);


#endif /* FSM_ELEVATOR_FSM_ELEVATOR_H_ */
