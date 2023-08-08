#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Adafruit_VEML7700.h"

#define PIN_LED_1 A0
#define PIN_LED_2 A1
#define PIN_LED_3 A2
#define PIN_LED_4 A3

#define SHT_DATA  A7
#define SHT_CLK   A10

#define V_3V3_PIN A8

#define RGB_DIN   A6

float voltage   = 0.0;

int   pin_value = 0;

int wave_1      = 0;
int wave_2      = 200;
int wave_3      = 100;
int wave_4      = 0;

#define DAC_step  0.03
#define LED_step  5

float x         = 0;

int LED_change_1  = 1;
int LED_change_2  = 1;
int LED_change_3  = 1;
int LED_change_4  = 1;

float LUX = 0.0;

Adafruit_NeoPixel pixels(1, RGB_DIN, NEO_GRB + NEO_KHZ800);
// Adafruit_VEML7700 veml = Adafruit_VEML7700();

void calc_DAC_change(void) {
  wave_1 = (int)(511.5 + 511.5 * sin(x));
  x += DAC_step;
}

void calc_LED_change(int led_value, int * led_change_value) {
  if ((led_value + (*led_change_value)*LED_step) >= 255) {
    *led_change_value = -1;
  } else if ((led_value + (*led_change_value)*LED_step) <=0) {
    *led_change_value = 1;
  }
}

void setup() {
  // veml.begin();
  // veml.setIntegrationTime(VEML7700_IT_25MS);

  analogWriteResolution(10);

  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);

  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_LED_3, LOW);
  digitalWrite(PIN_LED_4, LOW);

  pixels.begin();

  // SerialUSB.begin(115200);
  // while (!SerialUSB) delay(10);
}

void loop() {
  pixels.clear();

  calc_DAC_change();
  analogWrite(PIN_LED_1, wave_1);

  calc_LED_change(wave_2, &LED_change_2);
  calc_LED_change(wave_3, &LED_change_3);
  calc_LED_change(wave_4, &LED_change_4);

  wave_2 += LED_change_2 * LED_step;
  wave_3 += LED_change_3 * LED_step;
  wave_4 += LED_change_4 * LED_step;

  analogWrite(PIN_LED_2, wave_2);
  analogWrite(PIN_LED_3, wave_3);
  analogWrite(PIN_LED_4, wave_4);

  pixels.setPixelColor(0, pixels.Color(wave_2*0.05, wave_3*0.05, wave_4*0.05));
  pixels.show();

  voltage = analogRead(V_3V3_PIN);
  voltage = voltage/1024*3.3*2;

  // LUX = veml.readLux();

  // SerialUSB.print("lux: ");
  // SerialUSB.print(LUX);
  // SerialUSB.print(" | 3.3V value = ");
  // SerialUSB.println(voltage);

  delay(10);
}
