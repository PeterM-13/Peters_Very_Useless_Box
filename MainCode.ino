#include <Servo.h>

Servo servoArm; // Pin 9
Servo servoLid; // Pin 5
Servo servoFlag; // Pin 8

const int switchPin = 6;

// Variables for servo position limits.
const int armMin = 30;
const int armMax = 172;
const int lidMin = 19;
const int lidMax = 100;
const int flagMin = 45;
const int flagMax = 160;

bool ON = false;

int totalSwitches = 0;

long lastSwitch = 0;
int flagCount = 0;

// Variables that change the box's behavior.
const int flagLower = 120;
int holdTime = random(1750, 4000);
int pushNumber = random(2, 5);
int slamNumber = random(1, 5);
const int hitAngle = 111;
const int returnAngle = 75;
int hitNumber = random(3, 9);
int giveUp = random(12, 18);

void servoGoTo(Servo &servo, int targetAngle, int speed, int waitTime, bool checkSwitchDown=true, bool checkSwitchUp=true);

void setup() {
  //Serial.begin(9600);

  pinMode(switchPin, INPUT_PULLUP);

  // Servo Setup
  servoArm.attach(9); 
  servoLid.attach(5); 
  servoFlag.attach(8); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Start Positions
  servoArm.write(armMin);
  servoLid.write(lidMin);
  servoFlag.write(flagMin);

  delay(2000);
}


// Main Loop
void loop() {
  ON = !digitalRead(switchPin);

  if(ON){
    digitalWrite(LED_BUILTIN, HIGH);

    int angerLevel = map(constrain(totalSwitches, 0, 10),0,10,0,5);

    int randMode = random(1 + angerLevel, 6 + angerLevel);

    if(totalSwitches == giveUp){
      servoGoTo(servoLid, 160, 75, 750); // Lid Open
      servoGoTo(servoFlag, flagMax, 85, 100); // Raise flag
      servoGoTo(servoFlag, flagLower, 80, 500); // Lower falg
      servoGoTo(servoFlag, flagMax, 80, 500); // Raise flag
      servoGoTo(servoFlag, flagLower, 80, 500); // Lower falg
      servoGoTo(servoFlag, flagMax, 80, 500); // Raise flag
      servoGoTo(servoFlag, flagLower, 75, 500); // Lower falg
      servoGoTo(servoFlag, flagMax, 70, 500); // Raise flag
      servoGoTo(servoFlag, flagMin, 50, 400); // Hide flag
      servoGoTo(servoLid, lidMin, 75, 12000); // Lid Down
      if(!digitalRead(switchPin)){ 
        // Fast
        servoGoTo(servoLid, lidMax, 100, 200); // Lid Open
        servoGoTo(servoArm, armMax, 100, 400); // Switch Off
        servoGoTo(servoArm, armMin, 100, 200); // Arm Down
        servoGoTo(servoLid, lidMin, 100, 200); // Lid Down
      }
      flagCount ++;
      totalSwitches -= 5;
      giveUp += (flagCount*2);
      randMode = 0;
    }

    switch(randMode){
      case 1: // Normal 1
        servoGoTo(servoLid, lidMax, 85, 500); // Lid Open
        servoGoTo(servoArm, armMax, 90, 300); // Switch Off
        servoGoTo(servoArm, armMin, 85, 300); // Arm Down
        servoGoTo(servoLid, lidMin, 85, 0); // Lid Down
        break;
      case 2: // Normal 2
        servoGoTo(servoLid, lidMax, 95, 100); // Lid Open
        servoGoTo(servoArm, armMax, 85, 500); // Switch Off
        servoGoTo(servoArm, armMin, 85, 100); // Arm Down
        servoGoTo(servoLid, lidMin, 95, 0); // Lid Down
        break;
      case 3: // Normal, then check
        servoGoTo(servoLid, lidMax, 78, 500); // Lid Open
        servoGoTo(servoArm, armMax, 90, 300); // Switch Off
        servoGoTo(servoArm, armMin, 78, 300); // Arm Down
        servoGoTo(servoLid, lidMin, 88, 1000); // Lid Down
        if(digitalRead(switchPin)){
          servoGoTo(servoLid, lidMax, 100, 1500); // Lid Open
          servoGoTo(servoLid, lidMin, 88, 0); // Lid Down
        }
        break;
      case 4: // Fake it
        servoGoTo(servoLid, lidMax, 100, 250); // Lid Open
        servoGoTo(servoArm, armMax-12, 100, 400); // Switch Off
        servoGoTo(servoArm, armMin, 100, 200, false); // Arm Down
        servoGoTo(servoLid, lidMin, 100, 1750); // Lid Down
        servoGoTo(servoLid, lidMax, 75, 500); // Lid Open
        servoGoTo(servoArm, armMax, 90, 300); // Switch Off
        servoGoTo(servoArm, armMin, 75, 300); // Arm Down
        servoGoTo(servoLid, lidMin, 75, 0); // Lid Down
        break;
      case 5: // Slow Open
        servoGoTo(servoLid, lidMax, 40, 1200); // Lid Open
        servoGoTo(servoArm, armMax, 100, 350); // Switch Off
        servoGoTo(servoArm, armMin, 100, 200); // Arm Down
        servoGoTo(servoLid, lidMin, 100, 0); // Lid Down
        break;
      case 6: // Fast
        servoGoTo(servoLid, lidMax, 100, 200); // Lid Open
        servoGoTo(servoArm, armMax, 100, 400); // Switch Off
        servoGoTo(servoArm, armMin, 100, 200); // Arm Down
        servoGoTo(servoLid, lidMin, 100, 0); // Lid Down
        break;
      case 7: // Hold switch off
        holdTime = random(1750, 4000);
        servoGoTo(servoLid, lidMax, 100, 200); // Lid Open
        servoGoTo(servoArm, armMax, 100, holdTime); // Switch Off
        servoGoTo(servoArm, armMin, 60, 200); // Arm Down
        servoGoTo(servoLid, lidMin, 78, 100); // Lid Down
        break;
      case 8: // Repreated Pushes
        servoGoTo(servoLid, lidMax, 95, 300); // Lid Open
        pushNumber = random(2, 5);
        for(int i=0; i<pushNumber; i++){
          servoGoTo(servoArm, armMax, 95, 500, true, false); // Switch Off
          servoGoTo(servoArm, armMax-35, 95, 100);
        }
        servoGoTo(servoArm, armMax, 95, 400); // Switch Off
        servoGoTo(servoArm, armMin, 90, 200); // Arm Down
        servoGoTo(servoLid, lidMin, 90, 0); // Lid Down
        break;
      case 9: // Slam lid
        slamNumber = random(2, 5);
        for(int i=0; i<slamNumber; i++){
          servoGoTo(servoLid, lidMax, 100, 300); // Lid Open
          servoGoTo(servoLid, lidMin, 100, 500); // Lid Down
        }
        delay(1000);
        servoGoTo(servoLid, lidMax, 80, 400); // Lid Open
        servoGoTo(servoArm, armMax, 100, 300); // Switch Off
        servoGoTo(servoArm, armMin, 100, 200); // Arm Down
        servoGoTo(servoLid, lidMin, 100, 100); // Lid Down
        break;
      case 10: // Bang lid
        hitNumber = random(3, 9);
        servoGoTo(servoArm, returnAngle, 100, 1500); // Lower aown
        for(int i=0; i<hitNumber; i++){
          servoGoTo(servoArm, hitAngle, 100, 150); // Hit lid
          servoGoTo(servoArm, returnAngle, 100, 200); // Lower arm
        }
        delay(1800);
        servoGoTo(servoLid, lidMax, 100, 200); // Lid Open
        servoGoTo(servoArm, armMax, 100, 350); // Switch Off
        servoGoTo(servoArm, armMin, 75, 250); // Arm Down
        servoGoTo(servoLid, lidMin, 75, 100); // Lid Down
        break;
    }
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    totalSwitches ++;
    lastSwitch = millis();
  }
  if(millis() - lastSwitch > 10000){
   if(totalSwitches > 3 && flagCount >= 1){
      totalSwitches --;
    }
    int randVar = random(2, 8);
    if(digitalRead(switchPin) && randVar == 7){
      servoGoTo(servoLid, lidMax-40, 1, 2500); // Lid half open
      servoGoTo(servoLid, lidMin, 100, 0); // Lid Down
    }
    lastSwitch = millis();
  }
}


void servoGoTo(Servo &servo, int targetAngle, int speed, int waitTime, bool checkSwitchDown=true, bool checkSwitchUp = true) {
  int currentAngle = servo.read();

  int mappedSpeed = map(speed, 0, 100, 50, 0);

  int count = 0;
  int stop = random(0, 4);
  int angleBefore = armMin;

  if (currentAngle < targetAngle) {
    for (int i = currentAngle; i <= targetAngle; i++) {
      servo.write(i);
      delay(mappedSpeed);
      // Check if switched off when moving arm up
      if(checkSwitchUp && &servo==&servoArm && targetAngle==armMax && digitalRead(switchPin)){
        // Fast
        break;
        count ++;
      }
    }
  } else {
    for (int i = currentAngle; i >= targetAngle; i--) {
      // check if switched on while moving arm down
      if(count == 0){angleBefore = servo.read();}
      if(checkSwitchDown && &servo==&servoArm && targetAngle==armMin && angleBefore<hitAngle && !digitalRead(switchPin) && count<stop){
        // Fast
        servoGoTo(servoArm, armMax, 100, 400); // Switch Off
        servoGoTo(servoArm, angleBefore, 100, 500, false); // Arm Down
        count ++;
      }
      servo.write(i);
      delay(mappedSpeed);
    }
  }

  delay(waitTime);
}





// * An attemp to move 2 servos 'simultaneously', but didn't work *
//
// void servosGoTo(int angle1, int angle2, int speed1, int speed2, int waitTime) {
//   unsigned long previousMillis1 = 0;
//   unsigned long previousMillis2 = 0;
//   unsigned long interval1 = map(speed1, 0, 100, 50, 5);
//   unsigned long interval2 = map(speed2, 0, 100, 50, 5);

//   int currentAngle1 = servoArm.read();
//   int currentAngle2 = servoLid.read();

//   while (currentAngle1 != angle1 || currentAngle2 != angle2) {
//     if (millis() - previousMillis1 >= interval1 && currentAngle1 != angle1) {
//       previousMillis1 = millis();
//       currentAngle1 += (currentAngle1 < angle1) ? 1 : -1;
//       currentAngle1 = constrain(currentAngle1, armMin, armMax);
//       servoArm.write(currentAngle1);
//     }

//     if (millis() - previousMillis2 >= interval2 && currentAngle2 != angle2) {
//       previousMillis2 = millis();
//       currentAngle2 += (currentAngle2 < angle2) ? 1 : -1;
//       currentAngle2 = constrain(currentAngle2, lidMin, lidMax);
//       servoLid.write(currentAngle2);
//     }
//   }
//     delay(waitTime);
// }