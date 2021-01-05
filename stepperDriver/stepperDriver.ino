const int enable =  8;
const int xDir =    5;
const int yDir  =   6;
const int zDir =    7;
const int aDir =    13;
const int xStep =   2;
const int yStep =   3;
const int zStep =   4;
const int aStep =   12;


#include <Wire.h>



// An array to keep track of number of steps needed until state completed
// in the order of [a,x,y,z]
int steps[4] = {0,0,0,0};

//Nema 17 and CNC Shield has 3200 steps per rotation
// 800 steps is equal to 90 degrees of rotation
int steps_revolution = 3200;

//Sets the speed of the Stepper Motor
int stepDelay = 600;


void setup() {
  
  Serial.begin(9600);
  pinMode(xDir,OUTPUT);
  pinMode(xStep,OUTPUT);  
  pinMode(yDir,OUTPUT);
  pinMode(yStep,OUTPUT); 
  pinMode(zDir,OUTPUT);
  pinMode(zStep,OUTPUT);
  pinMode(aDir,OUTPUT);
  pinMode(aStep,OUTPUT);  
  pinMode(enable,OUTPUT);
  digitalWrite(enable,LOW);

  Wire.begin(4);                // join i2c bus with address #4

  //Calls the receiveEvent function when data is received from Master Controller
  Wire.onReceive(receiveEvent);
  

}

void loop() {
  
  step_motors();

}

//Steps a given motor one step in the given direction
// True is clockwise, False is counterclockwise
void step (bool dir, int dirPin, int stepperPin) {
  digitalWrite(dirPin,dir);
  digitalWrite(stepperPin, HIGH);
  digitalWrite(stepperPin, LOW);
  delayMicroseconds(stepDelay);

}


// Iterates through the steps array and steps each motor the required number of steps
// This allows for simultaneous movement of stepper motors
void step_motors() {
  
  for (int i = 0; i < 4; i++){
    //Rotate clockwise to loaded state
    if (steps[i]>0){
      //Case for A motor because A pins are not sequential on CNC Shield
      if (i == 0) {
        step (true, aDir,aStep);
        steps[i] -= 1;
        }else{
          step (true, (4+i),(1+i) );
          steps[i] -= 1;
        }
      //Rotate counterclockwise to unloaded state
    } else if (steps[i] < 0){
      if (i == 0) {
        step (false, aDir,aStep);
        steps[i] += 1;
        }else{
          step (false, (4+i),(1+i));
          steps[i] += 1;
        }
      
    }
    
  }
  
}


void receiveEvent(int howMany)
{
  // Reads value sent from Master Controller which reads the FSR's and updates LEDs
  int x = Wire.read();

  int remaining = 0;
  
  //Updates the amount of steps remaining for each Stepper Motor based on the state of the panel (loaded or unloaded)
  //Panel A : 0 is loaded, 1 is unloaded
  //Panel X : 2 is loaded, 3 is unloaded
  //Panel Y : 4 is loaded, 5 is unloaded
  //Panel Z : 6 is loaded, 7 is unloaded
  
    switch (x) {
      
      case 0:
        remaining = steps[0];
        steps[0] = remaining + 800;
        break;   
      case 1:
        remaining = steps[0];
        steps[0] = (remaining - 800);
        
        break;
      case 2:
        remaining = steps[1];
        steps[1] = remaining + 800;
        break;
      case 3:
        remaining = steps[1];
        steps[1] = (remaining - 800);
        break;
      case 4:
        remaining = steps[2];
        steps[2] = remaining + 800;
        break;
      case 5:
        remaining = steps[2];
        steps[2] = (remaining - 800);
        break;
      case 6:
        remaining = steps[3];
        steps[3] = remaining + 800;
        break;
      case 7:
        remaining = steps[3];
        steps[3] = (remaining - 800);
        break;
    }
}
