#include "HLKLD2410.h"
#include "GY_86.h"
#include "Tinker.h"
#include "cb_hanh_trinh.h"

//#include <LiquidCrystal_I2C.h>



// rgb_lcd lcd;
// LiquidCrystal_I2C lcd(lcd_address, 16, 2);
van_toc vt;
void setup() {
  //Serial.begin(9600);

  // LD2410_init();
  // GY_86_init();
  // //buffer_init();
  // while (1) {

  //   if (OpenCommandMode() == true) break;
  //   Serial.print(".");
  // }
  // Serial.println("Open command mode success");
  // ConfigParameters(2, 2, 0);
  // EnableEngineeringMode();
  // ConfigAllSensitivity(90, 85, 80, 75, 75, 65, 60, 50, 45, 85, 80, 70, 65, 60, 50, 45);
  // ReadParemeters();
  // CloseCommandMode();
 //i2c_init();
//  Serial.begin(9600);
//  init_pcf();
 tinker_init();
  //   // put your setup code here, to run once:
}

void loop() {
  //SendData();

  // put your main code here, to run repeatedly:
 // RecieveResponse();
  // Serial.print("so nguoi: ");
  // for (int k = 0; k < 64; k++) {

  //   Serial.print(cmd->RxData[k], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println(readUartData());


  // if (handleData()) {
  //   // lcd.setCursor(1, 1);
  //   // lcd.print("                          ");
  //   // lcd.setCursor(1, 1);
  //   // lcd.print("CANH BAO");
  //   Serial.println(GY_86_data().positionX);
  //   Serial.println(GY_86_data().positionY);
  //   Serial.println(GY_86_data().positionZ);
  //   // Serial.println("CANH BAO");
  // } else {
  //   // // lcd.setCursor(1, 1);
  //   // lcd.print("                          ");
  //   // lcd.setCursor(1, 1);
  //   // lcd.print("BINH THUONG");
  //  Serial.println(GY_86_data().positionX);
  //   Serial.println(GY_86_data().positionY);
  //   Serial.println(GY_86_data().positionZ);
  //   // Serial.println("BINH THUONG");
  // }
  // lcd.setCursor(12, 0);
  // lcd.print("                          ");
  // lcd.setCursor(12, 0);
  // lcd.print(readUartData());

  // if (handleData())
  //   Serial.println("canh bao");
  // else Serial.println("binh thuong");
  // vt = GY_86_data();
  // Serial.print("vi tri truc x: ");
  //  Serial.println(vt.velocityX);
  //  Serial.print("vi tri truc y: ");
  //   Serial.println(vt.velocityY);
  //   Serial.print("vi tri truc z: ");
  //   Serial.println(vt.velocityZ);
  //delay(10);
 SendData();
 //delay(1000);
 //pcf_check();
 //read_pcf();
 //Serial.println(handleData());
 delay(3000);
}