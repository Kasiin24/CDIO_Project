#include <ESP8266WiFi.h>

// Variables provisionales para intercambiar

double T=10;
double P=13;
double H=15;
double SAL=18;
double LAT=20;
double LONG=22;
double I=25;

#define NUM_FIELDS_TO_SEND 7 //Numero de medidas a enviar al servidor REST (Entre 1 y 8)

// Comentar/Descomentar para ver mensajes de depuracion en monitor serie y/o respuesta del HTTP server
#define PRINT_DEBUG_MESSAGES
#define PRINT_HTTP_RESPONSE

// Comentar/Descomentar para conexion Fuera/Dentro de UPV
//#define WiFi_CONNECTION_UPV

// Selecciona que servidor REST quieres utilizar entre ThingSpeak y Dweet
#define REST_SERVER_THINGSPEAK //Selecciona tu canal para ver los datos en la web (https://thingspeak.com/channels/360979)
//#define REST_SERVER_DWEET //Selecciona tu canal para ver los datos en la web (http://dweet.io/follow/PruebaGTI)

///////////////////////////////////////////////////////
/////////////// WiFi Definitions /////////////////////
//////////////////////////////////////////////////////

#ifdef WiFi_CONNECTION_UPV //Conexion UPV
  const char WiFiSSID[] = "GTI1";
  const char WiFiPSK[] = "1PV.arduino.Toledo";
#else //Conexion fuera de la UPV
  const char WiFiSSID[] = "MiFibra-9410";
  const char WiFiPSK[] = "XFXEwJT9";
#endif


///////////////////////////////////////////////////////
/////////////// SERVER Definitions /////////////////////
//////////////////////////////////////////////////////

#if defined(WiFi_CONNECTION_UPV) //Conexion UPV
  const char Server_Host[] = "maralgo2.upv.es";
  const int Server_HttpPort = 8080;
#elif defined(REST_SERVER_THINGSPEAK) //Conexion fuera de la UPV
  const char Server_Host[] = "api.thingspeak.com";
  const int Server_HttpPort = 80;
#else
  const char Server_Host[] = "dweet.io";
  const int Server_HttpPort = 80;
#endif

WiFiClient client;

///////////////////////////////////////////////////////
/////////////// HTTP REST Connection ////////////////
//////////////////////////////////////////////////////

#ifdef REST_SERVER_THINGSPEAK
  const char Rest_Host[] = "api.thingspeak.com";
  String MyWriteAPIKey="GWJPP62W3WVK43OQ"; // Escribe la clave de tu canal ThingSpeak
#else
  const char Rest_Host[] = "dweet.io";
  String MyWriteAPIKey="cdiocurso2017g07"; // Escribe la clave de tu canal Dweet
#endif


/////////////////////////////////////////////////////
/////////////// Pin Definitions ////////////////
//////////////////////////////////////////////////////

/////////////////////////////////////////////////////
/////////////// WiFi Connection ////////////////
//////////////////////////////////////////////////////

void connectWiFi()
{

  #ifdef PRINT_DEBUG_MESSAGES
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
  #endif

  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED)
  {

    #ifdef PRINT_DEBUG_MESSAGES
       Serial.println(".");
    #endif
    delay(500);
  }
  #ifdef PRINT_DEBUG_MESSAGES
     Serial.println( "WiFi Connected" );
     Serial.println(WiFi.localIP()); // Print the IP address
  #endif
}

/////////////////////////////////////////////////////
/////////////// HTTP POST  ThingSpeak////////////////
//////////////////////////////////////////////////////

void HTTPPost(String fieldData[], int numFields){

// Esta funcion construye el string de datos a enviar a ThingSpeak mediante el metodo HTTP POST
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar numFields al número adecuado de datos que necesitas enviar y activa los campos en tu canal web

    if (client.connect( Server_Host , Server_HttpPort )){

        // Construimos el string de datos. Si tienes multiples campos asegurate de no pasarte de 1440 caracteres

        String PostData= "api_key=" + MyWriteAPIKey ;
        for ( int field = 1; field < (numFields + 1); field++ ){
            PostData += "&field" + String( field ) + "=" + fieldData[ field ];
        }

        // POST data via HTTP
        #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( "Connecting to ThingSpeak for update..." );
        #endif
        client.println( "POST http://" + String(Rest_Host) + "/update HTTP/1.1" );
        client.println( "Host: " + String(Rest_Host) );
        client.println( "Connection: close" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Content-Length: " + String( PostData.length() ) );
        client.println();
        client.println( PostData );
        #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( PostData );
            Serial.println();
            //Para ver la respuesta del servidor
            #ifdef PRINT_HTTP_RESPONSE
              delay(500);
              Serial.println();
              while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
              Serial.println();
              Serial.println();
            #endif
        #endif
    }
}

////////////////////////////////////////////////////
/////////////// HTTP GET  ////////////////
//////////////////////////////////////////////////////

void HTTPGet(String fieldData[], int numFields){

// Esta funcion construye el string de datos a enviar a ThingSpeak o Dweet mediante el metodo HTTP GET
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar "numFields" al número adecuado de datos que necesitas enviar y activa los campos en tu canal web

    if (client.connect( Server_Host , Server_HttpPort )){
           #ifdef REST_SERVER_THINGSPEAK
              String PostData= "GET https://api.thingspeak.com/update?api_key=OTPDLK6UM6U4SP86&field1=0";
              PostData= PostData + MyWriteAPIKey ;
           #else
              String PostData= "GET http://dweet.io/dweet/for/";
              PostData= PostData + MyWriteAPIKey +"?" ;
           #endif

           for ( int field = 1; field < (numFields + 1); field++ ){
              PostData += "&field" + String( field ) + "=" + fieldData[ field ];
           }


           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( "Connecting to Server for update..." );
           #endif
           client.print(PostData);
           client.println(" HTTP/1.1");
           client.println("Host: " + String(Rest_Host));
           client.println("Connection: close");
           client.println();
           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( PostData );
              Serial.println();
              //Para ver la respuesta del servidor
              #ifdef PRINT_HTTP_RESPONSE
                delay(500);
                Serial.println();
                while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
                Serial.println();
                Serial.println();
              #endif
           #endif
    }
}
void subirThing() {

  String data[ NUM_FIELDS_TO_SEND + 1];  // Podemos enviar hasta 8 datos


  data[ 1 ] = String( T ); //Escribimos el dato 1. Recuerda actualizar numFields
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print( "Temperatura = " );
      Serial.println( data[ 1 ] );
  #endif

  data[ 2 ] = String( P ); //Escribimos el dato 2. Recuerda actualizar numFields
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print( "Presion = " );
      Serial.println( data[ 2 ] );
  #endif

  data[ 3 ] = String( H ); //Escribimos el dato 2. Recuerda actualizar numFields
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print( "Humedad = " );
      Serial.println( data[ 3 ] );
  #endif

  data[ 4 ] = String( SAL ); //Escribimos el dato 2. Recuerda actualizar numFields
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print( "Salinidad = " );
      Serial.println( data[ 4 ] );
  #endif

  data[ 5 ] = String( LAT ); //Escribimos el dato 2. Recuerda actualizar numFields
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print( "Latitud = " );
      Serial.println( data[ 5 ] );
  #endif

  data[ 6 ] = String( LONG ); //Escribimos el dato 2. Recuerda actualizar numFields
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print( "Longitud = " );
      Serial.println( data[ 6 ] );
  #endif

  data[ 7 ] = String( I ); //Escribimos el dato 2. Recuerda actualizar numFields
  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print( "Iluminacion = " );
      Serial.println( data[ 7 ] );
  #endif

  //Selecciona si quieres enviar con GET(ThingSpeak o Dweet) o con POST(ThingSpeak)
  HTTPPost( data, NUM_FIELDS_TO_SEND );
  //HTTPGet( data, NUM_FIELDS_TO_SEND );

  //Selecciona si quieres un retardo de 10seg para hacer pruebas o dormir el SparkFun
  delay( 10000 );
  //Serial.print( "Goodnight" );
  //ESP.deepSleep( sleepTimeSeconds * 1000000 );

}
