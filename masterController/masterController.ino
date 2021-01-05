
// Include the Arduino Stepper.h library:
#include <Wire.h>


const int aA                 = 0;
const int aX                 = 1;
const int aY                 = 2;
const int aZ                 = 3;

const int ledA               = 2;
const int ledX               = 3;
const int ledY               = 4;
const int ledZ               = 5;


// Array for LEDs in following order [a,x,y,z]
boolean lights[4] = {false, false, false, false};

int val_a = 0;
int state_a = 0;
int state_prev_a = 0;
unsigned long t_a = 0;
unsigned long t0_a = 0;

int val_x = 0;
int state_x = 0;
int state_prev_x = 0;
unsigned long t_x = 0;
unsigned long t0_x = 0;

int val_y = 0;
int state_y = 0;
int state_prev_y = 0;
unsigned long t_y = 0;
unsigned long t0_y = 0;


int val_z = 0;
int state_z = 0;
int state_prev_z = 0;
unsigned long t_z = 0;
unsigned long t0_z = 0;

// Debounce delay to avoid false state changes
unsigned long debounceDelay = 300; 

// Threshhold for Loaded/Unloaded reading of FSR
// Depending on mechanical configuration, this value 
// may vary for each panel

int fsr_a_threshhold = 500;
int fsr_x_threshhold = 400;
int fsr_y_threshhold = 600;
int fsr_z_threshhold = 400;



void setup() {
  Serial.begin(9600);
 
  Wire.begin();
  
  pinMode(ledA, OUTPUT);
  pinMode(ledX, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledZ, OUTPUT);
  
 
}

void loop() {
  // Run the State Machine of each Motor/FSR/LED combo
  SM_a();
  SM_x();
  SM_y();
  SM_z();

  //Send updated states if applicable
  transmit_states();
}
// Polling Functions

// Polls the state of the lights in the lights array
void poll_lights() {
  if (lights[0]){
    digitalWrite(ledA, HIGH);
  }else{
    digitalWrite(ledA, LOW);
    }
   if (lights[1]){
    digitalWrite(ledX, HIGH);
  }else{
    digitalWrite(ledX, LOW);
    }
    if (lights[2]){
    digitalWrite(ledY, HIGH);
  }else{
    digitalWrite(ledY, LOW);
    }
    if (lights[3]){
    digitalWrite(ledZ, HIGH);
  }else{
    digitalWrite(ledZ, LOW);
    }
}

//// sends State change to Stepper Driver controller and updates LED states
void transmit_states(){

  //Checks to see if state is loaded and was not previously loaded
  
    if (state_a == 5 && state_prev_a != 5){
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(0);        
    Wire.endTransmission();
    lights[0] = true;
  }
  //Checks to see if state is unloaded
  if (state_a == 4) {
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(1);        
    Wire.endTransmission();
    lights[0] = false;
  }
  if (state_x == 5 && state_prev_x != 5){
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(2);        
    Wire.endTransmission();
    lights[1] = true;
  }
  if (state_x == 4) {
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(3);        
    Wire.endTransmission();
    lights[1] = false;
  }
    if (state_y == 5 && state_prev_y != 5){
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(4);        
    Wire.endTransmission();
    lights[2] = true;
  }
  if (state_y == 4) {
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(5);        
    Wire.endTransmission();
    lights[2] = false;
  }
    if (state_z == 5 && state_prev_z != 5){
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(6);        
    Wire.endTransmission();
    lights[3] = true;
  }
  if (state_z == 4) {
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(7);        
    Wire.endTransmission();
    lights[3] = false;
  }
  poll_lights();
}

// STATE MACHINES
// Manages the state of the Motor/FSR/LED combo and updates according to 
// FSR reading

void SM_z (){
  state_prev_z = state_z;
  
  switch (state_z){
    case 0: //RESET
      state_z = 1;
      break;
      
    case 1: //START
      val_z = analogRead(aZ);

      if (val_z >= fsr_z_threshhold) {state_z = 2;}
      
      break;
      
     case 2: // SET TIME
      t0_z = millis();
      state_z = 3;
      break;
      
     case 3:// WAIT AND CHECK
      val_z = analogRead(aZ);
      t_z = millis();

      if (val_z < fsr_z_threshhold) {state_z = 0;}

      if (t_z - t0_z > debounceDelay){
        state_z = 5;
      }
      break;
      
     case 4: // UNLOADED
      state_z = 0;
      break;
      
     case 5: // LOADED
      val_z = analogRead(aZ);
      if (val_z < fsr_z_threshhold) {state_z = 4;}
      break; 
  }
}

void SM_y (){
  state_prev_y = state_y;
  
  switch (state_y){
    case 0: //RESET
      state_y = 1;
      break;
      
    case 1: //START
      val_y = analogRead(aY);

      if (val_y >= fsr_y_threshhold) {state_y = 2;}
      
      break;
      
     case 2: // SET TIME
      t0_y = millis();
      state_y = 3;
      break;
      
     case 3:// WAIT AND CHECK
      val_y = analogRead(aY);
      t_y = millis();

      if (val_y < fsr_y_threshhold) {state_y = 0;}

      if (t_y - t0_y > debounceDelay){
        state_y = 5;
      }
      break;
      
     case 4: // UNLOADED
      state_y = 0;
      break;
      
     case 5: // LOADED
      val_y = analogRead(aY);
      if (val_y < fsr_y_threshhold) {state_y = 4;}
      break;
     
    
  }
  

}

void SM_x (){
  state_prev_x = state_x;
  
  switch (state_x){
    case 0: //RESET
      state_x = 1;
      break;
      
    case 1: //START
      val_x = analogRead(aX);

      if (val_x >= fsr_x_threshhold) {state_x = 2;}
      
      break;
      
     case 2: // SET TIME
      t0_x = millis();
      state_x = 3;
      break;
      
     case 3:// WAIT AND CHECK
      val_x = analogRead(aX);
      t_x = millis();

      if (val_x < fsr_x_threshhold) {state_x = 0;}

      if (t_x - t0_x > debounceDelay){
        state_x = 5;
      }
      break;
      
     case 4: // UNLOADED
      state_x = 0;
      break;
      
     case 5: // LOADED
      val_x = analogRead(aX);
      if (val_x < fsr_x_threshhold) {state_x = 4;}
      break;
     
    
  }
  

}




void SM_a (){
  state_prev_a = state_a;
  
  switch (state_a){
    case 0: //RESET
      state_a = 1;
      break;
      
    case 1: //START
      val_a = analogRead(aA);

      if (val_a >= fsr_a_threshhold) {state_a = 2;}
      
      break;
      
     case 2: // SET TIME
      t0_a = millis();
      state_a = 3;
      break;
      
     case 3:// WAIT AND CHECK
      val_a = analogRead(aA);
      t_a = millis();

      if (val_a < fsr_a_threshhold) {state_a = 0;}

      if (t_a - t0_a > debounceDelay){
        state_a = 5;
      }
      break;
      
     case 4: // UNLOADED
      state_a = 0;
      break;
      
     case 5: // LOADED
      val_a = analogRead(aA);
      if (val_a < fsr_a_threshhold) {state_a = 4;}
      break;
  }
}
