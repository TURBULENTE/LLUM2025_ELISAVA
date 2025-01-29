#include <FastLED.h>

// Configuración del hardware
#define LED_PIN_1 25  // Tira Izquierda / Derecha
#define LED_PIN_2 26  // Tira Central
#define LED_PIN_3 27  // Tira Izquierda / Derecha
#define LED_PIN_4 33  // Tira Izquierda / Derecha
/*

#define LED_PIN_1 5  // Tira Izquierda / Derecha
#define LED_PIN_2 6  // Tira Central
#define LED_PIN_3 7  // Tira Izquierda / Derecha
*/

#define NUM_LEDS 399       // Número de LEDs // 398
#define LED_TYPE WS2812B   // Tipo de LED
#define MIN_BRIGHTNESS 8   // Brillo mínimo
#define MAX_BRIGHTNESS 128  // Brillo máximo

CRGB leds_1[NUM_LEDS];  // Arreglo de LEDs
CRGB leds_2[NUM_LEDS];
CRGB leds_3[NUM_LEDS];
CRGB leds_4[NUM_LEDS];

// Sensor
#define PIR_PIN 13
bool presencia = true;
const int ledPin = 2;

void setup() {
  // Inicializa la tira LED
  //Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN_1, GRB>(leds_1, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED_PIN_2, GRB>(leds_2, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED_PIN_3, GRB>(leds_3, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED_PIN_4, GRB>(leds_4, NUM_LEDS);

  FastLED.setBrightness(MAX_BRIGHTNESS);  // Configura el brillo inicial
  FastLED.clear();                        // Apaga todos los LEDs al inicio
  FastLED.show();
  pinMode(PIR_PIN, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {

  presencia = digitalRead(PIR_PIN);

  if (!presencia) {
    digitalWrite(ledPin, HIGH);
    //Serial.println("NO PRESENCIA");
    // Calcula el brillo para el efecto de respiración
    float breath = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
    // Escala el brillo entre el mínimo y el máximo
    uint8_t scaledBrightness = map(breath, 0, 255, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    FastLED.setBrightness(scaledBrightness);

    // Llena todos los LEDs con color blanco
    fill_solid(leds_1, NUM_LEDS, CRGB::White);
    fill_solid(leds_2, NUM_LEDS, CRGB::White);
    fill_solid(leds_3, NUM_LEDS, CRGB::White);
    fill_solid(leds_4, NUM_LEDS, CRGB::White);

    /*
    // Apaga los LEDs del 100 al 200
    for (int i = 0; i <= 60; i++) {
      leds_1[i] = CRGB::Black;  // Configura el color negro (apagado)
      leds_3[i] = CRGB::Black;  // Configura el color negro (apagado)
    }
    */

  } else {
    digitalWrite(ledPin, LOW);
    // Serial.println("PRESENCIA");
    //fill_solid(leds, NUM_LEDS, CRGB::Black);
    fadeToBlackBy(leds_1, NUM_LEDS, 16);
    fadeToBlackBy(leds_2, NUM_LEDS, 16);
    fadeToBlackBy(leds_3, NUM_LEDS, 16);
    fadeToBlackBy(leds_4, NUM_LEDS, 16);
  }





  // Actualiza la tira LED
  FastLED.show();
  delay(5);  // Ajusta la velocidad del efecto
}