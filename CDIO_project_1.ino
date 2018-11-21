#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "Sensores.h"




void setup() {

  Serial.begin(9600);
  Serial.println("Inicializando...");
  ads.begin();
  configuracion();

  Serial.println("Ajustando la ganancia...");
  ads.setGain(GAIN_ONE);

}

void loop() {

  //humedad();
  //salinidad();
  temperatura();
  midePresion(T);
  delay(3000);

} //()
