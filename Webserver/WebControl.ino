#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ESP32Servo.h>
#include <vector> 

#define BASE_PIN 18
#define SHOULDER_PIN 19

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASS";

AsyncWebServer server(80);

Servo baseServo;
Servo shoulderServo;

int basePos = 90;
int shoulderPos = 90;

// ====== Struct dan Vektor untuk Record/Play ======

struct ArmPosition {
  int base;
  int shoulder;
};

// Vektor (array dinamis) 
std::vector<ArmPosition> recordedSequence;

void setup() {
  Serial.begin(115200);

  // Mount LittleFS
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS Mount Failed!");
    return;
  }

  // Attach servos
  baseServo.attach(BASE_PIN);
  shoulderServo.attach(SHOULDER_PIN);
  baseServo.write(basePos);
  shoulderServo.write(shoulderPos);

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

  // Serve HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Handle slider requests
  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("base")) {
      basePos = request->getParam("base")->value().toInt();
      baseServo.write(basePos);
      // Serial.printf("Base: %d\n", basePos); // Bisa di-uncomment jika perlu
    }
    if (request->hasParam("shoulder")) {
      shoulderPos = request->getParam("shoulder")->value().toInt();
      shoulderServo.write(shoulderPos);
      // Serial.printf("Shoulder: %d\n", shoulderPos); // Bisa di-uncomment jika perlu
    }
    request->send(200, "text/plain", "OK");
  });

  // Handle reset request (smooth simultaneous movement)
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    int targetBase = 90;
    int targetShoulder = 90;
    int stepDelay = 15; // ms between steps

    // Move until both servos reach target
    while (basePos != targetBase || shoulderPos != targetShoulder) {
      if (basePos < targetBase) basePos++;
      else if (basePos > targetBase) basePos--;

      if (shoulderPos < targetShoulder) shoulderPos++;
      else if (shoulderPos > targetShoulder) shoulderPos--;

      baseServo.write(basePos);
      shoulderServo.write(shoulderPos);

      delay(stepDelay);
    }

    Serial.println("Arm back to initial position");
    request->send(200, "text/plain", "RESET DONE");
  });

  // record 
  server.on("/record", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    recordedSequence.push_back({basePos, shoulderPos});
    
    Serial.printf("Recorded pos: Base=%d, Shoulder=%d. Total steps: %d\n", 
                   basePos, shoulderPos, recordedSequence.size());
                   
    request->send(200, "text/plain", "Position Recorded");
  });

  // play 
  server.on("/play", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Playing sequence...");
    int stepDelay = 15; 

    // Iterasi melalui setiap posisi yang tersimpan di vektor
    for (const auto& pos : recordedSequence) {
      int targetBase = pos.base;
      int targetShoulder = pos.shoulder;

      Serial.printf("Moving to: Base=%d, Shoulder=%d\n", targetBase, targetShoulder);

      // Gunakan logika pergerakan yang sama seperti /reset
      // untuk bergerak ke titik target berikutnya
      while (basePos != targetBase || shoulderPos != targetShoulder) {
        if (basePos < targetBase) basePos++;
        else if (basePos > targetBase) basePos--;

        if (shoulderPos < targetShoulder) shoulderPos++;
        else if (shoulderPos > targetShoulder) shoulderPos--;

        baseServo.write(basePos);
        shoulderServo.write(shoulderPos);

        delay(stepDelay);
      }
    }

    Serial.println("Playback complete.");
    request->send(200, "text/plain", "PLAYBACK DONE");
  });


  server.begin();
}

void loop() {
  
}
