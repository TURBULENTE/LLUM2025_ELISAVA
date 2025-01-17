#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_PIN 10
#define LED_COUNT 398
#define BRIGHTNESS 80

const int trigPin = 6;
const int echoPin = 5;
const int numReadings = 20;  // Tamaño del buffer para el promedio

// Variables globales
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
int readings[numReadings] = {0};  // Buffer de lecturas
int readIndex = 0;
int total = 0;
float smoothedValue = 0;  // Valor suavizado por filtro exponencial

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
}

void loop() {
  // Leer distancia
  float distance = getDistance();
  Serial.print("Raw Distance: ");
  Serial.println(distance);

  // Aplicar filtro de promedio móvil
  float avgDistance = applyMovingAverage(distance);
  Serial.print("Average Distance: ");
  Serial.println(avgDistance);

  // Aplicar filtro exponencial suavizado
  smoothedValue = applyExponentialSmoothing(avgDistance, smoothedValue, 0.1);
  Serial.print("Smoothed Distance: ");
  Serial.println(smoothedValue);

  // Determinar brillo basado en el valor suavizado
  int brightness = map(constrain(smoothedValue, 0, 300), 0, 300, 0, 255);

  // Actualizar LEDs
  updateLEDs(brightness);

  delay(10);  // Breve pausa
}

// Función para obtener la distancia
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0343) / 2;  // Convertir a cm
}

// Función de promedio móvil
float applyMovingAverage(float newReading) {
  total -= readings[readIndex];        // Restar la lectura más antigua
  readings[readIndex] = newReading;    // Agregar la nueva lectura
  total += readings[readIndex];        // Sumar al total
  readIndex = (readIndex + 1) % numReadings;  // Avanzar índice circular
  return (float)total / numReadings;   // Retornar promedio
}

// Función de suavizado exponencial
float applyExponentialSmoothing(float newValue, float previousValue, float alpha) {
  return alpha * newValue + (1 - alpha) * previousValue;
}

// Función para actualizar LEDs
void updateLEDs(int brightness) {
  strip.fill(strip.Color(brightness, brightness, brightness, strip.gamma8(brightness)));
  strip.show();
}
