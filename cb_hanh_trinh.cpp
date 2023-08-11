#include "cb_hanh_trinh.h"

PCF8575 pcf_1(ADDRESS_1);
PCF8575 pcf_2(ADDRESS_2);

pcf_arr sta;
pcf_arr *status_pcf = &sta;

void init_pcf(){

    pcf_1.begin();
    if(pcf_1.isConnected()) Serial.println("pcf_1 connect thanh cong");
    else Serial.println("pcf_1 connect fail");
    pcf_2.begin();
    if(pcf_2.isConnected()) Serial.println("pcf_2 connect thanh cong");
    else Serial.println("pcf_2 connect fail");
    // pcf_1.digitalWrite(P0, HIGH);
    // pcf_2.digitalWrite(P0, HIGH);   
}
void read_pcf(){
  for(int i = 0; i<16; i++){
    //status_pcf->status_pcf_1[i] = pcf_1.read(i);
    // Serial.print("state ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(pcf_1.read(i));
    
    status_pcf->status_pcf_all[i] = pcf_1.read(i);
    //  Serial.print("state ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(status_pcf->status_pcf_all[i]);
  }
  for(int i = 16; i< pcf_pin; i++){
    //status_pcf->status_pcf_2[i-16] = pcf_2.read(i-16);
    // Serial.print("state ");
    // Serial.print(i-16);
    // Serial.print(" ");
    // Serial.println(pcf_2.read(i-16));
    status_pcf->status_pcf_all[i] = pcf_2.read(i-16);
    
    // Serial.print("state ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(status_pcf->status_pcf_all[i]);
    
  }
}
pcf_arr *sta_ptr(){
    read_pcf();
    return status_pcf;
}
void i2c_init(){
    Wire.begin();
    //Serial.begin(9600);
    while (!Serial)
    // Leonardo: wait for serial monitor
    Serial.println("\nI2C Scanner");
}
void i2c_scan(){
    byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
      Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      //if(address >= (uint9_t)(0x20)) address_1 = (uint8_t) address
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // wait 5 seconds for next scan
}
bool pcf_check(){
  read_pcf();
  for( int i =0; i <32; i++){
    if(status_pcf->status_pcf_all[i] == 0) return true;
    else if(status_pcf->status_pcf_all[i] == 1) return false;
  }
}