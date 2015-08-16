// button activated transmitter
// Flash this to an ATTiny85 8MHz
// wiring:
// PB3 button
// PB1 led
// PB2 (pin 7) tx
// It flashes the led on startup to show it is working
// the button should be a reed switch in the final implementation
// The ID should be changed to be unique forthis device

#include <VirtualWire.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#define ID 5
#define HOME 1
#define LOW_POWER 2
#define EVENT 1

#define DELAY 5000
#define TXD_PIN  7
#define VOLTAGE_PIN  3
#define ledPin 1       //PB0
#define buttonPin PB3

static uint8_t wakeFlag = 0;
static long voltage;
static int lowPowerCounter = 0; // so we don't send low power every loop

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {

  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
    setup_watchdog(9);                     //Setup watchdog to go off after 8sec
    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
    } // sleep

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
    }
ISR(WDT_vect) {
    wakeFlag = 1;
    }

void setup()
{
   pinMode(ledPin, OUTPUT);
    vw_set_tx_pin(TXD_PIN);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(1000);	 // Bits per sec
    // Flash quick sequence so we know setup has started
    for (int k = 0; k < 10; k = k + 1) {
        if (k % 2 == 0) {
            digitalWrite(ledPin, HIGH);
            }
        else {
            digitalWrite(ledPin, LOW);
            }
        delay(DELAY);
        } // for

    // Initialise the IO and ISR
}

void loop()
{
    int i;
    sleep();
    uint8_t message[5];
    message[0] = HOME;
    message[1] = ID;
    message[2] = EVENT;
    if (wakeFlag) {
      voltage = readVcc();
      if ((voltage < 1100 * 0.75) &&  lowPowerCounter-- < 0) {
        message[2] = LOW_POWER;
        vw_send(&message[0], 3);
        vw_wait_tx(); // Wait until the whole message is gone
        lowPowerCounter = 1000;
      }
    } else {
      digitalWrite(ledPin, true); // Flash a light to show transmiting
      vw_send(&message[0], 3);
      vw_wait_tx(); // Wait until the whole message is gone
      delay(DELAY);
      digitalWrite(ledPin, false);
    }
    wakeFlag=0;
}

