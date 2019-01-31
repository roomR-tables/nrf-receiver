#include <SPI.h>
#include "src/Nrf/Nrf.h"

RF24 rfradio(7, 8);
Nrf nrf(&rfradio);

const byte address[][6] = {"00006", "00008"};

void setup()
{
    Serial.begin(115200);

    nrf.radio->begin();
    nrf.radio->enableDynamicPayloads();

    // Do not use 0 as reading pipe! This pipe is already in use ase writing pipe
    nrf.radio->openReadingPipe(1, address[0]);
    nrf.radio->startListening();
}

void loop()
{
    receive();
}

void receive()
{
    char message[32] = "";
    nrf.readMessage(message);

    if (strlen(message) != 0)
    {
        Serial.println(message);
        char dir = getDirection(message);
        Serial.println(dir);
        int distance = getDistance(message);
        Serial.println(distance);
    }
}

char getDirection(char input[]){
  return input[0];
}

int getDistance(char input[]){
  String output = "";
  for(int i = 1; i < 32; i++){
    if(isdigit(input[i])){
      output += input[i];
    }else{
      break;
    }
  }
  return output.toInt();
}
