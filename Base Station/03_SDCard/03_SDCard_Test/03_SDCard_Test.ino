// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality

// Includes 
#include <SPI.h>
#include <SD.h>

File myFile;

// change this to match your SD shield or module;
const int CS_PIN = 16;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // init card
  Serial.print("Initializing SD card...  ");
  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // check card type
  uint8_t type = SD.cardType();
  Serial.print("Card type: ");
  
  switch(type)
  {
      case CARD_MMC: Serial.println("MMC"); break;
      case CARD_SD: Serial.println("SDSC"); break;
      case CARD_SDHC: Serial.println("SDHC"); break;
      default: Serial.println("Unknown");
  }

  // check card size  
  Serial.print("Card Size MB: ");
  Serial.println(SD.cardSize() / (1024 * 1024));

  // list files 
  File root = SD.open("/");
  while(true)
  {
      File entry = root.openNextFile();
  
      if(!entry)
          break;
  
      Serial.print("Found: ");
      Serial.println(entry.name());
  
      entry.close();
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("/test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("/test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop()
{
  // nothing happens after setup
}
