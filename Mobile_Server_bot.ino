#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Motor A
int IN1 = D5; // Replace with your actual pin number
int IN2 = D6; // Replace with your actual pin number

// Motor B
int IN3 = D7; // Replace with your actual pin number
int IN4 = D8; // Replace with your actual pin number

//For 4 wheel models either connect them corresponding to Motor A and Motor B or define new Pins to increase control

WiFiServer server(80);
ESP8266WebServer webServer(80);
void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
//Set the baud rate
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin("WIFI_NAME", "WIFI_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Setup routes for the web server
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/F", HTTP_GET, handleForward);
  webServer.on("/B", HTTP_GET, handleBackward);
  webServer.on("/L", HTTP_GET, handleLeft);
  webServer.on("/R", HTTP_GET, handleRight);
  webServer.on("/X", HTTP_GET, handleStop);
  // Start the web server
  webServer.begin();
}

void loop()
{
  webServer.handleClient();
}
//defined different functions to handle different commands

void handleForward() 
{
  Serial.println("Command received: Forward");
  moveForward();
  sendResponse("Moving forward");
}

void handleBackward() 
{
  moveBackward();
  sendResponse("Moving backward");
}

void handleRight() 
{
  turnRight();
  sendResponse("Turning right");
}

void handleLeft() 
{
  turnLeft();
  sendResponse("Turning left");
}

void handleStop() 
{
  stopMotors();
  sendResponse("Motors stopped");
}
void handleRoot() 
{
  String html = "<html><head>";
  html += "<style>";
  // CSS for button layout and appearance
  html += "body { display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #f0f0f0; margin: 0; }";
  html += ".container { text-align: center; }";
  html += ".direction { display: flex; justify-content: center; }";
  html += ".row { margin: 10px 0; }";
  html += "button { width: 100px; height: 100px; font-size: 24px; background-color: #4CAF50; color: white; border: none; border-radius: 10px; margin: 5px; cursor: pointer; }";
  html += "button:active { background-color: #45a049; }";
  html += "#stopButton { width: 120px; height: 120px; font-size: 30px; background-color: red; border-radius: 50%; }";
  html += ".arrow-up:after { content: '\\2191'; }";
  html += ".arrow-down:after { content: '\\2193'; }";
  html += ".arrow-left:after { content: '\\2190'; }";
  html += ".arrow-right:after { content: '\\2192'; }";
  html += "</style>";
  html += "</head><body>";
  
  html += "<div class='container'>";
  html += "<h1>Robot Control</h1>";
  html += "<div class='row'>";
  html += "<button class='arrow-up' onclick=\"sendCommand('F')\"></button>";
  html += "</div>";
  
  html += "<div class='direction'>";
  html += "<button class='arrow-left' onclick=\"sendCommand('L')\"></button>";
  html += "<button id='stopButton' onclick=\"sendCommand('X')\">Stop</button>";
  html += "<button class='arrow-right' onclick=\"sendCommand('R')\"></button>";
  html += "</div>";
  
  html += "<div class='row'>";
  html += "<button class='arrow-down' onclick=\"sendCommand('B')\"></button>";
  html += "</div>";
  html += "</div>";
  
  html += "<script>";
  html += "function sendCommand(command) {";
  html += "var xhr = new XMLHttpRequest(); xhr.open('GET', '/' + command, true); xhr.send();";
  html += "}";
  html += "</script>";
  
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}
void sendResponse(String message) 
{
  webServer.send(200, "text/plain", message);
}

void moveForward() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  }
//can add a delay function to control the amount by which it will turn
void turnLeft() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  }
//can add a delay function to control the degree of turn
void stopMotors() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
