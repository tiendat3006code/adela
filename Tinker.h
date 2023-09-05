#ifndef Tinker.h

#define Tinker .h

#include <HardwareSerial.h>
#include <stdint.h>
#include <stdbool.h>
#include <Ticker.h>
#include "HLKLD2410.h"
#include "GY_86.h"
#include "Tinker.h"
#include "cb_hanh_trinh.h"
#include <SPI.h>
#include "config.h"

//UART Tinker
#define TINKER_TX 2
#define TINKER_RX 4

typedef struct data_arr {
  unsigned char txdata[12 + pcf_pin];
  unsigned char rxdata[40];
  //int time_delay = 3000;
  unsigned char sync_buffer[12 + pcf_pin];
  unsigned char receivedData[10];
};
void tinker_init();
void SendData();
void ReceiveData();
void sync();

#endif