#include "AM1002_UART.h"

AM1002_UART::AM1002_UART(Stream *uart)
    : uart(uart)
{
}

void AM1002_UART::clearBuffer(void)
{
    idxBuffer = 0;
}

int16_t AM1002_UART::recvData(uint8_t cmd, uint32_t timeout)
{
    clearBuffer();
    uint32_t start = millis();

    uint8_t b;

    while (millis() - start < timeout)
    {
        while (uart->available())
        {
            b = uart->read();

            switch (idxBuffer)
            {
            case 0: // STX
            {
                if (b != AM1002_UART_STX_RCV)
                {
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            case 1: // LEN
            {
                if (b < 1)
                {
                    idxBuffer = 0;
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            case 2: // CMD
            {
                if (b != cmd)
                {
                    idxBuffer = 0;
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            default:
            {
                rxBuffer[idxBuffer++] = b;

                uint8_t len = rxBuffer[AM1002_UART_POS_LEN];

                if (idxBuffer >= len + 3)
                {
                    idxBuffer = 0;

                    uint8_t cs = 0;

                    for (uint8_t i = 0; i < len + 2; i++)
                    {
                        cs += rxBuffer[i];
                    }

                    cs = 256 - cs;

                    if (cs != rxBuffer[len + 2])
                    {
                        return -1;
                    }

                    uint8_t c = rxBuffer[AM1002_UART_POS_CMD];

                    uint8_t dataLength = rxBuffer[AM1002_UART_POS_LEN] - 1;

                    if (dataLength == 0)
                    {
                        return dataLength;
                    }

                    for (uint8_t i = 0; i < dataLength; i++)
                    {
                        *(data + i) = rxBuffer[AM1002_UART_POS_DAT + i];
                    }

                    return dataLength;
                }
                break;
            }
            }
        }
    }

    return -1;
}

bool AM1002_UART::measure(void)
{
    txBuffer[0] = AM1002_UART_STX_SND;
    txBuffer[1] = 0x01;
    txBuffer[2] = AM1002_UART_CMD_READ_MEASUREMENT_RESULT;

    uint8_t sum = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        sum += txBuffer[i];
    }

    // CS
    txBuffer[3] = 256 - sum;

    uart->write(txBuffer, 4);

    int16_t len = recvData(AM1002_UART_CMD_READ_MEASUREMENT_RESULT);

    if (len != 18)
    {
        return false;
    }

    tvoc = (data[0] << 8) + data[1];
    pm1p0 = (data[4] << 8) + data[5];
    pm2p5 = (data[6] << 8) + data[7];
    pm10 = (data[8] << 8) + data[9];
    temperature = (((data[10] << 8) + data[11]) - 500) / (float)10;
    humidity = ((data[12] << 8) + data[13]) / (float)10;

    return true;
}

bool AM1002_UART::requestSoftwareVersionNumber(void)
{
    txBuffer[0] = AM1002_UART_STX_SND;
    txBuffer[1] = 0x01;
    txBuffer[2] = AM1002_UART_CMD_READ_SOFTWARE_VERSION_NUMBER;

    uint8_t sum = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        sum += txBuffer[i];
    }

    // CS
    txBuffer[3] = 256 - sum;

    uart->write(txBuffer, 4);

    int16_t len = recvData(AM1002_UART_CMD_READ_SOFTWARE_VERSION_NUMBER);

    if (len != 13)
    {
        return false;
    }

    for (uint8_t i = 0; i < 13; i++)
    {
        softwareVersionNumber[i] = data[i];
    }

    return true;
}

bool AM1002_UART::requestSerialNumber(void)
{
    txBuffer[0] = AM1002_UART_STX_SND;
    txBuffer[1] = 0x01;
    txBuffer[2] = AM1002_UART_CMD_READ_SERIAL_NUMBER;

    uint8_t sum = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        sum += txBuffer[i];
    }

    // CS
    txBuffer[3] = 256 - sum;

    uart->write(txBuffer, 4);

    int16_t len = recvData(AM1002_UART_CMD_READ_SERIAL_NUMBER);

    if (len != 10)
    {
        return false;
    }

    for (uint8_t i = 0; i < 10; i++)
    {
        serialNumber[i] = data[i];
    }

    return true;
}

uint16_t AM1002_UART::getTvoc(void)
{
    return tvoc;
}

uint16_t AM1002_UART::getPm1p0(void)
{
    return pm1p0;
}

uint16_t AM1002_UART::getPm2p5(void)
{
    return pm2p5;
}

uint16_t AM1002_UART::getPm10(void)
{
    return pm10;
}

float AM1002_UART::getTemperature(void)
{
    return temperature;
}

float AM1002_UART::getHumidity(void)
{
    return humidity;
}

void AM1002_UART::readSoftwareVersionNumber(char *pData)
{
    for (uint8_t i = 0; i < 13; i++)
    {
        pData[i] = softwareVersionNumber[i];
    }
}

void AM1002_UART::readSerialNumber(uint8_t *pData)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        pData[i] = serialNumber[i];
    }
}
