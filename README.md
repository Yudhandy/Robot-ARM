# 2DoFarmgripper
**Robotics Practicum 2025/2026 — UGMURO**

A 2-DOF (Degree of Freedom) robot arm project with record & playback functionality. This project supports two platforms: 
**Arduino Uno** (joystick-based control) and **ESP32** (web server-based control).

---

# Arduino Uno (joystick-based control)
* **2-DOF Control:**
    * Base Rotation
    * Shoulder Movement
* # Joystick Control (Arduino Uno):
    * Manually operate the robot arm using a joystick module.
* # Record & Playback System (Arduino Uno):
    * Press the joystick button to start/stop **recording** movements.
    * Press an external 'Play' button to **replay** the last saved motion sequence.
* # Web Control (ESP32 Only):
    * A slider-based control interface accessed via Wi-Fi.
    * HTML page hosted directly on the ESP32 using **LittleFS**.

---

# Hardware Requirements

* Arduino Uno **or** ESP32
* 2x Servo Motors (e.g., SG90 or MG90S) for Base + Shoulder
* 1x Joystick Module (X, Y, Button) *(For Arduino mode only)*
* 1x Push Button *(For Arduino mode only)*
* External 5V power supply for servos (Highly recommended)
* Breadboard & jumper wires

---

# Pinout Reference

# 1. Arduino Uno (Joystick Control & Record/Play)
<img src="/Image/arduino.jpg" width="600"> | Component | Arduino Pin |
| :--- | :--- |
| Joystick X-axis | A0 |
| Joystick Y-axis | A1 |
| Joystick Button | D2 |
| Play Button | D3 |
| Servo Base | D9 |
| Servo Shoulder | D10 |

# 2. ESP32 (Web Control via LittleFS)
<img src="/Image/esp32.jpg" width="600"> | Component | ESP32 Pin |
| :--- | :--- |
| Servo Base | 18 |
| Servo Shoulder | 19 |


---

# Software Setup

# 1. Arduino Uno Mode (Joystick)

1.  Install the [Arduino IDE](https://www.arduino.cc/en/software).
2.  Select board: **Arduino Uno** from the *Tools* > *Board* menu.
3.  Open the `/RobotCotrol.ino` sketch.
4.  Connect your Arduino Uno to the computer and select the correct Port.
5.  Upload the sketch.
6.  Wire the components according to the Arduino Uno pinout table.

### 2. ESP32 Mode (Web Control)

1.  Install the [Arduino IDE](https://www.arduino.cc/en/software).
2.  Install ESP32 board support via the **Board Manager**.
3.  Install the **ESP32 LittleFS** plugin:
    * Follow the plugin installation guide [here](https://github.com/lorol/arduino-esp32fs-plugin).
4.  Inside the sketch folder, create a new folder named `data`.
5.  Place your `/data/index.html` file inside the `data` folder.
6.  Select your ESP32 board (e.g., *ESP32 Dev Module*).
7.  Upload the filesystem: Go to **Tools** → **ESP32 Data Upload**. (This uploads the `index.html` file to LittleFS).
8.  Open the `/WebControl.ino` sketch.
9.  Upload the sketch to the ESP32.
10. Open the **Serial Monitor** (set baud rate to 115200) to find the IP address assigned to the ESP32.
11. Open that IP address in your browser to start controlling the robot. 🚀
<img src="/Image/TampilanWeb.png" width="600"> ---

# ⚠️ Important Notes

* **Use a Separate Power Supply:** It is highly recommended to use an **external 5V power supply** for the servo motors. Do not draw 5V power from the Arduino/ESP32 pins, as this can cause the board to *reset* or become unstable when the servos move.
* **Common Ground:** Always connect the **GND** from the servo power supply to the **GND** of the Arduino/ESP32 board. This is mandatory for the servo control (PWM) signals to work correctly.
* **Servo Range:** Adjust the *min* and *max* angle values in the code if your servos have a different rotational range (e.g., cannot do 0-180 degrees).

# 📜 License

Distributed under the MIT License. See the `LICENSE` file for more information.
