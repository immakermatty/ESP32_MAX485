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
#include "MAX485_DMX.h"

#define SERIAL_BAUDRATE_DMX 250000
#define SERIAL_BAUDRATE_BREAK 57600
#define SERIAL_CONFIG SERIAL_8N2

#define DMX_MAX_CHANNEL 512
#define DMX_MIN_CHANNEL 1
#define DMX_MIN_TXSLOTS 24

MAX485_DMX::MAX485_DMX(HardwareSerial& serialInterface)
    : serial(serialInterface)
    , m_dmxSlots {}
    , m_txPin(-1)
    , m_rxPin(-1)
    , m_dePin(-1)
    , m_rePin(-1)
{
    memset(m_dmxSlots, 0, sizeof(m_dmxSlots));
}

void MAX485_DMX::begin(int8_t diPin, int8_t dePin, int8_t rePin, int8_t roPin)
{
    m_txPin = diPin;
    m_rxPin = roPin;

    m_dePin = dePin;
    m_rePin = rePin;

    pinMode(m_dePin, OUTPUT);
    pinMode(m_rePin, OUTPUT);

    // dePin & rePin together HIGH => MAX485 is a transmitter
    // LOW => MAX485 is a receiver

    //set as a receiver
    if (m_dePin != -1) {
        digitalWrite(m_dePin, LOW);
    }
    if (m_rePin != -1) {
        digitalWrite(m_rePin, LOW);
    }

    serial.begin(SERIAL_BAUDRATE_DMX, SERIAL_CONFIG, m_rxPin, m_txPin);
    serial.setTimeout(1);
}

void MAX485_DMX::end()
{
    serial.end();
}

// Function to read DMX buffer
// index
uint8_t MAX485_DMX::getValue(size_t channel)
{
    if (channel < DMX_MIN_CHANNEL)
        channel = DMX_MIN_CHANNEL;
    if (channel > DMX_MAX_CHANNEL)
        channel = DMX_MAX_CHANNEL;

    return (m_dmxSlots[channel]);
}

// Function to send DMX data
void MAX485_DMX::setValue(size_t channel, uint8_t value)
{
    if (channel < DMX_MIN_CHANNEL)
        channel = DMX_MIN_CHANNEL;
    if (channel > DMX_MAX_CHANNEL)
        channel = DMX_MAX_CHANNEL;

    m_dmxSlots[channel] = value;
}

// Function to update the DMX bus
void MAX485_DMX::writeOut(size_t channels, bool wait)
{
    if (channels < DMX_MIN_TXSLOTS)
        channels = DMX_MIN_TXSLOTS;
    if (channels > DMX_MAX_CHANNEL)
        channels = DMX_MAX_CHANNEL;

    m_dmxSlots[0] = 0x00;

    //set as a transmitter
    if (m_dePin != -1) {
        digitalWrite(m_dePin, HIGH);
    }
    if (m_rePin != -1) {
        digitalWrite(m_rePin, HIGH);
    }

    // sending break
    serial.updateBaudRate(SERIAL_BAUDRATE_BREAK);
    serial.write(0x00);
    serial.flush();

    // sending data
    serial.updateBaudRate(SERIAL_BAUDRATE_DMX);
    serial.write(m_dmxSlots, channels + 1);

    if (wait) {
        serial.flush();
    }

    //set as a receiver
    if (m_dePin != -1) {
        digitalWrite(m_dePin, LOW);
    }
    if (m_rePin != -1) {
        digitalWrite(m_rePin, LOW);
    }
}