#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6WeMlyE9z"  // Blynk Template ID
#define BLYNK_TEMPLATE_NAME "IoT Enabled Smart Irrigation System"
#define BLYNK_AUTH_TOKEN "rCiT_pjyqni9x6UuUvhra2p3W-mu-1ws"  // Blynk Auth Token

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "rCiT_pjyqni9x6UuUvhra2p3W-mu-1ws";  // Blynk Auth Token
char ssid[] = "RB308-3";  // WiFi 名稱
char pass[] = "RB30833083";  // WiFi 密碼

int soil_sensor = 36;  // 土壤濕度感測器 GPIO36
int val;

void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
}

void loop() {
    Blynk.run();
    val = 4095 - analogRead(soil_sensor);  // 讀取土壤濕度

    // **新增這行，印出土壤濕度到 Serial Monitor**
    Serial.print("土壤濕度: ");
    Serial.println(analogRead(soil_sensor));
    Serial.println(val);

    Blynk.virtualWrite(V0, (int)val);  // 將數據發送到 Blynk V0
    delay(1000);  // 延遲 1 秒
}
