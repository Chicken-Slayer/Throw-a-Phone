/*
Firmware for Throw-a-Phone
By Tejas Joshi "Chicken Slayer"
https://github.com/Chicken-Slayer/Throw-a-Phone
*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>

// HARDWARE CONFIGURATION:

// OLED:
#define I2C_SDA 8
#define I2C_SCL 9
#define SCREEN_W 128
#define SCREEN_H 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, OLED_RESET);

// GSM Module:
#define SIM_RX 20
#define SIM_TX 21
#define SIM_BAUD

HardwareSerial simSerial(1);
String simLine = "";

// Key Matrix:
#define ROWS 4
#define COLS 3

const uint8_t ROW_PINS[ROWS] = {3, 4, 5, 6};
const uint8_t COL_PINS[COLS] = {1, 7, 10};

const char KEYMAP[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// UART0 is connection to system; UART1 is used for communication to GSM Module

// Phone Configuration:
enum PhoneState {
  ST_IDLE,
  ST_TYPING,
  ST_CALLING,
  ST_INCOMING,
  ST_IN_CALL
};

PhoneState phoneState = ST_IDLE;

String dialingNumber = "";
String callerNumber = "";

// Debounce (limit rate of function execution to improve performance)
unsigned long lastPressTime = 0;
char lastPressKey  = 0;
const unsigned long DEBOUNCE_MS = 220;



// TODO: Properly configure most things:
// - Calling
// - Keys
// - OLED controls

void setup () {
    Serial.begin(115200);
    SIM800L.begin(115200);
    delay(3000);

}

void loop() {
    updateSerial();
}

void updateSerial() {
    delay(500);
    while (Serial.available())
    {
        SIM800L.write(Serial.read());
    }
    while (SIM800L.available())
    {
        Serial.write(SIM800L.read());
    }
}