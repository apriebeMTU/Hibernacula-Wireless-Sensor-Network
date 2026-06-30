// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality
// Credit: This program is an adapted version of the SetAndCalibrate.ino example from the MCP7940 
// library authored by SV-Zanshin. The GitHub for the library can be found here: 
// https://github.com/SV-Zanshin/MCP7940


#include <MCP7940.h>  // Include the MCP7940 RTC library
#include <Wire.h>
#define MCP7940_ADDR 0x6F
/***************************************************************************************************
** Declare all program constants                                                                  **
***************************************************************************************************/
const uint32_t SERIAL_SPEED{115200};     // Set the baud rate for Serial I/O
const uint8_t  SPRINTF_BUFFER_SIZE{32};  // Buffer size for sprintf()

const uint8_t  LED_PIN{2};              // Arduino built-in LED pin number
const uint8_t SDA_PIN {21};
const uint8_t SCL_PIN {22};

/***************************************************************************************************
** Declare global variables and instantiate classes                                               **
***************************************************************************************************/
MCP7940_Class rtc;                           // Create an instance of the MCP7940
char inputBuffer[SPRINTF_BUFFER_SIZE];  // Buffer for sprintf()/sscanf()

/***************************************************************************************************
** Configures the RTC to work with the TXCO                                                       **
***************************************************************************************************/
void configureForExternalOscillator() {
  // Clear ST (bit 7) in register 0x00
  Wire.beginTransmission(MCP7940_ADDR);
  Wire.write(0x00);
  Wire.endTransmission(false);
  Wire.requestFrom(MCP7940_ADDR, 1);
  uint8_t sec = Wire.read();
  sec &= ~(1 << 7); // Clear ST
  Wire.beginTransmission(MCP7940_ADDR);
  Wire.write(0x00);
  Wire.write(sec);
  Wire.endTransmission();

  // Set EXTOSC (bit 3) in register 0x07
  Wire.beginTransmission(MCP7940_ADDR);
  Wire.write(0x07);
  Wire.endTransmission(false);
  Wire.requestFrom(MCP7940_ADDR, 1);
  uint8_t control = Wire.read();
  control |= (1 << 3); // Set EXTOSC
  Wire.beginTransmission(MCP7940_ADDR);
  Wire.write(0x07);
  Wire.write(control);
  Wire.endTransmission();
}

// Inits I2C communication with the RTC. It then enables the external oscillator and starts 
// the oscillator 
void startRTC(MCP7940_Class MCP7940){
  while (!MCP7940.begin()) {  // Initialize RTC communications
    Serial.println(F("Unable to find MCP7940M. Checking again in 3s."));  // Show error text
    delay(3000);                                                          // wait a second
  }  // of loop until device is located
  
  Serial.println(F("MCP7940 initialized."));

  configureForExternalOscillator(); // Set the EXTOSC bit
  
  while (!MCP7940.deviceStatus()) {  // Turn oscillator on if necessary
    Serial.println(F("Oscillator is off, turning it on."));
    bool deviceStatus = MCP7940.deviceStart();  // Start oscillator and return state
    if (!deviceStatus) {                        // If it didn't start
      Serial.println(F("Oscillator did not start, trying again."));  // Show error and
      delay(1000);                                                   // wait for a second
    }                // of if-then oscillator didn't start
  }                  // of while the oscillator is off
  
  MCP7940.adjust();  // Set to library compile Date/Time
}

/***************************************************************************************************
** Method Setup(). This is an Arduino IDE method which is called upon boot or restart. It is only **
** called one time and then control goes to the main loop, which loop indefinately.               **
***************************************************************************************************/
void setup() {                 // Arduino standard setup method
  pinMode(LED_PIN, OUTPUT);  // Declare built-in LED as output
  
  Serial.begin(SERIAL_SPEED);  // Start serial port at Baud rate                        
  Wire.begin(SDA_PIN, SCL_PIN);
  startRTC(rtc);
}

/***************************************************************************************************
** This is the main program for the Arduino IDE, it is an infinite loop and keeps on repeating.   **
***************************************************************************************************/
void loop() {
  static uint8_t secs;                 // store the seconds value
  DateTime       now = rtc.now();  // get the current time
  
  if (secs != now.second()) {          // Output if seconds have changed
    sprintf(inputBuffer, "%04d-%02d-%02d %02d:%02d:%02d",
            now.year(),  // Use sprintf() to pretty print
            now.month(), now.day(), now.hour(), now.minute(),
            now.second());                         // date/time with leading zeros
    Serial.println(inputBuffer);                   // Display the current date/time
    secs = now.second();                           // Set the counter variable
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Toggle the LED if the seconds have changed
  }                                                
}
