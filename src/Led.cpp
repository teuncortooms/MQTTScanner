#include "Led.h"

Led::Led(int pin)
{
    this->ledPin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Led::ToggleState()
{
    ledState = !ledState;
}

bool Led::GetState()
{
    return this->ledState;
}

void Led::Update()
{
    digitalWrite(this->ledPin, this->ledState);
}