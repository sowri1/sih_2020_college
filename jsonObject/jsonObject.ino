#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define WIFI_SSID "jessie99"
#define WIFI_PASSWORD "kokokokoko"
#define FIREBASE_HOST "https://water-monitoring-ed0e1.firebaseio.com/" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "86y0IrBHN7sVmQfMmjTu7grPoAV2OYlKECkG9KVy"

//Define Firebase Data object
FirebaseData firebaseData;
String path;
int count = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    String user_name = "raja_420";
    unsigned long strt = millis();
    path = "/data/" + user_name;

    String jsonStr = "";

    FirebaseJson json1, json2;

    FirebaseJsonData jsonObj;
    String date = "2/1/2020";
    json2.set("daily_data/" + String(date) + "/flow_day", random(45, 100));    
    json1.set("series/" + String(strt) + "/turbidity", random(5, 20));
    json1.set("series/" + String(strt) + "/flow", random(10, 10)/10.0);

//    json1.toString(jsonStr, true);
//    Serial.println(jsonStr);
    if (Firebase.set(firebaseData, path + "", json2))
        Serial.println("PASSED");

    if (Firebase.set(firebaseData, path, json1))
        Serial.println("PASSED");
    else
    {
        Serial.println("FAILED");
//        Serial.println("REASON: " + firebaseData.errorReason());
    }
}

void loop()
{
  if(Serial.available() > 0){
    char* sz = Serial.read();
    
    FirebaseJson json2;
    String date = "2/1/2020";
    unsigned long crnt = millis();
    while(count<100){
      char buf[sizeof(sz)];
      serialResponse.toCharArray(buf, sizeof(buf));
      char *p = buf;
      char *str;
      FirebaseJson json1;
      while ((str = strtok_r(p, "|", &p)) != NULL)
        json1.set("series/" + String(strt) + "/turbidity", (int)str);


    if (Firebase.set(firebaseData, path, json1))
        Serial.println("PASSED");    
   }
    json2.set("daily_data/" + String(date) + "/flow_day", str);
    if (Firebase.set(firebaseData, path + "", json2))
        Serial.println("PASSED");
    count = 0;
  }

}
