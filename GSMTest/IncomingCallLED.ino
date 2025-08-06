#include <HardwareSerial.h>

#define MODEM_RX 16  // SIM800L TX ➝ ESP32 RX
#define MODEM_TX 17  // SIM800L RX ➝ ESP32 TX (via voltage divider)
#define LED_PIN 2    // Built-in LED on most ESP32 boards (change if needed)

HardwareSerial sim800(2);  // Use UART2

bool callIncoming = false;
unsigned long lastBlinkTime = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  sim800.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(2000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("SIM800L Call Detection Started");

  sendAT("AT");         // Check communication
  sendAT("ATE0");       // Disable echo
  sendAT("AT+CLIP=1");  // Enable caller ID (needed to detect number)
}

void loop() {
  // Read raw serial data from SIM800L
  while (sim800.available()) {
    String res = sim800.readStringUntil('\n');
    res.trim();

    // Clean out non-printable characters
    String cleanedRes = "";
    for (int i = 0; i < res.length(); i++) {
      char c = res[i];
      if (isPrintable(c)) {
        cleanedRes += c;
      }
    }

    Serial.println("My Res: " + cleanedRes);


    if (cleanedRes == "RING!" || cleanedRes == "RING") {
      Serial.println("📞 Incoming Call Detected");
      callIncoming = true;
    }

  }

  // Blink LED while call is incoming
  if (callIncoming) {
    unsigned long now = millis();
    if (now - lastBlinkTime >= 500) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  } else {
    digitalWrite(LED_PIN, LOW);  // Keep LED off if no call
  }
}

// Helper: send AT command and print reply
void sendAT(String cmd) {
  Serial.println("Sending: " + cmd);
  sim800.println(cmd);
  delay(300);
  unsigned long timeout = millis() + 2000;

  while (millis() < timeout) {
    if (sim800.available()) {
      String res = sim800.readStringUntil('\n');
      res.trim();
      if (res.length() > 0) {
        Serial.println("Reply: " + res);
      }
    }
  }
}
