#include "Tinker.h"

SPIClass SoftSPI(VSPI);

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
  pinMode(PIN_SOFT_SPI_MISO, INPUT);
  pinMode(PIN_SOFT_SPI_MOSI, OUTPUT);
  pinMode(PIN_SOFT_SPI_CLK, OUTPUT);
  pinMode(PIN_SPI_CS, OUTPUT);
  digitalWrite(PIN_SPI_CS, HIGH); // Chân CS nên ở mức cao khi không sử dụng

  // Khởi tạo SoftSPI
  SoftSPI.begin(PIN_SOFT_SPI_CLK, PIN_SOFT_SPI_MISO, PIN_SOFT_SPI_MOSI, -1);
}

void SendData() {
  van_toc velocity = GY_86_data();
  RecieveResponse();
  read_pcf();
  //luu tru cac ket qua cua GY-86 truoc khi gui
  data->txdata[0] = handleData();
  data->txdata[1] = velocity.velocityX;
  data->txdata[2] = velocity.velocityY;
  data->txdata[3] = velocity.velocityZ;
  data->txdata[4] = velocity.temperature;
  //luu tru cac ket qua cua HLKLD2410
  data->txdata[5] = DetectedHuman();
  data->txdata[6] = readUartData();
  //luu tru cac ket qua cua cb hanh trinh
  for(int i =0; i<32; i++){
    data->txdata[6+i] = sta_ptr()->status_pcf_all[i];
  }
  //end command
  data->txdata[38] = 255;
  data->txdata[39] = 255;
  data->txdata[40] = 255;
  // Gửi dữ liệu đi qua SoftSPI
  digitalWrite(PIN_SPI_CS, LOW); // Giảm CS xuống mức thấp trước khi truyền
  for (int i = 0; i < 40; i++) {
    SoftSPI.transfer(data->txdata[i]);
  }
  digitalWrite(PIN_SPI_CS, HIGH); // Khi gửi xong, nâng CS lên mức cao

  // Các xử lý khác (nếu cần)
  delay(1000); // Delay 1 giây trước khi gửi lại
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