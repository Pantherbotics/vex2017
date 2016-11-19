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

//----------------------Competition Includes-----------------------//
#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

//------------------------Joystick Mappings------------------------//
#define joyForward Ch3      //Forward on first stick
#define joyRotate Ch1       //Sideways on second stick

#define joyLeftTurnF Btn6U
#define joyLeftTurnS Btn6D
#define joyRightTurnF Btn5U
#define joyRightTurnS Btn5D

//--------------------------Motor Inverts--------------------------//
#define mInvertCenterA 1
#define mInvertCenterB -1
#define mInvertFrontLeftA -1
#define mInvertFrontLeftB 1
#define mInvertFrontRightA -1
#define mInvertFrontRightB -1
#define mInvertBackLeftA 1
#define mInvertBackLeftB 1
#define mInvertBackRightA 1
#define mInvertBackRightB 1

//--------------------------Encoder Inverts-------------------------//
#define eInvertFrontLeft -1
#define eInvertFrontRight -1
#define eInvertBackLeft 1
#define eInvertBackRight 1
#define eInvertCenter -1

//----------------------------Constants----------------------------//
const float deadzoneJoyForward = 10.5;    //Forward joystick deadzone
const float deadzoneJoyRotate = 10.5;     //Rotation joystick deadzone

const float incrementSlowTurn = 45;        //Button turn slow speed
const float incrementFastTurn = 55;        //Button turn fast speed
const float incrementForward  = 60;
const float incrementRotate = 60;

//----------------------------Variables----------------------------//
int targetAngle = 0;   //angle (in degrees) for gyroscope code to maintain
int lastAngleError = 0;

//0-FrontLeft; 1-FrontRight; 2-BackLeft; 3-BackRight; 4-Center
int targetDrive[5];    //Target encoder positions for all five drivetrain encoders
int errorDrive[5];     //Accumulated error for all five drivetrain encoders

float incF = 0;
float incR = 0;

bool deploy = false

//-------------------------Helper Functions-------------------------//

//Increments encoder target for a given motor
void incrementMotorTarget(int mIdx, int increment){targetDrive[mIdx] += increment;}

//Increments all five encoder targets at once
void incrementDriveTargets(int fl, int fr, int bl, int br, int ce){
  incrementMotorTarget(0,fl);
  incrementMotorTarget(1,fr);
  incrementMotorTarget(2,bl);
  incrementMotorTarget(3,br);
  incrementMotorTarget(4,ce);
}

void incrementTank(int left, int right){
  incrementMotorTarget(0,left);
  incrementMotorTarget(2,right);
}

//Reset all encoder targets, encoder values, and error values to zero
void resetEncoders(){
  targetDrive[0]=0;targetDrive[1]=0;targetDrive[2]=0;targetDrive[3]=0;targetDrive[4]=0;
  errorDrive[0]=0;errorDrive[1]=0;errorDrive[2]=0;errorDrive[3]=0;errorDrive[4]=0;
  SensorValue[encFrontLeft]=0;SensorValue[encFrontRight]=0;SensorValue[encBackLeft]=0;SensorValue[encBackRight]=0;SensorValue[encCenter]=0;
}

//Drivetrain Helper Functions
void setFrontLeftDrive(int fl) {if(fl==0){return;};motor[drFrontLeftA]  = fl * mInvertFrontLeftA; motor[drFrontLeftB]  = fl * mInvertFrontLeftB  * -1;}
void setFrontRightDrive(int fr){if(fr==0){return;};motor[drFrontRightA] = fr * mInvertFrontRightA;motor[drFrontRightB] = fr * mInvertFrontRightB * -1;}
void setBackLeftDrive(int bl)  {if(bl==0){return;};motor[drBackLeftA]   = bl * mInvertBackLeftA;  motor[drBackLeftB]   = bl * mInvertBackLeftB   * -1;}
void setBackRightDrive(int br) {if(br==0){return;};motor[drBackRightA]  = br * mInvertBackRightA; motor[drBackRightB]  = br * mInvertBackRightB  * -1;}
void setCenterDrive(int ce)    {if(ce==0){return;};motor[drCenterA] = ce * mInvertCenterA;motor[drCenterB] = ce * mInvertCenterB * -1;}

//Encoder Helper Functions
int getFrontLeftDrive() {return SensorValue[encFrontLeft]  * eInvertFrontLeft;}
int getFrontRightDrive(){return SensorValue[encFrontRight] * eInvertFrontRight;}
int getBackLeftDrive()  {return SensorValue[encBackLeft]   * eInvertBackLeft;}
int getBackRightDrive() {return SensorValue[encBackRight]  * eInvertBackRight;}
int getCenterDrive()    {return SensorValue[encCenter]     * eInvertCenter;}

//Nullify encoder error
void softResetError(){
  targetDrive[0]=getFrontLeftDrive();
  targetDrive[1]=getFrontRightDrive();
  targetDrive[2]=getBackLeftDrive();
  targetDrive[3]=getBackRightDrive();
  targetDrive[4]=getCenterDrive();
}

//-------------------------Program Functions-------------------------//
void fakeRotate(int power){
  setFrontLeftDrive(-power);
  setFrontRightDrive(-power);
  setBackLeftDrive(power);
  setBackRightDrive(power);
  softResetError();
}


void setGyroTargetToPosition(){
  targetAngle = SensorValue[gyroscope];
}

//Calculate motor power based on encoder error (PID)
int calcMotorTarget(int currentPos, int idx){
  int error = targetDrive[idx] - currentPos;
  int lastError = errorDrive[idx];
  errorDrive[idx] = error;
  int errorDiff = error - lastError;

  if (fabs(error) < 4){error = 0;};
  int power = (errorDiff* 0.6) + (error * 0.4);
  if (fabs(power) < 20){error = 0;};
  return power;
}

void driveTank(){
  int lPower = calcMotorTarget(getFrontLeftDrive(),0);
  int rPower = calcMotorTarget(getBackLeftDrive(),2);
  setFrontLeftDrive(lPower);
  setFrontRightDrive(lPower);
  setBackLeftDrive(rPower);
  setBackRightDrive(rPower);
  if((lPower/fabs(lPower))==(rPower/fabs(rPower))){
    setCenterDrive(lPower);
  }
}

//Helper function to calculate PID and set motor power for all drive motors
void calcMotorValues(){
  setFrontLeftDrive(calcMotorTarget(getFrontLeftDrive(),0));
  setFrontRightDrive(calcMotorTarget(getFrontRightDrive(),1));
  setBackLeftDrive(calcMotorTarget(getBackLeftDrive(),2));
  setBackRightDrive(calcMotorTarget(getBackRightDrive(),3));
  setCenterDrive(calcMotorTarget(getCenterDrive(),4));
}

void driveTargetsWithGyroCorrection(int fl, int fr, int bl, int br, int ce){
  int gyroError = (SensorValue[gyroscope] - targetAngle);
  float gyCr = (gyroError * 0.5);
  if (fabs(ce) <= 10) {gyCr = 0;} //Nullify error if we're not driving forward
  int lastAngleError = gyroError;
  gyCr = 0;
  incrementDriveTargets(fl - gyCr,
                        fr - gyCr,
                        bl + gyCr,
                        br + gyCr,
                        ce);
}

//Get joystick inputs, calculate encoder offset, and
void driveOnControllerInput () {
  //NOTE: 'Forward' on robot is oriented 'left' in respect to drivetrain (90deg counterclockwise shift)
  int rawFwd = vexRT[joyForward];
  int rawRot = vexRT[joyRotate];
  float smthFwd = 0.0;
  float smthRot = 0.0;

  //Deadzone calculations
  if (fabs(rawFwd) > deadzoneJoyForward){
    smthFwd = rawFwd/127.0;
  }else{
    smthFwd = 0;
  }

  if (fabs(rawRot) > deadzoneJoyRotate){
    smthRot = rawRot/127.0;
  }else{
    smthRot = 0;
  }

  if (vexRT[joyLeftTurnS] || vexRT[joyLeftTurnF]) {
    int str=(incrementSlowTurn*vexRT[joyLeftTurnS]) + (incrementFastTurn*vexRT[joyLeftTurnF]);
    //incrementDriveTargets(-str,-str,str,str,0);
    fakeRotate(100);
    setGyroTargetToPosition()

  }else if (vexRT[joyRightTurnS] || vexRT[joyRightTurnF]){
    int str=(incrementSlowTurn*vexRT[joyRightTurnS]) + (incrementFastTurn*vexRT[joyRightTurnF]);
    //incrementDriveTargets(str,str,-str,-str,0);
    fakeRotate(-100);
    setGyroTargetToPosition()

  } else{
    incF = smthFwd * incrementForward;
    incR = smthRot * incrementRotate;

    if (incR > 0){setGyroTargetToPosition();}
    incrementTank(incF + incR,   //Front
                  incF - incR);  //Back
  }
}

void gyroInit(){
  //Completely clear out any previous sensor readings by setting the port to "sensorNone"
  SensorType[gyroscope] = sensorNone;
  wait1Msec(1000);
  //Reconfigure port as a gyroscope and allow time for calibration.
  SensorType[gyroscope] = sensorGyro;
  wait1Msec(2000);
 //Set Roll-over point to +/- 100 full rotations
 SensorFullCount[gyroscope] = 36000;
}

void auto(){
	incrementDriveTargets(1500,1500,1500,1500,1500);
	calcMotorValues();
  while(fabs(errorDrive[0]) > 10){
    calcMotorValues();
  }
}

void deploySw(){

  SensorValue[solExtend] = !deploy;
  SensorValue[solDeployA] = !deploy;
  SensorValue[solDeployB] = !deploy;
  SensorValue[solDeployC] = !deploy;
  deploy = !deploy;
  wait1Msec(100);
}

//Pre-auton init
void pre_auton(){
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// bDisplayCompetitionStatusOnLcd = false;

}

//Autonomous control
task autonomous(){

}

//Manual Control
task usercontrol(){
  resetEncoders();
  gyroInit();
  while (true){
  	wait1Msec(50);
    driveOnControllerInput();
    driveTank();
    if(vexRT[Btn7U]){deploySw();};
  }
}
