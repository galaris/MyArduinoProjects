/*
   LACOSOX MP2.5 TEST

   lee la contaminación por MP2.5 y lo guarda en un archivo de texto para luego enviarlo
   a un servidor de datos. 
   Actualmente almacenando datos a: 
   https://thingspeak.com/channels/217483
   
*/

// Connect the Pin_3 of DSM501A to Arduino 5V
// Connect the Pin_5 of DSM501A to Arduino GND
// Connect the Pin_2 of DSM501A to Arduino D8
#include<string.h>
#include <SPI.h>
#include "ThingSpeak.h"

#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 5, 69 };

EthernetClient client;


int pin = 8;//DSM501A input D8
unsigned long duration;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio2 = 0;
float concentration2 = 0;

int i = 0;

unsigned long estabilization_time = 60000; //1 minutos

boolean debug1 = true;
boolean debug2 = false;

boolean sensor_estabilizado_ok = false;
boolean red_cargada = false;

// Arduino Ethernet shield: pin 4
const int chipSelect = 4;

const String archivo_datos_temporal = "datalog.txt";
boolean SIN_SD = false;


unsigned long myChannelNumber = XXXXXXX;
const char * myWriteAPIKey = "XXXXXXXX";

// convert from particles/0.01 ft3 to μg/m3
// 0.01ft3 = 0.0002831685m3 or 35.3147ft3  = 1m3
double pcs2ugm3 (double concentration_pcs)
{
  double pi = 3.14159;
  // All particles are spherical, with a density of 1.65E12 µg/m3
  double density = 1.65 * pow (10, 12);
  // The radius of a particle in the PM2.5 channel is .44 µm
  double r25 = 0.44 * pow (10, -6);
  double vol25 = (4 / 3) * pi * pow (r25, 3);
  double mass25 = density * vol25; // ug
  double K = 3531.47; // per m^3

  return concentration_pcs * K * mass25;
}

void cargar_red() {
  if (debug1) {
    Serial.print("Iniciando Ethernet...");
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("FAIL.");
    delay(10000);
  }
  else {
    Serial.print("OK.. IP:");
    Serial.println(Ethernet.localIP());
    red_cargada = true;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(8, INPUT);
  starttime = millis();

  Serial.println("    WELCOME TO ");
  Serial.println("Lacosox MP2.5 Test ");
  Serial.println("-------------------");

  if (debug1)
  {
    Serial.println("[+]time:" + String(millis()));
    Serial.println("[+]Estabilizando en segundo plano. Porfavor espere (" + String((estabilization_time - millis()) / 60000.0) + " minutos)...");
  }


  cargar_red();

  ThingSpeak.begin(client);


}
void loop()
{
  duration = pulseIn(pin, LOW);
  /* if (debug2)
    {
     Serial.print("[++]pulso:" + String(duration / 1000.0) + "ms ");
     Serial.println("now time:" + String(millis()));
    }
  */
  lowpulseoccupancy += duration;
  endtime = millis();

  if ((endtime - starttime) > sampletime_ms)
  {
    ratio2 = (lowpulseoccupancy / 10.0 ) / (endtime - starttime); // Porcentaje 0 - 100 lowpulseoccupancy is in microseconds
    concentration2 = (1.1 * pow(ratio2, 3) - 3.8 * pow(ratio2, 2) + 520 * ratio2 + 0.62); // pcs/0.01cf

    float concentration_ugm3 = pcs2ugm3(concentration2);

    String data = "[" + String(millis()) + "]" + " lowpulseoccupancy:" + String((lowpulseoccupancy / 1000.0)) + "ms" + " ratio2:" + String(ratio2) + " pcs/0.01cf:" + String(concentration2) + " ug/m^3:" + String(concentration_ugm3);

    if (debug1) {
      Serial.println("[+]" + data);
    }

    ThingSpeak.writeField(myChannelNumber, 1, concentration_ugm3, myWriteAPIKey);
    ThingSpeak.writeField(myChannelNumber, 2, concentration_ugm3, myWriteAPIKey);
    ThingSpeak.writeField(myChannelNumber, 3, concentration_ugm3, myWriteAPIKey);

    lowpulseoccupancy = 0;
    starttime = millis();
  }

  if (millis() > estabilization_time && !sensor_estabilizado_ok)
  {
    // pasado 3 minutos se considera estabilizado
    Serial.println("[+]*****Estabilizado completo*****");
    sensor_estabilizado_ok = true;
  }

  if (!red_cargada)
    cargar_red();

}