#include "wifi.h"
void setup() {
  #ifdef PRINT_DEBUG_MESSAGES
    Serial.begin(9600);
  #endif
  connectWiFi();
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print("Server_Host: ");
      Serial.println(Server_Host);
      Serial.print("Port: ");
      Serial.println(String( Server_HttpPort ));
      Serial.print("Server_Rest: ");
      Serial.println(Rest_Host);
  #endif
 }

void loop() {
  subirThing();
}
