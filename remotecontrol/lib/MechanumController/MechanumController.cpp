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
	long norm_maximum_val = maximum_val-deadzone;
	long proc_maximum_val = 1024;
	// check if the movements are zero
	if (!translate_x && !translate_y && !rotate) {
		speedFL->stop();
		speedFR->stop();
		speedBL->stop();
		speedBR->stop();
		return;
	}
	int proc_translate_x = map(translate_x, -norm_maximum_val, norm_maximum_val, -proc_maximum_val, proc_maximum_val);
	int proc_translate_y = map(translate_y, -norm_maximum_val, norm_maximum_val, -proc_maximum_val, proc_maximum_val);
	int proc_rotate = map(rotate, -norm_maximum_val, norm_maximum_val, -proc_maximum_val, proc_maximum_val);
	rawValueFL = rawValueFR = rawValueBL = rawValueBR = 0;
	// translation and rotation consideration
	rawValueFL += proc_translate_y + proc_translate_x + proc_rotate;
	rawValueFR += proc_translate_y - proc_translate_x - proc_rotate;
	rawValueBL += proc_translate_y - proc_translate_x + proc_rotate;
	rawValueBR += proc_translate_y + proc_translate_x - proc_rotate;
	// figure out max raw value; this will be used to scale the speed of the motors if max is greater than a motor can output
	maxRawValue = max(max(abs(rawValueFL),abs(rawValueFR)),max(abs(rawValueBL),abs(rawValueBR)));
	// if max value does not exceed the normalized allowed maximum value, use the normalized max as max raw value
	if (maxRawValue < proc_maximum_val) {
		maxRawValue = proc_maximum_val;
	}
	// calculate enc speeds and assign to speed controllers if the raw values were nonzero
	// FRONT LEFT
	if (rawValueFL) {
		encValueFL = map(rawValueFL,0,maxRawValue,minEncSpeed,maxEncSpeed);
		speedFL->setControlEnc(encValueFL);
	}
	else { 
		speedFL->stop();
	}
	// FRONT RIGHT
	if (rawValueFR) {
		encValueFR = map(rawValueFR,0,maxRawValue,minEncSpeed,maxEncSpeed);
		speedFR->setControlEnc(encValueFR);
	}
	else {
		speedFR->stop();
	}
	// BACK LEFT
	if (rawValueBL) {
		encValueBL = map(rawValueBL,0,maxRawValue,minEncSpeed,maxEncSpeed);
		speedBL->setControlEnc(encValueBL);
	}
	else {
		speedBL->stop();
	}
	// BACK RIGHT
	if (rawValueBR) {
		encValueBR = map(rawValueBR,0,maxRawValue,minEncSpeed,maxEncSpeed);
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

