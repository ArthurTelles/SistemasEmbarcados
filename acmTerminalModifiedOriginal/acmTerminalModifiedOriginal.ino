#include <cdcacm.h>
#include <usbhub.h>

#include "pgmstrings.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class ACMAsyncOper : public CDCAsyncOper
{
public:
    uint8_t OnInit(ACM *pacm);
};

uint8_t ACMAsyncOper::OnInit(ACM *pacm)
{
    uint8_t rcode;
    // Set DTR = 1 RTS=1
    rcode = pacm->SetControlLineState(3);

    if (rcode)
    {
        ErrorMessage<uint8_t>(PSTR("SetControlLineState"), rcode);
        return rcode;
    }

    LINE_CODING	lc;
    lc.dwDTERate	= 250000;
    lc.bCharFormat	= 0;
    lc.bParityType	= 0;
    lc.bDataBits	= 8;

    rcode = pacm->SetLineCoding(&lc);

    if (rcode)
        ErrorMessage<uint8_t>(PSTR("SetLineCoding"), rcode);

    return rcode;
}

USB     Usb;
//USBHub     Hub(&Usb);
ACMAsyncOper  AsyncOper;
ACM           Acm(&Usb, &AsyncOper);


char moveRight[] = "G0 X100\r\n";
char moveLeft[] = "G0 X0\r\n";
int lenghtRight = strlen(moveRight);
int lenghtLeft = strlen(moveLeft);
uint8_t rcode;

void setup()
{
  Serial.begin( 250000 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Digit 0 for right and 1 for left");

  if (Usb.Init() == -1)
      Serial.println("OSCOKIRQ failed to assert");

  delay( 200 );
}

void loop()
{
    Usb.Task();

    if( Acm.isReady()) {

       if(Serial.available()) {

        int select = Serial.read();

        if (select == 48) sendToRight();
        if (select == 49) sendToLeft();

        Serial.println(select);

         if (rcode) ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
       }
    }
}

int sendToRight()
{
  for (int i = 0; i < lenghtRight; i++){
    uint8_t data = moveRight[i];
    rcode = Acm.SndData(1, &data);
    Serial.print(moveRight[i]);
  }
  for (int i = 0; i < lenghtRight; i++){
    uint8_t data = moveRight[i];
    rcode = Acm.SndData(1, &data);
    Serial.print(moveRight[i]);
  }
  Serial.println("                         Moved right");
}

int sendToLeft()
{
    for (int i = 0; i < lenghtLeft; i++){
    uint8_t data = moveLeft[i];
    rcode = Acm.SndData(1, &data);
    Serial.print(moveLeft[i]);
  }
  for (int i = 0; i < lenghtLeft; i++){
    uint8_t data = moveLeft[i];
    rcode = Acm.SndData(1, &data);
    Serial.print(moveLeft[i]);
  }
  Serial.println("                         Moved left");
}
