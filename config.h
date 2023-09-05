#ifndef config

#define config

//gy-86 parameters
#define I2C_SDA_PIN 21 
#define I2C_SCL_PIN 22 
#define MPU6050_ADDRESS  0x68
#define threshold_gy_86 200
#define threshold_Vel_X -1
#define threshold_Vel_Y 4
#define threshold_Vel_Z 35

//HLK-2420 parameters
#define HLKLD2410
#define NO_TARGET 0x00
#define MOVEMENT_TARGET 0x01
#define STATIONARY_TARGET 0x02
#define MOVEMENT_AND_STATIONARY_TARGET 0x03
//#define threshold 100

//UART cam bien hien dien con nguoi
#define rx 16
#define tx 17

//dia chi I2C cua PCF8575
#define ADDRESS_1 0x21
#define ADDRESS_2 0x22

//so bo PCF8575 duoc su dung
#define number_of_pcf 2
//so chan khong su dung
#define unused_pin 8
//so chan duoc dung
#define pcf_pin number_of_pcf * 16 - unused_pin

#endif//config