#ifndef cb_hanh_trinh

#define cb_hanh_trinh

#include <stdint.h>
#include <stdbool.h>
#include <Ticker.h>
#include <Wire.h>
#include <PCF8575.h>

#define ADDRESS_1 0x21
#define ADDRESS_2 0x22

typedef struct pcf_arr{
    int status_pcf_1[16];
    int status_pcf_2[16];
    int status_pcf_all[32];
};

void init_pcf();
void read_pcf();
void i2c_init();
void i2c_scan();
void pcf_check();
struct pcf_arr *sta_ptr();
#endif