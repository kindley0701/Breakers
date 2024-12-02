#include <ps5Controller.h>
#include "../secrets.h"

void setup() {
  Serial.begin(115200);
  ps5.begin(MAC_ADDRESS); //replace with MAC address of your controller
  Serial.println("Ready.");
}

void loop() {
  if (ps5.isConnected()) {
    Serial.println("Connected!");
  }
  delay(3000);
}