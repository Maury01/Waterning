#include <WiFi.h>
#include <CTBot.h>

// Configuración de WiFi
const char* ssid = "CLARO_12EAD8";
const char* password = "DCADF394DF";
//const char* ssid = "Waterning";
//const char* password = "1235678";

// Configuración de Telegram
String token = "6235773808:AAHOz8x1hp9ffrBWx3gqa1BRNxuqtkPByH8";
CTBot myBot;
//int64_t chatId = 1678297407;
int64_t chatId = 1557029815;

//Config sensor
const int sensorPinAnalog = 34;
bool mensajeEnviado = false;
float porcentajeAnterior = 0;

/*int pinPulsador = 25;
int pinLed = 27;
bool ledEncendido = false;
int sensor = 12;*/

void setup() {
  Serial.begin(115200);

    /*pinMode(pinLed, OUTPUT);
    pinMode(sensor, OUTPUT);
    pinMode(pinPulsador, INPUT);
    digitalWrite(sensor, LOW);*/

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  myBot.setTelegramToken(token);
  while (!myBot.testConnection()) {
    delay(1000);
    Serial.println("Conectando a Telegram...");
  }
  Serial.println("Conectado a Telegram");
  myBot.sendMessage(chatId, "Conectado");
}

void loop() {
  int humedadAnalogica = analogRead(sensorPinAnalog);
  Serial.println(humedadAnalogica);
  String mensaje;
  
  //if (ledEncendido == true) {
    Calc_Humedad(mensaje);
    //delay(1000);
  //}
  
  
  if (!mensaje.isEmpty()) {
     myBot.sendMessage(chatId, mensaje);
    mensajeEnviado = true;
  }

  TBMessage msg;
  if (CTBotMessageText == myBot.getNewMessage(msg)) {
    if(msg.text.equalsIgnoreCase("/start")){
     myBot.sendMessage(chatId, "Bienvenido al servicio de Waterning");
    }
    if (msg.text.equalsIgnoreCase("Humedad")) {   
      float porcentajeHumedad = map(humedadAnalogica, 1309, 4095, 100, 0);
      mensaje = "La humedad de la planta es de: " + String(porcentajeHumedad) + "%";
      myBot.sendMessage(chatId, mensaje);
    }
    
  }
  delay(5000);
  //Switch();
   
}

void Calc_Humedad(String &mensaje) {
  int humedadAnalogica = analogRead(sensorPinAnalog);
  float porcentajeHumedad = map(humedadAnalogica, 1309, 4095, 100, 0);

  if (porcentajeHumedad != porcentajeAnterior) {
    if(porcentajeHumedad > 50){
      mensaje = "Humedad al " + String(porcentajeHumedad) + "% - No tengo sed";
    } else  if(porcentajeHumedad <= 50 && porcentajeHumedad > 30){
      mensaje = "Humedad al " + String(porcentajeHumedad) + "% - Hora de regar";
    } else if(porcentajeHumedad <= 30 && porcentajeHumedad > 10){
      mensaje = "Humedad al " + String(porcentajeHumedad) + "% - Tengo sed :,(";
    } else  if(porcentajeHumedad <= 10){
      mensaje = "Humedad al " + String(porcentajeHumedad) + "% - X_X";
    } 
    porcentajeAnterior = porcentajeHumedad;
  } else {
    mensaje = "";  // Si no se cumple ninguna condición, la cadena estará vacía
  }
}

/*void Switch(){
  if (digitalRead(pinPulsador) == HIGH) {
    ledEncendido = !ledEncendido;
    digitalWrite(pinLed, ledEncendido);
    digitalWrite(sensor, ledEncendido);
    if(ledEncendido == false){
      myBot.sendMessage(chatId, "Dispositivo Apagado");
    } else if(ledEncendido == true){
     myBot.sendMessage(chatId, "Dispositivo Encendido");
    }
    while (digitalRead(pinPulsador) == HIGH) {
      delay(1000);
    }
  }  
}*/

