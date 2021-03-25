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

#ifndef ESP32_MAX685_h
#define ESP32_MAX685_h

// DMX transmitter only library for one universe

class MAX485_DMX {
public:
    MAX485_DMX(HardwareSerial& serialInterface = Serial1);

    void begin(int8_t diPin, int8_t dePin, int8_t rePin, int8_t roPin);
    void end();

    uint8_t getValue(size_t channel);
    void setValue(size_t channel, uint8_t value);

    void writeOut(size_t channels, bool wait = true);

    HardwareSerial& serial;

private:
    int8_t m_txPin;
    int8_t m_rxPin;
    int8_t m_dePin;
    int8_t m_rePin;
    uint8_t m_dmxSlots[513];
};

#endif
