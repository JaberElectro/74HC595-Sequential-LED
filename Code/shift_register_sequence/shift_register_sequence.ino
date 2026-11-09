/*
 * 74HC595 Sequential LED Control
 * ------------------------------
 * Lights 8 LEDs progressively through a 74HC595 shift register using
 * only 3 Arduino pins.
 *
 * Controller used in the reconstructed implementation: Arduino Uno.
 *
 * Wiring:
 *   DS   (pin 14) -> D11
 *   SHCP (pin 11) -> D12
 *   STCP (pin 12) -> D8
 *   OE   (pin 13) -> GND   (outputs always enabled)
 *   MR   (pin 10) -> 5V    (never cleared)
 *   Q0..Q7 -> 470R -> LED -> GND
 */

const int DATA_PIN  = 11;   // DS
const int CLOCK_PIN = 12;   // SHCP
const int LATCH_PIN = 8;    // STCP

const unsigned long STEP_MS  = 300;
const unsigned long HOLD_MS  = 500;

// Shift one byte in and latch it to the outputs in a single step.
void updateRegister(byte value) {
  digitalWrite(LATCH_PIN, LOW);                    // outputs hold their old value
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, value);  // bit 0 ends up on Q0
  digitalWrite(LATCH_PIN, HIGH);                   // rising edge: all 8 outputs update together
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  updateRegister(0x00);                            // start with all LEDs off
}

void loop() {
  // Progressive activation: 0b00000001, 0b00000011, ... 0b11111111
  for (int i = 0; i < 8; i++) {
    byte pattern = (1 << (i + 1)) - 1;
    updateRegister(pattern);
    delay(STEP_MS);
  }

  delay(HOLD_MS);
  updateRegister(0x00);   // all off
  delay(HOLD_MS);
}
