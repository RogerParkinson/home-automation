

#include <stdio.h>
#include <VirtualWire.h>
#include <TinyWireS.h>

#define ledPin 1
#define TXD_PIN  3
#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)
#define DELAY 3000
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
    buflen = 0;
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
          delay(DELAY*5);
          digitalWrite(ledPin, false);
      } else {
        buflen = 0;
      }
    }
    if (TinyWireS.available()){           // got I2C input!
//      digitalWrite(ledPin, true);         // Flash a light to show received I2C request
      while (TinyWireS.available()) {
        byteRcvd = TinyWireS.receive();               // get the byte from master
      }
//      buflen=3;
      TinyWireS.send(buflen);             //send an initial byte with the length
      if (buflen > 0) {
        for (i=0;i<buflen;i++) {
          TinyWireS.send(buf[i]);           // send the current buffer
        }
      }
      buflen = 0;
      delay(DELAY*5);
//      digitalWrite(ledPin, false);
    }
    delay(DELAY);
}
