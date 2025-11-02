#include <Servo.h>

// --- Objek Servo ---
Servo baseServo;
Servo shoulderServo;

// --- Pinout Hardware ---
const int joyX = A0;
const int joyY = A1;
const int joyBtn = 2;
const int playBtn = 3;

const int baseServoPin = 5;
const int shoulderServoPin = 6;

// --- Pengaturan Rekaman ---
const int MAX_STEPS = 200;
int basePos[MAX_STEPS];
int shoulderPos[MAX_STEPS];
int stepCount = 0;

// --- Variabel State ---
bool recording = false;
bool lastJoyBtnState = HIGH;

// --- Posisi Servo ---
int currentBase = 90;
int currentShoulder = 90;
int targetBase = 90;
int targetShoulder = 90;

// --- Pengaturan Timer Non-Blocking ---
unsigned long lastMoveTime = 0;
const int MOVE_INTERVAL = 10; // Jeda untuk gerakan manual
const int DEADBAND = 2;

unsigned long lastRecordTime = 0;
const int RECORD_INTERVAL = 100; // Jeda antar rekaman


void setup() {
  baseServo.attach(baseServoPin);
  shoulderServo.attach(shoulderServoPin);

  pinMode(joyBtn, INPUT_PULLUP);
  pinMode(playBtn, INPUT_PULLUP);

  baseServo.write(currentBase);
  shoulderServo.write(currentShoulder);

  Serial.begin(9600);
  Serial.println("Sistem Kontrol Servo Siap.");
}

void loop() {

  // JOYSTICK CONTROL
  int joyValX = analogRead(joyX);
  int joyValY = analogRead(joyY);
  targetBase = map(joyValX, 0, 1023, 0, 180);
  targetShoulder = map(joyValY, 0, 1023, 0, 180);

  if (millis() - lastMoveTime >= MOVE_INTERVAL) {
    lastMoveTime = millis();
    if (abs(currentBase - targetBase) > DEADBAND) {
      if (currentBase < targetBase) currentBase++;
      else if (currentBase > targetBase) currentBase--;
    }
    if (abs(currentShoulder - targetShoulder) > DEADBAND) {
      if (currentShoulder < targetShoulder) currentShoulder++;
      else if (currentShoulder > targetShoulder) currentShoulder--;
    }
    baseServo.write(currentBase);
    shoulderServo.write(currentShoulder);
  }

  // RECORD
  if (recording && millis() - lastRecordTime >= RECORD_INTERVAL) {
    lastRecordTime = millis();
    if (stepCount < MAX_STEPS) {
      basePos[stepCount] = targetBase;
      shoulderPos[stepCount] = targetShoulder;
      stepCount++;
    }
  }

  bool joyBtnState = digitalRead(joyBtn);
  if (joyBtnState == LOW && lastJoyBtnState == HIGH) {
    recording = !recording;
    if (recording) {
      stepCount = 0;
      lastRecordTime = millis();
      Serial.println("Mulai Merekam...");
    } else {
      Serial.print("Berhenti Merekam. Langkah tersimpan: ");
      Serial.println(stepCount);
    }
    delay(50);
  }
  lastJoyBtnState = joyBtnState;


  //PLAY BACK
  if (digitalRead(playBtn) == LOW && stepCount > 0 && !recording) {
    Serial.println("Memutar Rekaman...");
    
    // Ulangi untuk setiap titik yang direkam
    for (int i = 0; i < stepCount; i++) {
      int playTargetBase = basePos[i];
      int playTargetShoulder = shoulderPos[i];
      
      // Terus bergerak sampai KEDUA servo mencapai target untuk titik ini
      while (currentBase != playTargetBase || currentShoulder != playTargetShoulder) {
        
        // Atur kecepatan playback di sini
        if (millis() - lastMoveTime >= MOVE_INTERVAL) { // Menggunakan timer yang sama
            lastMoveTime = millis();

            // Gerakkan servo base satu langkah lebih dekat ke targetnya
            if (currentBase < playTargetBase) currentBase++;
            else if (currentBase > playTargetBase) currentBase--;
            
            // Gerakkan servo shoulder satu langkah lebih dekat ke targetnya
            if (currentShoulder < playTargetShoulder) currentShoulder++;
            else if (currentShoulder > playTargetShoulder) currentShoulder--;
            
            // Perintahkan KEDUA servo untuk bergerak secara bersamaan
            baseServo.write(currentBase);
            shoulderServo.write(currentShoulder);
        }
      }
    }
    
    targetBase = currentBase;
    targetShoulder = currentShoulder;
    Serial.println("Pemutaran Selesai. Kembali ke mode kontrol manual.");
    delay(500);
  }
}
