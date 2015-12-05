#include <Wire.h>

#define  ADXL345_ADDRESS (0xA6 >> 1)
#define ADXL345_REGISTER_XLSB (0x32)
#define ADXL_REGISTER_PWRCTL (0x2D)
#define ADXL_PWRCTL_MEASURE (1 << 3)

#define ITG3200_ADDRESS (0xD0 >> 1)
#define ITG3200_REGISTER_XMSB (0x1D)
#define ITG3200_REGISTER_DLPF_FS (0x16)
#define ITG3200_FULLSCALE (0x03 << 3)
#define ITG3200_42HZ (0x03)

#define HMC5843_ADDRESS (0x3C >> 1)
#define HMC5843_REGISTER_XMSB (0x03)
#define HMC5843_REGISTER_MEASMODE (0x02)
#define HMC5843_MEASMODE_CONT (0x00)

int16_t GX, GY, GZ, AX, AY, AZ, smoothTimeOut, avgGX, avgGY, avgGZ, avgAX, avgAY, avgAZ, irRead;
const int flexPin0 = 0;
const int flexPin1 = 1;
const int flexPin2 = 2;
const int irPin = 3;

int accelerometer_data[3];
int gyro_data[3];
int magnetometer_data[3];
int flexRead0, flexRead1, flexRead2, flex00to100, flex10to100, flex20to100;

char c;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  smoothTimeOut = 0;
  avgGX = avgGY = avgGZ = 0;

  for(int i = 0; i < 3; ++i) {
    accelerometer_data[i] = magnetometer_data[i] = gyro_data[i] = 0;
  }
  
  init_adxl345();
  init_hmc5843();
  
}

void loop() {
   read_adxl345();
   read_hmc5843();

   GX = map(accelerometer_data[0], - 280, 280, -180, 180);
   GY = map(accelerometer_data[1], - 280, 280, -180, 180);
   GZ = map(accelerometer_data[2], - 280, 280, -180, 180);

   Serial.print("MX: ");Serial.print(magnetometer_data[0]);
   Serial.print(" MY: ");Serial.print(magnetometer_data[1]);
   Serial.print(" MZ: ");Serial.println(magnetometer_data[2]);
   

   AX = map(magnetometer_data[0], - 280, 280, -180, 180);
   AY = map(magnetometer_data[0], - 280, 280, -180, 180);
   AZ = map(magnetometer_data[0], - 280, 280, -180, 180);

   flexRead0 = analogRead(flexPin0);
   flexRead1 = analogRead(flexPin1);
   flexRead2 = analogRead(flexPin2);

   flex00to100 = map(flexRead0, 735, 930, 0, 100);
   flex10to100 = map(flexRead1, 500, 800, 0, 100);
   flex20to100 = map(flexRead2, 500, 800, 0, 100);
   smoothTimeOut = smoothTimeOut + 10;
   avgGX = avgGX + GX;
   avgGY = avgGY + GY;
   avgGZ = avgGZ + GZ;

   irRead = analogRead(irPin);
   irRead = 4800 / (irRead - 20);
  
    if(smoothTimeOut == 100){
    Serial.print("|GyX=");   Serial.print(avgGX / 10);
    Serial.print("|GyY=");   Serial.print(avgGY / 10);
    Serial.print("|GyZ=");   Serial.print(avgGZ / 10);
    Serial.print("|flex0="); Serial.print(flexRead0);
    Serial.print("|flex1="); Serial.print(flexRead1);
    Serial.print("|flex2="); Serial.print(flexRead2);
    Serial.print("|IR=");    Serial.print(irRead);
    Serial.print("\n");
    avgGX = 0;
    avgGY = 0;
    avgGZ = 0;
    smoothTimeOut = 0;
   }

   /*Serial.print("ACCEL: ");
   Serial.print(accelerometer_data[0]);
   Serial.print("\t");
   Serial.print(accelerometer_data[1]);
   Serial.print("\t");
   Serial.print(accelerometer_data[2]);
   Serial.print("\t");
   Serial.print("\n");*/

   delay(100);
}

void i2c_write(int address, byte reg, byte data) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void i2c_read(int address, byte reg, int count, byte* data) {
 int i = 0;

 Wire.beginTransmission(address);
 Wire.write(reg);
 Wire.endTransmission();
 Wire.beginTransmission(address);
 Wire.requestFrom(address,count);
 while(Wire.available()){
   c = Wire.read();
   data[i] = c;
   i++;
 }
 Wire.endTransmission();
} 

void init_adxl345() {
  byte data = 0;

  i2c_write(ADXL345_ADDRESS, ADXL_REGISTER_PWRCTL, ADXL_PWRCTL_MEASURE);

  i2c_read(ADXL345_ADDRESS, ADXL_REGISTER_PWRCTL, 1, &data);
  Serial.println((unsigned int)data);
}

void read_adxl345() {
 byte bytes[6];
 memset(bytes,0,6);

 i2c_read(ADXL345_ADDRESS, ADXL345_REGISTER_XLSB, 6, bytes);

 for (int i=0;i<3;++i) {
 accelerometer_data[i] = (int)bytes[2*i] + (((int)bytes[2*i + 1]) << 8);
 }
}

void init_itg3200() {
  byte data = 0;

  i2c_write(ITG3200_ADDRESS, ITG3200_REGISTER_DLPF_FS, ITG3200_FULLSCALE | ITG3200_42HZ);

  i2c_read(ITG3200_ADDRESS, ITG3200_REGISTER_DLPF_FS, 1, &data);

  Serial.println((unsigned int)data);
}

void read_itg3200() {
  byte bytes[6];
  memset(bytes,0,6);

  i2c_read(ITG3200_ADDRESS, ITG3200_REGISTER_XMSB, 6, bytes);
  for (int i=0;i<3;++i) {
  gyro_data[i] = (int)bytes[2*i + 1] + (((int)bytes[2*i]) << 8);
  }
}

void init_hmc5843() {
  byte data = 0;
  
  i2c_write(HMC5843_ADDRESS, HMC5843_REGISTER_MEASMODE, HMC5843_MEASMODE_CONT);

  i2c_read(HMC5843_ADDRESS, HMC5843_REGISTER_MEASMODE, 1, &data);
  Serial.println((unsigned int)data);
}

void read_hmc5843() {
 byte bytes[6];
 memset(bytes,0,6);

 i2c_read(HMC5843_ADDRESS, HMC5843_REGISTER_XMSB, 6, bytes);

 for (int i=0;i<3;++i) {
 magnetometer_data[i] = (int)bytes[2*i + 1] + (((int)bytes[2*i]) << 8);
 }
}
