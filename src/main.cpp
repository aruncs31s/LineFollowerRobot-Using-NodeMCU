#include <Arduino.h>
#include <line_follower.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENA D5

#define IR_1_PIN A0
#define IR_2_PIN D7

LineFollower robot(IN1, IN2, IN3, IN4, ENA, 0, IR_1_PIN, IR_2_PIN);


#include <Arduino.h>

#include "config.h"


// Replace with your network credentials
const char* ssid     = "pi_wifi";
const char* password = "12345678";

// Create an instance of the WebServer on port 80
ESP8266WebServer server(80);




int dutyCycle = 0;

String valueString = String(0);
#define PROGMEM 1024


void handleRoot() {
  const char html[] = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }
      .button { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; background-color: #4CAF50; border: none; color: white; padding: 12px 28px; text-decoration: none; font-size: 26px; margin: 1px; cursor: pointer; }
      .button2 {background-color: #555555;}
    </style>
    <script>
      function moveForward() { fetch('/forward'); }
      function moveLeft() { fetch('/left'); }
      function stopRobot() { fetch('/stop'); }
      function moveRight() { fetch('/right'); }
      function moveReverse() { fetch('/reverse'); }
    </script>
  </head>
  <body>
    <h1>ESP32 Motor Control</h1>
    <p><button class="button" onclick="moveForward()">FORWARD</button></p>
    <div style="clear: both;">
      <p>
        <button class="button" onclick="moveLeft()">LEFT</button>
        <button class="button button2" onclick="stopRobot()">STOP</button>
        <button class="button" onclick="moveRight()">RIGHT</button>
      </p>
    </div>
    <p><button class="button" onclick="moveReverse()">REVERSE</button></p>
  </body>
  </html>)rawliteral";
  server.send(200, "text/html", html);
}

void handleForward() {
  Serial.println("Forward");
  robot.moveForward();
  server.send(200);
  
}

void handleLeft() {
  Serial.println("Left");
  robot.turnLeft();
  server.send(200);
}

void handleStop() {
  Serial.println("Stop");
  robot.stop();
  server.send(200);
}

void handleRight() {
  Serial.println("Right");
  robot.turnRight();
  server.send(200);
}

void handleReverse() {
  Serial.println("Reverse");
  robot.moveBackward();
  server.send(200);
}

void handleSpeed() {
  // if (server.hasArg("value")) {
  //   valueString = server.arg("value");
  //   int value = valueString.toInt();
  //   if (value == 0) {
  //     ledcWrite(0, 0); // Channel 0 for enable1Pin
  //     ledcWrite(1, 0); // Channel 1 for enable2Pin
  //     digitalWrite(motor1Pin1, LOW); 
  //     digitalWrite(motor1Pin2, LOW); 
  //     digitalWrite(motor2Pin1, LOW);
  //     digitalWrite(motor2Pin2, LOW);   
  //   } else { 
  //     dutyCycle = map(value, 0, 100, 0, 255); // Correct mapping
  //     ledcWrite(0, dutyCycle); // Channel 0 for enable1Pin
  //     ledcWrite(1, dutyCycle); // Channel 1 for enable2Pin
  //     Serial.println("Motor speed set to " + String(value));
  //   }
  // }
  server.send(200);
}
void setup() {
    Serial.begin(115200);
    robot.init();
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 100) { // Add retry limit
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi. Please check credentials.");
    // while (true); // Halt execution
  }

  // Define routes
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/left", handleLeft);
  server.on("/stop", handleStop);
  server.on("/right", handleRight);
  server.on("/reverse", handleReverse);
  server.on("/speed", handleSpeed);

  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
  delay(100);
}