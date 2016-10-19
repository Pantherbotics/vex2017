#pragma config(Motor,  port1,           drCenterA,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           drFrontLeft,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           drBackLeft,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           inChainA,      tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           inChainC,      tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           drFrontRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           drBackRight,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           inChainB,      tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           inChainD,      tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          drCenterB,     tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"


//------------Joystick Mappings------------
#define joyForward Ch3;
#define JoyStrafe Ch1;
#define joyLeftTurn Ch5;
#define joyRightTurn Ch6;

//--------------Motor Inverts--------------
// Drive
#define mInvertCenterA 1;
#define mInvertCenterB 1;
#define mInvertFrontLeft 1;
#define mInvertFrontRight 1;
#define mInvertBackLeft 1;
#define mInvertBackRight 1;

//Intake
#define mInvertChainA 1;
#define mInvertChainB 1;
#define mInvertChainC 1;
#define mInvertChainD 1;

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;
}

task autonomous()
{
  // Remove this function call once you have "real" code.
  AutonomousCodePlaceholderForTesting();
}

task usercontrol()
{
  // User control code here, inside the loop

  while (true)
  {
    UserControlCodePlaceholderForTesting();
  }
}

void driveOnControllerInput {
  //Vertical Left Josystick - Forward/Back
  //Horizontal Right Joystick - Left/Right
  //Trigger Up - Fast Turn
  //Trigger Down - Slow Turn

  //NOTE: 'Forward' on robot is oriented 'left' (90deg counterclockwise shift)

}

void setIntakeMotors (int speed) {
  motor[inChainA] = speed;      
  motor[inChainB] = speed * -1; //A and B are opposite mechanically linked by axel
  motor[inChainC] = speed * -1; //A and C are opposite mechanically linked by gear
  motor[inChainD] = speed;      //C and D are opposite mechanically linked by axel
}


void setDriveMotors(int fL, int fR, int bL, int bR,int cn) {
  motor[drFrontRight] = fL * mInvertFrontRight;
  motor[drFrontLeft]  = fR * mInvertFrontLeft;
  motor[drBackLeft]   = bL * mInvertBackLeft;
  motor[drBackRight]  = bR * mInvertBackRight;
  motor[drCenterA]    = cn * mInvertCenterA;
  motor[drCenterA]    = cn * mInvertCenterB * -1;
}








