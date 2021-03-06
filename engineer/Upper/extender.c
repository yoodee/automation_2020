#include "extender.h"
#include "pneumatic.h"
#include "upper.h"
#include "engg_gpio.h"

/* VARIABLES: EXTENDER - related */
Extender extender;
extern struct upper_info upper;
extern upper_ctrl upper_controller;
/* END of VARIABLES: EXTENDER - related */

/* FUNCTIONS: EXTENDER - related */
void extender_extend(void);
void extender_retract(void);

int32_t extender_execute(struct upper_info* upperinf, upper_ctrl* upperctrl) {
	if (upperinf == NULL || upperctrl == NULL)
		return -RM_INVAL;
	
	if (extender.request == RETRACT)
		extender_retract();
	else if (extender.request == EXTEND)
		extender_extend();
	
	return RM_OK;
}

void extender_extend_cd(void) {
	if (extender.state != EXTENDER_EXTENDED) {
		osDelay(EXTENDER_RETRACT_MS);
		extender.state = EXTENDER_EXTENDED;
	}
}

void extender_extend(void) {
	pneum_1head_LOW(&(extender.cylinderX1));
}

void extender_retract_cd(void) {
	if (extender.state != EXTENDER_RETRACTED) {
		osDelay(EXTENDER_RETRACT_MS);
		extender.state = EXTENDER_RETRACTED;
	}
}

void extender_retract(void) {
	pneum_1head_HIGH(&(extender.cylinderX1));
}

void extender_state_change(int state) {
	extender.request = state;
}

int return_extender_state(void) {
	return extender.state;
}
/* END of FUNCTIONS: EXTENDER - related */

/* RTOS: EXTENDER - related */
void extender_task(void const *argument)
{
  uint32_t period = osKernelSysTick();
	
	extender.cylinderX1 = init_pneum_1head(EXTEND_GPIO_Port, EXTEND_Pin);
	extender.request = RETRACT;
	extender.state = RETRACT;
	
	for(;;) {
		extender_execute(&upper, &upper_controller);

    osDelayUntil(&period, 2);
	}
}
/* END of RTOS: EXTENDER - related */
