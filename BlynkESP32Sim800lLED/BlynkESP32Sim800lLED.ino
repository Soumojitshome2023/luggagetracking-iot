// ----------------------------------
// ✅ Required Libraries and Settings
// ----------------------------------
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3vvuCh4AM"
#define BLYNK_TEMPLATE_NAME "Test"
#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

// ----------------------------------
// ✅ Blynk & SIM Credentials
// ----------------------------------

char auth[] = "cUyc1Qh-H9bgtD3oTXqMzxsz425Yo5t1";  // Your Blynk auth token
char apn[]  = "bsnlnet";  // For BSNL SIM
char user[] = "";
char pass[] = "";

// ----------------------------------
// ✅ SIM800L Serial Connection Pins
// ----------------------------------
// SIM800L RX → ESP32 TX (e.g., GPIO17)
// SIM800L TX → ESP32 RX (e.g., GPIO16)
#define MODEM_TX 17
#define MODEM_RX 16
#define MODEM_BAUD 9600

HardwareSerial sim800(1);         // UART1
TinyGsm modem(sim800);            // GSM Modem
TinyGsmClient client(modem);      // GSM Client for Blynk

// ----------------------------------
// ✅ LED Pin (Built-in)
const int ledPin = 2;

// ----------------------------------
// ✅ Blynk Virtual Pin to control LED
BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(ledPin, value);
  Serial.println(value ? "🔆 LED ON" : "🌑 LED OFF");
}

// ----------------------------------
// ✅ Setup Function
void setup() {
  // Start serial
  Serial.begin(115200);
  delay(100);
  Serial.println("\n🚀 Starting Blynk via SIM800L...");

  // LED setup
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Begin SIM800L Serial
  sim800.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart modem
  Serial.println("📶 Restarting modem...");
  modem.restart();
  delay(1000);

  // Optional: Print modem info
  Serial.print("📟 Modem Info: ");
  Serial.println(modem.getModemInfo());

  // Start Blynk over GSM
  Serial.println("🌐 Connecting to Blynk...");
  Blynk.begin(auth, modem, apn, user, pass);
}

// ----------------------------------
// ✅ Loop Function
void loop() {
  Blynk.run();
}
