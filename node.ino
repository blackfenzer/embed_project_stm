#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>
SoftwareSerial StmSerial(D5, D6); // RX | TX

char ssid[] = "an";
const char *password = "samsung9";

void setup()
{

    // Set pin for receiving data from Nucleo401
    pinMode(D5, INPUT);
    pinMode(D6, OUTPUT);
    StmSerial.begin(9600);

    // nodeMcu Baud rate is 9600
    Serial.begin(115200);
    delay(2000);
    Serial.println("Starting...");
    delay(2000);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println("  Connected!");
    Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
}

void loop()
{
           
    if (StmSerial.available())
    {
        String buf = StmSerial.readStringUntil('$');
        
            Serial.println(buf);

            if (WiFi.status() == WL_CONNECTED)
            {
                
                 WiFiClientSecure client;
            HTTPClient http;
                client.setInsecure(); //the magic line, use with caution
                
                int httpResponseCode;
                if (buf == "0" || buf == "1")
                {
                  client.connect("https://embed-project-backend.vercel.app/watering", 443);
                  http.begin(client, "https://embed-project-backend.vercel.app/watering");
                  http.addHeader("Content-Type", "application/json");
                  httpResponseCode= http.POST(buf=="1"?"{\"value\":true}":"{\"value\":false}");
                }
                else
                {
                  client.connect("https://embed-project-backend.vercel.app", 443);
                  http.begin(client, "https://embed-project-backend.vercel.app");
                  http.addHeader("Content-Type", "application/json");
                  httpResponseCode= http.POST(buf);
                }
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);

                // Free resources
                http.end();
            }
            else
            {
                Serial.println("WiFi Disconnected");
            }
        
    }
}
// put your main code here, to run repeatedly: