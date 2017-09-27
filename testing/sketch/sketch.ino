#include "ScrapController.h"

#define FRONT_LEFT_PIN_INTERRUPT 2
#define FRONT_LEFT_PIN_CHECKER 14
#define FRONT_RIGHT_PIN_INTERRUPT 3
#define FRONT_RIGHT_PIN_CHECKER 15
#define BACK_LEFT_PIN_INTERRUPT 18
#define BACK_LEFT_PIN_CHECKER 16
#define BACK_RIGHT_PIN_INTERRUPT 19
#define BACK_RIGHT_PIN_CHECKER 17

#define FRONT_LEFT_MOTOR_PWM 4
#define FRONT_LEFT_MOTOR_PIN1 24
#define FRONT_LEFT_MOTOR_PIN2 25
#define FRONT_RIGHT_MOTOR_PWM 5
#define FRONT_RIGHT_MOTOR_PIN1 22
#define FRONT_RIGHT_MOTOR_PIN2 23
#define BACK_LEFT_MOTOR_PWM 12
#define BACK_LEFT_MOTOR_PIN1 34
#define BACK_LEFT_MOTOR_PIN2 35
#define BACK_RIGHT_MOTOR_PWM 6
#define BACK_RIGHT_MOTOR_PIN1 32
#define BACK_RIGHT_MOTOR_PIN2 33



ScrapEncoder encoderFL = ScrapEncoder(FRONT_LEFT_PIN_INTERRUPT, FRONT_LEFT_PIN_CHECKER);
ScrapEncoder encoderFR = ScrapEncoder(FRONT_RIGHT_PIN_INTERRUPT, FRONT_RIGHT_PIN_CHECKER);
ScrapEncoder encoderBL = ScrapEncoder(BACK_LEFT_PIN_INTERRUPT, BACK_LEFT_PIN_CHECKER);
ScrapEncoder encoderBR = ScrapEncoder(BACK_RIGHT_PIN_INTERRUPT, BACK_RIGHT_PIN_CHECKER);

ScrapMotor motorFL = ScrapMotor(FRONT_LEFT_MOTOR_PIN1, FRONT_LEFT_MOTOR_PIN2, FRONT_LEFT_MOTOR_PWM);
ScrapMotor motorFR = ScrapMotor(FRONT_RIGHT_MOTOR_PIN1, FRONT_RIGHT_MOTOR_PIN2, FRONT_RIGHT_MOTOR_PWM);
ScrapMotor motorBL = ScrapMotor(BACK_LEFT_MOTOR_PIN1, BACK_LEFT_MOTOR_PIN2, BACK_LEFT_MOTOR_PWM, -1);
ScrapMotor motorBR = ScrapMotor(BACK_RIGHT_MOTOR_PIN1, BACK_RIGHT_MOTOR_PIN2, BACK_RIGHT_MOTOR_PWM, -1);


ScrapMotorControl speedFL = ScrapMotorControl(motorFL, encoderFL);
ScrapMotorControl speedFR = ScrapMotorControl(motorFR, encoderFR);
ScrapMotorControl speedBL = ScrapMotorControl(motorBL, encoderBL);
ScrapMotorControl speedBR = ScrapMotorControl(motorBR, encoderBR);



bool isMoving = false;

void setup() {
	Serial.begin(9600);
	initEncoders();
	speedFL.setControlEnc(500);
	speedFR.setControlEnc(-500);
	speedBL.setControlEnc(-500);
	speedBR.setControlEnc(500);
	unsigned long startTime = micros();
	unsigned long currentTime = startTime;
	while (currentTime - startTime < 1000000) {
		performAllMovement();
		delay(2);
		currentTime = micros();
	}
	speedFL.setControlEnc(-500);
	speedFR.setControlEnc(500);
	speedBL.setControlEnc(500);
	speedBR.setControlEnc(-500);
	startTime = micros();
	currentTime = startTime;
	while (currentTime - startTime < 1000000) {
		performAllMovement();
		delay(2);
		currentTime = micros();
	}
	printEncodersToSerial();
	speedFL.stop();
	speedFR.stop();
	speedBL.stop();
	speedBR.stop();
	performAllMovement();
	/*motorFL.setMotor(255);
	motorBL.setMotor(255);
	motorFR.setMotor(255);
	motorBR.setMotor(255);
	delay(1000);
	printEncodersToSerial();
	motorFL.setMotor(255);
	motorBL.setMotor(255);
	motorFR.setMotor(255);
	motorBR.setMotor(255);
	delay(1000);
	printEncodersToSerial();
	motorFL.stop();
	motorBL.stop();
	motorFR.stop();
	motorBR.stop();
	printEncodersToSerial();*/
}

void loop() {
	delay(1000);
	printEncodersToSerial();
}

void printEncodersToSerial() {
	Serial.print("FL: " + (String)encoderFL.getCount());
	Serial.print("	FR: " + (String)encoderFR.getCount());
	Serial.print("	BL: " + (String)encoderBL.getCount());
	Serial.println("	BR: " + (String)encoderBR.getCount());
}

void performAllMovement() {
	speedFL.performMovement();
	speedFR.performMovement();
	speedBL.performMovement();
	speedBR.performMovement();
}

void initEncoders() {
	attachInterrupt(digitalPinToInterrupt(FRONT_LEFT_PIN_INTERRUPT),checkEncoderFL,CHANGE);
	attachInterrupt(digitalPinToInterrupt(FRONT_RIGHT_PIN_INTERRUPT),checkEncoderFR,CHANGE);
	attachInterrupt(digitalPinToInterrupt(BACK_LEFT_PIN_INTERRUPT),checkEncoderBL,CHANGE);
	attachInterrupt(digitalPinToInterrupt(BACK_RIGHT_PIN_INTERRUPT),checkEncoderBR,CHANGE);
}

void checkEncoderFL() {
	encoderFL.checkEncoderFlipped();
}

void checkEncoderFR() {
	encoderFR.checkEncoder();
}

void checkEncoderBL() {
	encoderBL.checkEncoderFlipped();
}

void checkEncoderBR() {
	encoderBR.checkEncoder();
}
