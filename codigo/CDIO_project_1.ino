#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "Sensores.h"




void setup() {

  Serial.begin(9600);
  Serial.println("Inicializando...");
  ads.begin();
  //configuracion();
  configGPS();

  acelconfig();
  medidas();

  //configuramos el modo de PIN coomo PULLUP para que por defecto estÃ© a nivel alto,
  // la interrupciÃ³n se producirÃ¡ a nivel bajo
  Serial.println("Configurando el dispositivo ");
  pinMode(interruptPin, INPUT_PULLUP);
  // Asociamos la interrupciÃ³n con el pin, con la funciÃ³n y con el umbral
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

  Serial.println("Ajustando la ganancia...");
  ads.setGain(GAIN_ONE);

}

void loop() {

  acelerea();
  //humedad();
  //salinidad();
  //temperatura();
  //midePresion(T);
  luz();
  gepeese();

  delay(3000);

} //()

void handleInterrupt() {
  interruptCounter++;
}
