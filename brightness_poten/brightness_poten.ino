
// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 9

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 398

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 80  // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);


/// Bat

const int trigPin = 6;
const int echoPin = 5;
float duration, distance;

//
int val_poten;
int pin_Poten = A0;


void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pin_Poten, INPUT);

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
}

void loop() {

  val_poten = analogRead(pin_Poten);
  val_poten = map(val_poten, 0, 1023, 0, 255);
  Serial.println(val_poten);

  strip.fill(strip.Color(val_poten, val_poten, val_poten, strip.gamma8(val_poten)));
  strip.show();
  //delay(10);
}
