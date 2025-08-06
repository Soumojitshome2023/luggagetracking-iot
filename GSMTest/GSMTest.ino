#include <HardwareSerial.h>

#define MODEM_RX 16  // SIM800L TX ➝ ESP32 RX
#define MODEM_TX 17  // SIM800L RX ➝ ESP32 TX (via voltage divider)
#define LED_PIN 4    // LED to blink on incoming call

HardwareSerial sim800(2);  // UART2

bool callIncoming = false;
unsigned long lastBlinkTime = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  sim800.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(1000);
  Serial.println("SIM800L Call Detection Started");

  sendAT("AT");        // Basic check
  sendAT("AT+CLIP=1"); // Enable caller ID (important to detect number)
}

void loop() {
  // Listen from SIM800L
  if (sim800.available()) {
    String res = sim800.readStringUntil('\n');
    res.trim();  // Remove \r\n

    if (res == "RING") {
      Serial.println("📞 Incoming Call Detected");
      callIncoming = true;
    }

    if (res.startsWith("+CLIP:")) {
      Serial.println("Caller Info: " + res);
      callIncoming = true;
    }

    if (res.indexOf("NO CARRIER") >= 0) {
      Serial.println("Call Ended");
      callIncoming = false;
      digitalWrite(LED_PIN, LOW);
    }
  }

  // Blink LED while callIncoming is true
  if (callIncoming) {
    unsigned long now = millis();
    if (now - lastBlinkTime >= 500) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

// Helper to send AT command
void sendAT(String cmd) {
  Serial.println("Sending: " + cmd);
  sim800.println(cmd);
  delay(1000);
  while (sim800.available()) {
    String r = sim800.readStringUntil('\n');
    r.trim();
    if (r.length() > 0)
      Serial.println("Reply: " + r);
  }
}
