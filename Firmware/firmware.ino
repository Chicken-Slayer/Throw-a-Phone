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

// display:
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
// - display controls

void setup () {
    Serial.begin(115200);
    Serial.println("[BOOT]Throw-a-Phone starting...")

    // Start display
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
    SIM800L.println("AT"); 
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

    // Will add actual functionality after physical device access
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
        delayMicroseconds(10);

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

// This function below can seem a little complicated, but it basically just handles the input that we get from the keys on the board.

void handleKey(char key) {
    switch (phoneState) { 
        case ST_IDLE:
            if (key >= '0' && key <= '9') {
                dialNumber = "";
                dialNumber += key;
                phoneState = ST_TYPING;
                updateDisplay();
            }
            break;

        case ST_TYPING:
        if (key >= '0' && key <= '9') {
            if (dialNumber.length() < 15) {
                dialNumber += key;
                updateDisplay();
            }
        } else if (key == '#') {
        // Backspace
            if (dialNumber.length() > 0) {
                dialNumber.remove(dialNumber.length() - 1);
            }
            if (dialNumber.length() == 0) {
                phoneState = ST_IDLE;
            }
            updateDisplay();
        } else if (key == '*') {
        // Dial
            if (dialNumber.length() > 0) {
                makeCall(dialNumber);
            }
        }
        break;

        case ST_CALLING:
            if (key == '#') {
                hangUp();
            }
            break;

        case ST_INCOMING:
            if (key == '*') {
                answerCall();
            } else if (key == '#') {
                hangUp();
            }
            break;


        case ST_IN_CALL:
            if (key == '#') {
                hangUp();
            }
            break;
    }
}

// TODO: GSM Module control (No access to actual device so far, might end up a buggy mess if I write code without testing on it.)

// Call functions: Just to keep things simplified in the above complicated mess
void makeCall(const String& number) {
    Serial.println("[CALL] Dialling " + number);
    phoneState = ST_CALLING;
    updateDisplay();
    SIM800L.println("ATD" + number + ";");
    updateSerial();
}

void answerCall() {
    Serial.println("[CALL] Answering");
    phoneState = ST_IN_CALL;
    updateDisplay();
    SIM800L.println("ATA");
    updateSerial();
}

void hangUp() {
    Serial.println("[CALL] Hanging up");
    SIM800L.println("ATH");
    updateSerial();
    resetToIdle();
}

void resetToIdle() {
    phoneState   = ST_IDLE;
    dialNumber   = "";
    callerNumber = "";
    updateDisplay();
}

// Display stuff
// This looks longer but perhaps one of the easier parts to understand and write

void drawHint(const char* hint) {
    display.setTextSize(1);
    display.setCursor(0, 24);
    display.print(hint);
}

void drawNumber(const String& num) {
    bool large = (num.length() <= 10);
    display.setTextSize(large ? 2 : 1);
    int charW = large ? 12 : 6;
    int x = max(0, (SCREEN_W - (int)num.length() * charW) / 2);
    int y = large ? 8 : 12;
    display.setCursor(x, y);
    display.print(num);
}

void updateDisplay() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    switch (phoneState) {

        case ST_IDLE:
            display.setTextSize(1);
            display.setCursor(38, 4);
            display.print("READY");
            display.setCursor(16, 20);
            display.print("Enter number...");
            break;

        case ST_TYPING:
            drawHint("* Call      # Del");
            drawNumber(dialNumber);
            break;

        case ST_CALLING:
            display.setTextSize(1);
            display.setCursor(30, 0);
            display.print("Calling...");
            drawNumber(dialNumber);
            drawHint("     # Cancel");
            break;

        case ST_INCOMING:
            display.setTextSize(1);
            display.setCursor(20, 0);
            display.print("Incoming Call");
            drawNumber(callerNumber.length() > 0 ? callerNumber : "Unknown");
            drawHint("* Answer   # Decline");
            break;

        case ST_IN_CALL: {
            display.setTextSize(1);
            display.setCursor(38, 0);
            display.print("In Call");
            String activeNum = (dialNumber.length() > 0) ? dialNumber : callerNumber;
            drawNumber(activeNum.length() > 0 ? activeNum : "Unknown");
            drawHint("       # Hang Up");
            break;
        }
    }

    display.display();
}

