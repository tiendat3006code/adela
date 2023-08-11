#include "Tinker.h"

SPIClass SoftSPI(VSPI);

//HardwareSerial tinker_uart(1);

data_arr dt;
data_arr *data= &dt;

// void buffer_init() {
//   buffer.begin(96000, SERIAL_8N1, Rx, Tx);
//   while (!buffer) {
//     Serial.print(".");
//   }
//   Serial.println("success");
// }
void tinker_init(){
  // pinMode(PIN_SOFT_SPI_MISO, INPUT);
  // pinMode(PIN_SOFT_SPI_MOSI, OUTPUT);
  // pinMode(PIN_SOFT_SPI_CLK, OUTPUT);
  // pinMode(PIN_SPI_CS, OUTPUT);
  // digitalWrite(PIN_SPI_CS, HIGH); // Chân CS nên ở mức cao khi không sử dụng

  // // Khởi tạo SoftSPI
  // SoftSPI.begin(PIN_SOFT_SPI_CLK, PIN_SOFT_SPI_MISO, PIN_SOFT_SPI_MOSI, -1);
  Serial.begin(9600);
  LD2410_init();
  while (1) {

    if (OpenCommandMode() == true) break;
    //Serial.print(".");
  }
  // Serial.println("Open command mode success");
  
  ConfigParameters(2, 2, 0);
  EnableEngineeringMode();
  ConfigAllSensitivity(90, 85, 80, 75, 75, 65, 60, 50, 45, 85, 80, 70, 65, 60, 50, 45);
  ReadParemeters();
  CloseCommandMode();
  GY_86_init();  
  init_pcf();
  
  // tinker_uart.begin(9600, SERIAL_8N1, TINKER_RX, TINKER_TX);
  // while(!tinker_uart){
  //   Serial.print(".");
  // }
  // Serial.println("connect to tinker success");
}

void SendData() {
  bool send_or_not = false;
  van_toc velocity = GY_86_data();
  RecieveResponse();
  read_pcf();
  //luu tru cac ket qua cua GY-86 truoc khi gui
  data->txdata[0] = 1;
  // data->txdata[1] = velocity.velocityX;
  // data->txdata[2] = velocity.velocityY;
  // data->txdata[3] = velocity.velocityZ;
  // data->txdata[4] = velocity.temperature;
  data->txdata[1] = 2;
  data->txdata[2] = 3;
  data->txdata[3] = 4;
  data->txdata[4] = handleData();
  //luu tru cac ket qua cua HLKLD2410
  data->txdata[5] = DetectedHuman();
  data->txdata[6] = readUartData();
  //luu tru cac ket qua cua cb hanh trinh
  for(int i =0; i<pcf_pin; i++){
    pcf_arr *pcf_ptr = sta_ptr();
    data->txdata[7+i] = pcf_ptr->status_pcf_all[i];
    if(data->txdata[7+i] == 0) send_or_not = true;
    else send_or_not = false;
    //Serial.println(send_or_not);
    // int randomValue = random(2); // Tạo số ngẫu nhiên từ 0 đến 1
    // int result = (randomValue == 0) ? 0 : 255;
    // data->txdata[6+i] = result;
    // Serial.print("state ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(pcf_ptr->status_pcf_all[i]);
  }
  //end command
  //data->txdata[38] = 255;
  data->txdata[31] = 5;
  data->txdata[32] = 6;
  data->txdata[33] = 7;
  data->txdata[34] = 8;
  // Gửi dữ liệu đi qua SoftSPI
  //digitalWrite(PIN_SPI_CS, LOW); // Giảm CS xuống mức thấp trước khi truyền
  // for (int i = 0; i < 40; i++) {
  //   SoftSPI.transfer(data->txdata[i]);
  // }
  // digitalWrite(PIN_SPI_CS, HIGH); // Khi gửi xong, nâng CS lên mức cao

  //if (send_or_not == true){
  size_t length = sizeof(data->txdata);
  //if (send_or_not == true){
  Serial.write((uint8_t)length);
  //Serial.write((uint8_t*)data, length);
  Serial.write((uint8_t*)data->txdata, length);
  Serial.flush();
  //delay(data->time_delay);
 // }



  //Serial.write(data->txdata, sizeof(data->txdata[40]));
  //Serial.println(data->txdata[0]);
  // Các xử lý khác (nếu cần)
  //delay(1000); // Delay 1 giây trước khi gửi lại
}

// void ReceiveData() {
//   memset(data->rxdata, 0, sizeof(data->rxdata));
//   int i;
//   int l;
//   if (buffer.available() > 0) {
//     for (i = 0; i < 64; i++) {
//       l = buffer.read();
//       if (l >= 0)
//         data->rxdata[i] = (uint8_t)l;
//       else
//         break;
//     }
//   }
//   buffer.flush();
// }
// void sync(){
//   memset(data->sync_buffer, 255, 10);
//   tinker_uart.write("hello tinker");
//   // if(tinker_uart.availble() > 0){
//   //   uint8_t receivedData = SerialUART.read(); // Đọc dữ liệu từ UART
//   //   Serial.print("Received data from Raspberry Pi: ");
//   //   Serial.println(receivedData);
//   // }
//   int i,l;
//   while(tinker_uart.available() > 0 && i <= 255){
//     l = tinker_uart.read();
//     if(l >0) l = data->receivedData[i++];
//     else {
//       tinker_uart.flush();
//       break;
//     }
//   }
// }