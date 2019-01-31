#include <SPI.h>
#include "src/Nrf/Nrf.h"

RF24 rfradio(0, 2);
Nrf nrf(&rfradio);

const byte address[][6] = {"00006", "00008"};

void setup()
{
    Serial.begin(115200);

    nrf.radio->begin();
    nrf.radio->enableDynamicPayloads();

    // Do not use 0 as reading pipe! This pipe is already in use ase writing pipe
    nrf.radio->openWritingPipe(address[0]);
    nrf.radio->openReadingPipe(1, address[1]);
    nrf.radio->startListening();
}

void loop()
{
    send();
    bool received = nrf.waitForResponse();
    if(received){
      char messsage[32] = "";
      nrf.readMessage(messsage);
      if (strlen(messsage) != 0)
      {
        Serial.println(messsage);
      }
    }
}

void send()
{
    char message[32] = "f1000";
    bool ok = nrf.sendMessage(message, sizeof(message));

    if (ok)
    {
        Serial.println("Message received!");
    }
    else
    {
        Serial.println("Message not received :-(");
    }

    delay(1000);
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
  Serial.println(input);
  String output = "";
  for(int i = 1; i < 32; i++){
    if(isdigit(input[i])){
      output += input[i];
    }else{
      break;
    }
  }
  Serial.print("String: ");
  Serial.println(output);
  return output.toInt();
}
