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

//--------------------LUZ-------------------
const double noLuz = 50 ;
const double siLuz = 29530 ;
int L;

//------------------GPS---------------------
#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h>

#define RX_PIN  12 // GPS RXI

#define TX_PIN  13 // GPS TX0
#define INIT_PIN 15 // Pin para  Inicializar el GPS


#define GPS_BAUD  9600  //  velocidad de comunicación serie

TinyGPSPlus gps; // Definimos el objeto gps

SoftwareSerial ss(RX_PIN,TX_PIN); // Creamos una UART software para comunicación con el GPS

// ---------------------------Funciones auxiliares GPS ---------------------

// Función espera 1s para leer del GPS
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while(ss.available())
    {
      gps.encode(ss.read());  // leemos del gps
    }
  } while(millis() - start < ms);
}
// Función para encender/apagar mediante un pulso
void switch_on_off()
{
   // Power on pulse
  digitalWrite(INIT_PIN,LOW);
  delay(200);
  digitalWrite(INIT_PIN,HIGH);
  delay(200);
  digitalWrite(INIT_PIN,LOW);
 }

//-----------------------Acelerómetro--------------------------
#include "Acel.h"
// Constantes y variables globales

const byte interruptPin = 4;
// declaramos como volatile la variable que cuenta el nÃºmero de interrupciones
// si no se hace asÃ­ el compilador podrÃ­a pensar que esta variable no se utiliza nunca y la borrarÃ­a
volatile byte interruptCounter = 0; // volatile porque puede cambiar en cualquier momento por motivos ajenos al SW
int numberOfInterrupts = 0;

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
//===============================Config GPS=================================
void configGPS () {
Serial.begin(9600); // Inicializar la comunicación con el monitor serie
 ss.begin(GPS_BAUD); // Inicializar la comunicación con el GPS

 pinMode(INIT_PIN,OUTPUT);
 switch_on_off(); // Pulso para encender el GPS

 Serial.println("Fecha      Hora       Latitud   Longitud   Alt    Rumbo   Velocidad");
 Serial.println("(MM/DD/YY) (HH/MM/SS)     (deg)       (deg)  (ft)                   (mph)");
 Serial.println("-------------------------------------------------------------------------");
}// ()
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
void luz(){



  double adc3 = ads.readADC_SingleEnded(3); //LEER valor del sensor


//L=(adc3-noLuz)*(100-0)/(siLuz-noLuz);//map to wapo del doble
L=map(adc3,noLuz,siLuz,0,100);

   //Mostrar valores leidos por sensor
 //Serial.print("AIN3: ");
  //Serial.println(adc3);


  //Mostrar valores de salinidad en %
  Serial.print("Luz (%): ");
  Serial.print(L);
  Serial.println("%");
}

// =============================GPS====================================
void gepeese() {
char gpsDate[10];
  char gpsTime[10];

  if(gps.location.isValid()){ // Si el GPS está recibiendo los mensajes NMEA

    sprintf(gpsDate,"%d/%d/%d", gps.date.month(),gps.date.day(),gps.date.year()); // Construimos string de datos fecha
    sprintf(gpsTime,"%d/%d/0%d", gps.time.hour(),gps.time.minute(),gps.time.second());  // Construimos string de datos hora

    Serial.print(gpsDate);
    Serial.print('\t');
    Serial.print(gpsTime);
    Serial.print('\t');
    Serial.print(gps.location.lat(),6);
    Serial.print('\t');
    Serial.print(gps.location.lng(),6);
    Serial.print('\t');
    Serial.print(gps.altitude.feet());
    Serial.print('\t');
    Serial.print(gps.course.deg(),2);
    Serial.print('\t');
    Serial.println(gps.speed.mph(),2);
  }
  else  // Si no recibe los mensajes
  {

    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
  }
  smartDelay(1000);
}// ()

void acelerea() {

  if(interruptCounter>0){

      interruptCounter--;
      numberOfInterrupts++;

      Serial.print("InterrupciÃ³n detectada. Total: ");
      Serial.println(numberOfInterrupts);
  }

}
