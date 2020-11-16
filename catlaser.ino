/*
Cat laser chaser toy
(c)2020 by Jurva-markus Vehasmaa

*/
#include <Servo.h>

byte laserPin = 10;
byte servo1Pin = 11;
byte servo2Pin = 12;

Servo servo1;
Servo servo2;


/*
Serial state machine for receiving commands

States:
 0 - wait comand, 1 - wait parameter, 2 - execute and ->0
*/
byte serialState = 0; 
byte command = 0;
byte parameter = 0;

/*
Time based state machine to actually make moves when commanded
*/
unsigned long currentTime; // Current time
unsigned long deltaTime = 500; // Time delay for calculations
unsigned long targetTime; // Time when time state machine should trigger next
/*
Variables to hold angle values for servos and travel
*/
byte P1A1 = 90; // First point
byte P1A2 = 90;
byte P2A1 = 90; // Second point
byte P2A2 = 90;
byte P3A1 = 90; // Support point to hold current angle
byte P3A2 = 90;
byte angleStep = 1; // How many degrees we change at step
byte executeState=0; // Stopped, default

void setup() {
  Serial.begin(9600);
  // Set up time based state machine
  currentTime = millis();
  targetTime = currentTime + deltaTime;
  
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  
  servo1.write(P3A1);
  servo2.write(P3A2);
  
}

void loop() {
  if(Serial.available()>0)
  {
    if(serialState == 1)
    {
      parameter = Serial.read();
      serialState = 2;
    }  
    if(serialState == 0)
    {
      command = Serial.read();
      serialState = 1;
    }  
  }
  if(serialState=2)
  {
    serialState=0;
    // Decode commands and parameter values...
    switch (command){
      case 1:
        // Laser state, parameter: 0=OFF 1=ON
    if(parameter == 1)
    {
      digitalWrite(laserPin, HIGH);
        }
    else
    {
      digitalWrite(laserPin, LOW);
      
    }
    break;
    case 2:
        // Set P1A1 (Point 1, Angle 1)
        P1A1 = parameter;
        break;
    case 4:
        // Set P1A2
        P1A2 = parameter;
        break;
    case 5:
        // Set P2A1
        P2A1 = parameter;
        break;
    case 6:
        // Set P2A2
        P2A2 = parameter;
        break;
    case 8:
        // Execute
        executeState=1;
        break;
    case 9:
        // Stopped
        executeState=0;
        break;
    default:
        serialState=0;
        
        // Do nothing
        break;
    
    }

    // After comand execution we acknoledge it to sender
    Serial.println("Ok");
  }

  // Here we add time based state machine that does movements
  if(targetTime<=millis())
  {
    // Execute state machine
    targetTime = currentTime + deltaTime;

    if(executeState>0)
    {
      // Run math to make move
      P3A1 = byte();
      P3A2 = byte();
      // Update servos
      servo1.write(P3A1);
      servo2.write(P3A2);
      // If P2 reached -> executeState=0 - stopped again.
      if (P3A1 == P2A1 & P3A2 == P2A2)
      {
        executeState=0;
      }  
    }
  }
}

