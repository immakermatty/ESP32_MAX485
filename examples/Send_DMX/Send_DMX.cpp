// MAX485_DMX - A Arduino library for sending and receiving DMX using the builtin serial hardware port.
// ESP32_MAX485.cpp: Library implementation file
//
// Copyright (C) 2015  Rick <ricardogg95@gmail.com>
// This work is licensed under a GNU style license.
//
// Last change: Marcel Seerig <https://github.com/mseerig>
// Total rewrite by @makermatty
//
// Documentation and samples are available at https://github.com/immakermatty/ESP32-DMX
//
// Wiring diagram available at
// http://hobbycomponents.com/images/forum/RS485_Module_HCMODU0081_Diagram.png
//
// DMX protocol reference https://en.wikipedia.org/wiki/DMX512

#include <Arduino.h>
#include <MAX485_DMX.h>

#define DI_PIN 17
#define DE_PIN 22
#define RE_PIN 21
#define RO_PIN 16

#define SERIAL_INTERFACE Serial1

MAX485_DMX dmx(DI_PIN, DE_PIN, RE_PIN, RO_PIN, SERIAL_INTERFACE);

void setup()
{
    //Serial.begin(115200);
    dmx.begin();
}

void loop()
{
    dmx.setValue(1, 255);
    dmx.setValue(2, 0);
    dmx.setValue(3, 255);
    dmx.setValue(4, 0);
    dmx.writeOut(4);

    delay(500);

    dmx.setValue(1, 0);
    dmx.setValue(2, 255);
    dmx.setValue(3, 0);
    dmx.setValue(4, 255);
    dmx.writeOut(4);

    delay(500);
}
