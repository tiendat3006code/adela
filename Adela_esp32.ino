#include "HLKLD2410.h"
#include "GY_86.h"
#include "Tinker.h"
#include "cb_hanh_trinh.h"

pcf_arr *pcf_ptr = sta_ptr();

//su dung ham nay de in ra trang thai cac cong tac hanh trinh
void printPCFStatus(){
  pcf_arr *pcf_ptr = sta_ptr();
  for(int i =0 ; i< pcf_pin; i++){
    if (pcf_ptr -> status_pcf_all[i] == 0){
    Serial.print("trang thai port ");
    Serial.print(i);
    Serial.print(" :");
    Serial.println(pcf_ptr -> status_pcf_all[i]);
    }
  }
}
void setup() {
  Serial.begin(9600);

  
  // init_pcf();
  
  tinker_init();
  // put your setup code here, to run once:
}

void loop() {
  SendData();
  
  // put your main code here, to run repeatedly:
  
  
  delay(1000);
}