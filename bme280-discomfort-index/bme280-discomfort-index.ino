/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
  Serial.begin(115200);
  while (!Serial);   // time to get serial running
  Serial.println(F("BME280 test"));

  unsigned status;

  // default settings
  status = bme.begin(0x76);
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
}


void loop() {
  printValues();
  printDiscomfortIndex();
  delay(delayTime);
}


void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");

  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}

void printDiscomfortIndex() {
  float tmp;
  float hmd;
  float di;

  tmp = bme.readTemperature();
  hmd = bme.readHumidity();

  // 不快指数 DI　（Tは乾球気温℃、Hは湿度％）
  //DI=0.81T+0.01H×(0.99T−14.3)+46.3

  di = (0.81 * tmp) + (0.01 * hmd) * ((0.99 * tmp) - 14.3) + 46.3;

  Serial.print("DiscomfortIndex = ");
  Serial.print(di);
  Serial.println("");

  // あーやばい。wikipedia不快指数のページ見ながら書いただけ
  // あれだなーコメントからドキュメント生成するやつやってみてー
  // 次はこの分岐を使って、ディスプレイに体感を表示する顔文字でも表示してみるか
  // 2021/07/12 23:53

  if (di < 50) {
    Serial.println("寒くてたまらない");
  } else if (50 <= di && di < 55) {
    Serial.println("寒くてたまらない");
  } else if (55 <= di && di < 60) {
    Serial.println("肌寒い");
  } else if (60 <= di && di < 65) {
    Serial.println("何も感じない");
  } else if (65 <= di && di < 70) {
    Serial.println("快適");
  } else if (70 <= di && di < 75) {
    Serial.println("不快感を持つ人が出始める");
  } else if (75 <= di && di < 80) {
    Serial.println("半数以上が不快に感じる");
  } else if (80 <= di && di < 85) {
    Serial.println("全員が不快に感じる");
  } else if (85 <= di) {
    Serial.println("暑くてたまらない");
  }

}
