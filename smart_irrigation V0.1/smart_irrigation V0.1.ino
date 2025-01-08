//// Treshhold Variable
//const int critical_Low = 10;
//const int Normal = 60;
//const int critical_High = 90;

int button;
int pump;

// Pins
//const int soilSensor = D7;
const int relayPump = D8;

//define moist 
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);


  //pin Setup
//  pinMode(soilSensor, INPUT);
  pinMode(relayPump, OUTPUT);
  pinMode(D2, INPUT);
  digitalWrite(relayPump, LOW);
//  digitalRead(D2, LOW);
  
//  //network initialization
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi...");
//  }
//  Serial.println("Connected to WiFi");

  pump = LOW;
}

void loop() {
  button = digitalRead(D2);
  
//  if (millis() - lastCheckTime > checkInterval) {
//    lastCheckTime = millis();

//  // Read sensor data
//  int moistureValue = analogRead(soilSensor);
//  int moisturePercentage = map(moistureValue, 0, 1023, 0, 100);
//  bool  pumpStatus = false;
//  Serial.print("Moisture Level: ");
//  Serial.println (moistureValue);
//  //Serial.println(moisturePercentage);
//
//  if (moisturePercentage <= critical_Low){
////      sendAlert("Kadar Air" + String(moisturePercentage) + "Memasuki batas Kritikal. Mengaktifkan Pompa Air");
//      digitalWrite (relayPump, HIGH);
//      pumpStatus = true;
//    } else if (moisturePercentage >= Normal && moisturePercentage < critical_High){
////      sendAlert("Kadar Air" + String(moisturePercentage) + "Tercukupi. Mematikan Pompa Air");
//      digitalWrite (relayPump, LOW);
//      pumpStatus = false;
//    } else if (moisturePercentage >= critical_High){
////      sendAlert("Kadar Air" + String(moisturePercentage) + "Berlebihan. Segera Benahi Sistem Drainase");
//      digitalWrite (relayPump, LOW);
//      pumpStatus = false;
//    }


//  sendtoPemantauan(PERMENIT, moisturePercentage, pumpStatus);
  if (button == HIGH){
    digitalWrite(relayPump, HIGH);
    Serial.print()
    } else {
    digitalWrite(relayPump, LOW)  ;
    }
  delay(1000);        // delay in between reads for stability
}
