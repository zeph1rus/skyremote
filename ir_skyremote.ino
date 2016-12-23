#include <IRremote.h>
#include <IRremoteInt.h>
#include "ir_skyremote.h"


int sending = 0;
unsigned long lastsent;
IRsend irsend;


void setup()
{
  // Pin 9 is output for IR setup is done in the library
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DN, INPUT_PULLUP);
  pinMode(PIN_L, INPUT_PULLUP);
  pinMode(PIN_R, INPUT_PULLUP);
  pinMode(PIN_GUIDE, INPUT_PULLUP);
  pinMode(PIN_SELECT, INPUT_PULLUP);
  pinMode(PIN_BACKUP, INPUT_PULLUP);
  pinMode(PIN_CHUP, INPUT_PULLUP);
  pinMode(PIN_CHDN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Setup Complete");
}

int scanKeys () {
  int keyscanned = 0;
  //scan_directions
  for ( int i = 10; i < 14 ; i++) {
    if (digitalRead(i) == LOW) {
      keyscanned = i;
    }
  }
  for ( int i = 2; i < 7 ; i++) {
    if (digitalRead(i) == LOW) {
      keyscanned = i;
    }
  }
  return keyscanned;
}

void sendSkyCode (unsigned long code) {
  for (int i = 0; i < 3; i++) {
    //RC6 requires we send the code three times
    //We send keyup event seperately once it's been released. 
    Serial.println("Sending Code");
    Serial.println(code,HEX);
    irsend.sendRC6(code, 24);
  }
  lastsent = code;
  sending = 1;
}

void sendSkyCodeOnce (unsigned long code) {
  Serial.println("Sending Code Once");
  Serial.println(code, HEX);
  irsend.sendRC6(code, 24);
}


void loop()
{

  switch (scanKeys()) {
    case PIN_UP:
      sendSkyCode(SKYPLUS_UP);
      break;
    case PIN_DN:
      sendSkyCode(SKYPLUS_DN);
      break;
    case PIN_L:
      sendSkyCode(SKYPLUS_L);
      break;
    case PIN_R:
      sendSkyCode(SKYPLUS_R);
      break;
    case PIN_GUIDE:
      sendSkyCode(SKYPLUS_GUIDE);
      break;
    case PIN_BACKUP:
      sendSkyCode(SKYPLUS_BACKUP);
      break;
    case PIN_CHUP:
      sendSkyCode(SKYPLUS_CHUP);
      break;
    case PIN_CHDN:
      sendSkyCode(SKYPLUS_CHDN);
      break;
    case PIN_SELECT:
      sendSkyCode(SKYPLUS_SELECT);
      
      break;
    case 0:
    default:
      /* Send code with bit 4 inverted, essentially 'keydown event' */
      if (sending == 1) {
        sendSkyCodeOnce(lastsent + INVERT_TOGGLE);
        sending = 0;
      }
      break;

  }
  delay(125);
}

