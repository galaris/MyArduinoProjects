int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;//sampe 1s ;
unsigned long lowpulseoccupancy = 0;

float ratio = 0;
float concentration = 0;
float particle = 0;




void setup() {
  Serial.begin(9600);
  pinMode(8,INPUT);
  starttime = millis();
}

void loop() {
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  if ((millis() - starttime) > sampletime_ms) //if the sampel time == 30s
  {
    ratio = lowpulseoccupancy / (sampletime_ms * 10.0);  // Integer percentage 0=>100
    concentration =  (0.0026 * pow(ratio, 2)) + (0.0859 * ratio) - 0.0184; // Equations based on Samyoung DSM501A spec sheet
    particle = (0.2069 * pow(ratio, 4)) - (9.7081 * pow(ratio, 3)) + (156.86 * pow(ratio, 2)) - (389.72 * ratio) + 2169.9; 

    ppmv=(float)(((concentrationPM25 * 0.0283168) / 100) *  ((0.08205 * temp) /0.01)) / 1000;

  
    Serial.print("lowpulseoccupancy = ");
    Serial.print(lowpulseoccupancy);

    Serial.print(" ||| ratio = ");
    Serial.print(ratio); 
    
    Serial.print(" ||| partciles/cm3= ");
    Serial.print(particle*3.534); // 1000/283 ml - convert particles/283ml to particles/liter (or partciles/cm3)
    
    Serial.print(" ||| concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf"); 
    
    lowpulseoccupancy = 0;
    starttime = millis();
  }
}

