#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"  
#define ldrPin 36
#define DHTPIN 23
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "gvyg9jhFdRaZTa7QsijVwejtw30lUuJd"; 
char ssid[] = "Wifi.id_Indomaret_Sagan"; 
char pass[] = "otomatisnyambung"; 

int sliderLux;
int sliderSuhu;
float t;
int tamp;
BlynkTimer timer;

WidgetLED led1(V6);
WidgetLED led2(V7);

BLYNK_WRITE(V4)
  {
  float valueLux = param.asInt(); 
  sliderLux=valueLux;
  }

  BLYNK_WRITE(V5)
  {
  int valueSuhu = param.asInt(); 
  sliderSuhu=valueSuhu;
  }
  
BLYNK_WRITE(V8)  
{
  int button = param.asInt();
  tamp=button;
}

void sensor()
{
  t = dht.readTemperature();
  int sensorCahaya = analogRead(ldrPin);
  int  ldr = ((0.009758 * sensorCahaya) + 10);
  int Suhu = t;

    
  Blynk.virtualWrite(V0, Suhu);
  Blynk.virtualWrite(V1, ldr);
  Blynk.virtualWrite(V2, ldr);
  Blynk.virtualWrite(V3, Suhu);

  
  if (isnan(t)) {
    Serial.println("Sensor tidak terbaca!");
    return;
  }
  
  Serial.println("Sistem Monitoring Pencahayaan dan Suhu Kandang Burung Walet");
  Serial.print("Temperature : "); 
  Serial.print(t); 
  Serial.println("*C\t");
  Serial.print("Kecerahan : ");
  Serial.print(ldr);
  Serial.println(" LUX\t");
  
   if(Suhu > sliderSuhu){
    led2.on();
    led2.setColor("#D3435C");
  }else{
    led2.on();
    led2.setColor("#23C48E");
  }
  
  if(ldr > sliderLux){
    led1.on();
    led1.setColor("#D3435C");
  }else{
    led1.on();
    led1.setColor("#23C48E");
  }
}



void setup()
{
  Serial.begin(115200);
  delay(10);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");  
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,18,114), 8080);
  dht.begin();
  timer.setInterval(1000, sensor);
}



  
void loop()
{
  Blynk.run();
  if(tamp == 1){
      timer.run();
    }else {
      Serial.println("off"); 
    }
}
