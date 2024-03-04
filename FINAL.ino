#include <PS2X_lib.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

PS2X ps2x;
int error = 0;
byte type = 0;
byte vibrate = 0;

//defines pins
const int stepPin = 6;  //PUL -Pulse
const int dirPin = 7; //DIR -Direction
const int enPin = 8;  //ENA -Enable

// Servo Motor pins

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 0 // this is the 'minimum' pulse length count (out of 40%)
#define SERVOMAX 600 // Maximum

uint8_t servonum;

// Setup variables
int16_t pulselen3 = 600;
int16_t pulselen2 = 100;
//int16_t pulselen1 = 0;
int16_t pulselen0 = 450;

void setup(){
  // Stepper Motor Setup Section
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);

  // Joystick Setup
  Serial.begin(57600);
  error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
  if (error == 0) {
    Serial.println("Found Controller, configured successful");
  }
  else {
    Serial.println("No controller found");
  }

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
  }
  
  // Servo Motor Setup
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);
  
   //Setup default
  myServo.setPWM(1,0,1500);
  myServo.setPWM(2,0,100);  
  myServo.setPWM(0,0,450);
  myServo.setPWM(3,0,600);
  delay(1000);

}

void loop(){

  // Button Input
   ps2x.read_gamepad(false, vibrate);

    // PAD UP
    
    if (ps2x.Button(PSB_PAD_UP)) {
      Serial.println("PAD UP");
      Serial.println(pulselen0);
      if (pulselen0 < SERVOMAX) {
      pulselen0 += 2;
      myServo.setPWM(0, 0, pulselen0);
      
      }
    delay(10);
    }
      
    // PAD DOWN
    
    if (ps2x.Button(PSB_PAD_DOWN)) {
      Serial.println("PAD DOWN");
      Serial.println(pulselen0);
    if (pulselen0 > 300) {
      pulselen0 -= 2;
      myServo.setPWM(0, 0, pulselen0);
      
    }
    delay(10);
  }

 // PAD RIGHT
    
    if (ps2x.Button(PSB_PAD_LEFT)) {
      Serial.println("PAD LEFT");
      myServo.writeMicroseconds(1, 1400);
      delay(10);
    }
    // PAD LEFT
    else if (ps2x.Button(PSB_PAD_RIGHT)) {
      Serial.println("PAD RIGHT");
      myServo.writeMicroseconds(1, 1800);
      delay(10);
    }
    else {
      myServo.writeMicroseconds(1, 1500);
      delay(10);
    }

    // Circle
    
    if (ps2x.Button(PSB_RED)) {
      Serial.println("Circle just pressed");
      Serial.print("Pulse 3 : ");
      Serial.println(pulselen3);
      Serial.print("Pulse 2 : ");
      Serial.println(pulselen2);
      if (pulselen3 > 100 && pulselen2 < 600) {
        pulselen3 -= 3;
        pulselen2 += 3;
        myServo.setPWM(2, 0, pulselen2);
        myServo.setPWM(3, 0, pulselen3);
      }
      delay(10);
    }

    // Square
    
    if (ps2x.Button(PSB_PINK)) {
      Serial.println("Square just pressed");
      Serial.print("Pulse 3 : ");
      Serial.println(pulselen3);
      Serial.print("Pulse 2 : ");
      Serial.println(pulselen2);
      if (pulselen2 > 100 && pulselen3 < 600) {
        pulselen3 += 3;
        pulselen2 -= 3;
        myServo.setPWM(2, 0, pulselen2);
        myServo.setPWM(3, 0, pulselen3);
      }
      delay(10);
    }

    // L1
    
    if (ps2x.Button(PSB_L1))
    {
      Serial.println("L1 Pressed");

        //Left rotation
        digitalWrite(dirPin,HIGH);
        // Makes 200 pulses for making one full cycle rotation
        for (int x = 0; x < 150; x++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(200);  // Reduce the delay here
        digitalWrite(stepPin, LOW);
        delayMicroseconds(200);  // Reduce the delay here
      }

    }  

    // R1
    
    if (ps2x.Button(PSB_R1)) {

      // Right rotation
      Serial.println("R1 Pressed");
      
      //Enables the motor direction to move right 
      digitalWrite(dirPin,LOW);
      for (int x = 0; x < 150; x++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(200);  // Reduce the delay here
        digitalWrite(stepPin, LOW);
        delayMicroseconds(200);  // Reduce the delay here
      }

    }
    
    delay(10);
}
