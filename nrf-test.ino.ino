#include <SPI.h>
#include "src/Nrf/Nrf.h"

Nrf nrf(0, 2);

const byte pipe[][13] = {"arduino_read", "pi_read"};

void setup()
{
    Serial.begin(115200);

    nrf.radio->begin();
    nrf.radio->enableDynamicPayloads();

    // Do not use 0 as reading pipe! This pipe is already in use ase writing pipe
    nrf.radio->openWritingPipe(pipe[1]);
    nrf.radio->openReadingPipe(1, pipe[0]);
    nrf.radio->startListening();
}

void loop()
{
    ping();
}

void ping()
{
    Serial.print(millis());
    Serial.print(": ");

    // send a message
    char message[32] = "ping";
    bool ok = nrf.sendMessage(message);

    if (ok)
    {
        bool success = nrf.waitForResponse();

        if (!success)
        {
            Serial.println("No response");
        }
        else
        {
            char response[32] = "";
            nrf.readMessage(response);

            Serial.print("Got response; response=");
            Serial.print(response);

            if (strcmp(response, message) != 0)
            {

                Serial.print("; equal=False");
            }
            else
            {
                Serial.print("; equal=True");
            }

            Serial.println();
        }
    }
    else
    {
        Serial.println("Ping not received");
    }

    delay(500);
}
