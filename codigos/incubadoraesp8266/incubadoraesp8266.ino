// Including the ESP8266 WiFi library

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
const int bonbillo1relay =  1;
// Replace with your network details
const char* ssid = "YOUR_NETWORK_NAME";
const char* password = "YOUR_NETWORK_PASSWORD";
char linebuf[80];
// Web Server on port 80
WiFiServer server(80);
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;
String header;

String temperaturaestablecida = String(5);

int tl = 37;
// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// only runs once on boot
void setup() {

  WiFiManager wifiManager;
  wifiManager.autoConnect("incubadora" );
  
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();
float h = dht.readHumidity();
float t = dht.readTemperature();
  // Connecting to WiFi network
  /*Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
*/
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(1000);

  // Printing the ESP IP address
  Serial.println(WiFi.localIP());

  
}

// runs over and over again


boolean currentLineIsBlank = true;
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && blank_line) {
          // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
          float h = dht.readHumidity();
          // Read temperature as Celsius (the default)
          float t = dht.readTemperature();
          // Read temperature as Fahrenheit (isFahrenheit = true)
          float f = dht.readTemperature(true);
          // Check if any reads failed and exit early (to try again).
          if (isnan(h) || isnan(t) || isnan(f)) {
            Serial.println("Failed to read from DHT sensor!");
            strcpy(celsiusTemp, "Failed");
            strcpy(fahrenheitTemp, "Failed");
            strcpy(humidityTemp, "Failed");
          }
        
         else {
            // Computes temperature values in Celsius + Fahrenheit and Humidity
            float hic = dht.computeHeatIndex(t, h, false);
            dtostrf(hic, 6, 2, celsiusTemp);
            float hif = dht.computeHeatIndex(f, h);
            dtostrf(hif, 6, 2, fahrenheitTemp);
            dtostrf(h, 6, 2, humidityTemp);
            // You can delete the following Serial.print's, it's just for debugging purposes
           /* Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print(" %\t Temperature: ");
            Serial.print(t);
            Serial.print(" *C ");
            Serial.print(f);
            Serial.print(" *F\t Heat index: ");
            Serial.print(hic);
            Serial.print(" *C ");
            Serial.print(hif);
            Serial.print(" *F");
            Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print(" %\t Temperature: ");
            Serial.print(t);
            Serial.print(" *C ");
            Serial.print(f);
            Serial.print(" *F\t Heat index: ");
            Serial.print(hic);
            Serial.print(" *C ");
            Serial.print(hif);
            Serial.println(" *F");
          */}
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

          // your actual web page that displays temperature and humidity
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head></head><body><h1>incubadora</h1><h3>Temperature in Celsius: ");
          client.println(celsiusTemp);
          client.println("*C</h3><h3>Temperature in Fahrenheit: ");
          client.println(fahrenheitTemp);
          client.println("*F</h3><h3>Humidity: ");
          client.println(humidityTemp);
          client.println("<p>Position: <span id=\"tl\"></span></p>");
          client.println("<p><h1>relay o bonbillo #1 </h1><a href=\"on1\"><button>ON</button></a>&nbsp;<a href=\"off1\"><button>OFF</button></a></p>");
         
          client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"termometro\" onchange=\"servo(this.value)\" value=\"" + valueString + "\"/>");
          client.println("<script>var slider = document.getElementById(\"servoSlider\");");
          client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
          client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
          client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
          client.println("$.get(\"/?value=\" + tl + \"&\"); {Connection: close};}</script>");

          client.println("%</h3><h3>");
          client.println("</body></html>");

          if (header.indexOf("GET /?value=") >= 0) {
            pos1 = header.indexOf('=');
            pos2 = header.indexOf('&');
            temperaturaestablecida = header.substring(pos1 + 1, pos2);

            //Rotate the servo
            
            Serial.println(temperaturaestablecida);
           
          client.println();
          break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf, "GET /on1") > 0) {
            Serial.println("relay1 ON");
            digitalWrite(tl, HIGH);
          }
          else if (strstr(linebuf, "GET /off1") > 0) {
            Serial.println("relay1 off");
            digitalWrite(bonbillo1relay, LOW);
          }
        }   
        
    if (t >= tl ) {
      digitalWrite(bonbillo1relay, LOW);

      delay(1000);
    }
    else if (t < tl) {
      digitalWrite(bonbillo1relay, HIGH);

      delay(1000);
    }
        }
        } 
        }
      // closing the client connection
      delay(1);
      client.stop();
      Serial.println("Client disconnected.");
   

 
  }
}
