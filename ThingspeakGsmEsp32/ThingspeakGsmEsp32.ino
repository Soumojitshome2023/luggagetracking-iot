#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

// ThingSpeak server
const char server[] = "api.thingspeak.com";
const int port = 80;

// Your ThingSpeak API Key
const String apiKey = "16FCSO4YK6LKRO3G"; // 👈 replace with your Write API Key

// GPRS settings
const char apn[] = "bsnlnet";
const char user[] = "";
const char pass[] = "";

// SIM800L connected to Serial1 (GPIO 16, 17)
#define MODEM_RX 16
#define MODEM_TX 17
HardwareSerial sim800(1);
TinyGsm modem(sim800);
TinyGsmClient client(modem);

void setup()
{
  Serial.begin(115200);
  delay(10);

  sim800.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  Serial.println("🔄 Restarting modem...");
  modem.restart();

  delay(1000);
  modem.init(); // optional but helpful

  Serial.print("🔎 SIM status: ");
  Serial.println(modem.getSimStatus());

  Serial.print("📶 Signal Quality: ");
  Serial.println(modem.getSignalQuality());

  Serial.print("🌐 Is Network Connected: ");
  Serial.println(modem.isNetworkConnected() ? "Yes" : "No");

  Serial.println("📡 Waiting for network...");
  if (!modem.waitForNetwork(30000))
  {
    Serial.println("❌ Network failed");
    while (true)
      ;
  }
  Serial.println("✅ Network connected");

  Serial.println("🌐 Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.println("❌ GPRS failed");
    while (true)
      ;
  }
  Serial.println("✅ GPRS connected");

  Serial.print("🌐 Is GPRS Connected: ");
  Serial.println(modem.isGprsConnected() ? "Yes" : "No");

  // Compose the HTTP GET request for ThingSpeak
  float value1 = 23.45; // Example value
  float value2 = 67.89; // Optional second value

  // String url = "/update?api_key=" + apiKey + "&field1=" + String(value1) + "&field2=" + String(value2);
  String url = "/update?api_key=16FCSO4YK6LKRO3G&field1=89&field2=92";

  Serial.println("📤 Sending data to ThingSpeak...");
  if (client.connect(server, port))
  {
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ESP32-SIM800L");
    client.println("Connection: close");
    client.println(); // Very important

    // Wait and read response
    unsigned long timeout = millis();
    while (client.connected() && millis() - timeout < 5000)
    {
      while (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
        timeout = millis();
      }
    }

    client.stop();
    Serial.println("✅ Data sent!");
  }
  else
  {
    Serial.println("❌ Connection to ThingSpeak failed.");
  }
}

void loop()
{
  // nothing here
}
