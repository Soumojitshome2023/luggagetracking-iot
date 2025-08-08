#include <WiFi.h>

// Change to your mobile hotspot credentials
const char* ssid = "Soumojit Shome";
const char* password = "soumojit9062300@";

// Buzzer pin (D4 on ESP32)
#define BUZZER_PIN 4

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // buzzer OFF initially

  WiFi.begin(ssid, password);
  Serial.println("Connecting to hotspot...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to hotspot!");
}

void loop() {
  long rssi = WiFi.RSSI();  // Signal strength in dBm

  // Path-loss model formula (2.0~4.0 is environment factor)
  float distance = pow(10.0, ((-69 - rssi) / (10 * 2.0)));

  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.print(" dBm | Approx Distance: ");
  Serial.print(distance, 4);  // show 4 decimal places
  Serial.println(" meters");

  // If distance less than 0.02 meters (~2 cm), start buzzer
  if (distance > 1) {
    digitalWrite(BUZZER_PIN, HIGH);  // buzzer ON
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // buzzer OFF
  }


  delay(700);
}
