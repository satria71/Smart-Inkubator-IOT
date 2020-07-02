#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


char auth[] = "2NRZPPV53VCNXjJA5yTtYtqbtD1vcl-N"; //Token
char ssid[] = "Wifi Id";
char pass[] = "sempurna";

const int relay = 5; // Relay pin (D1)
int relayOn = LOW;
int relayOff = HIGH;

WidgetLCD lcd(V0);

#define DHTPIN 0  // Suhu D3 Led D4
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
//  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(t)) {
    Serial.println("Sensor tidak terbaca!");
    return;
  }
  Blynk.virtualWrite(V5, t); //suhu virtual 5
//  Blynk.virtualWrite(V6, h); //kelembaban virtual 6

  if(t < 20){
      Blynk.notify("Suhu Terlalu Rendah!!!");
      digitalWrite(relay, relayOn);
      Blynk.virtualWrite(V0, t); //Lcd
      Blynk.virtualWrite(V1, t); //Grafik
      lcd.print(0,0,"Suhu : "+ String (t) +" C");
      lcd.print(0,1,"Suhu Dingin");
      delay(1000);
      lcd.clear();
      lcd.print(0,0,"Status Lampu");
      lcd.print(0,1,"Menyala");
      delay(1000);
      lcd.clear();
  }else if(t > 20 && t < 28){
      digitalWrite(relay, relayOn);
      Blynk.virtualWrite(V0, t); //Lcd
      Blynk.virtualWrite(V1, t); //Grafik
      lcd.print(0,0,"Suhu : "+ String (t) +" C");
      lcd.print(0,1,"Suhu Rendah");
      delay(1000);
      lcd.clear();
      lcd.print(0,0,"Status Lampu");
      lcd.print(0,1,"Menyala");
      delay(1000);
      lcd.clear();
    }else if(t >= 28 && t <= 31){
      digitalWrite(relay, relayOn);
      Blynk.virtualWrite(V0, t);
      Blynk.virtualWrite(V1, t); //Grafik
      lcd.print(0,0,"Suhu : "+ String (t) +" C");
      lcd.print(0,1,"Suhu Normal");
      delay(1000);
      lcd.clear();
      lcd.print(0,0,"Status Lampu");
      lcd.print(0,1,"Menyala");
      delay(1000);
      lcd.clear();
    }else if(t > 31 && t <=35){
      digitalWrite(relay, relayOff);
      Blynk.virtualWrite(V0, t);
      Blynk.virtualWrite(V1, t); //Grafik
      lcd.print(0,0,"Suhu : "+ String (t) +" C");
      lcd.print(0,1,"Suhu Tinggi");
      delay(1000);
      lcd.clear();
      lcd.print(0,0,"Status Lampu");
      lcd.print(0,1,"Mati");
      delay(1000);
      lcd.clear();
    }else if(t > 35){
      Blynk.notify("Suhu Terlalu Tinggi!!!");
      digitalWrite(relay, relayOff);
      Blynk.virtualWrite(V0, t);
      Blynk.virtualWrite(V1, t); //Grafik
      lcd.print(0,0,"Suhu : "+ String (t) +" C");
      lcd.print(0,1,"Suhu Berbahaya");
      delay(1000);
      lcd.clear();
      lcd.print(0,0,"Status Lampu");
      lcd.print(0,1,"Mati");
      delay(1000);
      lcd.clear();
    }
}
void setup()
{
  pinMode(relay, OUTPUT);
  digitalWrite(relay, relayOff);

  lcd.clear();
 
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}
void loop()
{
  Blynk.run();
  timer.run();
}
