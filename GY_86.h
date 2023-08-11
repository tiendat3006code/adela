#ifndef GY_86

#define GY_86

#include <Wire.h>    
#include <MPU6050.h>  
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <cmath>
#include "config.h"


typedef struct van_toc {
  float velocityX = 0;
  float velocityY = 0;
  float velocityZ = 0;
  float positionX = 0;
  float positionY = 0;
  float positionZ = 0;
  int16_t temperature   = 0; 
};
 
void GY_86_init();
van_toc GY_86_data();
bool handleData();
#endif//GY-86