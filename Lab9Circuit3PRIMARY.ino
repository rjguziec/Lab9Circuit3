/*
  Lab 9, Circuit 3 PRIMARY
  Written: April 10, 2025
  I/O Pins
  ...
  D2: PushButton
  D4: MSB for 7 segment BCD (Bit 3)
  D5: 7 segment bit 2
  D6: 7 Segment bit 1
  D7: LSB for 7 segment BCD (Bit 0)
  D10: SS (pin 1) on 74165
  D11: MOSI (pin 10) on 74165
  D12: MISO (pin 9) on 74165
  D13: SLK (pin 2) on 74165
*/

// Gross global variables
unsigned char pNum = 0;  // Secondary number
unsigned char aNum = 0;  // Primary number

void setup() {

  cli();  // Globally disable interrupts

  randomSeed(5);  // Random seed

  // Primary configuration
  DDRB = 0x2C;  // SCK, MOSI, SS are output pins (PRIMARY)
  PORTB &= 0xFB;   // SS low
  SPCR = 0x70;  // Enable SPI, LSB first, primary mode, prescaler of 4

  // 7 Segment configuration
  DDRD = 0xF0;  // 7 Segment BCD output

  // Extneral  interrupt Block
  EICRA = 0x03;  // Enable rising edge trigger for D2
  EIMSK = 0x01;  // Enable external interrupt for D2
  
  sei();  // Globally re-enable interrupts
}

void loop() {

    

}

// External interrupt that chooses a random value for our primary number, 
// and displays it to our 7 Segment BCD
ISR(INT0_vect){
  pNum = random(1,10);  // Pick a random number!
  unsigned char bcdArray[10] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90};  // BCD array

  PORTD &= 0x0F;  // Selectively clear BCD output
  PORTD |= bcdArray[pNum];  // Set BCD Output
}

void spiTxRx(unsigned char dataToWrite) {
  PORTB &= 0xFB; // enable SPI write
  SPDR = dataToWrite;

  // Wait until transfer is complete
  while (!(SPSR & (1 << SPIF)));

  PORTB |= 0x04; // disable SPI write
  
  return SPDR;
}


