#define DATAIN 11//MOSI
#define DATAOUT  12//MISO 
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss

#define MSGREADY 3
#define SRDY 4

#define PINLED 6

volatile byte data;

// delays
int srdy_delay = 3;

void setup()
{
  Serial.begin(9600);

  // SPI setup
  pinMode(DATAIN, INPUT);
  pinMode(DATAOUT, OUTPUT);
  pinMode(SPICLOCK, INPUT);
  pinMode(SLAVESELECT, INPUT);

  // MCU outputs to signal readyness
  pinMode(MSGREADY, OUTPUT);
  pinMode(SRDY, OUTPUT);

  // IO pins
  pinMode(PINLED, OUTPUT);
  digitalWrite(PINLED, HIGH);
  delay(500);
  digitalWrite(PINLED, LOW);

  // Set the SPI Control Register (SPCR)
  DDRB &= ~((1<<2)|(1<<3)|(1<<5));   // SCK, MOSI and SS as inputs
  DDRB |= (1<<4);                    // MISO as output

  SPCR &= ~(1<<MSTR);                // Set as slave 
  SPCR |= (1<<SPR0)|(1<<SPR1);       // divide clock by 128
  //SPCR |= (1<<SPIE);                 // Enable SPI Interrupt
  SPCR |= (1<<SPE);                  // Enable SPI

  //sei();

  digitalWrite(PINLED, HIGH);
  
  // TODO manual reset into software

}

/*
ISR (SPI_STC_vect)
{
    data = SPDR;
    // do something with the received data
}
*/


void test_message(uint8_t *message, int length)
{
 // toggle the SMSGRDY to indicate transmission
 digitalWrite(MSGREADY, LOW);
 if (digitalRead(SLAVESELECT) == 0)
 {
    Serial.println("Host enabled"); 
 }
 else
 {
   while(digitalRead(SLAVESELECT))
   {
     Serial.println("Waiting for host to be enabled");
   }
 }
 
 digitalWrite(SRDY, LOW);       // TODO hardware or software SRDY??
 delayMicroseconds(srdy_delay);
 digitalWrite(SRDY, HIGH);
 
 // Wait untill received sync byte
 while(!(SPSR & (1 << SPIF)));    // Wait untill the data is received
 data = SPDR;                     // received data
 Serial.print("syncbyte: ");
 Serial.println(data, HEX);
 
 Serial.print("sending: ");
 for (int i = 1; i < length; i++)
 {
   digitalWrite(SRDY, LOW);       // TODO hardware or software SRDY??
   delayMicroseconds(srdy_delay);
   digitalWrite(SRDY, HIGH);
   
   SPDR = message[i];              // send the data
   Serial.print("[");
   Serial.print(message[i], HEX);
   Serial.print("]");
   while(!(SPSR & (1 << SPIF)));   // wait untill transmission is complete
 
 }
 Serial.println("#");
}

void loop()
{
  uint8_t message[4] = { 0xA5, 0x01, 0x4B, 0xEE };
  test_message(message, 4);
  
  //Serial.println(data, HEX);

}

