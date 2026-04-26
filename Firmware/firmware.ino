/*
Firmware for Throw-a-Phone
By Tejas Joshi "Chicken Slayer"
https://github.com/Chicken-Slayer/Throw-a-Phone
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

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

SoftwareSerial SIM800L(SIM_RX, SIM_TX);
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
    Serial.println("[BOOT]Throw-a-Phone starting...")

    // Start OLED
    Wire.begin(I2C_SDA, I2C_SCL);
    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 12);
    display.print("Throw-a-Phone initialising...");
    display.display();

    // Starting up the Switch Matrix 
    for (int r = 0; r < ROWS; r++) {
        pinMode(ROW_PINS[r], OUTPUT);
        digitalWrite(ROW_PINS[r], HIGH);
    }
    for (int c = 0; c < COLS; c++) {
        pinMode(COL_PINS[c], INPUT_PULLUP);
    }

    // GSM Module startup
    SIM800L.begin(115200);
    delay(3000);
    SIM800L.println("AT"); \\ Handshake
    updateSerial();
    SIM800L.println("AT+CSQ");
    updateSerial();
    SIM800L.println("AT+CCID");
    updateSerial();
    SIM800L.println("AT+CREG?");
    updateSerial();
    SIM800L.println("AT+CLIP=1");
    updateSerial();
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


// Used to scan the key matrix and add the "typing" functionality.
char scanKeys() {
    for (int r = 0; r < ROWS; r++) {
        digitalWrite(ROW_PINS[r], LOW);
        delayMicroseconds(10);  // settle time for input pull-up

        for (int c = 0; c < COLS; c++) {
            if (digitalRead(COL_PINS[c]) == LOW) {
            char key = KEYMAP[r][c];
            unsigned long now = millis();

            // Accept key if it's a new key or enough time has passed
            bool newKey   = (key != lastPressKey);
            bool debounced = (now - lastPressTime >= DEBOUNCE_MS);

            if (newKey || debounced) {
                lastPressKey  = key;
                lastPressTime = now;
                digitalWrite(ROW_PINS[r], HIGH);

                Serial.printf("[KEY] '%c' pressed\n", key);
                return key;
        }
      }
    }
        digitalWrite(ROW_PINS[r], HIGH);
    }
    lastPressKey = 0;
    return 0;
}