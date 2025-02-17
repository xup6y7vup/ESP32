#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>

// Replace with your WiFi credentials
const char* ssid = "RB308-3";
const char* password = "RB30833083";

// Replace with your computer's local IP address
const char* serverAddress = "YOUR_COMPUTER_IP";  // e.g., "192.168.1.100"
const int serverPort = 5000;

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

int PIR_S = 14; // PIR sensor on GPIO14

void startCameraServer();

void setup() {
    Serial.begin(115200);
    Serial.println();

    pinMode(PIR_S, INPUT);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    startCameraServer();
}

void loop() {
    int motionDetected = digitalRead(PIR_S);
    if (motionDetected == 1) {
        Serial.println("Motion detected! Capturing image...");
        sendImageToServer();
        delay(10000);  // Avoid repeated captures in a short time
    }
}

// ** Function to Send Image to Local Server **
void sendImageToServer() {
    camera_fb_t * fb = esp_camera_fb_get();  // Capture image
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    WiFiClient client;
    if (!client.connect(serverAddress, serverPort)) {
        Serial.println("Connection to server failed");
        esp_camera_fb_return(fb);
        return;
    }

    Serial.println("Connected to server, sending image...");

    // HTTP POST Request
    client.println("POST /upload HTTP/1.1");
    client.println("Host: " + String(serverAddress));
    client.println("Content-Type: multipart/form-data; boundary=ESP32BOUNDARY");
    client.println("Content-Length: " + String(fb->len + 150));
    client.println();
    client.println("--ESP32BOUNDARY");
    client.println("Content-Disposition: form-data; name=\"image\"; filename=\"esp32-cam.jpg\"");
    client.println("Content-Type: image/jpeg");
    client.println();
    
    client.write(fb->buf, fb->len);  // Send image data
    client.println();
    client.println("--ESP32BOUNDARY--");

    String response = client.readString();
    Serial.println("Server response: " + response);

    esp_camera_fb_return(fb);  // Free memory
    client.stop();
}
