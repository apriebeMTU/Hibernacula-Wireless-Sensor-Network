#include <SPI.h>
#include <RFM69.h>

//------------------------------------------------------
// Addresses
//------------------------------------------------------
#define NETWORKID          100
#define BASE_ADDRESS       1
#define REPEATER_ADDRESS   2
#define NODE_ADDRESS       3

//------------------------------------------------------
// Pins
//------------------------------------------------------
#define RFM69_CS_B     21
#define RFM69_CS_N     10

#define RFM69_INT      2
#define RFM69_RST      14

#define GREEN_LED      6
#define ORANGE_LED     7
#define RED_LED        8

#define POWER_PIN      9

//------------------------------------------------------
// Radios
//------------------------------------------------------
RFM69 nRadio(RFM69_CS_N, RFM69_INT);
RFM69 bRadio(RFM69_CS_B, RFM69_INT);

//------------------------------------------------------

char call[] = "Hello World!";
char response[] = "And hello back to you";

// true = listen to base
// false = talk to node
bool dir = true;

//------------------------------------------------------

// Write all the LEDs low 
void clearIndicators(){
  digitalWrite(RED_LED, LOW);
  digitalWrite(ORANGE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);   
}

//------------------------------------------------------

void setup()
{
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(POWER_PIN, OUTPUT);
  
  pinMode(RFM69_CS_B, OUTPUT);
  digitalWrite(RFM69_CS_B, HIGH);

  pinMode(RFM69_CS_N, OUTPUT);
  digitalWrite(RFM69_CS_N, HIGH);

  pinMode(RFM69_RST, OUTPUT);

  digitalWrite(POWER_PIN, HIGH);

  SPI.begin();

  digitalWrite(RFM69_RST, LOW);
  delay(5);
  digitalWrite(RFM69_RST, HIGH);
  delay(5);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!bRadio.initialize(RF69_915MHZ, REPEATER_ADDRESS, NETWORKID)){
    digitalWrite(RED_LED, HIGH);
    while(1);
  }

  bRadio.setHighPower();
  bRadio.setPowerLevel(31);

  if (!nRadio.initialize(RF69_915MHZ, REPEATER_ADDRESS, NETWORKID)){
    digitalWrite(ORANGE_LED, HIGH);
    while(1);
  }

  nRadio.setHighPower();
  nRadio.setPowerLevel(31);    

  digitalWrite(GREEN_LED, HIGH);
  delay(3000);
}

//------------------------------------------------------

void loop()
{
  clearIndicators();
  //--------------------------------------------------
  // Listen to base station
  //--------------------------------------------------
  if (dir){
    bRadio.setMode(RF69_MODE_RX);   //Maybe should or could be RF69_MODE_STANDBY
    nRadio.setMode(RF69_MODE_SLEEP);
    delay(10);
    digitalWrite(RED_LED, HIGH);
    
    Serial.println("Listening...");
    unsigned long start = millis();
    while (millis() - start < 3000){
      if (bRadio.receiveDone()){
         Serial.print("Received from ");
         Serial.println(bRadio.SENDERID);

         Serial.print("Message: ");
         Serial.println((char *)bRadio.DATA);

         if (bRadio.ACKRequested()){
            bRadio.sendACK();
            bRadio.send(bRadio.SENDERID, response, sizeof(response));        
         }
         break;
      }
    }
    digitalWrite(GREEN_LED, HIGH);
  }

  //--------------------------------------------------
  // Talk to node
  //--------------------------------------------------
  else{
    bRadio.setMode(RF69_MODE_SLEEP);
    nRadio.setMode(RF69_MODE_TX);    // Maybe should or could be RF69_MODE_STANDBY
    delay(10);
    digitalWrite(RED_LED, HIGH);
    bool ok = nRadio.sendWithRetry(NODE_ADDRESS, call, sizeof(call));

    if (ok){
      Serial.println("Packet acknowledged.");
      unsigned long start = millis();

      while (millis() - start < 2000){
        if (nRadio.receiveDone()){
          Serial.print("Reply: ");
          Serial.println((char *)nRadio.DATA);

          if (nRadio.ACKRequested()){
            nRadio.sendACK();
          }
          break;
        }
      }
    }
    else{
      Serial.println("No ACK received.");
    }
    digitalWrite(ORANGE_LED, HIGH);
  }

  delay(500);
  dir = !dir;
}
