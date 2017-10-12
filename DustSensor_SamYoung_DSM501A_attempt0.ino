const int Vout2 = A0;
const int Vout1 = A1;

// the setup function runs once when you press reset or power the board
void setup() {
        Serial.begin(9600);
        pinMode(Vout2, INPUT);
        pinMode(Vout1, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {

        Serial.println(analogRead(Vout1));
        Serial.println(analogRead(Vout2));
        Serial.println("==============");
        delay(1000);
}

