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

#define SOIL_SENSOR 36  // 土壤濕度感測器 GPIO36
#define RELAY_PIN 13    // 繼電器 GPIO13

int val;
bool autoMode = true;  // 預設為自動模式（true = 自動, false = 遠端）

// **Blynk 按鈕控制模式 (V1)**
BLYNK_WRITE(V1) {
    autoMode = param.asInt();  // 按鈕的值 (0 = 遠端模式, 1 = 自動模式)
    Serial.print("模式切換為: ");
    Serial.println(autoMode ? "自動模式" : "遠端模式");
}

// **Blynk 按鈕遠端控制繼電器 (V2)**
BLYNK_WRITE(V2) {
    if (!autoMode) {  // 只有在 "遠端模式" 才能手動控制
        int relayState = param.asInt();
        digitalWrite(RELAY_PIN, relayState);
        Serial.print("遠端模式 - 灑水開關: ");
        Serial.println(relayState ? "開啟" : "關閉");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(SOIL_SENSOR, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);  // 預設關閉灑水

    Blynk.begin(auth, ssid, pass);
}

void loop() {
    Blynk.run();
    val = 4095 - analogRead(SOIL_SENSOR);  // 讀取土壤濕度

    Serial.print("土壤濕度: ");
    Serial.println(val);

    Blynk.virtualWrite(V0, val);  // 回傳土壤濕度到 Blynk V0

    if (autoMode) {  // **自動模式**
        if (val < 3000) {  // 濕度夠，不澆水
            digitalWrite(RELAY_PIN, LOW);
            Serial.println("自動模式 - 濕度足夠，關閉灑水。");
        } else {  // 土壤乾燥，開啟灑水
            digitalWrite(RELAY_PIN, HIGH);
            Serial.println("自動模式 - 土壤乾燥，開啟灑水！");
        }
    }

    delay(1000);
}
