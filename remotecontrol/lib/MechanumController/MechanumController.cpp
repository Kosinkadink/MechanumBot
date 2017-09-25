#include "MechanumController.h"

MechanumController::MechanumController() {

}

MechanumController::MechanumController(ScrapMotorControl& speed_FL, ScrapMotorControl& speed_FR, ScrapMotorControl& speed_BL, ScrapMotorControl& speed_BR) {
	speedFL = &speed_FL;
	speedFR = &speed_FR;
	speedBL = &speed_BL;
	speedBR = &speed_BR;
}


void MechanumController::performMovement() {
	inputControlValues();
	speedFL->performMovement();
	speedFR->performMovement();
	speedBL->performMovement();
	speedBR->performMovement();
}

void MechanumController::inputControlValues() {
	// initialize raw values to consider
	int maxRawValue;
	int rawValueFL, rawValueFR, rawValueBL, rawValueBR;
	int encValueFL, encValueFR, encValueBL, encValueBR;
	// check if the movements are zero
	if (!translate_x && !translate_y && !rotate) {
		speedFL->stop();
		speedFR->stop();
		speedBL->stop();
		speedBR->stop();
		return;
	}
	int proc_translate_x = map(translate_x, -maximum_val, maximum_val, -1024, 1024);
	int proc_translate_y = map(translate_y, -maximum_val, maximum_val, -1024, 1024);
	int proc_rotate = map(rotate, -maximum_val, maximum_val, -1024, 1024);
	rawValueFL = rawValueFR = rawValueBL = rawValueBR = 0;
	// translation and rotation consideration
	rawValueFL += proc_translate_y + proc_translate_x + proc_rotate;
	rawValueFR += proc_translate_y - proc_translate_x - proc_rotate;
	rawValueBL += proc_translate_y - proc_translate_x + proc_rotate;
	rawValueBR += proc_translate_y + proc_translate_x - proc_rotate;
	// figure out max raw value
	maxRawValue = max(max(abs(rawValueFL),abs(rawValueFR)),max(abs(rawValueBL),abs(rawValueBR)));
	// calculate enc speeds and assign to speed controllers
	encValueFL = (int)(((float)rawValueFL/maxRawValue)*maxEncSpeed);
	if (abs(encValueFL) >= minEncSpeed)
		speedFL->setControlEnc(encValueFL);
	else
		speedFL->stop();
	encValueFR = (int)(((float)rawValueFR/maxRawValue)*maxEncSpeed);
	if (abs(encValueFR) >= minEncSpeed)
		speedFR->setControlEnc(encValueFR);
	else
		speedFR->stop();
	encValueBL = (int)(((float)rawValueBL/maxRawValue)*maxEncSpeed);
	if (abs(encValueBL) >= minEncSpeed)
		speedBL->setControlEnc(encValueBL);
	else
		speedBL->stop();
	encValueBR = (int)(((float)rawValueBR/maxRawValue)*maxEncSpeed);
	if (abs(encValueBR) >= minEncSpeed)
		speedBR->setControlEnc(encValueBR);
	else
		speedBR->stop();
	Serial.print(encValueFL);
	Serial.print(" ");
	Serial.print(encValueFR);
	Serial.print(" ");
	Serial.print(encValueBL);
	Serial.print(" ");
	Serial.println(encValueBR);
}