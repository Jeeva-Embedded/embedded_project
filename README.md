
🌱 Crop Smart Analyzer

A soil and environmental sensing system built using the **STM32 B-L475E-IOT01A** development board, OLED display, RS485 Modbus sensors, and WiFi. It reads **NPK, temperature, humidity, pH, and EC**, displays the results, and hosts a simple webpage with real-time data.

---
 Features

Reads soil nutrients: **Nitrogen, Phosphorous, Potassium**
 Measures **Temperature** and **Humidity** (via DHT11)
 Monitors **pH** and **Electrical Conductivity**
 Displays readings on a 128x64 OLED
 Hosts a WiFi-based web server to show live data
 Uses RS485 Modbus protocol for sensor communication
 Connects to ThingSpeak (optional, function stubbed)
 Built with **PlatformIO** for STM32 (B-L475E-IOT01A)

---

 Project Structure

```
CropSmartAnalyzer/
├── include/
│   └── (header files if needed)
├── lib/
│   └── (custom libraries if any)
├── src/
│   └── main.cpp               # Main application code
├── platformio.ini            # PlatformIO configuration
├── README.md                 # This file
```

---

 Requirements

Hardware:

* STM32 Nucleo B-L475E-IOT01A board
* OLED 128x64 I2C display
* DHT11 sensor (for Temp & Humidity)
* RS485 Module + Soil Sensors (NPK, pH, EC)
* WiFi network

Software:

* [PlatformIO IDE](https://platformio.org/install)
* USB connection to PC
* Adafruit libraries, DHT11, and WiFi libraries

---

PlatformIO Setup

Install PlatformIO

Install the [PlatformIO extension](https://platformio.org/platformio-ide) in VS Code.


2. PlatformIO Configuration (`platformio.ini`)

Here’s the recommended `platformio.ini`:

```ini
[env:stm32l475vg]
platform = ststm32
board = disco_l475vg_iot01a
framework = arduino
monitor_speed = 9600
upload_protocol = stlink
lib_deps =
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit GFX Library@^1.11.9
    adafruit/DHT sensor library@^1.4.4
    bblanchon/ArduinoJson@^6.21.3
    knolleary/PubSubClient@^2.8
```




 3. Upload the Firmware

1. Connect the STM32 board via USB.
2. Build and upload the code:

```bash
pio run --target upload
```

3. Open Serial Monitor:

```bash
pio device monitor
```

---

 Sensor Commands & Communication

The following RS485 command frames are used to read sensors:

| Parameter    | MODBUS Command (Hex)      |
| ------------ | ------------------------- |
| Nitrogen     | `01 03 00 1E 00 01 E4 0C` |
| Phosphorous  | `01 03 00 1F 00 01 B5 CC` |
| Potassium    | `01 03 00 20 00 01 85 C0` |
| Temperature  | `01 03 00 12 00 02 64 0E` |
| Humidity     | `01 03 00 12 00 01 24 0F` |
| pH           | `01 03 00 06 00 01 64 0B` |
| Conductivity | `01 03 00 15 00 01 95 CE` |

These are sent via `Serial4` (RS485), and results are parsed from the response.

---

 Display Output

The 128x64 OLED cycles between:

* NPK values (Nitrogen, Phosphorous, Potassium)
* Temperature, Humidity, pH

---

 Web Server

Once connected to WiFi, access the device by navigating to its IP address (printed on Serial Monitor) in a browser.

Example Output:

```
http://192.168.1.xxx
```

Displays a live HTML table with the most recent sensor readings.
