#include <SFE_BMP180.h> // Añade la libreria del sensor BMP180

Adafruit_ADS1115 ads(0x48); // construct an ads1115 at address 0x48
//----------------SALINIDAD----------------
#define salinity_power_pin 5
#define salinity_input_pin 1
const int siSal = 25000;  // Medimos valor sin sal (salinidad 0%)
const int noSal = 17200;  // Medimos valor con mucha sal (salinidad 100%)
//------------------PRESION-----------------------
SFE_BMP180 pressure;
#define ALTITUDE 10.0

//-----------------------------------------

//----------------HUMEDAD----------------
const int AirValue = 20200;  // Medimos valor en seco (humedad0%)
const int WaterValue = 13000;  // Medimos valor en agua (humedad100%)

//---------------------------------------

// ------------------TEMPERATURA------------
const double frio = 9307 ;
const double caliente = 16490 ;
double T;

//==============================CONFIGURACION==============================
void configuracion() {
  pinMode(salinity_power_pin, OUTPUT); //define el pin 5 como salida de 3.3V
  if (pressure.begin()) //Iniciamos el sensor de presion
  Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
    
  }
}
//=========================================================================

//==============================HUMEDAD==============================
void humedad(){

  int adc0 = ads.readADC_SingleEnded(0); //LEER valor del sensor
  int humitat = map(adc0, AirValue, WaterValue, 0, 100); //Escalar los valores para verlo en porcentaje

  //LIMITAR los valores entre 0 y 100
  if(humitat<0){
    humitat=0;
  } // if()
  if(humitat>100){
    humitat=100;
  } // if()
  //---------------------------------

  /* //Mostrar valores leidos por sensor
  Serial.print("AIN0: ");
  Serial.println(adc0);
  */

  //Mostrar valores de humedad en %
  Serial.print("Humedad (%): ");
  Serial.print(humitat);
  Serial.println("%");
  //-------------------------------

} // ()
//===================================================================

//==============================SALINIDAD==============================
int salinity_reading( int power_pin, int input_pin ) {

  int reading;
  digitalWrite( power_pin, HIGH );    //  Turn on the sensor
  reading = ads.readADC_SingleEnded(input_pin);  //  Read voltage
  delay(100);                         //  Wait to settle
  digitalWrite( power_pin, LOW );     //  Turn off the sensor

  return reading;

} // ()

// --------------------------------------------------------------

int salinidad(){

  int adc1 = salinity_reading( salinity_power_pin, salinity_input_pin ); //LEER valor del sensor

  int salinity = map(adc1, noSal, siSal, 0, 100);


  //LIMITAR los valores entre 0 y 100
  if(salinity>100){
    salinity = 100;
  } // if()
  if(salinity<0){
    salinity = 0;
  } // if()
  //---------------------------------

   //Mostrar valores leidos por sensor
  //Serial.print("AIN1: ");
  //Serial.println(adc1);


  //Mostrar valores de salinidad en %
  Serial.print("Salinidad (%): ");
  Serial.print(salinity);
  Serial.println("%");
  //---------------------------------

} // ()
//=====================================================================

//==============================TEMPERATURA==============================

// --------------------------------------------------------------
void temperatura(){



  double adc2 = ads.readADC_SingleEnded(2); //LEER valor del sensor

  
T=(adc2-frio)*(38.6-12.2)/(caliente-frio)+12.2;//map to wapo del doble

   //Mostrar valores leidos por sensor
 // Serial.print("AIN2: ");
  //Serial.println(adc2);


  //Mostrar valores de salinidad en %
  Serial.print("Temperatura (º): ");
  Serial.print(T);
  Serial.println("º");
  //---------------------------------

} // ()
void midePresion(double &T){
  char status;
  double P,p0,a;

  status = pressure.startPressure(3);
  if (status != 0)
  {
    // Espera a que la medición termine:
    delay(status);


    // Ten en cuenta que la medicion se guarda en la variable P.
    // Ten en cuenta que la función necesita la temperatura en la variable (T).

    status = pressure.getPressure(P,T);
    if (status != 0)
    {
     
      // Imprime la medida:
      Serial.print("Presión absoluta: ");
      Serial.print(P,2);
      Serial.print(" mb, ");
      Serial.println();

      // The pressure sensor returns abolute pressure, which varies with altitude.
      // El sensor retorna la presión absoluta, la cual varia con la altitud
      // Para eliminar los efectos de la altitud, usa la funcion sealevel() junto con tu altitud actual
      // Este numero es comunmente usado en los partes meteorológicos.

      p0 = pressure.sealevel(P,ALTITUDE); // Estamos a 10 metros sobre el nivel del mar
      Serial.print("Presion relativa (nivel del mar): ");
      Serial.print(p0,2);
      Serial.print(" mb, ");
      Serial.println();


    } // if()
    else Serial.println("Error iniciando las medidas de presión \n");
  }// if()
  else Serial.println("Error iniciando las medidas de presión \n");
}// midePresion()
//=====================================================================
