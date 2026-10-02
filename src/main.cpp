#include <Arduino.h>

const int BUTTON_PIN = 4;
const int POT_PIN = 34;
const int STATUS_LED_PIN = 2;
const int PWM_LED_PIN = 18;

const int PWM_CHANNEL = 0;
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

bool isEnabled = false;
int rawPotValue = 0;
int appliedDuty = 0;
bool statusLedOn = false;

int scaleToDuty(int raw) {
    int clamped = constrain(raw, 0, 4095);
    return map(clamped, 0, 4095, 0, 255);
}

void readInputs() {
    isEnabled = (digitalRead(BUTTON_PIN) == LOW);
    rawPotValue = analogRead(POT_PIN);
}

void processInputs() {
    if (isEnabled) {
        statusLedOn = true;
        appliedDuty = scaleToDuty(rawPotValue);
    } else {
        statusLedOn = false;
        appliedDuty = 0;
    }
}

void updateOutputs() {
    digitalWrite(STATUS_LED_PIN, statusLedOn ? HIGH : LOW);
    ledcWrite(PWM_CHANNEL, appliedDuty);
}

void setup() {
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(STATUS_LED_PIN, OUTPUT);

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_LED_PIN, PWM_CHANNEL);

    updateOutputs();
}

void loop() {
    readInputs();
    processInputs();
    updateOutputs();

    Serial.print("Enabled: ");
    Serial.print(isEnabled ? "YES" : "NO");
    Serial.print(" | ADC: ");
    Serial.print(rawPotValue);
    Serial.print(" | Applied Duty: ");
    Serial.println(appliedDuty);

    delay(20);
}