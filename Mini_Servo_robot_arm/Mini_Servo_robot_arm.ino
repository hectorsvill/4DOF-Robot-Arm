/* Mini_Servo_robot_arm
 * by: @hectorsvill
 * 
 * Components:
 *  - 4 x SG90 Servo
 *  - 5v-9v power suply module
 *  - arduino nano
 */

#include <Servo.h>

#define delayTime 50
#define servoCount 4

// servo pin numbers
#define servo0Pin 7
#define servo1Pin 8
#define servo2Pin 9
#define servo3Pin 10

// servo min max anggles 
#define servo0Min 0
#define servo0Max 175

#define servo1Min 60
#define servo1Max 100

#define servo2Min 60
#define servo2Max 180

#define servo3Min 0
#define servo3Max 180

#define RESETANGLE 90

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

Servo servoList[] = {servo0, servo1, servo2, servo3};
int servoMinList[] = {servo0Min, servo1Min, servo2Min, servo3Min};
int servoMaxList[] = {servo0Max, servo1Max, servo2Max, servo3Max};

void setup() {
  Serial.begin(9600);
  servo0.attach(servo0Pin);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  delay(1000);
}

int state = 0;

void loop() {
  if (state == 0) {
    commandLineState();
  }
}

// command line state
// ex: s0 120 - move servo number 0 to angle 120
// ex: s0 +10 - move servo number 0 from current positon + 10
// ex: s0 -10 - move servo number 0 from current positon - 10
// ex: pa - print angle   
// ra: reset servo angle 

void commandLineState() {
      if (Serial.available()) {
      
      String str = Serial.readString();

      int servoNumber = String(str[1]).toInt();
      if ((str[0] == 's') && (servoNumber < servoCount)) {
        int intStart = str[3] == '+' || str[3] == '-' ? 4 : 3;
        
        String valueStr = "";
        for (int i = intStart; i < str.length(); i++) {
          valueStr += str[i];
        }
        
        int value = valueStr.toInt();
        int angle = servoList[servoNumber].read();
        
        if (str[3] == '+') {
          value = angle + value;
          servoList[servoNumber].write(value);
        } else if(str[3] == '-') {
          value = angle - value;
          servoList[servoNumber].write(value);
        } else {
          servoList[servoNumber].write(value);  
        }
        
        String message = "servo " + String(servoNumber) + " to angle " + String(value);
        Serial.println(message);
      }else if (str[0] == 'p' && str[1] == 'a') {
        readServoAngles();
      } else if (str[0] == 'r' && str[1] == 's') {
        resetPosition();
      }
    }
}
int resetPosition() {
  for (int i = 0; i < servoCount; i++) { servoList[i].write(RESETANGLE); }
  Serial.println("Reseting Servos to angle " + String(RESETANGLE));
}
int *readServoAngles() {
  int arr[4] = {};
  
  for (int i = 0; i < servoCount; i++) {
    arr[i] = servoList[i].read();
  }
  
  String str = "servo 0: " + String(arr[0]) + "\nservo 1: " + String(arr[1]) + "\nservo 2: " + String(arr[2]) + "\nservo 3: " + String(arr[3]);
  Serial.println(str);
  return arr;
}

void swingServo(Servo servo, int maxPosition) {
  for (int i = 0; i <  maxPosition; i+=5) {
    servo.write(i);
//    Serial.println(i);
    delay(delayTime);
  }

  for (int i = maxPosition; i >= 0; i-=5) {
    servo.write(i);
//    Serial.println(i);
    delay(delayTime);
  }  
}

void randomPosition() {
  int num = random(0, 170);
  Serial.println(num);
  servo1.write(num);  
  delay(250);
}

void goToZero(Servo servo) { 
  while (servo.read() > 0) {
    int pinPosition = servo.read();
    pinPosition -= 5;
    servo.write(pinPosition);
    
    String str = "goToZero at position: " + String(pinPosition);
//    Serial.println(str);

    if (pinPosition < 0) {
      pinPosition = 0;
    }
    
    delay(delayTime);
  }
}
