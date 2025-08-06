#include <HardwareSerial.h>

#define MODEM_RX 16
#define MODEM_TX 17

HardwareSerial sim800(2); // Use UART2

void setup() {
  Serial.begin(115200);
  sim800.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(2000);

  Serial.println("SIM800L Raw Data Monitor");
  sim800.println("AT");  // Test command
}

void loop() {
  // Print all raw incoming bytes from SIM800L
  while (sim800.available()) {
    uint8_t byteRead = sim800.read();
    
    // Print raw byte as ASCII (best for normal debug)
    Serial.write(byteRead);

    // Optional: Print in HEX format for debugging gibberish
    // Serial.print("0x");
    // Serial.print(byteRead, HEX);
    // Serial.print(" ");
  }
}
