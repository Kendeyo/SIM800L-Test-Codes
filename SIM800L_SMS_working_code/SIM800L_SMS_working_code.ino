/*
 * SIM800L_SMS_working_code.ino
 *
 * SIM800L SMS Test code
 *
 * Created on: March 10, 2025
 * Author: Kennedy Odeyo
 * (c) 2025, Kennedy Odeyo
 * License: MIT
 *
 */

#include <SoftwareSerial.h>

SoftwareSerial mySerial(16,17);

void setup() {

  Serial.begin(9600);


  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+254793036309\"");  // enter your phone number here (prefix country code)
  updateSerial();
  mySerial.print("Hello from sim800L");  // enter your message here
  updateSerial();
  mySerial.write(26);
  Serial.println("done");
}

void loop() {

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+254793036309\"");  // enter your phone number here (prefix country code)
  updateSerial();
  mySerial.print("Hello from Superb Tech");  // enter your message here
  updateSerial();
  mySerial.write(26);
  Serial.println("done");
  delay(3000);
   
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());  //Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());  //Forward what Software Serial received to Serial Port
  }
}
