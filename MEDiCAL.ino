#include <WiFiS3.h>
#include <Servo.h>
const char ssid[] = "Christian's iPhone";
const char pass[] = "christian";

WiFiServer server(80);
Servo myServo;
const int vibPin = A5;

void setup() {
  pinMode(vibPin, INPUT);
  Serial.begin(9600);
  delay(1000);

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();   

  if (client) {                            
    Serial.println();
    Serial.println("new client");           
    String currentLine = "";               
    while (client.connected()) {         
      if (client.available()) {          
        char c = client.read();             
        // Serial.write(c);                 
        if (c == '\n') { //https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples/#simple-webserver
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            break;
          } 
          else {
            currentLine = "";
          }
        } 
        else if (c != '\r') {
          currentLine += c; 
        }

        //BUCKET CHECK
        int len = currentLine.length() - 1;
        if (currentLine.substring(0, len).endsWith("GET /")) {
          dispense(currentLine.charAt(len) - '0');
        }
        //END BUCKET CHECK
      }
      
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void dispense(int motorNum) {
  int tol;
  int motorSpeed;
  if (motorNum == 1){
    motorNum = 5;
    motorSpeed = 88;
    tol = 112;
  }
  else if (motorNum == 2){
    motorNum = 6;
    motorSpeed = 85;
    tol = 95;
  }
  else if (motorNum == 3){
    motorNum = 9;
    motorSpeed = 85;
    tol = 20;
  }
  else if (motorNum == 4){
    motorNum = 10;
    motorSpeed = 88;
    tol = 112;
  }
  else if (motorNum == 5){
    motorNum = 11;
    motorSpeed = 88;
    tol = 112;
  }
  // tol = 30; //ambient vibrations
  // motorNum = 10;
  myServo.attach(motorNum);
  myServo.write(motorSpeed);
  int state = analogRead(vibPin);
  
  while(state < tol){
    state = analogRead(vibPin);
    // Serial.println(state);
    delay(100);
  }
  myServo.detach();
  // Serial.print("Motor ");
  // Serial.print(motorNum);
  // Serial.print(" was hit\n");
}
