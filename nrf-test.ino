#include <SPI.h>
#include "src/Nrf/Nrf.h"

RF24 rfradio(7, 8);
Nrf nrf(&rfradio);

const byte address[6] = "00001";

void setup()
{
    Serial.begin(115200);

    nrf.radio->begin();
    nrf.radio->enableDynamicPayloads();

    // Do not use 0 as reading pipe! This pipe is already in use ase writing pipe
    // nrf.radio->openWritingPipe(address);
    nrf.radio->openReadingPipe(1, address);
    nrf.radio->startListening();
}

void loop()
{
    receive();
}

void send()
{
    char message[32] = "Hello World!";
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
    }
}