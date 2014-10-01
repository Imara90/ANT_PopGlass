/*******************************************************************************************
 *  Initial start for the Ant network communication
 *  a wireless 
 *
 *
 *
 *
 *******************************************************************************************/

// Reviews
// TODO Need to check the settings in antmessage and antdefines!
// TODO Need to set the order of functionality 
// TODO setup the entire confguration
// TODO determine the delays necessary for writing to ANT module
// TODO determine the headers necessary
// TODO add functionality of the accelerometer
// TODO power up and down


///////////////////////////////////////////////////////////////////////////////
// Includes from demo
///////////////////////////////////////////////////////////////////////////////
 
//#include "ANTInterface.h"
//#include "System.h"
//#include "timer.h"
#include "antdefines.h"
#include "antmessage.h" 	/// Displays the message sizes for the ANT communication
#include "types.h"
// #include "Atod.h" /// also just necessary for the MP controller
//#include "Config.h" /// defines the MP430F2013 microcontroller, but we use Arduino
// #include "BitSyncSerial.h" /// needs configuration

// Arduino Standard libraries
#include <SPI.h>
 

///////////////////////////////////////////////////////////////////////////////
// ANT Network Setup definitions
///////////////////////////////////////////////////////////////////////////////

#define CHANNEL_0_FREQUENCY_HOP

// Network Settings
#define DEFAULT_NETWORK               ((UCHAR) 0x00)

// RF Frequency
#define DEFAULT_RF_FREQ               ((UCHAR) 0x42)     ///< decimal 66 (2466 MHz)

// Transmission Type
#define TRANS_TYPE                    ((UCHAR) 0x01)    

// Device Types
#define DEVICE_TYPE                   ((UCHAR) 0x01)	

// Device Number (Pre-defined, no Device pairing supported)
#define DEVICE_NUM                    ((USHORT) 0x0031)  ///< decimal 49
#define DEVICE_NUM_MSB                ((UCHAR) 0x00)
#define DEVICE_NUM_LSB                ((UCHAR) 0x31)

// Message Periods
#define MSG_PERIOD_4HZ                ((USHORT) 0x2000)	 ///< decimal 8192 (4Hz)
#define MSG_PERIOD_4HZ_MSB            ((UCHAR) 0x20)	 ///< decimal 8192 (4Hz)
#define MSG_PERIOD_4HZ_LSB            ((UCHAR) 0x00)	 ///< decimal 8192 (4Hz)

// Channel Number
#define CHANNEL_0                     ((UCHAR) 0x00)


/////////////////////////////////////////////////////////////////////////////
// Channel 0 Configuration Commands
static const UCHAR aucANTAssignChannel0[]    = {MESG_ASSIGN_CHANNEL_SIZE, MESG_ASSIGN_CHANNEL_ID, CHANNEL_0, CHANNEL_TYPE_MASTER, DEFAULT_NETWORK};               ///< Channel 0, Master Channel, Network 0 
static const UCHAR aucANTSetChannelID0[]     = {MESG_CHANNEL_ID_SIZE, MESG_CHANNEL_ID_ID, CHANNEL_0, DEVICE_NUM_LSB, DEVICE_NUM_MSB, DEVICE_TYPE, TRANS_TYPE};    ///< Channel 0, Device ID 49, Device Type 1, Man ID 3
static const UCHAR aucANTSetChannelPeriod0[] = {MESG_CHANNEL_MESG_PERIOD_SIZE, MESG_CHANNEL_MESG_PERIOD_ID, CHANNEL_0, MSG_PERIOD_4HZ_LSB, MSG_PERIOD_4HZ_MSB};   ///< Channel 0, Mesg Period 4 Hz
static const UCHAR aucANTSetChannelRFFreq0[] = {MESG_CHANNEL_RADIO_FREQ_SIZE, MESG_CHANNEL_RADIO_FREQ_ID, CHANNEL_0, DEFAULT_RF_FREQ};                            ///< Channel 0, RF Freq 2466 MHz
static const UCHAR aucANTOpenChannel0[]      = {MESG_OPEN_CHANNEL_SIZE, MESG_OPEN_CHANNEL_ID, CHANNEL_0};                                                         ///< Open Channel 0

#if defined (CHANNEL_0_FREQUENCY_HOP)
   #define NUM_FREQUENCY_HOPS       ((UCHAR) 0x04)
   #define RF_FREQ_2                ((UCHAR) 0x45)
   #define RF_FREQ_3                ((UCHAR) 0x48)
   #define RF_FREQ_4                ((UCHAR) 0x4B)
   
   static const UCHAR  aucANTSetChannelRFFreq1[]   = {MESG_CHANNEL_RADIO_FREQ_SIZE, MESG_CHANNEL_RADIO_FREQ_ID, CHANNEL_0, RF_FREQ_2};
   static const UCHAR  aucANTSetChannelRFFreq2[]   = {MESG_CHANNEL_RADIO_FREQ_SIZE, MESG_CHANNEL_RADIO_FREQ_ID, CHANNEL_0, RF_FREQ_3};
   static const UCHAR  aucANTSetChannelRFFreq3[]   = {MESG_CHANNEL_RADIO_FREQ_SIZE, MESG_CHANNEL_RADIO_FREQ_ID, CHANNEL_0, RF_FREQ_4};
   static const USHORT apucANTSetChannelRFFreqHop[] = {(USHORT)aucANTSetChannelRFFreq0, (USHORT)aucANTSetChannelRFFreq1, (USHORT)aucANTSetChannelRFFreq2, (USHORT)aucANTSetChannelRFFreq3};
#endif // CHANNEL_0_FREQUENCY_HOP

// Channel 0 Configuration Command List
static const USHORT apucChannelConfig0[] ={  (USHORT)aucANTAssignChannel0,\
                                             (USHORT)aucANTSetChannelID0,\
                                             (USHORT)aucANTSetChannelPeriod0,\
                                             (USHORT)aucANTSetChannelRFFreq0,\
                                             (USHORT)aucANTOpenChannel0\
                                          };

/////////////////////////////////////////////////////////////////////////////
// Channel N Configuration Commands

// Next channel configuration block


/////////////////////////////////////////////////////////////////////////////
// Function declaration

void message();
uint8_t checkSum(UCHAR *buf, int length);
void ANT_send(uint8_t buf[], int length);
void write_test(uint8_t *message, int length);

/////////////////////////////////////////////////////////////////////////////
// delays

const int srdy_delay = 3; // minimum of 2.5 micro seconds 


/////////////////////////////////////////////////////////////////////////////
// Message headers

#define ANT_ResetSystem 0x4A     // software reset does not change synchronization


/////////////////////////////////////////////////////////////////////////////
// Pinout declaration
// MOSI - D11, MISO - D12, SCK - D13, but could virtually be any pin for the ANT

#define LEDPIN 5
#define ChipSelectPin 10
#define RESETPIN 4
#define SRDY 6
#define SEN 2 // INT0
#define SMSGRDY 7


void setup()
{ 
  // The arduino is the slave device while the ANT device is master
    // Set outputs
  SPI.begin();                         // Sets the inputs and outputs, pulling SCK and MOSI low and SS high
  pinMode(ChipSelectPin, OUTPUT); 
  
  Serial.begin(115200);
  
  // LED for debugging purposes
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  
  // Pins for configuration
  pinMode(RESETPIN, OUTPUT);           // Active low, so should be pulled high otherwise
  pinMode(SEN, INPUT);   // TODO input or interrupt - Ant signals a message
  pinMode(SRDY, OUTPUT); // active low indicator toggles to receive per byte
  pinMode(SMSGRDY, OUTPUT); // indicating the host sending to the ANT module, also active low
  
  // Active low configurations
  digitalWrite(RESETPIN, HIGH);        // Active low  
  digitalWrite(SRDY, HIGH); // active low
  digitalWrite(SMSGRDY, HIGH); // active low
  
   // initialize SPI:
  SPI.setBitOrder(LSBFIRST);           // Set the mode according to the ANT master (LSB first)
  SPI.setDataMode(SPI_MODE3);          // Base value clock is 1, data captured on rising edge (CPOL = 1, CHPA = 1) 
  SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 is default, should be 4

  // Reset the ANT module - TODO experiment where we should do this, no difference at front 
  digitalWrite(RESETPIN, LOW);         // active
  delay(500);
  digitalWrite(RESETPIN, HIGH);        // low
  
  byte resetted = SPI.transfer(ANT_ResetSystem);
  Serial.print(resetted, HEX);
  Serial.println(" , resetted!");
  
  // TODO initialize all byte serial devices
  
  // TODO enable the interrupts
  
  // TODO setup the channel
  // TODO setup shared adress

  
  // Signal setup complete
  digitalWrite(LEDPIN, HIGH);
  delay(500);  
}


void loop()
{
  
  uint8_t test_message[8] = {0xA5, 0x01, 0x4B, 0xEE};
  write_test(test_message, 4);
  delay(500);
  Serial.println("\n\n\n\nboop\n\n\n\n\n");
}

void write_test(uint8_t *message, int length)
{ 
  byte syncbyte = 0;
  
  // toggle the SMSGRDY to indicate transmission
 digitalWrite(SMSGRDY, LOW);
 if (digitalRead(SEN) == 0)
 {
   Serial.println("Directly SEN low");
 }
 while(digitalRead(SEN))
 {
    // Wait for SEN to toggle, indicating ANT is ready to receive
   Serial.println("Waiting for ANT to ack and toggle SEN");
 }
 
 digitalWrite(SRDY, LOW);       // TODO hardware or software SRDY??
 delayMicroseconds(srdy_delay);
 digitalWrite(SRDY, HIGH);
 
 // write slave select low to select ANT device
 //digitalWrite(ChipSelectPin, LOW);
 
 // 0x00 to select the first incoming byte
 syncbyte = SPI.transfer(0x00);
 Serial.print("syncbyte: ");
 Serial.println(syncbyte, HEX);
 
 // if ANT allows sending and returns a readflag (1), send the rest of the package
 if (syncbyte & 0x80)
 {
     // set SMSGRDY high again for it is actively low
    digitalWrite(SMSGRDY, HIGH); 
    
    // Write the message byte by byte after the sync byte
    for (int i = 1; i < length; i++)
    {
      Serial.print(".");
      // toggle SRDY indicating ready to send
      digitalWrite(SRDY, LOW);       // TODO hardware or software SRDY??
      delayMicroseconds(srdy_delay);
      digitalWrite(SRDY, HIGH);
      
      // Transfer a single byte
      SPI.transfer(message[i]);
      
      delayMicroseconds(100); // Not sure whether this is necessary
      
    }
    Serial.println("*");
 }
 
 // Write ss high again
 //digitalWrite(ChipSelectPin, HIGH);
 
}

// create checksum for message
uint8_t checkSum(UCHAR *buf, int length)
{
  int i;
  UCHAR chksum = buf[0];
  for (i = 1; i < length; i++)
  {
    chksum ^= buf[i]; 
  }
  return chksum;
}
