#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#define PERMENIT 1
#define PERJAM 60
#define PERHARI 1440
#define tanah "2"
#define pompa "6"

// Network credentials
const char* ssid = "NURULFIKRI";
const char* password = "Novelty!@#";
WiFiClientSecure client;

// water Variable
const int dry = 2620;
const int wet = 990;

//// Timing
//unsigned long lastCheckTime = 0;
//const unsigned long checkInterval = 30000;
unsigned long counting;

//Telegram crendentias
String telegramBotToken = "7949925076:AAHngTGhhm89VCALLmhbnygxmfeV4-rUpts";
String chat_id = "1871698947";

//pemantauan.com crendential
String API_KEY= "b4884d6b6c1b42ed1edb6565ec9c94c7";
const char* serverName = "https://www.pemantauan.com/submission/";


// Pin Konfigurasi
#define SOIL_SENSOR_PIN 34
#define RELAY_PIN 23        

int State = -1;
bool pumpStatus = false;

void setup() {
  Serial.begin(115200);

  // Inisialisasi pin
  pinMode(SOIL_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Awal relay off
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // Membaca nilai kelembaban dari sensor
  int soilMoisture = analogRead(SOIL_SENSOR_PIN);
  int waterPercentage = map(soilMoisture, wet, dry, 100, 0);
  Serial.print("Soil Moisture: ");
  Serial.print(waterPercentage);
  Serial.println("%");

  // Logika kontrol
  int nowState;
  if (waterPercentage <= 10) {
    nowState = 0;
    delay(1000);
  } else if (waterPercentage <= 60) {
    nowState = 1;
    delay(1000);
  } else if (waterPercentage >= 90) {
    nowState = 2;
    delay(1000);
  } else {
    nowState = -1;
    delay(10000);
  }
  if (nowState != State) {
    State = nowState;

    switch (nowState) {
      case 0:
        digitalWrite(RELAY_PIN, HIGH);
        pumpStatus = true;
        sendTelegramMessage("Kelembaban Rendah, Pompa Dinyalakan");
        break;

      case 1:
        digitalWrite(RELAY_PIN, LOW);
        pumpStatus = false;
        sendTelegramMessage("Pompa dimatikan! Kebutuhan air tercukupi.");
        break;

      case 2:
        digitalWrite(RELAY_PIN, LOW);
        pumpStatus = false;
        sendTelegramMessage("Bahaya overwatering! Periksa sistem drainase.");
        break;

      default:
        break;
    }
  }

  sendtoPemantauan(PERMENIT, waterPercentage, pumpStatus);
  // Tunggu 1 detik sebelum membaca ulang
//  delay(30000);
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + String(telegramBotToken) +
                 "/sendMessage?chat_id=" + String(chat_id) +
                 "&text=" + message;

    Serial.println("Mengirim pesan ke Telegram: " + url);
    http.begin(client, url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Pesan terkirim!");
      Serial.println("Respon Telegram: " + payload);
    } else {
      Serial.print("Gagal mengirim pesan. HTTP Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi tidak terhubung. Gagal mengirim pesan.");
  }
}

void sendtoPemantauan(int frekuensi,float waterPercentage,bool pumpStatus) {
  int writeTimeRequired=60000;

  if ((millis() - counting) > frekuensi*writeTimeRequired) {
    if(WiFi.status() == WL_CONNECTED){
      std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
      client->setInsecure();
      HTTPClient http;

      http.begin(*client, "https://www.pemantauan.com/submission/");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String httpRequestData1 = "apikey=" + API_KEY;
      httpRequestData1 = httpRequestData1 + "&obyek1=" + tanah;
      httpRequestData1 = httpRequestData1 + "&value1=" + waterPercentage;
      httpRequestData1 = httpRequestData1 + "&obyek2=" + pompa;
      httpRequestData1 = httpRequestData1 + "&value2=" + pumpStatus;

      int httpResponseCode1 = http.POST(httpRequestData1);
      if (httpResponseCode1 > 0) {
          Serial.printf("Mengirim data...1 code: %d\n", httpResponseCode1);
          String payload = http.getString();
          Serial.print("Respon server: ");
          Serial.println(payload);
      } else {
          Serial.printf("Mengirim data...1 gagal, error: %s\n", http.errorToString(httpResponseCode1).c_str());
      }
      http.end();


    counting = millis();
    }
  }
}
