#define LED_BLUE_BIT    3
#define LED_GREEN_BIT   1
#define LED_RED_BIT     6

#define FOSC 16000000
#define BAUD 9600

unsigned char readData = NULL;

void setup() {
  DDRB |= (1 << LED_BLUE_BIT) | (1 << LED_GREEN_BIT);
  DDRD |= (1 << LED_RED_BIT);

  // USART: Asynchronous Normal Mode
  // UCSRA = 0b 0000 0000;
  // -> (0) U2X: (double the USART transmission speed) Normal
  // UCSRB = 0b 1001 1000;
  // -> (1) RXCIE: RX complete interrupt enable
  // -> (1) RXEN: Receiver enable
  // -> (1) TXEN: Transmitter enable
  // UCSRC = 0b 1000 0110;
  // -> (00) UMSLE01, UMSLE00: (UMSEL0 Bits Settings) Asynchronous USART
  // -> (00) UPM01, UPM00: (Parity mode) disabled
  // -> (1) USBS0: (USBS bit settings) 1-bit
  // -> (011) UCSZ02, UCSZ01, UCSZ00: (Character size) 8-bit

  uint16_t ubrr = FOSC / 16 / BAUD - 1;
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;

  UCSRB |= (1 << RXCIE);
  UCSRB |= (1 << RXEN) | (1 << TXEN);
  UCSRC |= (1 << USBS0);
  UCSRC |= (1 << UCSZ01) | (1 << UCSZ00);
}

ISR(USART_RX_vect){
  readData = UDR0;
}

void loop() {
  if (readData == 'r'){
    PORTB &= ~(1 << LED_BLUE_BIT);
    PORTB &= ~(1 << LED_GREEN_BIT);
    PORTD |=  (1 << LED_RED_BIT);
  }
  else if (readData == 'g'){
    PORTB &= ~(1 << LED_BLUE_BIT);
    PORTD &= ~(1 << LED_RED_BIT);
    PORTB |=  (1 << LED_GREEN_BIT);
  }
  else if (readData == 'b'){
    PORTB |=  (1 << LED_BLUE_BIT);
    PORTB &= ~(1 << LED_GREEN_BIT);
    PORTD &= ~(1 << LED_RED_BIT);
  }
  else if (readData == 'o'){
    PORTB &= ~(1 << LED_BLUE_BIT);
    PORTB &= ~(1 << LED_GREEN_BIT);
    PORTD &= ~(1 << LED_RED_BIT);
  }
  else{
    readData = NULL;
  }
}


#define LED_RED     6
#define LED_GREEN   9
#define LED_BLUE    11

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    char readData = Serial.read();

    if (readData == 'r'){
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (readData == 'g'){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (readData == 'b'){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
    }
    else if (readData == 'o'){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
  }
}
