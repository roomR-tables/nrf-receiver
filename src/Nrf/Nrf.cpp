#include "Arduino.h"
#include "Nrf.h"

Nrf::Nrf(RF24 *rfradio)
{
    radio = rfradio;
}

void Nrf::readMessage(char *message)
{
    if (radio->available())
    {
        char message[32] = "";
        
        while (radio->available())
        {
            uint8_t len = radio->getDynamicPayloadSize();
            radio->read(message, len);
        }

        Serial.println(message);
    }
}

bool Nrf::sendMessage(char *message, uint8_t len)
{
    radio->stopListening();
    bool ok = radio->write(message, len);
    radio->startListening();

    return ok;
}

bool Nrf::waitForResponse()
{
    unsigned long started_waiting_at = millis();
    bool timeout = false;

    while (!radio->available() && !timeout)
    {
        if (millis() - started_waiting_at > 200)
        {
            timeout = true;
        }
    }

    return !timeout;
}