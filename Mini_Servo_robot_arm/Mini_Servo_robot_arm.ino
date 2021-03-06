/* Mini_Servo_robot_arm
 * by: @hectorsvill
 *
 * Components:
 *  - 4 x SG90 Servo
 *  - 5v-9v power suply module
 *  - arduino nano
 */

#include <Servo.h>

#define DELAYTIME 50
#define SERVOCOUNT 4

// servo pin numbers
#define S0PIN 7
#define S1PIN 8
#define S2PIN 9
#define S3PIN 10

// servo min max angles
#define S0MIN 0
#define S0MAX 180

#define S1MIN 60
#define S1MAX 180

#define S2MIN 0
#define S2MAX 180

#define S3MIN 0
#define S3MAX 180

#define RESETANGLE 90

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

Servo servoList[] = {servo0, servo1, servo2, servo3};
int servoMinList[] = {S0MIN, S1MIN, S2MIN, S3MIN};
int servoMaxList[] = {S0MAX, S1MAX, S2MAX, S3MAX};

void setup() {
  Serial.begin(9600);
  servo0.attach(S0PIN);
  servo1.attach(S1PIN);
  servo2.attach(S2PIN);
  servo3.attach(S3PIN);
  delay(1000);
}

int state = 0;

void loop() {
  if (state == 0) {
    commandLineState();
  }

  delay(DELAYTIME);
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
      
      if ((str[0] == 's') && isDigit(str[1]) && (String(str[1]).toInt() < SERVOCOUNT)) {
        int servoId = String(str[1]).toInt();
        Servo servo = servoList[servoId];
        
        int intStart = str[3] == '+' || str[3] == '-' ? 4 : 3;
        String valueStr = "";

        for (int i = intStart; i < str.length(); i++) {
          valueStr += str[i];
        }

        int value = valueStr.toInt();

        if (str[3] == '+' || str[3] == '-') {
          Serial.println(str[3]);
          value = move(servoId, String(str[3]), value);
        } else {
          value = move(servoId, value);
        }

        String message = "servo " + String(servoId) + " to angle " + String(servo.read());
        Serial.println(message);

      }else if (str[0] == 'p' && str[1] == 'a') {
        readServoAngles();
      } else if (str[0] == 'r' && str[1] == 's') {
        resetPosition();
      }
    }
}

int move(int servoId, String op, int value) {
  Servo servo = servoList[servoId];
  int angle = servo.read();

  if (op[0] == '+') {
    int newAngle = angle + value;
    value = angleIsInRange(servoId, newAngle) == true ? newAngle : angle;
  } else if(op[0] == '-') {
    int newAngle = angle - value;
    value = angleIsInRange(servoId, newAngle) == true ? newAngle : angle;
  }

  servo.write(value);
  return value;
}

int move(int servoId, int value) {
  Servo servo = servoList[servoId];
  int currentAngle = servo.read();
  int newValue = angleIsInRange(servoId, value) == true ? value : currentAngle;
  servo.write(newValue);
  return newValue;
}


bool angleIsInRange(int servoId, int angle) {
  return (angle >= servoMinList[servoId] && angle <= servoMaxList[servoId]) ? true : false;
}

int resetPosition() {
  for (int i = 0; i < SERVOCOUNT; i++) { servoList[i].write(RESETANGLE); }
  Serial.println("Reseting Servos to angle " + String(RESETANGLE));
}

int *readServoAngles() {
  int arr[4] = {};

  for (int i = 0; i < SERVOCOUNT; i++) {
    arr[i] = servoList[i].read();
  }

  String str = "servo 0: " + String(arr[0]) + "\nservo 1: " + String(arr[1]) + "\nservo 2: " + String(arr[2]) + "\nservo 3: " + String(arr[3]);
  Serial.println(str);
  return arr;
}

 void swingServo(int servoId) {
  Servo servo = servoList[servoId];
  int maxPosition = servoMaxList[servoId];
  
   for (int i = 0; i <  maxPosition; i+=5) {
     servo.write(i);
     delay(DELAYTIME);
   }

   for (int i = maxPosition; i >= 0; i-=5) {
     servo.write(i);
     delay(DELAYTIME);
   }
 }
