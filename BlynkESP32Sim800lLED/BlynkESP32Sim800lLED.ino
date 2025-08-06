// ------------------------------
// ✅ Blynk Credentials
// ------------------------------
#define BLYNK_TEMPLATE_ID "TMPL3vvuCh4AM"
#define BLYNK_TEMPLATE_NAME "Test"
#define BLYNK_AUTH_TOKEN "cUyc1Qh-H9bgtD3oTXqMzxsz425Yo5t1"

// ------------------------------
// ✅ TinyGSM & Blynk Libraries
// ------------------------------
#define TINY_GSM_MODEM_SIM800
#define BLYNK_PRINT Serial

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

// ------------------------------
// ✅ SIM800L UART Setup
// ------------------------------
#define MODEM_RX 16
#define MODEM_TX 17
#define MODEM_BAUD 9600

HardwareSerial SerialAT(1);  // Use UART1 for modem
TinyGsm modem(SerialAT);     // Modem object
TinyGsmClient client(modem); // GSM client for Blynk

// ------------------------------
// ✅ BSNL APN Settings
// ------------------------------
const char apn[]  = "bsnlnet";
const char user[] = "";
const char pass[] = "";

// ------------------------------
// ✅ ESP32 Built-in LED
// ------------------------------
const int ledPin = 2;

// ------------------------------
// ✅ Blynk LED Control
// ------------------------------
BLYNK_WRITE(V0) {
  int pinValue = param.asInt(); // Read value from app
  digitalWrite(ledPin, pinValue);
  Serial.print("LED: ");
  Serial.println(pinValue ? "ON" : "OFF");
}

// ------------------------------
// ✅ Setup
// ------------------------------
void setup() {
  // Serial Monitor
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("🚀 Starting Blynk via SIM800L...");

  // Set LED pin as output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Start UART for SIM800L
  SerialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart modem
  Serial.println("📶 Initializing modem...");
  modem.restart();

  // Optional: Show modem info
  String modemInfo = modem.getModemInfo();
  Serial.print("📟 Modem Info: ");
  Serial.println(modemInfo);

  // Connect to Blynk over GSM
  Serial.println("🌐 Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, modem, apn, user, pass);
}

// ------------------------------
// ✅ Loop
// ------------------------------
void loop() {
  Blynk.run();
}
