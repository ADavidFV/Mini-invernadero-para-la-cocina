#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>  // Librería para comunicación serial por software

// Incluimos librería
#include <DHT.h>
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
#define DHTPIN2 7
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
// Inicializamos el sensor DHT11
DHT dht2(DHTPIN2, DHTTYPE);
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
//-----------humedad
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//------------pantalla
SoftwareSerial BTserial(1, 0);  // RX, TX (Pines para la comunicación Bluetooth)
// BT
int ledPin = 3;      // El número del pin al que está conectado el LED
int ventilador = 5;  // El número del pin al que está conectado el ventilador
int bomba = 9;

void setup() {
  // Inicializamos comunicación serie
  Serial.begin(9600);
  display.begin(9600);
  BTserial.begin(9600);  // Inicialización de la comunicación Bluetooth
  // Comenzamos el sensor DHT
  dht.begin();
  dht2.begin();
  pinMode(ledPin, OUTPUT);      // Configura el pin como salida
  pinMode(ventilador, OUTPUT);  // Configura el pin como salida
  pinMode(bomba, OUTPUT);       // Configura el pin como salida


  // .........config de pantalla
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    display.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 10);
  display.println("EcoLife");
  display.display();
  delay(500);
}


void loop() {

  senTemperatura();
  // luzUv();
}

void senTemperatura() {
  // Esperamos 5 segundos entre medidas
  delay(1000 / 10);
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  float h2 = dht2.readHumidity();
  // Leemos la t en grados centígrados (por defecto)
  float t = dht.readTemperature();
  float t2 = dht2.readTemperature();
  // Leemos la t en grados Fahreheit
  float f = dht.readTemperature(true);
  float f2 = dht2.readTemperature(true);
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    display.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
  // Calcular el índice de calor en Fahreheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Humedad: ");
  display.print(h2);
  //Serial.println(h);
  // \t: Este es un carácter de tabulación,
  // que agrega espacios en blanco para desplazar el texto
  display.println(" % ");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  // display.setCursor(50, 0);
  display.print("Temperatura: ");
  display.print(t);
  //Serial.println(t);
  display.println(" C ");
  // display.print(f);
  // display.println(" *F\t");
  display.println("Indice de calor: ");
  display.print(hic);
  //Serial.println(hic);
  display.println(" C ");
  // display.print(hif);
  // display.println("*F");
  digitalWrite(ventilador, LOW);
  digitalWrite(bomba, LOW);
  //------------------------------

  while (hic > 28) {
    // Esperamos 5 segundos entre medidas
    delay(1000 / 10);
    // Leemos la humedad relativa
    float h = dht.readHumidity();
    // Leemos la t en grados centígrados (por defecto)
    float t = dht.readTemperature();
    // Calcular el índice de calor en grados centígrados
    float hic = dht.computeHeatIndex(t, h, false);
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);

    digitalWrite(ventilador, HIGH);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("--EMERGENCIA--");
    // display.print(h);
    // \t: Este es un carácter de tabulación,
    // que agrega espacios en blanco para desplazar el texto
    // display.println(" % ");
    // display.setCursor(50, 0);
    display.print("Temperatura: ");
    display.print(t);
    display.println(" C ");
    // display.print(f);
    // display.println(" *F\t");
    display.println("Indice de calor: ");
    display.print(hic);
    display.println(" C ");
    display.display();

    digitalWrite(bomba, LOW);

    if (hic < 28) {
      break;
    }
  }
  while (h2 < 50) {
    // Esperamos 5 segundos entre medidas
    delay(1000 / 10);
    // Leemos la humedad relativa
    float h2 = dht2.readHumidity();
    // Leemos la t en grados centígrados (por defecto)
    float t2 = dht2.readTemperature();
    // Calcular el índice de calor en grados centígrados
    float hic2 = dht2.computeHeatIndex(t2, h2, false);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("--Riego ATM--");
    // display.print(h);
    // \t: Este es un carácter de tabulación,
    // que agrega espacios en blanco para desplazar el texto
    // display.println(" % ");
    // display.setCursor(50, 0);
    display.print("Humedad: ");
    display.print(h2);
    //Serial.println(h);
    display.println(" % ");
    // display.print(f);
    // display.println(" *F\t");
    display.println("Temperatura: ");
    display.print(t2);
    display.println(" C ");
    display.display();

    digitalWrite(bomba, HIGH);

    if (h2 > 60) {
      break;
    }
  }

  if (BTserial.available()) {
    //Serial.println("BT Disponible");
    char command = BTserial.read();

    if (command == '1') {
      digitalWrite(bomba, HIGH);  // Enciende el LED cuando recibe el comando '1'
      Serial.println("LED Encendido");
    } else if (command == '0') {
      digitalWrite(bomba, LOW);  // Apaga el LED cuando recibe el comando '0'
      Serial.println("LED Apagado");
    }
  }

  display.display();
}
