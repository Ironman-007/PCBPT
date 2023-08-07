#pragma once

#include <Arduino.h>

#define SERIAL_DEBUG false

#define CORAL_RORATION 0

#define CORAL_NUM 1

#define LED_ON  LOW
#define LED_OFF HIGH

// RGB LED pins
#define RGB_R_PIN D17
#define RGB_G_PIN D16
#define RGB_B_PIN D25

// Stepper pins
#define STEP_PIN  D26
#define DIR_PIN   D27

#define MS1_PIN              D1
#define MS2_PIN              D2
#define MS3_PIN              D4

#define LIMIT_SW_0           D3

extern volatile bool Coral_homed;

extern void pin_init(void);

extern void TURN_ON_LED(bool R, bool G, bool B);

#define PIN_NUM 10

// #define HOME_POS_6           117.5
// #define HOME_POS_5           -10
// #define HOME_POS_4           90

enum M_control_bool {
    BIGGER  = true,
    SMALLER = false
};

// Coral #6
/*
struct CoralConfig {
    uint8_t I2C_ADDR = 0x36;

    int     DIR2HOME  = LOW;
    int     DIR2BOARD = HIGH;

    bool    IS_ROT    = false;
    bool    HAS_SERVO = false;

    uint8_t PINS2INIT[PIN_NUM] = {RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, STEP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, LIMIT_SW_0};
    int     PINS2INIT_MODE[PIN_NUM] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT};
    int     PINS2INIT_STAT[PIN_NUM] = {LED_OFF, LED_OFF, LED_OFF, LOW, HIGH, HIGH, HIGH, LOW};

    float   HOME_POS = 117.5;

    float   LIMIT_L  = 0;
    float   LIMIT_H  = 117.5;

    bool    M_control = BIGGER;

    int     RIGHT_ANGLE_STEPS = 290;
};
*/

// Coral #5
/*
struct CoralConfig {
    uint8_t I2C_ADDR = 0x35;

    int     DIR2HOME  = HIGH;
    int     DIR2BOARD = LOW;

    bool    IS_ROT    = false;
    bool    HAS_SERVO = false;

    uint8_t PINS2INIT[PIN_NUM] = {RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, STEP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, LIMIT_SW_0};
    int     PINS2INIT_MODE[PIN_NUM] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT};
    int     PINS2INIT_STAT[PIN_NUM] = {LED_OFF, LED_OFF, LED_OFF, LOW, HIGH, HIGH, HIGH, LOW};

    float   HOME_POS = -10;

    float   LIMIT_L  = -10;
    float   LIMIT_H  = 120;

    bool    M_control = SMALLER;

    int     RIGHT_ANGLE_STEPS = 290;
};
*/

// Coral #4
/*
struct CoralConfig {
    uint8_t I2C_ADDR  = 0x34;

    int     DIR2HOME  = HIGH;
    int     DIR2BOARD = LOW;

    bool    IS_ROT    = true;
    bool    HAS_SERVO = true;

    uint8_t PINS2INIT[PIN_NUM] = {RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, STEP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, LIMIT_SW_0};
    int     PINS2INIT_MODE[PIN_NUM] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT};
    int     PINS2INIT_STAT[PIN_NUM] = {LED_OFF, LED_OFF, LED_OFF, LOW, HIGH, HIGH, HIGH, LOW};

    float   HOME_POS = 90;

    float   LIMIT_L  = -90;
    float   LIMIT_H  = 90;

    bool    M_control = SMALLER;

    int     RIGHT_ANGLE_STEPS = 370;
};
*/

// Coral #3
// /*
struct CoralConfig {
    uint8_t I2C_ADDR = 0x33;

    int     DIR2HOME  = HIGH;
    int     DIR2BOARD = LOW;

    bool    IS_ROT    = true;
    bool    HAS_SERVO = true;

    uint8_t PINS2INIT[PIN_NUM] = {RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, STEP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, LIMIT_SW_0};
    int     PINS2INIT_MODE[PIN_NUM] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT};
    int     PINS2INIT_STAT[PIN_NUM] = {LED_OFF, LED_OFF, LED_OFF, LOW, HIGH, HIGH, HIGH, LOW};

    float   HOME_POS = 90;

    float   LIMIT_L  = -10;
    float   LIMIT_H  = 120;

    bool    M_control = SMALLER;

    int     RIGHT_ANGLE_STEPS = 430;
};
// */

// Coral #2
/*
struct CoralConfig {
    uint8_t I2C_ADDR = 0x32;

    int     DIR2HOME  = HIGH;
    int     DIR2BOARD = LOW;

    bool    IS_ROT    = false;
    bool    HAS_SERVO = false;

    uint8_t PINS2INIT[PIN_NUM] = {RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, STEP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, LIMIT_SW_0};
    int     PINS2INIT_MODE[PIN_NUM] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT};
    int     PINS2INIT_STAT[PIN_NUM] = {LED_OFF, LED_OFF, LED_OFF, LOW, HIGH, HIGH, HIGH, LOW};

    float   HOME_POS = -13;

    float   LIMIT_L  = -10;
    float   LIMIT_H  = 120;

    bool    M_control = SMALLER;

    int     RIGHT_ANGLE_STEPS = 290;
};
*/

// Coral #1
/*
struct CoralConfig {
    uint8_t I2C_ADDR = 0x31;

    int     DIR2HOME  = LOW;
    int     DIR2BOARD = HIGH;

    bool    IS_ROT    = false;
    bool    HAS_SERVO = false;

    uint8_t PINS2INIT[PIN_NUM] = {RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, STEP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, LIMIT_SW_0};
    int     PINS2INIT_MODE[PIN_NUM] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT};
    int     PINS2INIT_STAT[PIN_NUM] = {LED_OFF, LED_OFF, LED_OFF, LOW, HIGH, HIGH, HIGH, LOW};

    float   HOME_POS = -9;

    float   LIMIT_L  = -20;
    float   LIMIT_H  = 110;

    bool    M_control = SMALLER;

    int     RIGHT_ANGLE_STEPS = 290;
};
*/

extern CoralConfig Coral;