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
#define SERIAL_CONFIG_DMX SERIAL_8N2
#define SERIAL_BAUDRATE_BREAK 76800
#define SERIAL_CONFIG_BREAK SERIAL_8N1

#define DMX_MAX_CHANNEL 512
#define DMX_MIN_CHANNEL 1
#define DMX_MIN_TXSLOTS 24

MAX485_DMX::MAX485_DMX(HardwareSerial& serialInterface)
    : m_serial(serialInterface)
    , m_txPin(-1)
    , m_rxPin(-1)
    , m_dmxSlots {}
{
    memset(m_dmxSlots, 0, sizeof(m_dmxSlots));
}

void MAX485_DMX::begin(int8_t di_pin, int8_t de_pin, int8_t re_pin, int8_t ro_pin)
{
    m_txPin = di_pin;
    m_rxPin = ro_pin;

    // de_pin & re_pin together HIGH => MAX485 is a transmitter
    // LOW => MAX485 is a receiver

    //lets set it pernamently as a transmitter
    if (de_pin != -1) {
        pinMode(de_pin, INPUT_PULLUP);
    }
    if (re_pin != -1) {
        pinMode(re_pin, INPUT_PULLUP);
    }

    m_serial.begin(SERIAL_BAUDRATE_DMX, SERIAL_CONFIG_DMX, m_rxPin, m_txPin);
}

void MAX485_DMX::end()
{
    m_serial.end();
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

    // sending break
    m_serial.begin(SERIAL_BAUDRATE_BREAK, SERIAL_CONFIG_BREAK, m_rxPin, m_txPin);
    m_serial.write(0);
    m_serial.flush();

    // sending data
    m_serial.begin(SERIAL_BAUDRATE_DMX, SERIAL_CONFIG_DMX, m_rxPin, m_txPin);
    m_serial.write(m_dmxSlots, channels + 1);

    if (wait) {
        m_serial.flush();
    }
}
