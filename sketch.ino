#include <WiFi.h>
#include <ThingSpeak.h>
#include "DHTesp.h"

//  -----------DHTESP-----------
DHTesp dhtSensor;
TempAndHumidity data;
const int DHT_PIN = 15;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";
WiFiClient client;
unsigned long myChannelNumber = 2094622;
const char * myWriteAPIKey = "E2XF49Y6L7LASSJ2";

int statusCode;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature));
  Serial.println("-------------");
  delay(1000); // this speeds up the simulation

  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect");
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");;
  }

  ThingSpeak.setField(1, data.temperature);
  statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(statusCode == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem Writing data. HTTP error code: " + String(statusCode));
  }

  Serial.println("**************");

    delay(10000);
}
