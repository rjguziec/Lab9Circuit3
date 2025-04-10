/*
  Lab 9, Circuit 3 SECONDARY
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

// Initialize global variables
// pNum me, sNum other Arduino
volatile unsigned char pNum = 0;
volatile unsigned char sNum = 0;

void setup() {
  // GLobally disable interrupts
  cli();
  randomSeed(12);  // Secondary seed

  // Configure SPI pins
  // MISO only output (D12)
  DDRB = 0x10;

  // Configure for secondary mode
  SPCR = 0x70;
  PORTB &= 0xFB; // SS low (grounded anyway)        // LOOK HERE FOR PROBLEMS YO

  // 7-Segment output (D4-D7)
  DDRD |= 0xF0;

  // RGB LED output (RED = D8, GREEN = D9, BLUE = D3)
  DDRB |= 0x03;   // D8, D9
  DDRD |= 0x08;   // D3

  // External Interrupt on D2 (INT0)
  EICRA = 0x03;   // Rising edge
  EIMSK = 0x01;   // Enable INT0

  sei();
}

void loop() {
  // Nothing to see here...
}

ISR(INT0_vect) {
  // Generate random number
  pNum = random(1,10);

  // Display random number (pNum) on 7-segment (BCD)
  // Clear display
  PORTD &= 0x0F; 
  // Shift pNum from D0-D3 to D4-D7 and display on BCD
  PORTD |= (pNum << 4); 

  // SPI send & receive
  sNum = spiTxRx(pNum);

  // Compare results                                // DON'T 4GET TO CODE THE RGB RESULT
  // RGB LED Green
  // YOU WIN
  if(pNum > sNum) {
    // Green
  }
  // RGB LED Red
  // YOU LOST
  else if(pNum < sNum) {
    // Red
  }
  // RGB LED Blue
  // TIE
  else {
    // Blue
  }
}

unsigned char spiTxRx(unsigned char data) {
  SPDR = data;
  
  // Wait for transfer complete
  while (!(SPSR & (1 << SPIF)));  
  
  return SPDR;
}
