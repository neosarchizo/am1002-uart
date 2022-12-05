#include <SoftwareSerial.h>
#include <AM1002_UART.h>

SoftwareSerial mySerial(2, 3);

AM1002_UART am1002(&mySerial);

char version[14];
uint8_t serialNumber[10];

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  while (!Serial)
  {
  }
}

void loop() {
  bool success = am1002.measure();

  if (success)
  {
    Serial.print("TVOC : ");
    Serial.println(am1002.getTvoc());

    Serial.print("PM 1.0 : ");
    Serial.println(am1002.getPm1p0());

    Serial.print("PM 2.5 : ");
    Serial.println(am1002.getPm2p5());

    Serial.print("PM 10 : ");
    Serial.println(am1002.getPm10());

    Serial.print("Temperature : ");
    Serial.println(am1002.getTemperature());

    Serial.print("Humidity : ");
    Serial.println(am1002.getHumidity());
  }

  success = am1002.requestSoftwareVersionNumber();

  if (success)
  {
    am1002.readSoftwareVersionNumber(version);
    Serial.print("Version : ");
    Serial.println(version);
  }

  success = am1002.requestSerialNumber();

  if(success)
  {
    am1002.readSerialNumber(serialNumber);
    Serial.print("Serial Number : ");

    Serial.print((serialNumber[0] << 8) + serialNumber[1]);
    Serial.print(" ");

    Serial.print((serialNumber[2] << 8) + serialNumber[3]);
    Serial.print(" ");

    Serial.print((serialNumber[4] << 8) + serialNumber[5]);
    Serial.print(" ");

    Serial.print((serialNumber[6] << 8) + serialNumber[7]);
    Serial.print(" ");

    Serial.println((serialNumber[8] << 8) + serialNumber[9]);
  }

  delay(3000);
}