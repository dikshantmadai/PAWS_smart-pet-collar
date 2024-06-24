#include <HTTPClient.h>
#include <WiFi.h>

int x = 0;
const char* ssid = "SSRC-Sunway";
const char* password = "sunway@123";
const int pulsePin = 2;  // GPIO pin connected to the pulse input of HW-827
volatile unsigned long pulseCount = 0;  // Variable to store the pulse count
unsigned long lastPulseTime = 0;  // Variable to store the last pulse time

String serverName = "https://api.thingspeak.com/update?api_key=ZI541W45EPJMTG94&field1=0";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void IRAM_ATTR handlePulse() {
  unsigned long currentTime = micros();
  if (currentTime - lastPulseTime > 100) {  // Ignore noise pulses shorter than 100 microseconds
    pulseCount++;
  }
  lastPulseTime = currentTime;
}

void setup() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.begin(115200);

  pinMode(pulsePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pulsePin), handlePulse, RISING);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      delay(1000);  // Wait for 1 second
      detachInterrupt(digitalPinToInterrupt(pulsePin));

      unsigned long pulses = pulseCount;
      pulseCount = 0;

      attachInterrupt(digitalPinToInterrupt(pulsePin), handlePulse, RISING);

      Serial.print("Pulses per second: ");
      Serial.println(pulses);
      sendData(pulses);
    }
    else {
      Serial.println("WiFi not connected");
    }
    lastTime = millis();
  }
}

void sendData(unsigned long dta) {
  HTTPClient http;
  Serial.println(dta);
  String url = serverName + "&field1=" + String(dta);
  http.begin(url.c_str());
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("Response code is: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("ERROR CODE: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
