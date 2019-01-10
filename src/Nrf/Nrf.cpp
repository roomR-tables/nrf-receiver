#include "Nrf.h"

Nrf::Nrf(int ce, int csn)
{
    RF24 radio(ce, csn);
    this->radio = &radio;
}

void Nrf::readMessage(char *message)
{
    if (this->radio->available())
    {
        while (this->radio->available())
        {
            uint8_t len = this->radio->getDynamicPayloadSize();
            this->radio->read(message, len);
        }
    }
}

bool Nrf::sendMessage(char *message)
{
    this->radio->stopListening();
    bool ok = this->radio->write(message, sizeof(message));
    this->radio->startListening();

    return ok;
}

bool Nrf::waitForResponse()
{
    unsigned long started_waiting_at = millis();
    bool timeout = false;

    while (!this->radio->available() && !timeout)
    {
        if (millis() - started_waiting_at > 200)
        {
            timeout = true;
        }
    }

    return !timeout;
}