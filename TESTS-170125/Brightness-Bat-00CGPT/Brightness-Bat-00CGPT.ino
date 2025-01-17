
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Requerido para Adafruit Trinket 16 MHz
#endif

// Configuración de NeoPixel
#define LED_PIN 9
#define LED_COUNT 398
#define BRIGHTNESS 80  // Brillo inicial (máximo = 255)
float distance;
// Configuración del sensor ultrasónico
const int trigPin = 6;
const int echoPin = 5;

// Configuración de suavizado
const int numReadings = 20;
const int MAX_DISTANCE = 300;  // Distancia máxima en cm para mapear

// Variables globales
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
int readings[numReadings] = {0};
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  // Inicialización de Serial y pines
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicialización de la tira LED
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);

  // Inicializar las lecturas a 0
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);  // Solo para Trinket 5V 16 MHz
#endif
}

void loop() {
  // Obtener la distancia del sensor ultrasónico
 distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  // Actualizar el promedio suavizado
  average = getSmoothedDistance(distance);
  Serial.print("Average: ");
  Serial.println(average);

  // Mapear la distancia a un valor de brillo
  int brightness = map(constrain(average, 0, MAX_DISTANCE), 0, MAX_DISTANCE, 0, 255);

  // Actualizar los LEDs
  updateLEDs(brightness);

  delay(10);  // Breve pausa
}

// Función para obtener la distancia del sensor ultrasónico
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0343) / 2;  // Convertir a distancia en cm
}

// Función para calcular el promedio suavizado
int getSmoothedDistance(float distance) {
  total -= readings[readIndex];         // Restar la lectura antigua
  readings[readIndex] = distance;       // Agregar la nueva lectura
  total += readings[readIndex];         // Sumar la nueva lectura
  readIndex = (readIndex + 1) % numReadings;  // Avanzar el índice (circular)
  return total / numReadings;           // Retornar el promedio
}

// Función para actualizar los LEDs
void updateLEDs(int brightness) {
  strip.fill(strip.Color(0, 0, 0, strip.gamma8(brightness)));
  strip.show();
}
