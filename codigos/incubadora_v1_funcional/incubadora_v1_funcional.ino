    // Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "DHT.h"
#include <Servo.h>

IPAddress ip(192,168,1,70);
IPAddress gateway(192,168,1,70);
IPAddress subnet(255,255,255,0);
Servo myservo;
// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
const int bonbillo1relay = 4 ;
const int bonbillo2relay = 0 ;
const int ventilador = 2 ;
const int servoPin = 16;

String valueString = String(5);

String header;

int tl = 17;
// Web Server on port 80
WiFiServer server(80);
char linebuf[80];
boolean currentLineIsBlank = true;
// DHT Sensor
const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

String bonbillo1relay2 = "on";
String bonbillo2relay2 = "on";

float h = dht.readHumidity();
float t = dht.readTemperature();
// only runs once on boot
void setup() {
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("incubadora" );
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  dht.begin();
    WiFi.config(ip, gateway, subnet);
  // Connecting to WiFi network
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
  digitalWrite(ventilador, HIGH );
myservo.attach(servoPin);
}

// runs over and over again
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
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // You can delete the following Serial.print's, it's just for debugging purposes
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
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("relay on");
              bonbillo2relay2 = "on";
              digitalWrite(bonbillo1relay, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("relay off");
              bonbillo2relay2= "off";
              digitalWrite(bonbillo1relay, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("relay2 on");
              bonbillo1relay2 = "on";
              digitalWrite(bonbillo2relay, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("relay2 off");
              bonbillo1relay2 = "off";
              digitalWrite(bonbillo2relay, LOW);
            
            }
            client.println("<style>.button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // your actual web page that displays temperature and humidity
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>Incubadora</h1><h3>Temperatura en Celsius y mas: ");
              if (atoi(celsiusTemp) >= 32) {
            client.println("<div style=\"color: #930000;\">");
          }
          else if (atoi(celsiusTemp) < 32 && atoi(celsiusTemp) >= 5) {
            client.println("<div style=\"color: #006601;\">");
          }
          else if (atoi(celsiusTemp) < 5) {
            client.println("<div style=\"color: #009191;\">");
          }    client.println(celsiusTemp);
          client.println("*C</p><p>");
          client.println(fahrenheitTemp);
          client.println("*F</p></div><p>");
            client.println(humidityTemp);
            
            client.println("% humedad relativa</h3><h3>");
                       // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>bonbillo1relay <h6></h6>- State " + bonbillo2relay2 + "</p>");
            // If the output5State is off, it displays the ON button       
            if (bonbillo2relay2=="on") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>bonbillo2relay<br/> - State " + bonbillo1relay2 + "</p>");
            // If the output4State is off, it displays the ON button       
            if (bonbillo1relay2=="on") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<h6>para la  garantisar la seguridad del ebrion <p>solo se puede prender  una ves sea</p> <p>la temperatura mayor a 37 grados C se podra manipular</p><p> los bombillos que asea tempertuara va estar apagada</p></h6></body></html>");     
         
            break;
       
        
        
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
        
          }
         
            
       
         if (tl > t ) {
      digitalWrite(bonbillo1relay, LOW);
      digitalWrite(bonbillo2relay, LOW);
      delay(5);
    }
    else {
      digitalWrite(bonbillo1relay, HIGH);
      digitalWrite(bonbillo2relay, HIGH);
      delay(5);
    }
           
    
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
      }}}}
}
