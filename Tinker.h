#ifndef Tinker.h

#define Tinker.h

#include <HardwareSerial.h>
#include <stdint.h>
#include <stdbool.h>
#include <Ticker.h>
#include "HLKLD2410.h"
#include "GY_86.h"
#include "Tinker.h"
#include "cb_hanh_trinh.h"
#include <SPI.h>

// Khai báo các chân GPIO cho SoftSPI
#define PIN_SOFT_SPI_MISO 19
#define PIN_SOFT_SPI_MOSI 23
#define PIN_SOFT_SPI_CLK  18
#define PIN_SPI_CS   5

//UART Tinker
#define Rx 3
#define Tx 1

typedef struct data_arr{
  int txdata[40];
  int rxdata[40];
  int time_delay;
};
void tinker_init();
void SendData();
void ReceiveData();

#endif