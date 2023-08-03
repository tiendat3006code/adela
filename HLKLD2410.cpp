#include "HLKLD2410.h"

//Variables
Target_Status status;
Command_Structure command;
Command_Structure *cmd = &command;
HardwareSerial LD2410(1);
MODE mode = NORMAL_MODE;

int32_t movement[9];
int32_t stationary[9];
int32_t current_movement[9];
int32_t current_stationary[9];


void LD2410_init() {
  LD2410.begin(256000, SERIAL_8N1, rx, tx);
  while (!LD2410) {
    Serial.print(".");
  }
  Serial.println("success");
}

int readUartData() {
  int count_movement = 0;
  int count_stationary = 0;
  int movement_distance = 0;
  int stationary_distance = 0;
  int distance = 0;
  memset(current_movement, 0, 9);
  memset(current_stationary, 0, 9);
  if (mode == NORMAL_MODE) {
    switch (cmd->RxData[8]) {
      case NO_TARGET: status = NoTarget; break;
      case MOVEMENT_TARGET: status = MovementTarget; break;
      case STATIONARY_TARGET: status = StationaryTarget; break;
      case MOVEMENT_AND_STATIONARY_TARGET: status = MovementStationaryTarget; break;
      default: break;
    }
    movement_distance = (int32_t)((cmd->RxData[10] << 8) | cmd->RxData[9]);
    stationary_distance = (int32_t)((cmd->RxData[13] << 8) | cmd->RxData[12]);
    distance = (int32_t)((cmd->RxData[16] << 8) | cmd->RxData[15]);
  } else if (mode == ENGINEERING_MODE) {
    switch (cmd->RxData[8]) {
      case NO_TARGET: status = NoTarget; break;
      case MOVEMENT_TARGET: status = MovementTarget; break;
      case STATIONARY_TARGET: status = StationaryTarget; break;
      case MOVEMENT_AND_STATIONARY_TARGET: status = MovementStationaryTarget; break;
      default: break;
    }
    movement_distance = (int32_t)((cmd->RxData[10] << 8) | cmd->RxData[9]);
    stationary_distance = (int32_t)((cmd->RxData[13] << 8) | cmd->RxData[12]);
    distance = (int32_t)((cmd->RxData[16] << 8) | cmd->RxData[15]);
    for (int k = 19; k < 28; k++) {
      current_movement[k - 19] = cmd->RxData[k];
    }
    for (int k = 28; k < 36; k++)
      current_stationary[k - 29] = cmd->RxData[k];
    for (int k = 1; k < 9; k++) {
      if ((current_movement[k] - current_movement[k + 1]) >= 30) count_movement++;
      if ((current_stationary[k] - current_stationary[k + 1]) >= 30) count_stationary++;
    }
    // if((current_movement[0] - current_movement[2]) >= 30) count_movement++;
    // if((current_movement[2] - current_movement[4]) >= 30) count_movement++;
    // if((current_movement[4] - current_movement[6]) >= 30) count_movement++;
    // if((current_movement[6] - current_movement[8]) >= 30) count_movement++;
    //if(count_movement <= count_stationary){
    // Serial.print("So nguoi dem duoc: ");
    // Serial.println(count_movement);
    if (status == MOVEMENT_TARGET)
      return count_movement;
    else if (status == STATIONARY_TARGET)
      return count_stationary;
    else if (status == NO_TARGET)
      return 0;
    else if (status == MOVEMENT_AND_STATIONARY_TARGET) {
      if (count_movement <= count_stationary) return count_movement;
      else return count_stationary;
    }
    // return count_movement + count_stationary;
    //}
    //if(count_movement < (count_stationary - 2)) Serial.println(count_stationary);
    //else {
    // Serial.print("So nguoi dem duoc: ");
    // Serial.println(count_stationary);
    //return count_stationary - 2;
    //}
  }
}
bool DetectedHuman() {
  if (status == NO_TARGET)
    return false;
  else if (status == MOVEMENT_TARGET
           || status == STATIONARY_TARGET
           || status == MOVEMENT_AND_STATIONARY_TARGET) 
           return true;
}
void SendCommand() {
  cmd->TxData[0] = 0xFD;
  cmd->TxData[1] = 0xFC;
  cmd->TxData[2] = 0xFB;
  cmd->TxData[3] = 0xFA;
  cmd->TxData[4] = cmd->DataLength;
  cmd->TxData[5] = 0x00;
  cmd->TxData[5 + cmd->DataLength + 1] = 0x04;
  cmd->TxData[5 + cmd->DataLength + 2] = 0x03;
  cmd->TxData[5 + cmd->DataLength + 3] = 0x02;
  cmd->TxData[5 + cmd->DataLength + 4] = 0x01;
  LD2410.flush();
  delay(100);
  LD2410.write(cmd->TxData, 64);
}

//delay 100 ms mỗi lần receive
bool RecieveResponse() {
  memset(cmd->RxData, 0, sizeof(cmd->RxData));
  int i;
  int l;
  if (LD2410.available() > 0) {
    for (i = 0; i < 64; i++) {
      l = LD2410.read();
      if (l >= 0)
        cmd->RxData[i] = (uint8_t)l;
      else
        break;
    }
  }
  LD2410.flush();
  if (cmd->RxData[0] == 0xFD
      && cmd->RxData[1] == 0xFC
      && cmd->RxData[2] == 0xFB
      && cmd->RxData[3] == 0xFA
      && cmd->RxData[8] == 0x00
      && cmd->RxData[9] == 0x00) return true;
  else return false;
}

bool OpenCommandMode() {
  cmd->DataLength = 4;
  cmd->TxData[6] = 0xFF;
  cmd->TxData[7] = 0x00;
  cmd->TxData[8] = 0x01;
  cmd->TxData[9] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) return true;
  else return false;
}

void CloseCommandMode() {
  cmd->DataLength = 2;
  cmd->TxData[6] = 0xFE;
  cmd->TxData[7] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse())
    Serial.println("Close command mode success");
  else Serial.println("Close command mode fail");
}

void ReadParemeters() {
  cmd->DataLength = 2;
  cmd->TxData[6] = 0x61;
  cmd->TxData[7] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) {
    Serial.print("Max distanse: ");
    Serial.println(cmd->RxData[12]);
    Serial.print("Min distance: ");
    Serial.println(cmd->RxData[13]);
    Serial.print("Motion sensitivity gate 0->8: ");
    for (int k = 14; k < 23; k++) {
      movement[k - 14] = cmd->RxData[k];
      Serial.print(cmd->RxData[k]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("Rest sensitivity gate 0->8: ");
    for (int k = 23; k < 32; k++) {
      stationary[k - 23] = cmd->RxData[k];
      Serial.print(cmd->RxData[k]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("Delay time: ");
    int value = (int)((cmd->RxData[33] << 8) | cmd->RxData[32]);
    Serial.println(value);
  } else Serial.println("Read data fail");
  //if(cmd->RxData[0]== 0xFD) Serial.print("yeah");
}

void EnableEngineeringMode() {
  cmd->DataLength = 2;
  cmd->TxData[6] = 0x62;
  cmd->TxData[7] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) Serial.println("Turn on engineering mode success");
  else Serial.println("Turn on engineering mode fail");
  mode = ENGINEERING_MODE;
}

void CloseEngineeringMode() {
  cmd->DataLength = 2;
  cmd->TxData[6] = 0x63;
  cmd->TxData[7] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) Serial.println("Turn off engineering mode success");
  else Serial.println("Turn off engineering mode fail");
  mode = NORMAL_MODE;
}

void ReadFirmwareVersion() {
  cmd->DataLength = 2;
  cmd->TxData[6] = 0xA0;
  cmd->TxData[7] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) {
    Serial.print("Version: ");
    for (int k = 6; k < 64; k++) {

      Serial.print(cmd->RxData[k], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else Serial.println("Read version fail");
  //if(cmd->RxData[0]== 0xFD) Serial.print("yeah");
}

void Reset() {
  cmd->DataLength = 2;
  cmd->TxData[6] = 0xA2;
  cmd->TxData[7] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) Serial.println("Restore success");
  else Serial.println("Restore fail");
}

void Restart() {
  cmd->DataLength = 2;
  cmd->TxData[6] = 0xA3;
  cmd->TxData[7] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) Serial.println("Reset success");
  else Serial.println("Reset fail");
}

void BluetoothSettings(bool mode) {
  cmd->DataLength = 4;
  cmd->TxData[6] = 0xA4;
  cmd->TxData[7] = 0x00;
  if (mode == true) cmd->TxData[8] = 0x01;
  else cmd->TxData[8] = 0x00;
  cmd->TxData[9] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) Serial.println("success");
  else Serial.println("fail");
}

void GetMACAddress() {
  cmd->DataLength = 4;
  cmd->TxData[6] = 0xA5;
  cmd->TxData[7] = 0x00;
  cmd->TxData[8] = 0x01;
  cmd->TxData[9] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) {
    Serial.print("MAC Address: ");
    for (int k = 10; k < 16; k++) {
      Serial.print(cmd->RxData[k], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else Serial.println("fail");
}

void ConfigParameters(int moving_distance, int resting_distance, int delay_time) {
  cmd->DataLength = 20;
  cmd->TxData[6] = 0x60;
  cmd->TxData[7] = 0x00;
  cmd->TxData[8] = 0x00;
  cmd->TxData[9] = 0x00;
  cmd->TxData[10] = moving_distance;
  cmd->TxData[11] = 0x00;
  cmd->TxData[12] = 0x00;
  cmd->TxData[13] = 0x00;
  cmd->TxData[14] = 0x01;
  cmd->TxData[15] = 0x00;
  cmd->TxData[16] = resting_distance;
  cmd->TxData[17] = 0x00;
  cmd->TxData[18] = 0x00;
  cmd->TxData[19] = 0x00;
  cmd->TxData[20] = 0x02;
  cmd->TxData[21] = 0x00;
  cmd->TxData[22] = delay_time & 0xFF;
  cmd->TxData[23] = (delay_time >> 8) & 0xFF;
  // cmd->TxData[22] = 0x06;
  // cmd->TxData[23] = 0x00;
  cmd->TxData[24] = 0x00;
  cmd->TxData[25] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) Serial.println("config success");
  else Serial.println("config fail");
}

void ConfigSensitivity(int gate, int motion_sensitivity, int static_sensitivity) {
  cmd->DataLength = 20;
  cmd->TxData[6] = 0x64;
  cmd->TxData[7] = 0x00;
  cmd->TxData[8] = 0x00;
  cmd->TxData[9] = 0x00;
  if (gate == 9) {
    cmd->TxData[10] = 0xFF;
    cmd->TxData[11] = 0xFF;
  }
  cmd->TxData[10] = gate;
  cmd->TxData[11] = 0x00;
  cmd->TxData[12] = 0x00;
  cmd->TxData[13] = 0x00;
  cmd->TxData[14] = 0x01;
  cmd->TxData[15] = 0x00;
  cmd->TxData[16] = motion_sensitivity & 0xFF;
  cmd->TxData[17] = (motion_sensitivity >> 8) & 0xFF;
  cmd->TxData[18] = 0x00;
  cmd->TxData[19] = 0x00;
  cmd->TxData[20] = 0x02;
  cmd->TxData[21] = 0x00;
  cmd->TxData[22] = static_sensitivity & 0xFF;
  cmd->TxData[23] = (static_sensitivity >> 8) & 0xFF;
  cmd->TxData[24] = 0x00;
  cmd->TxData[25] = 0x00;
  SendCommand();
  delay(100);
  if (RecieveResponse()) Serial.println("config success");
  else Serial.println("config fail");
}

void ConfigAllSensitivity(int value_1, int value_2, int value_3, int value_4, int value_5, int value_6, int value_7, int value_8, int value_9, int value_10, int value_11, int value_12, int value_13, int value_14, int value_15, int value_16) {
  ConfigSensitivity(0, value_1, 0);
  ConfigSensitivity(1, value_2, 0);
  ConfigSensitivity(2, value_3, value_10);
  ConfigSensitivity(3, value_4, value_11);
  ConfigSensitivity(4, value_5, value_12);
  ConfigSensitivity(5, value_6, value_13);
  ConfigSensitivity(6, value_7, value_14);
  ConfigSensitivity(7, value_8, value_15);
  ConfigSensitivity(8, value_9, value_16);
}