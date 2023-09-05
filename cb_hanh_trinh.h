#ifndef cb_hanh_trinh

#define cb_hanh_trinh

#include <stdint.h>
#include <stdbool.h>
#include <Ticker.h>
#include <Wire.h>
#include <PCF8575.h>
#include "config.h"

typedef struct pcf_arr {
  //int status_pcf_1[16];
  //int status_pcf_2[16];
  bool status_pcf_all[pcf_pin];
  unsigned char pcf_address[number_of_pcf];
};

void init_pcf();
void read_pcf();
void i2c_init();
void i2c_scan();
bool pcf_check();
struct pcf_arr *sta_ptr();
#endif