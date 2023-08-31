#include <WiFiNINA.h>

// Defining the WiFi credentials
#define SECRET_SSID "MM"
#define SECRET_PASSWORD "1234567890"

// Storing WiFi credentials in variables
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

WiFiClient client;

// Defining the hostname for the IFTTT service
char HOST_NAME[] = "maker.ifttt.com";

// Defining the paths and query strings for different events
String PATH_NAME_LIGHT_DETECTION = "/trigger/Light_Detection/with/key/nswG5TNaqUPCB_Bv_WRM5oRMss2K0uz3yXGpZWlLbHq";
String queryString1 = "?value1= NO_Light_Present";
String queryString2 = "?value1= Light_present";

// Defining the analog pin for the LDR sensor
const int ldrPin = A0;

boolean lastLightState = false;

void setup() {
  // Connecting to WiFi using the specified credentials
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial); // Wait until Serial is ready

  // Attempt to connect to the web server on port 80
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection failed");
  }
}

void loop() {
  // Read the light level from the LDR sensor
  int lightLevel = analogRead(ldrPin);
  Serial.println(lightLevel);

  // Determine the current light state based on the light level
  boolean currentLightState = (lightLevel > 400);

  // Check if the light state has changed
  if (currentLightState != lastLightState) {
    lastLightState = currentLightState; // Update the last light state

    // Trigger the appropriate IFTTT event based on the light state
    if (currentLightState) {
      triggerIFTTTEvent1(PATH_NAME_LIGHT_DETECTION); // Light detected event
    } else {
      triggerIFTTTEvent2(PATH_NAME_LIGHT_DETECTION); // Light not detected event
    }

    delay(30000); // Wait for a specified interval before checking again
  }
}

void triggerIFTTTEvent1(String pathName) {
  sendIFTTTRequest(pathName, queryString1); // Send IFTTT request with query string 1
}

void triggerIFTTTEvent2(String pathName) {
  sendIFTTTRequest(pathName, queryString2); // Send IFTTT request with query string 2
}

void sendIFTTTRequest(String pathName, String queryString) {
  // Construct and send the HTTP GET request to the IFTTT service
  client.println("GET " + pathName + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println();

  // Read and print the response from the IFTTT service
  while (client.connected()) {
    if (client.available()) {
      char c = client.read(); // Read a character from the response
      Serial.print(c); // Print the response character
    }
  }

  // client.stop();
  Serial.println();
  Serial.println("Disconnected");
}
