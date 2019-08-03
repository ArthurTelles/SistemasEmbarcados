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


char moveRight[] = "G1 X100\r\n";
char moveLeft[] = "G1 X0\r\n";
char moveUp[] = "G1 Y100\r\n";
char moveDown[] = "G1 Y0\r\n";

int lenghtRight = strlen(moveRight);
int lenghtLeft = strlen(moveLeft);
int lenghtUp = strlen(moveUp);
int lenghtDown = strlen(moveDown);
uint8_t rcode;

void (* resetFunc)(void) = 0;

void setup()
{
  Serial.begin( 250000 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("W = UP ; S = DOWN ; A = LEFT ; D = RIGHT"); // Once the conections is established the user can use the keyboard to move the 3D printer.

  if (Usb.Init() == -1)
      Serial.println("OSCOKIRQ failed to assert");

  
  delay( 2000 );
}

void loop()
{
    Usb.Task();

    if( Acm.isReady()) {

       if(Serial.available()) { // If the serial is avaible and the user used a valid input the program will call the respective function.
        int select = Serial.read();

        if (select == 100) sendToRight();
        
        if (select == 97) sendToLeft();
        
        if (select == 119) sendToUp();
        
        if (select == 115) sendToDown();

        Serial.println(select);

        delay(5000);
        resetFunc(); //After the movement is completed the system is restarted so the comunication can be re-established.
        delay(100);

         if (rcode) ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
       }
    }
}

int sendToRight() // Sends the GCode that informs the 3D printer to move to the RIGHT in the X axis.
{
  for (int i = 0; i < lenghtRight; i++){
    uint8_t data = moveRight[i];
    rcode = Acm.SndData(1, &data);
  }

  for (int i = 0; i < lenghtRight; i++){
    uint8_t data = moveRight[i];
    rcode = Acm.SndData(1, &data);
  }
  Serial.println("                         Moved right");
}

int sendToLeft() // Sends the GCode that informs the 3D printer to move to the LEFT in the X axis.
{
    for (int i = 0; i < lenghtLeft; i++){
    uint8_t data = moveLeft[i];
    rcode = Acm.SndData(1, &data);
  }
  
  for (int i = 0; i < lenghtLeft; i++){
    uint8_t data = moveLeft[i];
    rcode = Acm.SndData(1, &data);
  }
  Serial.println("                         Moved left");
}

int sendToUp() // Sends the GCode that informs the 3D printer to move UP in the Y axis.
{
    for (int i = 0; i < lenghtUp; i++){
    uint8_t data = moveUp[i];
    rcode = Acm.SndData(1, &data);
  }
  
  for (int i = 0; i < lenghtUp; i++){
    uint8_t data = moveUp[i];
    rcode = Acm.SndData(1, &data);
  }
  Serial.println("                         Moved up");
}

int sendToDown() // Sends the GCode that informs the 3D printer to move DOWN in the Y axis.
{
    for (int i = 0; i < lenghtDown; i++){
    uint8_t data = moveDown[i];
    rcode = Acm.SndData(1, &data);
  }
  
  for (int i = 0; i < lenghtDown; i++){
    uint8_t data = moveDown[i];
    rcode = Acm.SndData(1, &data);
  }
  Serial.println("                         Moved Down");
}
