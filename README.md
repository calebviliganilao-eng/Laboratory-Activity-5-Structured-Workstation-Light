# Laboratory Activity 5: Structured Workstation Light

## Overview

This project demonstrates the use of analog and digital inputs and outputs using an ESP32 microcontroller. A push button is used to enable or disable a workstation light, while a potentiometer controls the brightness of the LED using Pulse Width Modulation (PWM). A separate LED serves as a status indicator to show whether the system is enabled or disabled.

The activity uses a structured Read-Process-Write architecture to organize the program and control the components.

## Project Features

* **Safety Switch:** Uses a push button connected to GPIO 4 with an internal pull-up resistor (`INPUT_PULLUP`) to enable or disable the workstation light.
* **Brightness Control:** Uses a potentiometer connected to GPIO 34 to adjust the brightness of the LED.
* **PWM Control:** Uses PWM on GPIO 18 with a frequency of 5 kHz and 8-bit resolution to control LED brightness.
* **Status Indicator:** Uses an LED connected to GPIO 2 to indicate whether the workstation light is enabled.
* **Read-Process-Write Architecture:** Uses separate functions (`readInputs()`, `processInputs()`, and `updateOutputs()`) to organize the program.
* **Serial Monitor Output:** Displays the system status, raw potentiometer reading, and applied PWM duty cycle.

## Hardware Components

* 1x ESP32 Microcontroller
* 1x Tactile Push Button
* 1x Potentiometer
* 2x LEDs
* 2x 220Ω–330Ω Resistors
* 1x Breadboard
* Jumper Wires
* 1x USB Cable

## Pin Wiring Connections

| **Component** | **Pin**          | **Connection**                       |
| ------------- | ---------------- | ------------------------------------ |
| Push Button   | Terminal 1       | GPIO 4                               |
| Push Button   | Terminal 2       | GND                                  |
| Potentiometer | Outer Terminal 1 | 3V3                                  |
| Potentiometer | Center Terminal  | GPIO 34                              |
| Potentiometer | Outer Terminal 2 | GND                                  |
| Status LED    | Anode (+)        | GPIO 2 through a 220Ω–330Ω resistor  |
| Status LED    | Cathode (-)      | GND                                  |
| PWM LED       | Anode (+)        | GPIO 18 through a 220Ω–330Ω resistor |
| PWM LED       | Cathode (-)      | GND                                  |

**Note:** Connect the potentiometer to 3V3 instead of 5V or VIN to avoid damaging the ESP32 analog input.

## Circuit Image

**Disclaimer:** The image below is intended for documentation and reference purposes. Your actual circuit setup may look different depending on your wiring and components.

[ Circuit Image ](images/circuit-image.png)

## Working

**Disclaimer:** The image below is intended to show the actual working condition of the project. Replace the placeholder with your own photo showing the completed activity.

[ Working Image ](images/working-image.png)

## Source Code

```cpp
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
```

## Observation Summary

| **Button State**                         | **Input Logic** | **Status LED (GPIO 2)** | **PWM LED (GPIO 18)**     |
| ---------------------------------------- | --------------- | ----------------------- | ------------------------- |
| Released                                 | HIGH            | OFF                     | OFF                       |
| Pressed                                  | LOW             | ON                      | Depends on potentiometer  |
| Pressed with minimum potentiometer value | LOW             | ON                      | OFF or minimum brightness |
| Pressed with maximum potentiometer value | LOW             | ON                      | Maximum brightness        |

## Serial Monitor Output

The Serial Monitor displays the current state of the system, the potentiometer reading, and the PWM duty cycle.

Example output:

```text
Enabled: NO | ADC: 0 | Applied Duty: 0
Enabled: YES | ADC: 2048 | Applied Duty: 127
Enabled: YES | ADC: 4095 | Applied Duty: 255
Enabled: NO | ADC: 3000 | Applied Duty: 0
```

**Note:** These are sample values. Actual readings may vary depending on the potentiometer position and the ESP32 ADC.

## How to Run the Project

1. Connect the ESP32 and other components based on the wiring table.
2. Connect the potentiometer to 3V3, GPIO 34, and GND.
3. Connect the push button and LEDs to their corresponding GPIO pins.
4. Connect the ESP32 to your computer using a USB cable.
5. Open the project in Arduino IDE or PlatformIO.
6. Select the correct ESP32 board and COM port.
7. Upload the source code to the ESP32.
8. Open the Serial Monitor and set the baud rate to 115200.
9. Press and hold the push button to enable the workstation light.
10. Rotate the potentiometer to adjust the brightness of the PWM LED.
11. Release the push button to turn OFF both LEDs.
12. Observe the Serial Monitor to check the system status, ADC reading, and applied duty cycle.

## Expected Output

* When the push button is released, both LEDs remain OFF.
* When the push button is pressed, the status LED turns ON and the workstation light is enabled.
* The potentiometer adjusts the brightness of the workstation light while the button is pressed.
* When the potentiometer is at its minimum value, the PWM LED turns OFF or produces minimum brightness.
* When the potentiometer is at its maximum value, the PWM LED reaches maximum brightness.
* The Serial Monitor displays the system status, ADC reading, and applied duty cycle.

## Conclusion

This activity demonstrates how to control a workstation light using analog and digital inputs with an ESP32 microcontroller. The push button acts as an enable switch, while the potentiometer adjusts the brightness of the LED using PWM. The status LED provides feedback about the system's current state.

By applying the Read-Process-Write architecture, the program becomes more organized and easier to understand. The Serial Monitor also helps observe the input values and output behavior during testing.

## Disclaimer

This README file is intended for educational and documentation purposes. The circuit and working images should represent the actual project whenever possible. Any sample images or placeholders should be replaced with the actual results of the activity.
