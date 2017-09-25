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
	int minMaxEncSpeedDiff;
	int rawValueFL, rawValueFR, rawValueBL, rawValueBR;
	int encValueFL, encValueFR, encValueBL, encValueBR;
	long norm_maximum_val = maximum_val-deadzone;
	// check if the movements are zero
	if (!translate_x && !translate_y && !rotate) {
		speedFL->stop();
		speedFR->stop();
		speedBL->stop();
		speedBR->stop();
		return;
	}
	int proc_translate_x = map(translate_x, -norm_maximum_val, norm_maximum_val, -1024, 1024);
	int proc_translate_y = map(translate_y, -norm_maximum_val, norm_maximum_val, -1024, 1024);
	int proc_rotate = map(rotate, -norm_maximum_val, norm_maximum_val, -1024, 1024);
	rawValueFL = rawValueFR = rawValueBL = rawValueBR = 0;
	// translation and rotation consideration
	rawValueFL += proc_translate_y + proc_translate_x + proc_rotate;
	rawValueFR += proc_translate_y - proc_translate_x - proc_rotate;
	rawValueBL += proc_translate_y - proc_translate_x + proc_rotate;
	rawValueBR += proc_translate_y + proc_translate_x - proc_rotate;
	// figure out max raw value
	maxRawValue = max(max(abs(rawValueFL),abs(rawValueFR)),max(abs(rawValueBL),abs(rawValueBR)));
	minMaxEncSpeedDiff = maxEncSpeed - minEncSpeed;
	// calculate enc speeds and assign to speed controllers if the raw values were nonzero
	// FRONT LEFT
	if (rawValueFL) {
		encValueFL = (int)(((float)rawValueFL/maxRawValue)*minMaxEncSpeedDiff) + minEncSpeed;
		speedFL->setControlEnc(encValueFL);
	}
	else { 
		speedFL->stop();
	}
	// FRONT RIGHT
	if (rawValueFR) {
		encValueFR = (int)(((float)rawValueFR/maxRawValue)*minMaxEncSpeedDiff) + minEncSpeed;
		speedFR->setControlEnc(encValueFR);
	}
	else {
		speedFR->stop();
	}
	// BACK LEFT
	if (rawValueBL) {
		encValueBL = (int)(((float)rawValueBL/maxRawValue)*minMaxEncSpeedDiff) + minEncSpeed;
		speedBL->setControlEnc(encValueBL);
	}
	else {
		speedBL->stop();
	}
	// BACK RIGHT
	if (rawValueBR) {
		encValueBR = (int)(((float)rawValueBR/maxRawValue)*minMaxEncSpeedDiff) + minEncSpeed;
		speedBR->setControlEnc(encValueBR);
	}
	else {
		speedBR->stop();
	}
	// print values for debug purposes
	Serial.print(encValueFL);
	Serial.print(" ");
	Serial.print(encValueFR);
	Serial.print(" ");
	Serial.print(encValueBL);
	Serial.print(" ");
	Serial.println(encValueBR);
}

void MechanumController::setTranslateX(long val) {
	if (val < 0) {
		val += deadzone;
	}
	else if (val > 0) {
		val -= deadzone;
	}
	translate_x = val;
}

void MechanumController::setTranslateY(long val) {
	if (val < 0) {
		val += deadzone;
	}
	else if (val > 0) {
		val -= deadzone;
	}
	translate_y = val;
}

void MechanumController::setRotate(long val) {
	if (val < 0) {
		val += deadzone;
	}
	else if (val > 0) {
		val -= deadzone;
	}
	rotate = val;
}

