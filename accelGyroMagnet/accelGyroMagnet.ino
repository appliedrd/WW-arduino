 /*!
  * accelGyro.ino
  *
  * I2C addr:
  *   0x68: connect SDIO pin of the BMI160 to GND which means the default I2C address
  *   0x69: set I2C address by parameter
  *
  * Through the example, you can get the sensor data by using getSensorData:
  * get acell by paremeter onlyAccel;
  * get gyro by paremeter onlyGyro;
  * get both acell and gyro by paremeter bothAccelGyro.
  * 
  * With the rotation of the sensor, data changes are visible.
  *
  * Copyright   [DFRobot](http://www.dfrobot.com), 2016
  * Copyright   GNU Lesser General Public License
  *
  * version  V1.0
  * date  2017-11-27
  */
#include <DFRobot_BMI160.h>
DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x69;

// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MLX90393
// This code is designed to work with the MLX90393_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products
#include<Wire.h>
// MLX90393 I2C Address is 0x0C(12)
#define Addr 0x0C

void setup(){
  Serial.begin(115200);
  delay(100);

  // Initialise I2C communication as MASTER
  Wire.begin();
  
  //init the hardware bmin160  
  if (bmi160.softReset() != BMI160_OK){
    Serial.println("BMI160 reset false");
    while(1);
  }
  
  //set and init the bmi160 i2c address
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK){
    Serial.println("BMI160 init false");
    while(1);
  }

  // Start MLX90393 I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Write register command
  Wire.write(0x60);
  // Set AH = 0x00, BIST disabled
  Wire.write(0x00);
  // Set AL = 0x5C, Hall plate spinning rate = DEFAULT, GAIN_SEL = 5
  Wire.write(0x5C);
  // Select address register, (0x00 << 2)
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
   
  // Read status byte
  if(Wire.available() == 1)
  {
  unsigned int c = Wire.read();
  }
   
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Write register command
  Wire.write(0x60);
  // Set AH = 0x02
  Wire.write(0x02);
  // Set AL = 0xB4, RES for magnetic measurement = 0
  Wire.write(0xB4);
  // Select address register, (0x02 << 2)
  Wire.write(0x08);
  // Stop I2C Transmission
  Wire.endTransmission();
   
  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
   
  // Read status byte
  if(Wire.available() == 1)
  {
  unsigned int c = Wire.read();
  }
  delay(300);
}

void loop(){  
  int i = 0;
  int rslt;
  int16_t accelGyro[6]={0}; 
  unsigned int data[7];
  
  //get both accel and gyro data from bmi160
  //parameter accelGyro is the pointer to store the data
  rslt = bmi160.getAccelGyroData(accelGyro);
  if(rslt == 0){
    for(i=0;i<6;i++){
      if (i<3){
        //the first three are gyro datas
        Serial.print("G");Serial.print(accelGyro[i]*3.14/180.0);Serial.print(",");
      }else{
        //the following three data are accel datas
        Serial.print("A");Serial.print(accelGyro[i]/16384.0);Serial.print(",");
      }
    }
    //Serial.println();
  }else{
    Serial.println("err");
  }
  delay(100);
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Start single meaurement mode, ZYX enabled
  Wire.write(0x3E);
  // Stop I2C Transmission
  Wire.endTransmission();
   
  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
   
  // Read status byte
  if(Wire.available() == 1)
  {
  unsigned int c = Wire.read();
  }
  delay(100);
 
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Send read measurement command, ZYX enabled
  Wire.write(0x4E);
  // Stop I2C Transmission
  Wire.endTransmission();
   
  // Request 7 bytes of data
  Wire.requestFrom(Addr, 7);
   
  // Read 7 bytes of data
  // status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
  if(Wire.available() == 7);
  {
  data[0] = Wire.read();
  data[1] = Wire.read();
  data[2] = Wire.read();
  data[3] = Wire.read();
  data[4] = Wire.read();
  data[5] = Wire.read();
  data[6] = Wire.read();
  }
  /*Serial.print(data[0]);Serial.print("\t");
  Serial.print(data[1]);Serial.print("\t");
  Serial.print(data[2]);Serial.print("\t");
  Serial.print(data[3]);Serial.print("\t");
  Serial.print(data[4]);Serial.print("\t");
  Serial.print(data[5]);Serial.print("\t");
  Serial.print(data[6]);Serial.println("\t"); */
   
  // Convert the data
  int xMag = data[1] * 256 + data[2];
  int yMag = data[3] * 256 + data[4];
  int zMag = data[5] * 256 + data[6];
   
  // Output data to serial monitor
    Serial.print("M");Serial.print(xMag);Serial.print(",");
    Serial.print("M");Serial.print(yMag);Serial.print(",");
    Serial.print("M");Serial.println(zMag);
  /* Serial.print("Magnetic Field in X-Axis : ");
  Serial.println(xMag);
  Serial.print("Magnetic Field in Y-Axis : ");
  Serial.println(yMag);
  Serial.print("Magnetic Field in Z-Axis : ");
  Serial.println(zMag); */
  delay(350);
  /*
   * //only read accel data from bmi160
   * int16_t onlyAccel[3]={0};
   * bmi160.getAccelData(onlyAccel);
   */

  /*
   * ////only read gyro data from bmi160
   * int16_t onlyGyro[3]={0};
   * bmi160.getGyroData(onlyGyro);
   */
}
