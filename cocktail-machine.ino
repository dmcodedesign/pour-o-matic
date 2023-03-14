#include <WiFi.h>
#include <FirebaseESP32.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define FIREBASE_HOST "SEU_PROJETO.firebaseio.com"
#define FIREBASE_AUTH "SEU_TOKEN_DE_AUTORIZACAO"

AsyncWebServer server(80);

// Configuração do WiFi
const char* ssid = "";
const char* password = "";

// Configuração do Firebase
const char* firebaseHost = "";
const char* firebaseAuth = "";

// Configuração das bombas
const int bomba1Pin = 2;
const int bomba2Pin = 4;
const int bomba3Pin = 5;
const int bomba4Pin = 12;
const int bomba5Pin = 13;
const int bomba6Pin = 14;
const int bomba7Pin = 15;
const int bomba8Pin = 16;

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);

  // Inicializa as bombas
  pinMode(bomba1Pin, OUTPUT);
  pinMode(bomba2Pin, OUTPUT);
  pinMode(bomba3Pin, OUTPUT);
  pinMode(bomba4Pin, OUTPUT);
  pinMode(bomba5Pin, OUTPUT);
  pinMode(bomba6Pin, OUTPUT);
  pinMode(bomba7Pin, OUTPUT);
  pinMode(bomba8Pin, OUTPUT);

  // Configura a interface web para a conexão à Internet
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<form method='POST' action='/wifi'><label>SSID: <input type='text' name='ssid'></label><br><label>Password: <input type='password' name='password'></label><br><input type='submit' value='Submit'></form>");
  });
  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
    String ssid;
    String password;
    if (request->hasParam("ssid") && request->hasParam("password")) {
      ssid = request->getParam("ssid")->value();
      password = request->getParam("password")->value();
      Serial.print("Conectando ao WiFi...");
      WiFi.begin(ssid.c_str(), password.c_str());
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("Conectado!");
      request->redirect("/firebase");
    } else {
      request->send(400, "text/plain", "Parametros faltando");
    }
  });

  // Configura a interface web para a conexão ao Firebase
  server.on("/firebase", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<form method='POST' action='/firebase'><label>Host: <input type='text' name='host'></label><br><label>Auth: <input type='text' name='auth'></label><br><input type='submit' value='Submit'></form>");
  });
  server.on("/firebase", HTTP_POST, [](AsyncWebServerRequest *request) {
    String host;
    String auth;
    if (request->hasParam("host") && request->hasParam("auth")) {
      host = request->getParam("host")->value();
      auth = request->getParam("auth")->value();
      Firebase.begin(host.c_str(), auth.c_str());
      request->redirect("/");
    } else {
      request->send(400, "text/plain", "Parametros faltando");
    }

    // Inicializa o servidor HTTP
      server.begin();
    }

    void loop() {
      if (WiFi.status() != WL_CONNECTED) {
        return;
      }

      // Lê as variáveis do Firebase
      Firebase.getBool(firebaseData, "/bombas/bomba1");
      bool bomba1 = firebaseData.boolData();
      Firebase.getBool(firebaseData, "/bombas/bomba2");
      bool bomba2 = firebaseData.boolData();
      Firebase.getBool(firebaseData, "/bombas/bomba3");
      bool bomba3 = firebaseData.boolData();
      Firebase.getBool(firebaseData, "/bombas/bomba4");
      bool bomba4 = firebaseData.boolData();
      Firebase.getBool(firebaseData, "/bombas/bomba5");
      bool bomba5 = firebaseData.boolData();
      Firebase.getBool(firebaseData, "/bombas/bomba6");
      bool bomba6 = firebaseData.boolData();
      Firebase.getBool(firebaseData, "/bombas/bomba7");
      bool bomba7 = firebaseData.boolData();
      Firebase.getBool(firebaseData, "/bombas/bomba8");
      bool bomba8 = firebaseData.boolData();

      // Atualiza o estado das bombas
      digitalWrite(bomba1Pin, bomba1 ? HIGH : LOW);
      digitalWrite(bomba2Pin, bomba2 ? HIGH : LOW);
      digitalWrite(bomba3Pin, bomba3 ? HIGH : LOW);
      digitalWrite(bomba4Pin, bomba4 ? HIGH : LOW);
      digitalWrite(bomba5Pin, bomba5 ? HIGH : LOW);
      digitalWrite(bomba6Pin, bomba6 ? HIGH : LOW);
      digitalWrite(bomba7Pin, bomba7 ? HIGH : LOW);
      digitalWrite(bomba8Pin, bomba8 ? HIGH : LOW);

      // Aguarda um segundo antes de ler novamente as variáveis do Firebase
      delay(1000);
    }
