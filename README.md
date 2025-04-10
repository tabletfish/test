
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




ISR(INT0_vect){
  PORTB &= ~LED_GREEN;
  PORTB |= LED_RED;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB &= ~LED_RED;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB |= LED_RED;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB |= LED_GREEN;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB &= ~LED_GREEN;
}




#define LED_GREEN 8
#define LED_RED 9
#define EXTERNAL_SW 2

void setup() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(EXTERNAL_SW, INPUT);
  attachInterrupt(0, background, FALLING);
}

void loop() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
}

void background() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  digitalWrite(LED_RED, LOW);

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  digitalWrite(LED_RED, HIGH);

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  digitalWrite(LED_GREEN, HIGH);

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  digitalWrite(LED_GREEN, LOW);
  }




#define LED_GREEN 0x01
#define LED_RED   0x02
#define EXTERNAL_SW 0x04

void setup() {
  DDRB = LED_GREEN | LED_RED;
  DDRD &= ~EXTERNAL_SW;
  PCICR |= 0x04;
  PCMSK2 |= 0x04;
}

void loop() {
  PORTB |= LED_GREEN;
  PORTB &= ~LED_RED;
}



ISR(PCINT2_vect){
  PORTB &= ~LED_GREEN;
  PORTB |= LED_RED;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB &= ~LED_RED;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB |= LED_RED;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB |= LED_GREEN;

  for(uint16_t i = 0; i <= 64000; i++){
    asm("nop");
  }

  PORTB &= ~LED_GREEN;
}








  
#define OC2A 0x08
#define OC2B 0x10

volatile int8_t deltaA = 1;
volatile int8_t deltaB = -1;

ISR(TIMER2_COMPA_vect){
  if(OCR2A == 255){
    deltaA = -1;
  }
  else if(OCR2A == 0){
    deltaA = 1;
  }
  OCR2A += deltaA;
}

ISR(TIMER2_COMPB_vect){
  if(OCR2B == 255){
    deltaB = -1;
  }
  else if(OCR2B == 0){
    deltaB = 1;
  }
  OCR2B += deltaB;
}

void setup() {
  cli();
  DDRB |= OC2A;
  DDRD |= OC2B;

  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

  TCCR2A |= (1 << COM2A1);
  TIMSK2 |= (1 << OCIE2A);

  TCCR2A |= (1 << COM2B1);
  TIMSK2 |= (1 << OCIE2B);

  OCR2A = 0;
  OCR2B = 255;

  sei();
}

void loop() {
}







#define OC2A 0x08
#define OC0A 0x40

volatile int8_t delta1 = 1;
volatile int8_t delta2 = -1;

ISR(TIMER2_COMPA_vect){
  if(OCR2A == 255){
    delta1 = -1;
  }
  else if(OCR2A == 0){
    delta1 = 1;
  }
  OCR2A += delta1;
}

ISR(TIMER0_COMPA_vect){
  if(OCR0A == 255){
    delta2 = -1;
  }
  else if(OCR0A == 0){
    delta2 = 1;
  }
  OCR0A += delta2;
}

void setup() {
  cli();

  DDRB |= OC2A;
  DDRB |= OC0A;

  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  TCCR2A |= (1 << COM2A1);
  TIMSK2 |= (1 << OCIE2A);

  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  TCCR0B |= (1 << CS01);
  TCCR0A |= (1 << COM0A1);
  TIMSK0 |= (1 << OCIE0A);

  OCR2A = 0;
  OCR0A = 255;

  sei();
}

void loop() {
}






#define EXT_SWITCH 0x04
#define OC2B 0x08
#define NUM_FREQ 6

const float frequencies[NUM_FREQ] = {
  261.63,
  293.66,
  329.63,
  349.23,
  400.00,
  493.88
};

volatile int8_t freq_count = 0;

ISR(INT0_vect){
  freq_count = (freq_count + 1) % NUM_FREQ;
  float freq_target = frequencies[freq_count];
  OCR2A = F_CPU / 256 / freq_target - 1;
  OCR2B = OCR2A / 2;

  for(uint16_t j = 0; j < 50; j++){
    for(uint16_t i = 0; i < 64000; i++){
      asm("nop");
    }
  }
}

void setup() {
  cli();

  DDRD &= ~EXT_SWITCH;
  EICRA |= 0x03;
  EIMSK |= 0x01;

  DDRD |= OC2B;

  TCCR2A = (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS22) | (1 <<






#define EXT_SWITCH 0x04
#define OC2B 0x08
#define NUM_FREQ 6

const float frequencies[NUM_FREQ] = {
  261.63,
  293.66,
  329.63,
  349.23,
  400.00,
  493.88
};

volatile int8_t freq_count = 0;

ISR(INT0_vect){
  freq_count = (freq_count + 1) % NUM_FREQ;
  float freq_target = frequencies[freq_count];
  OCR2A = F_CPU / 256 / freq_target - 1;
  OCR2B = OCR2A / 2;

  for(uint16_t j = 0; j < 50; j++){
    for(uint16_t i = 0; i < 64000; i++){
      asm("nop");
    }
  }
}

void setup() {
  cli();

  DDRD &= ~EXT_SWITCH;
  EICRA |= 0x03;
  EIMSK |= 0x01;

  DDRD |= OC2B;

  TCCR2A = (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
  TCCR2A |= (1 << COM2B1);

  float freq_target = frequencies[freq_count];
  OCR2A = F_CPU / 256 / freq_target - 1;
  OCR2B = OCR2A / 2;

  sei();
}

void loop() {
}


  
