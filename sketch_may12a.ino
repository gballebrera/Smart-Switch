//Incluyes estas dos librerias
#include <WiFi.h>
#include <TuyaSmart.h>

// Aquí pones las credenciales de tu wifi
const char* ssid = "tuSSID";
const char* password = "tuContrasena";

//He puesto credenciales de Tuya Smart Home, te creas una cuenta y pones las credenciales aquí
#define DEVICE_ID "tuDeviceID"
#define DEVICE_KEY "tuDeviceKey"
#define DEVICE_TYPE "tuDeviceType"

// Pin donde está conectado el relé
const int relayPin = 23;

// Pin donde está conectado el interruptor
const int switchPin = 22;

// Estado del relé
bool ledState = false;

// Lo de la librería TuyaSmart
TuyaSmart tuya(DEVICE_ID, DEVICE_KEY, DEVICE_TYPE);

// Aquí se cambia el estado del relé
void changeLedState(bool state) {
  digitalWrite(relayPin, state ? HIGH : LOW);
  ledState = state;
}

// Aquí recibo los datos de la aplicación Tuya Smart Life
void onTuyaCommand(const String& value) {
  if (value == "true") {
    changeLedState(true);
  } else if (value == "false") {
    changeLedState(false);
  }
}
//Esto solo lo hace una vez
void setup() {
  // Configuro el pin del relé como salida
  pinMode(relayPin, OUTPUT);

  // Configuro el pin del interruptor como entrada con pull-up interno
  pinMode(switchPin, INPUT_PULLUP);

  // Me conecto al WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Inicio la comunicación con Tuya Smart Life
  tuya.begin(onTuyaCommand);
}
//Esto es lo que va a hacer todo el rato en bucle
void loop() {
  // Se verifica el estado del interruptor y se cambia el estado del relé
  bool switchState = digitalRead(switchPin);
  if (switchState == LOW && ledState == false) {
    changeLedState(true);
    tuya.sendDPData("1", "bool", "true");
  } else if (switchState == LOW && ledState == true) {
    changeLedState(false);
    tuya.sendDPData("1", "bool", "false");
  }

  // Actualizo la conexión con Tuya Smart Life todo el rato
  tuya.update();
}
