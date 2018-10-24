/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
Servo myservo1;
Servo myservo2;
Servo myservo3;


int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo1.attach(10);
  myservo2.attach(11);
  myservo3.attach(12);
}

void loop() {
  myservo.write(90);
  delay(1000);
  myservo1.write(180);
  myservo2.write(180);
  delay(1000);
  myservo2.write(160);
  delay(1000);
  myservo3.write(0);
  delay(1000);
  myservo1.write(90);
  myservo2.write(90);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo2.write(60);
  delay(1000);
  myservo3.write(180);
  delay(1000);
  myservo2.write(90);
  delay(1000);

  
}
