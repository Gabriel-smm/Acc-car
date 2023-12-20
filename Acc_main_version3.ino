#include <Wire.h>

/*
    * Created by Cleber Borges - FunBots - @cleber.funbots  *     *

      Instagram: https://www.instagram.com/cleber.funbots/
      Facebook: https://www.facebook.com/cleber.funbots
      YouTube: https://www.youtube.com/channel/UCKs2l5weIqgJQxiLj0A6Atw
      Telegram: https://t.me/cleberfunbots

*/

// Inclusion of libraries
#include<Wire.h>

// Address of the I2C of the sensor MPU-6050
const int MPU = 0x68;

// Variables that will store the values
float GyrX, GyrY, AccX, AccY, AccZ, Temp, GyrZ; 
int PadX, PadY;


void setup() {
  // Initializes the Serial
  Serial.begin(9600);

  // Initializes MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Configuration of the scale range
  /*
    Wire.write(0b00000000); // scale range +/-250°/s
    Wire.write(0b00001000); // scale range +/-500°/s
    Wire.write(0b00010000); // scale range +/-1000°/s
    Wire.write(0b00011000); // scale range +/-2000°/s
  */
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0b00000000);  // Change this command for the desired scale range
  Wire.endTransmission();

  // Configuration of the scale range
  /*
      Wire.write(0b00000000); // scale range +/-2g
      Wire.write(0b00001000); // scale range +/-4g
      Wire.write(0b00010000); // scale range +/-8g
      Wire.write(0b00011000); // scale range +/-16g
  */
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00000000);  // Change this command for the desired scale range
  Wire.endTransmission();

 // Led Setup

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

}

void loop() {
  // Initializes the data transmission 
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // request the data

  // store the values in its respective variables
  AccX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AccY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AccZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Temp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyrX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyrY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyrZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // Print the data
  /* Divide the variable for the respective value of the scale chosen
      Acc
      +/-2g = 16384
      +/-4g = 8192
      +/-8g = 4096
      +/-16g = 2048

      Gyros
      +/-250°/s = 131
      +/-500°/s = 65.6
      +/-1000°/s = 32.8
      +/-2000°/s = 16.4
  */


// we won't use this acc info
/*     
  Serial.print(AccX / 2048);
  Serial.print(" ");
  Serial.print(AccY / 2048);
  Serial.print(" ");
  Serial.println(AccZ / 2048);
*/

// We will use this print here to visualize what is going on with our data

  Serial.print(GyrX / 131);
  Serial.print(" ");
  Serial.print(GyrY / 131);
  Serial.print(" ");
  //Serial.print(GyrZ / 131); 
  //Serial.print(" ");
  Serial.print(" ");
  Serial.println(PadX);  
  Serial.print(" ");
  Serial.print(PadY);

 // the idea here is to store the current (and fixed) position of the sensor instead of the variation of the position
  PadX = 0 + (3 + (GyrX / 131)); // my sensor was showing "-3°" in the motionless state, so I corrected this imprecision here
  PadY = 0 + (GyrY / 131);

// turning the LEDs on using the sensor data
  led_Ah();

  // delay of 10ms
  delay(10);
}


void led_Ah() {

  int led = 0;


  if (PadX < -150) {
    led = 2;
  } else if (PadX > 150) {
    led = 6;
  }

  if (PadY < -150) {
    led = 5;
  } else if (PadY > 150) {
    led = 3;
  }

  if (led > 0) {
    digitalWrite(led, HIGH);
    digitalWrite(4, LOW);
  } else {
    digitalWrite(4, HIGH);
    digitalWrite(led, LOW);
  }
}


