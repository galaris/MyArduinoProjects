/**
 * @file    DSM501A.ino
 * @version V1.0.0
 * @date    2016/06/06
 * @brief   Description: this file is sample program for DSM501A Dust Sensor.
 *
 * Function List:
 *  Show();//display on the TFT LCD
 *  print_DSM501A_particle1(); //test the dust sensor
 *  print_temperature_humidity();//test the temperature and humidity sensor
 *  
 *  
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Keyboy.chen         2016/06/06     1.0.0                   
 * </pre>
 */

#include "MeOrion.h"
#include <SoftwareSerial.h>

const int pin = 2 ;// the input pin connect with  the Vout2 of Dust Sensor 
MeHumiture humiture(PORT_6);
MeSerial mySerial(PORT_5);


float duration;
float starttime;
float sampletime_ms = 30000;
float lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

#define TIME_MAX  30
float pm25val = 0.05; 
float pm25coef = 0.00207916725464941;//The coefficient of pm2.5
float pm25[TIME_MAX+1] ;//collect data of pm2.5 after a period of time

void Show(void)
{  
  //show the X and Y axes
  mySerial.print("PL(0,220,300,220,2);");//X axis
  mySerial.print("DS16(280,222,'t/s',2);");
  mySerial.print("DS16(0,222,'0',2);");  //0
  mySerial.print("DS16(115,222,'30',2);");  //30
  mySerial.print("DS16(235,222,'60',2);");  //60

  mySerial.print("PL(0,10,0,220,2);");//Y axis
  mySerial.print("DS16(0,0,'PM2.5',2);");
  mySerial.print("PL(0,170,5,170,2);");//10
  mySerial.print("DS16(8,160,'10',2);");
  mySerial.print("PL(0,120,5,120,2);");//20
  mySerial.print("DS16(8,110,'20',2);");
  mySerial.print("PL(0,70,5,70,2);");//30 
  mySerial.print("DS16(8,60,'30',2);");
  
  //show the huimidity
  mySerial.print("DS16(120,0,'Humidity (%): ");
  mySerial.print( humiture.getHumidity() );
  mySerial.print(" ',2);");

  //show the temperature
  mySerial.print("DS16(120,16,'Temperature (oC) = ");
  mySerial.print( humiture.getTemperature() );
  mySerial.print(" ',2);");

  //show the pm2.5
  if((pm25[0]>=0.0)&&(pm25[0]<=35.0))
  {
  mySerial.print("DS16(120,32,'PM25 value(ug/m3): ");
  mySerial.print(pm25val);
  mySerial.print(" ',2);");
  }
  //when the pm25 superscale,
  else if(pm25[0]>35.0)
  {
  mySerial.print("DS16(120,32,'PM25 value(ug/m3): ");
  mySerial.print(pm25val);
  mySerial.print(" ',1);");//red
    
   }

   //move the curve of pm25
  if((pm25[0]>0.0)&&(pm25[0]<=35.0))
  {
    for(int j=TIME_MAX;j>0;j--)
    { 
      mySerial.print("PL(");
      mySerial.print(j*10);
      mySerial.print(",");
      mySerial.print(220-(int)(pm25[j]*5));
      mySerial.print(",");
      mySerial.print((j-1)*10);
      mySerial.print(",");
      mySerial.print((int)(220-(int)(pm25[j-1]*5)));
      mySerial.print(",2);");

      pm25[j] = pm25[j-1]; 
    }
  }
  else 
  {
    mySerial.print("DS64(100,100,'Caution!',1);");
    }      
  mySerial.println(" ");  
}
void setup() 
{
  Serial.begin(9600); 
  mySerial.begin(9600);
  
  pinMode(2,INPUT);
  
  starttime = millis();//to get the machine running time ,the unit is ms. 

  //initialize the array,just for display
  for(int j=TIME_MAX;j>0;j--)
    pm25[j]=0.05;
    pm25[0]=1.5;

}

void loop() 
{
  mySerial.print("CLS(0);"); // clear the screen with c color 
  mySerial.print("DR0;");    // the screen displays in upright way
  
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)
 {
   ratio = lowpulseoccupancy/(sampletime_ms*10);  // Integer percentage 0=>100. The unit for time of low pulse occupancy is us.
   concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
   pm25val = pm25coef * concentration * 10; // 10 to transform 0.01 cf to 0.1 ft
   
   pm25[0] = pm25val;
       
   lowpulseoccupancy = 0;
   starttime = millis();

 }
     humiture.update();

    Show(); 
    delay(1000);   

//   print_temperature_humidity();
//    delay(1000); 

//  print_DSM501A_particle1(); 
//    delay(1000);
      
}


//void print_temperature_humidity(void)
//{ 
//  mySerial.print("CLS(0);"); // clear the screen with c color 
//  mySerial.print("DR0;");    // the screen displays in upright way
//  
//  mySerial.print("DS16(150,0,'Humidity (%): ");
//  mySerial.print( humiture.getHumidity() );
//  mySerial.print(" ',2);");
//  
//  mySerial.print("DS16(150,16,'Temperature (oC) = ");
//  mySerial.print( humiture.getTemperature() );
//  mySerial.println(" ',2);");
//
//} 

//  void print_DSM501A_particle1(void)
//{
//
//  mySerial.print("CLS(0);"); // clear the screen with c color 
//  mySerial.print("DR0;");    // the screen displays in upright way
//
//  mySerial.print("DS24(0,0,'LPoccupy: ");
//  mySerial.print(lowpulseoccupancy);
//  mySerial.print(" ',2);");
//  
//  mySerial.print("DS24(0,32,'Ratio(%): ");
//  mySerial.print( ratio );
//  mySerial.print(" ',2);");
//
//  mySerial.print("DS24(0,64,'concentration(pcs/0.01cft): ',2);");
//  mySerial.print("DS24(0,96,'");
//  mySerial.print(concentration );
//  mySerial.print(" ',2);");
//
//  mySerial.print("DS24(0,128,'PM25 value(ug/m3): ");
//  mySerial.print(pm25val);
//  mySerial.println(" ',2);");
//
//  Serial.print("pm25:");
//  Serial.print(pm25val);
//
//}