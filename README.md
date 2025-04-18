#// Master of SPI
#include <SPI.h>

char data;

uint8_t transfer_SPI(uint8_t data){
  // Activate slave
  digitalWrite(SS, LOW);     // ①

  // Send data
  SPDR = data;
  while(!(SPSR & (1 << SPIF)));

  // Deactivate slave
  digitalWrite(SS, HIGH);    // ②

  return SPDR;
}

void setup() {
  Serial.begin(9600);

  // SPI (Serial Peripheral Interface) - MASTER
  // SPCR (SPI Control Register) = 0b 0101 0001;
  // => (1) SPE: SPI Enable
  // => (1) MSTR: Master/Slave Select
  // => (01) SPR1:0: prescaler fosc / 16
  // SPSR (SPI Status Register) = 0b 0000 0000;
  // => SPI2X = 0: Double Speed Bit

  // Data direction
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);

  // Set registers
  SPCR |= (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0);
}

void loop () {
  if (Serial.available()){
    data = Serial.read();
    transfer_SPI(data);
  }
  else {
    // Send NULL: request data
    data = transfer_SPI(NULL);
    if (data != NULL){
      Serial.write(data);
    }
  }
  delay(1);
}





// Slave of SPI
#include <LiquidCrystal_I2C.h>
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char buf[10];
volatile uint8_t data;
volatile uint8_t pos = 0;
volatile boolean flush = false;

ISR (SPI_STC_vect) {
  if (Serial.available()) {
    data = Serial.read();
    SPDR = data;
  }
  else {
    // data transferred from master
    data = SPDR;

    if (data != NULL) {
      if (data == '\n' || data == '\r') {
        flush = true;
      }
      else {
        buf[pos++] = data;
      }
    }

    if (pos >= sizeof(buf)) {
      flush = true;
    }
  }
}

void setup() {
  Serial.begin(9600);

  // SPI (Serial Peripheral Interface) - SLAVE
  // SPCR (SPI Control Register) = 0b 1100 0001;
  // => (1) SPIE: SPI Interrupt Enable
  // => (1) SPE: SPI Enable
  // => (0) MSTR: Master/Slave Select
  // SPR1:0 = 01: prescaler 16
  // SPSR (SPI Status Register) = 0b 0000 0000;
  // => SPI2X = 0: Double SPI Speed Bit

  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);

  // Set registers
  SPCR |= (1 << SPIE) | (1 << SPE) | (0 << SPR1) | (1 << SPR0);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
}

void loop() {
  if (flush) {
    lcd.clear();
    buf[pos] = NULL;

    Serial.println(buf);
    lcd.setCursor(1, 1);
    lcd.print(buf);

    pos = 0;
    flush = false;
  }
  delay(1);
}


