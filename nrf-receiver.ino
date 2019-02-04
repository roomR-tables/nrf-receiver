#include <SPI.h>
#include "src/Nrf/Nrf.h"

RF24 rfradio(7, 8);
Nrf nrf(&rfradio);

const byte address[][12] = {"arduino_read", "pi_read"};
//const byte address[][6] = {"00006", "00008"};

void setup()
{
    Serial.begin(115200);

    nrf.radio->begin();
    nrf.radio->enableDynamicPayloads();

    // Do not use 0 as reading pipe! This pipe is already in use ase writing pipe
    nrf.radio->openWritingPipe(address[1]);
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
    nrf.readMessage(message, nrf.radio->getDynamicPayloadSize());
    if (strlen(message) != 0)
    {
        Serial.println(message);
        char dir = getDirection(message);
        Serial.println(dir);
        int distance = getDistance(message);
        Serial.println(distance);

        char sendMsg[32] = "Done";
        bool ok = nrf.sendMessage(sendMsg, sizeof(sendMsg));
        if (ok){
          Serial.println("Arduino - Message Received");
        }else{
          Serial.println("Arduino - Message Received failed");
        }
    }
}

char getDirection(char input[]){
  return input[0];
}

long getDistance(char input[]){
  String output = "";
  for(int i = 1; i < 32; i++){
    if(isdigit(input[i])){
//      Serial.println(" Digit: " + input[i]);
      output += input[i];
    }else{
      break;
    }
  }
//  Serial.println(output);
  return output.toInt();
}
