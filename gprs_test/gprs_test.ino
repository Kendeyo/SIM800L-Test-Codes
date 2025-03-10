/*
 * gprs_test.ino
 *
 * SIM800L GPRS Test code
 *
 * Created on: March 10, 2025
 * Authors: Peter Kagwe,Kennedy Odeyo
 * (c) 2025
 *
 *Copyright (c) [Year], [Copyright Holder]
 *All rights reserved.

 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <SoftwareSerial.h>

SoftwareSerial sim800(16, 17);

const char* apn = "safaricom"; // Replace with your mobile network APN
const char* apiKey = "5L3AE9G9SVHPDIX9"; // Replace with your ThingSpeak API key

int battery = 87;
float temperature = 23.78;
float humidity = 60.45;

void setup() {
  Serial.begin(9600);
  sim800.begin(9600);

  Serial.println("Initializing SIM800...");
  sendATCommand("AT", 1000);
  sendATCommand("AT+CMGF=1", 1000); // Set SMS mode to TEXT
  sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", 1000); // Set connection type
  sendATCommand("AT+SAPBR=3,1,\"APN\",\"" + String(apn) + "\"", 1000); // Set APN
  sendATCommand("AT+SAPBR=1,1", 3000); // Open GPRS
  sendATCommand("AT+SAPBR=2,1", 1000); // Check GPRS status
}

void loop() {
  sendDataToThingSpeak();
  delay(5000); // Send data every 5 seconds
}

void sendDataToThingSpeak() {
  char url[200];
  sprintf(url, "GET /update?api_key=%s&field1=%d&field2=%.2f&field3=%.2f HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n", 
          apiKey, battery, temperature, humidity);

  Serial.println("Sending data to ThingSpeak...");
  
  sendATCommand("AT+HTTPINIT", 1000); // Initialize HTTP service
  sendATCommand("AT+HTTPPARA=\"CID\",1", 1000); // Set HTTP bearer profile
  sendATCommand("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update?api_key=" + String(apiKey) + "&field1=" + String(battery) + "&field2=" + String(temperature) + "&field3=" + String(humidity) + "\"", 1000);
  sendATCommand("AT+HTTPACTION=0", 5000); // Start GET request
  sendATCommand("AT+HTTPREAD", 3000); // Read response
  sendATCommand("AT+HTTPTERM", 1000); // Terminate HTTP
}

void sendATCommand(String command, int waitTime) {
  sim800.println(command);
  delay(waitTime);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}
