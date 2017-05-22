#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper Rechts(stepsPerRevolution, 8, 10, 9, 11);
Stepper  Links(stepsPerRevolution, 2, 4, 3, 5);
const int trigPinVoor = 13;
const int echoPinVoor = 12;
const int trigPinOnder = 6;
const int echoPinOnder = 7;
const int trigPinZijkant = A5;
const int echoPinZijkant = A4;
long distanceVoor;
long distanceOnder;
long distanceZijkant;

void setup() {
  Rechts.setSpeed(110);
  Links.setSpeed(110);
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(trigPinVoor, OUTPUT);
  pinMode(echoPinVoor, INPUT);
  pinMode(trigPinOnder, OUTPUT);
  pinMode(echoPinOnder, INPUT);
  pinMode(trigPinZijkant, OUTPUT);
  pinMode(echoPinZijkant, INPUT);
  pinMode(A2, OUTPUT); //inputvoor
  pinMode(A1, OUTPUT); //inputlinks
  pinMode(A0, OUTPUT); //inputonder
  digitalWrite(A2, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A0, LOW);
}

void loop() {
  digitalWrite(trigPinVoor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinVoor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinVoor, LOW);
  distanceVoor = microsecondsToCentimeters(pulseIn(echoPinVoor, HIGH, 1500));
  
  digitalWrite(trigPinOnder, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinOnder, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinOnder, LOW);
  // step one revolution  in one direction:
  distanceOnder = microsecondsToCentimeters(pulseIn(echoPinOnder, HIGH, 1500));
  
  digitalWrite(trigPinZijkant, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinZijkant, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinZijkant, LOW);
  // step one revolution  in one direction:
  distanceZijkant = microsecondsToCentimeters(pulseIn(echoPinZijkant, HIGH, 1500));
 //Serial.println(distanceZijkant  );
    
  if(distanceOnder > 10 || distanceOnder == 0){
    digitalWrite(A0, HIGH);
    for(int i=0; i<240  ; i++) {
      Rechts.step(-4);
      Links.step(4);
    }  
    digitalWrite(A0, LOW);
  }else if(stilstand(distanceVoor, distanceZijkant, distanceOnder)) {
    Serial.println("vastgelopen");
  }else if(distanceZijkant < 6 && distanceZijkant != 0){
    digitalWrite(A1, HIGH);
     for(int i=0; i<10; i++) {
      Rechts.step(-2);
      Links.step(2);
    }
    digitalWrite(A1, LOW);
  }else if(distanceVoor < 5 && distanceVoor != 0 && (distanceZijkant >30 || distanceZijkant == 0)) {
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    for(int i=0; i<240; i++) {
      Rechts.step(4);
      Links.step(-4);
    }
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
  }else if(distanceVoor < 7 && distanceVoor != 0){
    digitalWrite(A2, HIGH);
    for(int i=0; i<240  ; i++) {
      Rechts.step(-4);
      Links.step(4);
    }
    digitalWrite(A2, LOW);
  }else if(distanceZijkant > 30 || distanceZijkant == 0) {
    digitalWrite(A1, HIGH);
    for(int i=0; i<10; i++) {
      Rechts.step(6);
      Links.step(2);
    }
    digitalWrite(A1, LOW);  
  }else{
     for(int i=0; i<8; i++) {
       Rechts.step(6);
       Links.step(5);
      }
    }
  
 /*
  } if(digitalRead(A3) == LOW) //Muur aan links
  {
    for(int i=0; i<70; i++) {
      Rechts.step(-2);
      Links.step(2);
    }
  }
  if(digitalRead(A4) == LOW) { //Muur linksvoor
    for(int i=0; i<150; i++) {
      Links.step(-4);
      Rechts.step(-10);
    }
    for(int i=0; i<50; i++) {
      Links.step(-2);
      Rechts.step(-2);
    }
  }
  if(digitalRead(6) == LOW) //Muur aan rechts
  {
    for(int i=0; i<100; i++) {
      Links.step(-2);
    }
  }
  if(digitalRead(7) == LOW) { //Muur rechtsvoor
    for(int i=0; i<150; i++) {
      Rechts.step(-4);
      Links.step(-10);
    }
  }*/
}
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int vo;
int zi;
int on;
long timePassed;
bool stilstand(int v, int z, int o) {
  if(v==vo && z == zi && o==on) {
    if(millis() - timePassed > 7500) {
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      for(int i=0; i<60; i++) {
        Rechts.step(-4);
        Links.step(-4);
      }
      digitalWrite(A1, LOW);
      for(int i=0; i<110; i++) {
        Rechts.step(-4);
        Links.step(4);
      }
      digitalWrite(A0, LOW);
      digitalWrite(A2, LOW);
      timePassed = -1;
      return true;
    }
  } else {
    vo = v;
    zi = z;
    on = o;
    timePassed = millis();
    return false;
  }
}

