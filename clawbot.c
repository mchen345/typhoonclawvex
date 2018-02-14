#pragma config(Motor,  port1,           right,         tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port6,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           arm,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          left,          tmotorVex393_HBridge, openLoop)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in8,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl3,  ClawEncoder_in, sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  ArmEncoder_in,  sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           drive_left,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           drive_right,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           scissor_left,  tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port4,           scissor_right, tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port5,           bottom_left,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           bottom_right,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           claw_top,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           arm_top,       tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//////////////////////////////////////////////////////////PARAMETER///////////////////////////////////////////////////////////////////

//PID CONSTANTS
float straight_p = 3.5;					//P constant for Straight_PID
float default_pid_straight = 100;			//Default motor value for PID_Straight
float default_pid_turn = 100; 				//Default motor value for PID_Turn
float default_pid_arm = 100; //Default motor value for PID_arm
float default_pid_claw = 70; //Default motor value for PID_claw
float default_pid_bottom = 70; //Default motor value for PID_bottom
float moveScissor_p = 0.2;
float default_pid_scissor = 100;
float initial, difference=0.0;

//Bottom claw motor values
int bottom_out_fast = 125;
int bottom_in_fast = -125;
int bottom_out_slow = 50;
int bottom_in_slow = -70;
//Scissor values
//float ratio; Unused Variable
//float difference; Unused Variable
//float scissor_p = 2; Unused Variable

///////////////////////////////////////////////////////FUNCTION_PROTOTYPE//////////////////////////////////////////////////////////////
void auto_straight_ms(float t);//Go straight (millisecond)
void auto_straight_cm(float cm);//Go straight (centimeter)
void auto_turn_degree(float degree);//turn (degree)
void auto_scissor(float degree);//move scissor (degree)
void auto_claw_arm(float degree);//move top claw arm (degree)
void auto_top_claw(float degree);//move top claw(degree)
void auto_bottomIn(float t);//move bottom claw in (millisecond)
void auto_bottomOut(float t);//move bottom claw out (millisecond)
void moveBot();//Drive train controlled by one joystick (ch1&ch2)
void moveScissor();//Scissor motor controlled by partner joystick (ch3Xmtr2)
void moveClaw_Bottom();//move bottom claw by partner joystick (btn5Xmtr2&btn6Xmtr2) for fast and slow
void moveClaw_top();//move top claw by partner joystick (Btn8UXmtr2&Btn8DXmtr2)
void auto();//autonomous main function

/////////////////////////////////////////////////////////MAIN/////////////////////////////////////////////////////////////////////////
task main()
{
	while(true){
		moveBot();
		moveScissor();
		moveClaw_Bottom();
	}
}
///////////////////////////////////////////////////////////AUTO////////////////////////////////////////////////////////////////////////

void auto(){

}

///////////////////////////////////////////////////////////AUTO_PID////////////////////////////////////////////////////////////////////

//PID_Straight
void auto_straight_ms(float t){ //Go straight for certain milisecond
	clearTimer(T1);
	SensorValue(gyro) = 0;
	initial = SensorValue(gyro);
	while(time1[T1] <= t){
		difference = SensorValue(gyro)-initial;
		motor[drive_right] = -1*(default_pid_straight-difference*straight_p); 		//Setting speed of motor accourding to the difference
		motor[drive_left] = default_pid_straight+difference*straight_p;
		//Setting speed of motor accourding to the difference
	}
}

void auto_straight_cm(float cm){ //Go straight for certain cmc
	float initial, difference=0;
	SensorValue[I2C_1] = 0;
	SensorValue[gyro]=0;
	while(SensorValue(I2C_1)*0.09163 < cm){
		initial = SensorValue[gyro];
		difference = SensorValue[gyro]-initial;
		motor[drive_right] = default_pid_straight+difference*straight_p; 		//Setting speed of motor accourding to the difference
		motor[drive_left] = default_pid_straight+difference*straight_p; 		//Setting speed of motor accourding to the difference
	}
	SensorValue[I2C_1] = 0;
}

//PID_Turn
void auto_turn_degree(float degree){ //Degree --> -360~360
	degree = degree*10;
	SensorValue[gyro] = 0;
	if (degree < 0){
		//Turn right
		while(SensorValue[gyro] >= degree){
			motor[drive_right] = -1*default_pid_turn;
			motor[drive_left] = default_pid_turn;
		}
	}
	else{
		//Turn left
		while(SensorValue[gyro] <= degree){
			motor[drive_right] = default_pid_turn;
			motor[drive_left] = -1*default_pid_turn;
		}
	}
}
///////////////////////////////////////////////////////////AUTO_FCN/////////////////////////////////////////////////////////////////////
//Measures 627.2 ticks per revolution --> Intergrated Encoder
void auto_scissor(float degree){
	if (degree > 0){
		while(SensorValue(I2C_1) <= (1.742*degree)){ //Might explode
			if (SensorValue(I2C_1)> SensorValue(I2C_2)){
				motor[scissor_right] = default_pid_scissor+default_pid_scissor*moveScissor_p;
			}
			if(SensorValue(I2C_1)< SensorValue(I2C_2)){
				motor[scissor_left] = default_pid_scissor+default_pid_scissor*moveScissor_p;
			}
		}
	}
	else{
		while(SensorValue(I2C_1) >= (1.742*degree)){ //Might explode
			if (SensorValue(I2C_1)> SensorValue(I2C_2)){
				motor[scissor_right] = -1*(default_pid_scissor+default_pid_scissor*moveScissor_p);
			}
			if(SensorValue(I2C_1)< SensorValue(I2C_2)){
				motor[scissor_left] = -1*(default_pid_scissor+default_pid_scissor*moveScissor_p);
			}
		}
	}
}

void auto_claw_arm(float degree){
	if (degree > 0){
		while(SensorValue(ArmEncoder_in) <= degree){
			motor[arm_top] = default_pid_arm;
		}
		}else{
		while(SensorValue(ArmEncoder_in)>= degree){
			motor[arm_top] = -1*default_pid_arm;
		}
	}
}

void auto_top_claw(float degree){
	if (degree > 0){
		while(SensorValue(ClawEncoder_in) <= degree){
			motor[arm_top] = default_pid_claw;
		}
		while(SensorValue(ClawEncoder_in)>= degree){
			motor[arm_top] = -1*default_pid_claw;
		}
	}
}
void movewheel(){//motor controlled by one joystick
	float y = vexRT[Ch2]/2;//get joystick value
	//change to left side control 2017/11/10
	float x = vexRT[Ch1]/2;//get joystick value
	//change to left 2017/11/10
	y*=0.9;
	x*=0.9;
	float left_motor;//variable for speed of left motor
	float right_motor;//variable for speed of right motor
	float power, st;//power for speed to distribute, st for steering
	power = sqrt(x*x+y*y);//calculating power
	if(y>=0&&power!=0){//check joystick position of y
		st = x * 100/power;//set value for st
		if(x>=0){//check joystick position of x
			left_motor = power;//set speed

void auto_bottomIn(float t){
	clearTimer(T2);
	while(time1[T2] < t){
		motor[bottom_left] = default_pid_bottom;
		motor[bottom_right] = default_pid_bottom;
	}
}

void auto_bottomOut(float t){
	clearTimer(T2);
	while(time1[T2] < t){
		motor[bottom_left] = default_pid_bottom;
		motor[bottom_right] = default_pid_bottom;
	}
}

//////////////////////////////////////////////////////////DRIVER CONTROL///////////////////////////////////////////////////////////////

void moveBot(){																	//Motor controlled by one joystick
	float y = vexRT[Ch2];														//Get joystick value
	//Change to left side control 2017/11/10
	float x = vexRT[Ch1];														//Get joystick value
	//Change to left 2017/11/10
	float left_motor;							//Variable for speed of left motor
	float right_motor;							//Variable for speed of right motor
	float power, st;							//Power for speed to distribute, st for steering
	power = sqrt(x*x+y*y);						//Calculating power
	if(y>=0&&power!=0){							//Check joystick position of y
		st = x * 100/power;						//Set value for st
		if(x>=0){								//Check joystick position of x
			left_motor = power;//Set speed
			right_motor = (1-0.02*st)*power;
		}
		else{
			right_motor = power;
			left_motor = (1+0.02*st)*power;
		}
	}
	if(y<=0&&power!=0){
		power*=-1;
		st = x * 100/power;
		if(x>=0){
			left_motor = (1+0.02*st)*power;
			right_motor = power;
		}
		else{
			right_motor = (1-0.02*st)*power;
			left_motor = power;
		}
	}
	if(x==0&&y==0){
		motor[drive_left] = 0;
		motor[drive_right] = 0;
	}
	motor[drive_left] = left_motor;				//Set speed for left motor
	motor[drive_right] = right_motor;			//Set speed for right motor
}

void movearm(){
	/*
	if(vexRT[Btn5U] == 1){//check of button 5U is pressed
		motor[port7] = 75;//lift arm if it does not exceed limit
	}
	else if(vexRT[Btn5D] == 1){//check of button 5D is pressed
		motor[port7] = -50;//lower arm
	}
	else{
		motor[port7] = 15;//prevent arm from falling down due to gravity
	}
	*/
	//2017/11/10 change to joystick control
	if(vexRT[ch3]>-10&&vexRT[ch3]<10){
		motor[port7] = 15;
	}else{
		motor[port7] = vexRT[ch3]/2;
	}
}

void moveclaw(){
	/*
	if(vexRT(Btn6U) == 1){//check of button 6U is pressed
		motor[port6] = 100;//open claw
	}
	else if(vexRT(Btn6D) == 1){//check of button 6D is pressed
		motor[port6] = -25;//close claw
	}else{
		motor[port6] = 0;//assure claw is shut tightly
	}	
	*/
	//change control 2017/11/10
	if(vexRT[Btn6U]==1
		||vexRT[Btn8R]==1)
	{
		motor[port6] = 100;
	}
	else if(vexRT[Btn8D]==1
					||vexRT[Btn6D]==1)
	{
		motor[port6] = -25;
	}else{
		motor[port6] = 0;
	}
}	
/*
void lineFollow(float motor1, float motor10, float difference, float a){
	if(sensorvalue(armstop)==1){//button for safety
			difference = sensorValue(light_right)-sensorValue(light_left);//difference between two light sensor
			motor[port1] = motor1+difference*a;//setting speed of motor, check if robot should turn
			motor[port10] = motor10+difference*a;//setting speed of motor, check if robot should turn
	}else{
			motor[port1] = 0;//stop if button is not pressed
			motor[port10] = 0;//stop if button is not pressed
void moveScissor(){
	if (SensorValue(I2C_1) > SensorValue(I2C_2)){
		//difference = SensorValue(I2C_1) - SensorValue(I2C_2);
		motor[scissor_right] = vexRT[Ch3Xmtr2]+vexRT[Ch3Xmtr2]*moveScissor_p;
		motor[scissor_left] = vexRT[Ch3Xmtr2];
		}else if(SensorValue(I2C_1) < SensorValue(I2C_2)){
		//difference = SensorValue(I2C_2) - SensorValue(I2C_1);
		motor[scissor_left] = vexRT[Ch3Xmtr2]+vexRT[Ch3Xmtr2]*moveScissor_p;
		motor[scissor_right] = vexRT[Ch3Xmtr2];
		}else{
		motor[scissor_left] = vexRT[Ch3Xmtr2];
		motor[scissor_right] = vexRT[Ch3Xmtr2];
	}
}

void moveClaw_Bottom(){
	if(vexRT[Btn6UXmtr2]==1){					//Move bottom claw out
		motor[bottom_left]=bottom_out_fast;
		motor[bottom_right]=bottom_out_fast;
		}else if(vexRT[Btn6DXmtr2]==1){				//Move bottom claw in
		motor[bottom_left]=bottom_in_fast;
		motor[bottom_right]=bottom_in_fast;
		}else if(vexRT[Btn5UXmtr2]==1){
		motor[bottom_left]=bottom_out_slow;
		motor[bottom_right]=bottom_out_slow;
		}else if(vexRT[Btn5DXmtr2]==1){				//Move bottom claw in
		motor[bottom_left]=bottom_in_slow;
		motor[bottom_right]=bottom_in_slow;
		}else{										//Stop bottom claw
		motor[bottom_left]=0;
		motor[bottom_right]=0;
	}
}

void moveClaw_top(){
	if(vexRT[Btn8UXmtr2]==1){
		motor[claw_top] = 100;
		}else if(vexRT[Btn8DXmtr2]==1){
		motor[claw_top] = -100;
		}else{
		motor[claw_top] = 10;
	}
}
*/
/*void testStraight(){
	motor[port1]=vexRT[ch2];
	motor[port10]=-vexRT[ch2];
}
*/

task main(){
	// variable initializing for line following mechanism
	/*
	float motor1 = 25;//initializing right motor speed
	float motor10 = -25;//initializing left motor speed
	float difference = 0;//difference between two sensor values
	float a = -0.03325//adjusting constant
	*/
	while(true){
		movearm();
		moveclaw();
		movewheel();
	}
void moveArm_top(){

}
