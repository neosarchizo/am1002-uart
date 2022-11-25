#ifndef __AM1002_UART_H__
#define __AM1002_UART_H__

#include "Arduino.h"
#include <stdint.h>
#include <stdbool.h>

#define AM1002_UART_STX_SND 0x11
#define AM1002_UART_STX_RCV 0x16

#define AM1002_UART_CMD_READ_MEASUREMENT_RESULT 0x16
#define AM1002_UART_CMD_READ_SOFTWARE_VERSION_NUMBER 0x1E
#define AM1002_UART_CMD_READ_SERIAL_NUMBER 0x1F

#define AM1002_UART_POS_STX 0
#define AM1002_UART_POS_LEN 1
#define AM1002_UART_POS_CMD 2
#define AM1002_UART_POS_DAT 3

class AM1002_UART {
public:
    AM1002_UART(Stream *uart = &Serial);

    bool measure(void);
    bool requestSoftwareVersionNumber(void);
    bool requestSerialNumber(void);

    uint16_t getTvoc(void);
    uint16_t getPm1p0(void);
    uint16_t getPm2p5(void);
    uint16_t getPm10(void);
    float getTemperature(void);
    float getHumidity(void);

    void readSoftwareVersionNumber(char * pData);
    void readSerialNumber(uint8_t * pData);
private:
    void clearBuffer(void);

    int16_t recvData(uint8_t cmd, uint32_t timeout = 1000);

    Stream *uart;
    uint8_t rxBuffer[256];
    uint8_t idxBuffer;
    uint8_t txBuffer[256];
    uint8_t data[256];

    uint16_t tvoc;
    uint16_t pm1p0;
    uint16_t pm2p5;
    uint16_t pm10;
    float temperature;
    float humidity;

    char softwareVersionNumber[13];
    uint8_t serialNumber[10];
};

#endif // __AM1002_UART_H__
