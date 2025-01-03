
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <WiFiST.h>
#include <WiFiServerST.h>
#include <WiFiClientST.h>
#include <dht11.h>
//                      RX    TX
HardwareSerial Serial4(PA1, PA0);

//void connectToWiFi();
void sendToThingSpeak();
byte nitrogen();
byte phosphorous();
byte potassium();
byte Temeprature();
byte Humidity();
byte pH();
byte Conductivity();
void Display();
void connection();
void printWifiStatus();
int dataRead(char *c);

SPIClass SPI_3(PC12, PC11, PC10);
WiFiClass WiFi(&SPI_3, PE0, PE1, PE8, PB13);

const int READ_BUFFER = 100;             // Size of reading buffer in WiFi device
uint8_t buf_read[READ_BUFFER]= {0};      // Reading buffer
int8_t read_index = READ_BUFFER;         // Reading index in buf_read

WiFiClient client;

char ssid[] = "Jarvis";      // your network SSID (name)
char pass[] = "Jeeva@2003";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

#define DHT11PIN 8

dht11 DHT11;
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
// RO to pin 8 & DI to pin 9 when using AltSoftSerial
#define RE 6
#define DE 7
 
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
const byte temp[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0e};
const byte hum[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0f};
const byte ph[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};
const byte ec[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xce};
 

byte values[11];
byte val1, val2, val3,val4,val5,val6,val7;
String page = "";
void setup() {
  Serial.begin(9600);
  Serial4.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
 
  // put RS-485 into receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  delay(500);
  display.clearDisplay();
  display.setCursor(5, 30);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Crop Smart Analyzer");
  display.setCursor(25, 40);
  display.setTextSize(1);
  display.print("Initializing");
  display.display();
  delay(3000);
 display.clearDisplay();


  while (!Serial) {
    ;
  }
  // set the LED pin mode
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize the WiFi module:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi module not detected");
    // don't continue
    while (true);
  }

  // print firmware version:
  String fv = WiFi.firmwareVersion();
  Serial.print("Firwmare version: ");
  Serial.println(fv);

  if (fv != "C3.5.2.5.STM") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WiFi network ");
    // print the network name (SSID)
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected.\nNetwork information:");
  // you're connected now, so print out the status
  printWifiStatus();
  // start the web server on port 80
  server.begin();
 




  //Initializing and Configuring OLED display
 
 
  //void Display();
  
  //void Display();
  //delay(3000);

  /*
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println("Start");

  while (WiFi.status() != WL_CONNECTED)
  {
  delayMicroseconds(1000);
  int count=0;
  Serial.print(".");
   Serial.print(count);
   //count = count+1;
    if(count == 0)
    {
    break;
    }

  }
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.localIP());
    delayMicroseconds(2000);
 /* server.on("/", []()
  {
    page = "<html><head><title>Smart Energy Meter using IoT</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color:  green ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Smart Energy Meter using IoT</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>Parameters</th><th>Value</th><th>Units</th></tr><tr><td>Voltage</td><td>"+String(val2)+"</td><td>Volts</td></tr>";
    page += "<tr><td>Current</td><td>"+String(val1)+"</td><td>Amperes</td></tr><tr><td>Power Factor</td><td>"+String(val4)+"</td><td>XXXX</td>";
    page += "<tr><td>Power</td><td>"+String(val3)+"</td><td>Watts</td></tr><tr>";
    page += "</tr><tr><td>Frequency</td><td>"+String(val5)+"</td><td>Hz</td></tr>";
    page += "<meta http-equiv=\"refresh\" content=\"3\">";
    server.send(200, "text/html", page);
  });
  server.begin();
  */
}
 
void loop() {
  //connectToWiFi();
  //delay(3000);
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);

 delay(2000); 


  Serial.print("Nitrogen: ");
  val1 = nitrogen();
  Serial.print(" = ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  delay(5000);
 
  Serial.print("Phosphorous: ");
  val2 = phosphorous();
  Serial.print(" = ");
  Serial.print(val2);
  Serial.println(" mg/kg");
  delay(5000);
 
  Serial.print("Potassium: ");
  val3 = potassium();
  Serial.print(" = ");
  Serial.print(val3);
  Serial.println(" mg/kg");
  delay(5000);

  Serial.print("Temperature: ");
  val4 = DHT11.temperature;
  Serial.print(" = ");
  Serial.print(val4);
  Serial.println(" c");
  delay(5000);

  Serial.print("Humidity: ");
  val5 = DHT11.humidity;
  Serial.print(" = ");
  Serial.print(val5);
  Serial.println(" %");
  delay(5000);;

  Serial.print("pH:");
  val6 = pH();
  Serial.print(" = ");
  Serial.print(val6);
  Serial.println(" ph");
  delay(5000);

  Serial.print("Electrical Conductivity: ");
  val7 = Conductivity();
  Serial.print(" = ");
  Serial.print(val7);
  Serial.println(" us/cm");
  delay(5000);
  Serial.println();
  
  display.clearDisplay(); 
  delay(1000);
  display.clearDisplay(); 
  display.setTextSize(2);
  display.setCursor(0, 5);
  display.print("N: ");
  display.print(val1);
  display.setTextSize(1);
  display.print(" mg/kg");
 
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.print("P: ");
  display.print(val2);
  display.setTextSize(1);
  display.print(" mg/kg");
 
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print("K: ");
  display.print(val3);
  display.setTextSize(1);
  display.print(" mg/kg");
 
  display.display();
  delay(3000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 5);
  display.print("Temp: ");
  display.print(val4);
  display.setTextSize(1);
  display.print(" C");
 
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.print("Hum: ");
  display.print(val5);
  display.setTextSize(1);
  display.print(" %");
 
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print("pH: ");
  display.print(val6);
  display.setTextSize(1);
  display.print(" pH");
 
  display.display();
  delay(3000);
  display.clearDisplay();
  delay(1000);
client = server.available();               // listen for incoming clients

  if (client) {
    char c = 0x35;                           // if you get a client,
    Serial.println("New client connected");  // print a message out the serial port
    String currentLine = "";                 // make a String to hold incoming data from the client
    while (client.connected()) {             // loop while the client's connected
      dataRead(&c);
      if (c == '\n') {                       // if the byte is a newline character
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP header, separe the body with a blank line
          client.println("HTTP/1.1 200 OK");
          //client.println("Content-Length: ");
          client.println("Connection: Close");
          client.println("Content-Type: text/html; charset=iso-8859-1");
          client.println();

          // the content of the HTTP response follows the header:
          client.print("<html><head>\n<title>WiFi Web Server</title>\n<style>\ndiv.a {\ntext-align: center;\n}\ntable {\nfont-family: arial, sans-serif;\nborder-collapse: collapse;\nwidth: 100%;\n}\ntd, th {\nborder: 1px solid #dddddd;\ntext-align: left;\npadding: 8px;\n}\ntr:nth-child(even) {\nbackground-color: #dddddd;\n}\n</style>\n</head>");
          client.print("<body>\n<h2>Crop Smart Analyzer</h2>\n<table>\n<tr>\n<th>Parameters</th>\n<th>Value</th>\n<th>Unit</th>\n</tr>\n<tr>\n<td>Nitrogen</td>\n<td>");
          client.print(val1);
          client.print("</td>\n<td>mg/kg</td>\n</tr>\n<tr>\n<td>Phosporous</td>\n<td>");
          client.print(val2);
          client.print("</td>\n<td>mg/kg</td>\n</tr>\n<tr>\n<td>Potassium</td>\n<td>");
          client.print(val3);
          client.print("</td>\n<td>mg/kg</td>\n</tr>\n<tr>\n<td>Temperature</td>\n<td>");
          client.print(val4);
          client.print("</td>\n<td>C</td>\n</tr>\n<tr>\n<td>Humidity</td>\n<td>");
          client.print(val5);
          client.print("</td>\n<td>%</td>\n</tr>\n<tr>\n<td>pH</td>\n<td>");
          client.print(val6);
          client.print("</td>\n<td>pH</td>\n</tr>\n<tr>\n<td>Electrical Conductivity</td>\n<td>");
          client.print(val7);
          client.print("</td>\n<td>us/cm</td>\n</tr>\n</table>\n");
          client.print("<meta http-equiv=\"refresh\" content=\"3\">\n");
          // The HTTP response ends with another blank line:
          client.print("</body></html>\n");

          // break out of the while loop:
          break;
        }
      //}
      // } 
      else {  // if you got a newline, then clear currentLine:
           currentLine = "";
         }
      }
      // } else if (c != '\r') {   // if you got anything else but a carriage return character,
      //   currentLine += c;       // add it to the end of the currentLine
      // }

      // // Check to see if the client request was "GET /H" or "GET /L":
      // if (currentLine.endsWith("GET /H")) {
      //   digitalWrite(LED_BUILTIN, HIGH);        // GET /H turns the LED on
      // }

      // if (currentLine.endsWith("GET /L")) {
      //   digitalWrite(LED_BUILTIN, LOW);         // GET /L turns the LED off
      // }
    }
    // close the connection:
    //client.stop();
    //server.begin();
    //Serial.println("Client disconnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're connected to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the IP address of your WiFi module:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print the MAC address of your WiFi module:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  for (uint8_t i = 0; i < 6; i++) {
    if (mac[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i != 5) {
      Serial.print(":");
    } else {
      Serial.println();
    }
  }

  // print the received signal strength (RSSI):
  int32_t rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

/*
 * Read data in the WiFi device. Reading byte by byte in very slow. This function
 * allows to read READ_BUFFER byte in the device and returns byte by byte the result.
 */
int dataRead(char *c) {
  if (read_index == READ_BUFFER) {     // No data available in the current buffer
    client.read(buf_read, READ_BUFFER);
    read_index = 0;
  }
  *c = buf_read[read_index];
  read_index++;

  if (*c != '\0') {
    return 1;
  } else {
    return 0;
  }
}
byte nitrogen() {
  // clear the receive buffer
  Serial4.flush();
 
  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
 
  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++ ) Serial4.write( nitro[i] );
 
  // wait for the transmission to complete
  Serial4.flush();
  
  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
 
  // delay to allow response bytes to be received!
  delay(200);
 
  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial4.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}
 
byte phosphorous() {
  Serial4.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(phos); i++ ) Serial4.write( phos[i] );
  Serial4.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial4.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}
 
byte potassium() {
  Serial4.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(pota); i++ ) Serial4.write( pota[i] );
  Serial4.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial4.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}

byte Temeprature() {
  Serial4.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(temp); i++ ) Serial4.write( temp[i] );
  Serial4.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial4.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}

byte Humidity() {
  Serial4.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(hum); i++ ) Serial4.write( hum[i] );
  Serial4.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial4.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}

byte pH() {
  Serial4.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(ph); i++ ) Serial4.write( ph[i] );
  Serial4.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial4.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}

byte Conductivity() {
  Serial4.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(ec); i++ ) Serial4.write( ec[i] );
  Serial4.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial4.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}