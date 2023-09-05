#ifndef HLKLD2410

#define HLKLD2410

#include <HardwareSerial.h>
#include <stdint.h>
#include <stdbool.h>
#include <Ticker.h>
#include "config.h"

typedef struct Command_Structure {
  unsigned char TxData[64];
  unsigned char RxData[64];
  unsigned char DataLength;
  //int timeout;
};
typedef enum MODE {
  ENGINEERING_MODE,
  NORMAL_MODE,
};
typedef enum Target_Status {
  NoTarget,
  MovementTarget,
  StationaryTarget,
  MovementStationaryTarget,
};

//khoi dong
void LD2410_init();
//gui lenh
void SendCommand();
//nhan lenh moi 100ms
bool RecieveResponse();
//mo che do cai dat
bool OpenCommandMode();
//check xem co nguoi hay khong
bool DetectedHuman();
//tat che do cai dat
void CloseCommandMode();
//doc cac thong so
void ReadParemeters();
//bat che do nang cao
void EnableEngineeringMode();
//tat che do nang cao
void CloseEngineeringMode();
//doc firmware
void ReadFirmwareVersion();
//reset lai cac thong so
void Reset();
//khoi dong lai module
void Restart();
//mode 1 nếu muốn bật bluetooth, 0 nếu muốn tắt
void BluetoothSettings(bool mode);
//doc dia chi MAC
void GetMACAddress();
//chinh cac thong so nhu khoang cach va do tre
//distance tu 2->8, time tu 0->65535s
void ConfigParameters(int moving_distance, int resting_distance, int delay_time);
//chinh do nhay
//gate tu 0->8, 9 neu muon chinh mot lan tu 0->8, sensitivity tu 0->100
void ConfigSensitivity(int gate, int motion_sensitivity, int static_sensitivity);
//xu ly response tu cam bien va dem so nguoi
int readUartData();
//chinh do nhay tu cong 0->9
void ConfigAllSensitivity(int value_1, int value_2, int value_3, int value_4, int value_5, int value_6, int value_7, int value_8, int value_9, int value_10, int value_11, int value_12, int value_13, int value_14, int value_15, int value_16);
#endif  //HLK_LD2410