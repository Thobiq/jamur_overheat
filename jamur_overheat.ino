const char* ssid = "SSID";
const char* password = "PASSWORD";

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TAMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Monitoring Jamur Overheat"
#define BLYNK_AUTH_TOKEN "TOKEN"

#define DHTPin 23
#define FanPin 21
#define MistPin 19
#define LedPin 2

#include <Bonezegei_DHT11.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

Bonezegei_DHT11 dht(DHTPin);

void setup() {
  delay(1000);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  pinMode(FanPin, OUTPUT);
  pinMode(MistPin, OUTPUT);
  pinMode(LedPin, OUTPUT);
  digitalWrite(MistPin, LOW);
  digitalWrite(FanPin, LOW);
  digitalWrite(LedPin, HIGH);
  dht.begin();
}

void loop() {
  delay(3000);
  Blynk.run();
  if(dht.getData()){
    float tempCel = dht.getTemperature();
    int hum = dht.getHumidity();
    Serial.print("Suhu: ");
    Serial.print(tempCel);
    Serial.println("°C");

    Serial.print("Kelembapan: ");
    Serial.print(hum);
    Serial.println("%");

    Blynk.virtualWrite(V2, tempCel);
    Blynk.virtualWrite(V3, hum);

    if(tempCel > 26.0 && tempCel != 0){
      digitalWrite(FanPin, LOW);
      digitalWrite(MistPin, LOW);
      digitalWrite(LedPin, LOW);
      Blynk.virtualWrite(V0, tempCel);
      digitalWrite(LedPin, LOW);
    } else if(tempCel == 0){
      digitalWrite(LedPin, HIGH);
      digitalWrite(FanPin, HIGH);
      digitalWrite(MistPin, HIGH);
    } else if(tempCel < 24.0 && tempCel != 0){
      digitalWrite(LedPin, LOW);
      digitalWrite(FanPin, HIGH);
      digitalWrite(MistPin, HIGH);
      Blynk.virtualWrite(V0, tempCel);
    }
    else {
      digitalWrite(FanPin, HIGH);
      digitalWrite(MistPin, HIGH);
      digitalWrite(LedPin, LOW);
      Blynk.virtualWrite(V0, tempCel);
    }

    if(hum == 0 ){
      digitalWrite(MistPin, HIGH);
      digitalWrite(LedPin, HIGH);
    } else if (hum > 90 && hum != 0){
      digitalWrite(MistPin, HIGH);
      digitalWrite(LedPin, LOW);
      Blynk.virtualWrite(V1, hum);
    } else if (hum < 80 && hum != 0){
      digitalWrite(MistPin, LOW);
      Blynk.virtualWrite(V1, hum);
    } 

  } else{
    Serial.println("Nothing Data :)");
  }

}