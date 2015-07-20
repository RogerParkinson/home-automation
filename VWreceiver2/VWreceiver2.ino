// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <stdio.h>
#include <VirtualWire.h>
#include <TinyWireS.h>

#define ledPin 4
#define TXD_PIN  3
#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)
#define DELAY 5000
#define BUFFER_LENGTH  15

static uint8_t rByte = 0;
static uint8_t buf[BUFFER_LENGTH];
static uint8_t buflen = 0;


void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(TXD_PIN, INPUT);
    // Initialise the IO and ISR
    vw_set_rx_pin(TXD_PIN);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(1000);	 // Bits per sec
    for (int k = 0; k < 10; k = k + 1) {
        if (k % 2 == 0) {
            digitalWrite(ledPin, HIGH);
            }
        else {
            digitalWrite(ledPin, LOW);
            }
        delay(DELAY);
        } // for

    vw_rx_start();       // Start the receiver PLL running
    TinyWireS.begin(I2C_SLAVE_ADDR);      // init I2C Slave mode
}

void loop()
{
    buf[0] = 0;
    byte byteRcvd = 0;
    int i;

    if (buflen == 0) {
      buflen = BUFFER_LENGTH;
      if (vw_get_message(buf, &buflen)) {// Non-blocking
          digitalWrite(ledPin, true);       // Flash a light to show received good message
          delay(DELAY);
          digitalWrite(ledPin, false);
      } else {
        buflen = 0;
      }
    }
    if (TinyWireS.available()){           // got I2C input!
      digitalWrite(ledPin, true);         // Flash a light to show received I2C request
      byteRcvd = TinyWireS.receive();     // get the byte from master
      TinyWireS.send(buflen);             //send an initial byte with the length
      for (i=0;i<buflen;i++) {
        TinyWireS.send(buf[i]);           // send the current buffer
      }
      buflen = 0;
      delay(DELAY);
      digitalWrite(ledPin, false);
    }
    delay(DELAY);
}
