
#include "Arduino.h"
#include <nRF24L01.h>
#include <RF24.h>

class Nrf
{
  public:
    RF24 *radio;
    Nrf(int ce, int csn);

    void readMessage(char *message);
    bool sendMessage(char *message);
    bool waitForResponse();
};