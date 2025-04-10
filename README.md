
#define LED_GREEN  0x01
#define LED_RED    0x02
#define EXTERNAL_SW 0x04

void setup() {
  DDRB = LED_GREEN | LED_RED;
  DDRD &= ~EXTERNAL_SW;
  EICRA |= 0x01;
  EIMSK |= 0x01;
}

void loop() {
  PORTB |= LED_GREEN;
  PORTB &= ~LED_RED;
}
