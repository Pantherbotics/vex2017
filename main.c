#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyroscope,      sensorGyro)
#pragma config(Sensor, dgtl1,  solExtend,      sensorDigitalOut)
#pragma config(Sensor, dgtl2,  solDeployA,     sensorDigitalOut)
#pragma config(Sensor, dgtl3,  solDeployB,     sensorDigitalOut)
#pragma config(Sensor, dgtl4,  solDeployC,     sensorDigitalOut)
#pragma config(Sensor, I2C_1,  encFrontRight,  sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  encBackRight,   sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  encCenter,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  encBackLeft,    sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  encFrontLeft,   sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           drCenterA,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           drFrontLeftA,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           drBackLeftA,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           drFrontLeftB,  tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           drBackLeftB,   tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           drFrontRightA, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           drBackRightA,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           drFrontRightB, tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           drBackRightB,  tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          drCenterB,     tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"


//----------------Joystick Mappings----------------//
#define joyForward Ch1      //Forward on first stick
#define joyRotate Ch3       //Sideways on second stick

#define joyLeftTurnF Btn5U
#define joyLeftTurnS Btn5D
#define joyRightTurnF Btn6U
#define joyRightTurnS Btn6D

//------------------Motor Inverts------------------//

#define mInvertCenterA -1
#define mInvertCenterB -1
#define mInvertFrontLeftA 1
#define mInvertFrontLeftB 1
#define mInvertFrontRightA -1
#define mInvertFrontRightB -1
#define mInvertBackLeftA 1
#define mInvertBackLeftB 1
#define mInvertBackRightA 1
#define mInvertBackRightB 1

#define eInvertFrontLeft -1
#define eInvertFrontRight -1
#define eInvertBackLeft 1
#define eInvertBackRight 1
#define eInvertCenter -1

//--------------------Constants--------------------//
const float deadzoneJoyForward = 1.5;
const float deadzoneJoyRotate = 1.5;

//Variables/
int targetAngle = 0;
int targetDrive[5]; //0-FrontLeft;1-FrontRight;2-BackLeft;3-BackRight;4-Center

void incrementDriveTargets(int fl, int fr, int bl, int br, int ce){
  targetDrive[0] = targetDrive[0] + fl;
  targetDrive[1] = targetDrive[1] + fr;
  targetDrive[2] = targetDrive[2] + bl;
  targetDrive[3] = targetDrive[3] + br;
  targetDrive[4] = targetDrive[4] + ce;
}

//Drivetrain Helper Functions
void setFrontLeftDrive(int fl) {motor[drFrontLeftA]  = fl * mInvertFrontLeftA; motor[drFrontLeftB]  = fl * mInvertFrontLeftB  * -1;}
void setFrontRightDrive(int fr){motor[drFrontRightA] = fr * mInvertFrontRightA;motor[drFrontRightB] = fr * mInvertFrontRightB * -1;}
void setBackLeftDrive(int bl)  {motor[drBackLeftA]   = bl * mInvertBackLeftA;  motor[drBackLeftB]   = bl * mInvertBackLeftB   * -1;}
void setBackRightDrive(int br) {motor[drBackRightA]  = br * mInvertBackRightA; motor[drBackRightB]  = br * mInvertBackRightB  * -1;}
void setCenterDrive(int ce)    {motor[drCenterA] = ce * mInvertCenterA;motor[drCenterB] = ce * mInvertCenterB * -1;}

//Encoder Helper Functions
int getFrontLeftDrive() {return SensorValue[encFrontLeft]  * eInvertFrontLeft;}
int getFrontRightDrive(){return SensorValue[encFrontRight] * eInvertFrontRight;}
int getBackLeftDrive()  {return SensorValue[encBackLeft]   * eInvertBackLeft;}
int getBackRightDrive() {return SensorValue[encBackRight]  * eInvertBackRight;}
int getCenterDrive()    {return SensorValue[encCenter]     * eInvertCenter;}

int calcMotorTarget(int currentPos, int targetPos){
  int goal = targetPos - currentPos;
  if (fabs(goal) < 4){goal = 0;};
  int power = goal * 0.3;
  return power;
}

void calcMotorValues(){
  setFrontLeftDrive( calcMotorTarget(getFrontLeftDrive(),  targetDrive[0]));
  setFrontRightDrive(calcMotorTarget(getFrontRightDrive(), targetDrive[1]));
  setBackLeftDrive(  calcMotorTarget(getBackLeftDrive(),   targetDrive[2]));
  setBackRightDrive( calcMotorTarget(getBackRightDrive(),  targetDrive[3]));
  setCenterDrive(    calcMotorTarget(getCenterDrive(),     targetDrive[4]));
}

void resetEncoders(){targetDrive[0] = 0;targetDrive[1] = 0;targetDrive[2] = 0;targetDrive[3]=0;targetDrive[4]=0;
  SensorValue[encFrontLeft]=0;SensorValue[encFrontRight]=0;SensorValue[encBackLeft]=0;SensorValue[encBackRight]=0;SensorValue[encCenter]=0;
}

void incrementMotorTarget(int mIdx, int increment){targetDrive[mIdx] += increment;}

void driveOnControllerInput () {

  //NOTE: 'Forward' on robot is oriented 'left' in respect to motors (90deg counterclockwise shift)
  int rawStr = vexRT[joyForward];
  int rawFwd = vexRT[joyRotate];
  int smthFwd = 0;
  int smthStr = 0;

  //Deadzone calculations
  if (rawStr > deadzoneJoyRotate || rawStr < -deadzoneJoyRotate){
    smthStr = rawStr;
  }else{
    smthStr = 0;
  }

  if (rawFwd > deadzoneJoyForward || rawFwd < -deadzoneJoyForward){
    smthFwd = rawFwd;
  }else{
    smthFwd = 0;
  }

  if (vexRT[joyLeftTurnS] || vexRT[joyLeftTurnF]) {
    int str=50*vexRT[joyLeftTurnS]+90*vexRT[joyLeftTurnF];
    incrementDriveTargets(-str,-str,str,str,0);

  }else if (vexRT[joyRightTurnS] || vexRT[joyRightTurnF]){
    int str=50*vexRT[joyRightTurnS]+90*vexRT[joyRightTurnF];
    incrementDriveTargets(str,str,-str,-str,0);

  } else{
    float gyroDiff = (SensorValue[gyroscope] - targetAngle) * 0.3;
    incrementDriveTargets(smthFwd,   //FrontLeft
                          smthFwd,   //FrontRight
                          smthFwd,   //BackLeft
                          smthFwd,   //BackRight
                          smthFwd);  //Center
  }
}
void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// bDisplayCompetitionStatusOnLcd = false;

}

task autonomous(){
}

task usercontrol(){
  resetEncoders();
  while (true){
  	wait1Msec(50);
    driveOnControllerInput();
  	calcMotorValues();
  	writeDebugStreamLine("FL%i, FR%i, BL%i, BR%i, CE%i", getFrontLeftDrive(), getFrontRightDrive(), getBackLeftDrive(), getBackRightDrive(), getCenterDrive())
  }
}
