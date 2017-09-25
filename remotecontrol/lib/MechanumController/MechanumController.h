#ifndef MECHANUMCONTROLLER
#define MECHANUMCONTROLLER
#include "Arduino.h"
#include "ScrapController.h"


class MechanumController {
	private:
		int minEncSpeed = 100;
		int maxEncSpeed = 2000;
		long translate_x = 0;
		long translate_y = 0;
		long rotate = 0;
		long maximum_val = 32768;
		ScrapMotorControl* speedFL;
		ScrapMotorControl* speedFR;
		ScrapMotorControl* speedBL;
		ScrapMotorControl* speedBR;
	public:
		void attachSpeedFL(ScrapMotorControl& speed_control) { speedFL = &speed_control; };
		void attachSpeedFR(ScrapMotorControl& speed_control) { speedFR = &speed_control; };
		void attachSpeedBL(ScrapMotorControl& speed_control) { speedBL = &speed_control; };
		void attachSpeedBR(ScrapMotorControl& speed_control) { speedBR = &speed_control; };
		MechanumController();
		MechanumController(ScrapMotorControl& speed_FL, ScrapMotorControl& speed_FR, ScrapMotorControl& speed_BL, ScrapMotorControl& speed_BR);
		void setTranslateX(long val) { translate_x = val; };
		void setTranslateY(long val) { translate_y = val; };
		void setRotate(long val) { rotate = val; };
		void performMovement();
		void inputControlValues();

};


#endif