#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// Configuración de red Wi-Fi
const char* ssid = "";
const char* password = "";
// Configuración de la API
const char* apiURL = "https://app.saulgomezc.site/iot-curso-itp-db/backend/getValuesGiroscopio.php";
const char* serverURL = "https://app.saulgomezc.site/iot-curso-itp-db/backend/setStatusGiroscopio.php";
const char* servoURL = "https://app.saulgomezc.site/iot-curso-itp-db/backend/setStatusServo.php";
Adafruit_MPU6050 mpu;
//Variables de Servo
Servo servoX;  // Servo para statusX
Servo servoY;  // Servo para statusY
Servo servoZ;  // Servo para statusZ
int servoPinX = 18;
int servoPinY = 19;
int servoPinZ = 23;
int servoStatusX;
int servoStatusY;
int servoStatusZ;

void setup() {
  Serial.begin(115200);
   
  //Configuración para conectarse a Wifi******
  Serial.print("Conectando a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi");
  Serial.println("Direccion IP : ");
  Serial.println(WiFi.localIP());

  //CONGIG DEL SERVO
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servoX.setPeriodHertz(50);
  servoY.setPeriodHertz(50);
  servoZ.setPeriodHertz(50);
  servoX.attach(servoPinX, 500, 2000);
  servoY.attach(servoPinY, 500, 2000);
  servoZ.attach(servoPinZ, 500, 2000);

  // Inicialización del sensor MPU6050
  Serial.println("Inicializando el sensor MPU6050...");
  if (!mpu.begin()) {
    Serial.println("Error al encontrar el sensor MPU6050. Verifica la conexión.");
    while (1);
  }
  Serial.println("Sensor MPU6050 encontrado");
  Serial.println();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float ax, ay, az;
  String url = serverURL;
  if (validacion(a.acceleration.x, a.acceleration.y, a.acceleration.z, ax, ay, az)) { // Validación de restricción a los datos
    url += "?x=" + String(a.acceleration.x) + "&y=" + String(a.acceleration.y) + "&z=" + String(a.acceleration.z);
    ax = a.acceleration.x;
    ay = a.acceleration.y;
    az = a.acceleration.z;
  }
  getAPIResponse(url); // Realizar la solicitud HTTP GET al servidor
  servoJSON();
  delay(100); // Esperar 1 segundo antes de realizar la siguiente solicitud
}

String getAPIResponse(const String& url) {
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  String payload = "";
  if (httpCode == HTTP_CODE_OK) {
    payload = http.getString();
  } else {
    Serial.print("Error en la solicitud HTTP: ");
    Serial.println(httpCode);
  }
  http.end();
  return payload;
}

void servoJSON(){
  String jsonPayload = getAPIResponse(apiURL);
  DynamicJsonDocument jsonDoc(1024);  // Analizar la respuesta JSON
  deserializeJson(jsonDoc, jsonPayload);
  JsonObject obj = jsonDoc[0]; // Obtener el primer objeto del JSON
  servoStatusX  = obj["statusX"]; // Obtener el valor de status
  servoStatusY  = obj["statusY"];
  servoStatusZ  = obj["statusZ"];
  servoStatusX = map(servoStatusX, -10, 10, 0, 180); // Limitar los valores en el rango de 0 a 180
  servoStatusY = map(servoStatusY, -10, 10, 0, 180);
  servoStatusZ = map(servoStatusZ, -10, 10, 0, 180);
  servoX.write(servoStatusX);
  servoY.write(servoStatusY);
  servoZ.write(servoStatusZ);
  Serial.printf("statusX: %d\nstatusY: %d\nstatusZ: %d\n\n", servoStatusX, servoStatusY, servoStatusZ);
  String url = servoURL;//Envía a la BD el status de los servomotores
  url += "?x=" + String(servoStatusX) + "&y=" + String(servoStatusY) + "&z=" + String(servoStatusZ);
  getAPIResponse(url);
}

bool validacion(float x, float y, float z, float px, float py, float pz){
  float dif = 0.3;
  float pxs = px + dif;
  float pxi = px - dif;
  float pys = py + dif;
  float pyi = py - dif;
  float pzs = pz + dif;
  float pzi = pz - dif;
  if (x <= pxi || x >=pxs || y <= pyi || y >=pys || z <= pzi || z >=pzs){
    return true;
  }else{
    return false;
  }
}
