/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Slsed1000";
const char* password = "slsed010407";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign output variables to GPIO pins
const int player1plus = 14;
const int player1minus = 27;
const int player2plus = 12;
const int player2minus = 13;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

int pointsA = 0;
int pointsB = 0;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(2, OUTPUT);
  pinMode(player1plus, INPUT);
  pinMode(player1minus, INPUT);
  pinMode(player2plus, INPUT);
  pinMode(player2minus, INPUT);

  // Set outputs to LOW
  digitalWrite(2, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(2, HIGH);  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta http-equiv=\"refresh\" content=\"1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body {display: flex; margin: 0;}");
            client.println("div {width: 50%; height: 100vh; text-align: center; vertical-align: middle;}");
            client.println("p {position: sticky; margin: 0; font-size: 60vh; top:15%}");
            client.println("#pointsA {background-color: #455a64; color: #ff0000; font-family: sans-serif;}");
            client.println("#pointsB {background-color: #455a64; color: #0000ff; font-family: sans-serif;}");
            client.println("</style></head>");
            
            // Web Page Heading
            client.println("<body>");
            
            client.println("<div id=\"pointsA\"><p>"); 
            client.println(pointsA);
            client.println("</p></div>");
            client.println("<div id=\"pointsB\"><p>"); 
            client.println(pointsB);
            client.println("</p></div>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  }

  if (digitalRead(player1plus)){
    pointsA++;
  }
  if (digitalRead(player1minus)){
    pointsA -=1;
  }
  if (digitalRead(player2plus)){
    pointsB++; 
  }
  if (digitalRead(player2minus)){
    pointsB -=1;  
  }
  delay(400); 

}