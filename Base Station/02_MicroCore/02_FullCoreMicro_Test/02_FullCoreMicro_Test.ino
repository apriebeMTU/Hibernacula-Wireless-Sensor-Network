// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality

// Includes 
#include <Wire.h>
#include <MCP7940.h>

//Defines 
#define MCP7940_ADDR 0x6F

// Define pins
#define MFP_PIN     25     // MFP connected to D3
#define LED_PIN     2    // Onboard LED
#define SDA_PIN     21
#define SCL_PIN     22
#define BUTTON_PIN  13

//Enums 
/*! ///< Enumeration of MCP7940 alarm types */
enum alarmTypes {
  matchSeconds,
  matchMinutes,
  matchHours,
  matchDayOfWeek,
  matchDayOfMonth,
  Unused1,
  Unused2,
  matchAll,
  Unknown
};


//Globals 
MCP7940_Class rtc;
DateTime now;
DateTime alarmTime;


const uint8_t  ALARM_INTERVAL{2};     ///< Interval seconds for alarm
const uint32_t SERIAL_SPEED{115200};

volatile bool alarmTriggered = false;
volatile bool button = true;

//ISRs
void wakeISR() {
  alarmTriggered = true; 
}

void buttonISR(){
  button = !button; 
}

//User Functions 

// Configures the EXTOSC bit for an external clock source
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

void setupAlarm(MCP7940_Class MCP7940) {
  MCP7940.setSQWState(false);
  MCP7940.setAlarmPolarity(true);
}


//Main
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MFP_PIN, INPUT_PULLUP);  // MFP is open-drain
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // MFP is open-drain

  attachInterrupt(digitalPinToInterrupt(MFP_PIN), wakeISR, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);

  Serial.begin(SERIAL_SPEED);
  Wire.begin(SDA_PIN, SCL_PIN);
  startRTC(rtc);
  setupAlarm(rtc);
  alarmTriggered = true;

}

void loop() {
  if (alarmTriggered) {
    Serial.println("Alarm triggered!");

    if (button){
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
    else{
      digitalWrite(LED_PIN, LOW);
    }

    //reset alarm
    now = rtc.now();
    alarmTime = now + TimeSpan(0, 0, 0, ALARM_INTERVAL);

    rtc.clearAlarm(0); 
    rtc.setAlarm(0, matchAll, alarmTime, true);

    Serial.print("The time is: ");
    Serial.print(now.hour()); Serial.print(":");
    Serial.print(now.minute()); Serial.print(":");
    Serial.println(now.second());

    Serial.print("Alarm set for ");
    Serial.print(alarmTime.hour()); Serial.print(":");
    Serial.print(alarmTime.minute()); Serial.print(":");
    Serial.println(alarmTime.second());

    alarmTriggered = false;
  }
}
