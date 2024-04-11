#include <Arduino.h>
#include "../lib/SparkFun_BNO08x_Arduino_Library-main/src/SparkFun_BNO08x_Arduino_Library.h"

BNO08x myIMU;

#define BNO08X_INT  A4
//#define BNO08X_INT  -1
#define BNO08X_RST  A5
//#define BNO08X_RST  -1

#define BNO08X_ADDR 0x4B

void setReports(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // En caso de usar USB en tu placa de forma nativa, descomentar
  //(while(!Serial) delay(10);

  Wire.begin();

  // Buscando el sensor
  if (myIMU.begin(BNO08X_ADDR, Wire, BNO08X_INT, BNO08X_RST) == false) {
   Serial.println("BNO08x not detected at default I2C address. Check your jumpers and the hookup guide. Freezing..."); // No encontrado
    while (1)
    ;
  }
  // Encontrado
  Serial.println("BNO08x found!");

  setReports();

  Serial.println("Reading events");
  delay(100);
}

void setReports(void) {
  Serial.println("Setting desired reports");
  // GYRO reports
  if (myIMU.enableGyro() == true) {
    Serial.println(F("Gyro enabled"));
    Serial.println(F("Output in form x, y, z, in radians per second"));
  } else {
    Serial.println("Could not enable gyro");
  }

  // Accelerometer reports
  if (myIMU.enableAccelerometer() == true) {
    Serial.println(F("Accelerometer enabled"));
    Serial.println(F("Output in form x, y, z, in m/s^2"));
  } else {
    Serial.println("Could not enable accelerometer");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);

  // Detecta si se ha reiniciado el sensor
  if (myIMU.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

  // Has a new event come in on the Sensor Hub Bus?
  if (myIMU.getSensorEvent() == true) {

    // is it the correct sensor data we want?
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_GYROSCOPE_CALIBRATED) {

      float x = myIMU.getGyroX();
      float y = myIMU.getGyroY();
      float z = myIMU.getGyroZ();

      Serial.print(x, 2);
      Serial.print(F(","));
      Serial.print(y, 2);
      Serial.print(F(","));
      Serial.print(z, 2);

      Serial.println();
    }

    if (myIMU.getSensorEventID() == SENSOR_REPORTID_ACCELEROMETER) {

      float x = myIMU.getAccelX();
      float y = myIMU.getAccelY();
      float z = myIMU.getAccelZ();

      Serial.print(x, 2);
      Serial.print(F(","));
      Serial.print(y, 2);
      Serial.print(F(","));
      Serial.print(z, 2);

      Serial.println();
    }
  }
}

