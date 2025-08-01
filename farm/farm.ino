#include <WiFi.h>
#include <WebServer.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

#define NUM_SENSORES 5
#define RELAY_AGUA 26
#define RELAY_LUZ 25
#define RELAY_AQUECEDOR 33
#define RELAY_VENTILACAO 32
#define NUM_LEITURAS 10
#define INTERVALO_COLETA 180000

const int soilPins[NUM_SENSORES] = {34, 35, 32, 33, 25};

const char* ssid = "BLOQUEADA MASTER";
const char* password = "Jatefalo!";

IPAddress local_IP(192, 168, 1, 105);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);
BLEScan* scanner;

float ruuviTemp = NAN;
float ruuviHum = NAN;
float ruuviPress = NAN;
const char* ruuviMAC = "CE:77:73:81:BD:D5";

unsigned long ultimaColeta = 0;

bool estadoAgua = false, estadoLuz = false, estadoAquecedor = false, estadoVentilacao = false;

int getSoilPercent(int value) {
  return constrain(map(value, 4095, 0, 0, 100), 0, 100);
}

int leituraBrutaMedia(int pin) {
  int soma = 0;
  for (int i = 0; i < NUM_LEITURAS; i++) {
    int leitura = analogRead(pin);
    soma += leitura;
    Serial.printf("Leitura no pino %d: %d\n", pin, leitura);
    delay(10);
  }
  return soma / NUM_LEITURAS;
}

void atualizarReleManual() {
  digitalWrite(RELAY_AGUA, estadoAgua);
  digitalWrite(RELAY_LUZ, estadoLuz);
  digitalWrite(RELAY_AQUECEDOR, estadoAquecedor);
  digitalWrite(RELAY_VENTILACAO, estadoVentilacao);
}

void handleToggle() {
  if (server.hasArg("agua")) estadoAgua = server.arg("agua") == "on";
  if (server.hasArg("luz")) estadoLuz = server.arg("luz") == "on";
  if (server.hasArg("aquecedor")) estadoAquecedor = server.arg("aquecedor") == "on";
  if (server.hasArg("ventilacao")) estadoVentilacao = server.arg("ventilacao") == "on";
  atualizarReleManual();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Projeto Calvin</title></head><body style='font-family:sans-serif;'>";
  html += "<h3>🌱 Umidade do Solo</h3>";
  for (int s = 0; s < NUM_SENSORES; s++) {
    int leituraMedia = leituraBrutaMedia(soilPins[s]);
    int umidade = getSoilPercent(leituraMedia);
    html += "<p>Sensor " + String(s + 1) + ": " + String(umidade) + "%</p>";
  }

  html += "<h3>🌡️ Temperatura Ambiente</h3><p>";
  html += isnan(ruuviTemp) ? "Sensor desconectado ou leitura inválida" : String(ruuviTemp, 2) + "°C";
  html += "</p>";

  html += "<h3>💧 Umidade Relativa</h3><p>";
  html += isnan(ruuviHum) ? "Sensor desconectado ou leitura inválida" : String(ruuviHum, 2) + "%";
  html += "</p>";

  html += "<h3>🌬️ Pressão Atmosférica</h3><p>";
  html += isnan(ruuviPress) ? "Sensor desconectado ou leitura inválida" : String(ruuviPress, 2) + " hPa";
  html += "</p>";

  html += "<form action='/toggle'>";
  html += "<button name='agua' value='" + String(estadoAgua ? "off" : "on") + "'>Água</button>";
  html += "<button name='luz' value='" + String(estadoLuz ? "off" : "on") + "'>Luz</button>";
  html += "<button name='aquecedor' value='" + String(estadoAquecedor ? "off" : "on") + "'>Aquecedor</button>";
  html += "<button name='ventilacao' value='" + String(estadoVentilacao ? "off" : "on") + "'>Ventilação</button>";
  html += "</form></body></html>";

  server.send(200, "text/html", html);
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice d) {
    String macDetectado = d.getAddress().toString().c_str();
    macDetectado.toUpperCase();
    String macEsperado = String(ruuviMAC);
    macEsperado.toUpperCase();

    Serial.print("Dispositivo encontrado: ");
    Serial.println(macDetectado);

    if (macDetectado == macEsperado && d.haveManufacturerData()) {
      std::string md = std::string(d.getManufacturerData().c_str(), d.getManufacturerData().length());
      const uint8_t* data = (const uint8_t*)md.data();
      if (md.length() >= 26 && data[0] == 0x99 && data[1] == 0x04 && data[2] == 0x05) {
        int16_t temp_raw = (data[3] << 8) | data[4];
        uint16_t hum_raw = (data[5] << 8) | data[6];
        uint16_t press_raw = (data[7] << 8) | data[8];

        ruuviTemp = temp_raw * 0.005f;
        ruuviHum = hum_raw * 0.0025f;
        ruuviPress = (press_raw + 50000) / 100.0f;

        Serial.printf("Ruuvi CORRIGIDO: temp=%.2f °C, hum=%.2f%%, press=%.2f hPa\n", ruuviTemp, ruuviHum, ruuviPress);
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  pinMode(RELAY_AGUA, OUTPUT);
  pinMode(RELAY_LUZ, OUTPUT);
  pinMode(RELAY_AQUECEDOR, OUTPUT);
  pinMode(RELAY_VENTILACAO, OUTPUT);

  atualizarReleManual();

  BLEDevice::init("");
  scanner = BLEDevice::getScan();
  scanner->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  scanner->setActiveScan(true);

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
}

void loop() {
  server.handleClient();

  static unsigned long ultimaScan = 0;
  if (millis() - ultimaScan > 15000) {
    scanner->start(1, false);
    ultimaScan = millis();
  }
}
