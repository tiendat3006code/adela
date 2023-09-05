#include "cb_hanh_trinh.h"

// PCF8575 pcf_1(ADDRESS_1);
// PCF8575 pcf_2(ADDRESS_2);

PCF8575 pcf[number_of_pcf];

pcf_arr sta;
pcf_arr *status_pcf = &sta;

bool scan_success_or_failure = false;

void init_pcf() {
  i2c_init();
  while (1) {
    i2c_scan();
    if (scan_success_or_failure) break;
  }

  // pcf[0] = PCF8575(ADDRESS_1);
  // pcf[0].begin();
  // pcf[1] = PCF8575(ADDRESS_2);
  // pcf[1].begin();
  for (int i = 0; i < number_of_pcf; i++) {
    pcf[i] = PCF8575(status_pcf->pcf_address[i]);
    pcf[i].begin();
    if (pcf[i].isConnected()) {
      Serial.print("bo pcf so ");
      Serial.print(i);
      Serial.println(" ket noi thanh cong");
    } else {
      Serial.print("bo pcf so ");
      Serial.print(i);
      Serial.println(" ket noi that bai");
    }
  }
  // pcf_1.begin();
  // if (pcf_1.isConnected()) Serial.println("pcf_1 connect thanh cong");
  // else Serial.println("pcf_1 connect fail");
  // pcf_2.begin();
  // if (pcf_2.isConnected()) Serial.println("pcf_2 connect thanh cong");
  // else Serial.println("pcf_2 connect fail");
  // pcf_1.digitalWrite(P0, HIGH);
  // pcf_2.digitalWrite(P0, HIGH);
}
void read_pcf() {
  for (int k = 0; k < number_of_pcf; k++) {
    if (k == number_of_pcf - 1) {
      for (int i = 0 + 16 * k; i < pcf_pin; i++) {
        status_pcf->status_pcf_all[i] = pcf[k].read(i - 16 * k);
      }
    } else {
      for (int i = 0 + k * 16; i < 16 + k * 16; i++) {
        status_pcf->status_pcf_all[i] = pcf[k].read(i - k * 16);
      }
    }
  }
//  for (int i = 0; i < 16; i++) {
    //status_pcf->status_pcf_1[i] = pcf_1.read(i);
    // Serial.print("state ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(pcf_1.read(i));

  //  status_pcf->status_pcf_all[i] = pcf_1.read(i);
    //  Serial.print("state ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(status_pcf->status_pcf_all[i]);
//  }
//  for (int i = 16; i < pcf_pin; i++) {
    //status_pcf->status_pcf_2[i-16] = pcf_2.read(i-16);
    // Serial.print("state ");
    // Serial.print(i-16);
    // Serial.print(" ");
    // Serial.println(pcf_2.read(i-16));
  //  status_pcf->status_pcf_all[i] = pcf_2.read(i - 16);

    // Serial.print("state ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(status_pcf->status_pcf_all[i]);
//  }
}
pcf_arr *sta_ptr() {
  read_pcf();
  return status_pcf;
}
void i2c_init() {
  Wire.begin();
  //Serial.begin(9600);
  while (!Serial)
    // Leonardo: wait for serial monitor
    Serial.println("\nI2C Scanner");
}
void i2c_scan() {
  byte error, address;
  int nDevices;
  int i = 0;
  memset(status_pcf->pcf_address, 0, number_of_pcf);
  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address ");
      if (address < 16)
        Serial.print("0");
      Serial.print(address);
      Serial.println("  !");
      if (i < number_of_pcf && address > 30) {
        char hexString[3];
        sprintf(hexString, "%02X", address);
        sscanf(hexString, "%hhx", &status_pcf->pcf_address[i]);
        //status_pcf->pcf_address[i] = (unsigned char) address;
        Serial.print("ket qua luu trong array: ");
        Serial.println(status_pcf->pcf_address[i]);
        i++;
        scan_success_or_failure = true;
      }

      //if(address >= (uint9_t)(0x20)) address_1 = (uint8_t) address
      nDevices++;
    } else if (error == 4) {
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

  delay(5000);  // wait 5 seconds for next scan
}
bool pcf_check() {
  read_pcf();
  for (int i = 0; i < 32; i++) {
    if (status_pcf->status_pcf_all[i] == 0) return true;
    else if (status_pcf->status_pcf_all[i] == 1) return false;
  }
}