#define SOIL_SENSOR 36  // 土壤濕度感測器 GPIO36
#define RELAY_PIN 13    // 繼電器 GPIO13

int counter = 0; // 記錄執行次數

void setup() {
    Serial.begin(115200);
    pinMode(SOIL_SENSOR, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
    if (counter >= 10) {
        Serial.println("測試結束，ESP32 停止執行！");
        Serial.println("關閉灑水。");
        digitalWrite(RELAY_PIN, LOW); // 繼電器關閉
        while (true);  // 進入無限迴圈，完全停止
    }

    int val = analogRead(SOIL_SENSOR);
    Serial.print("測試次數: ");
    Serial.print(counter + 1);
    Serial.print(" / 10，土壤濕度值: ");
    Serial.println(val);

    if (val < 3000) {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("濕度足夠，關閉灑水。");
    } else {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("土壤乾燥，開啟灑水！");
    }

    counter++;
    delay(500);
}
