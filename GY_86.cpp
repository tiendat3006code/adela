#include "GY_86.h"

MPU6050 mpu;
float previousdisplacementX = 0.0;
float previousdisplacementY = 0.0;
float previousdisplacementZ = 0.0;
float currentdisplacementX = 0.0;
float currentdisplacementY = 0.0;
float currentdisplacementZ = 0.0;
float previousAccX = 0.0;
float previousAccY = 0.0;
float previousAccZ = 0.0;
float prepositionX = 0;
float prepositionY = 0;
float prepositionZ = 0;

van_toc vantoc;
// float positionX = 0.0;
// float positionY = 0.0;
// float positionZ = 0.0;
unsigned long previousTime = 0;

void GY_86_init() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  mpu.initialize();
  Serial.println("config gy-86 success");
}

van_toc GY_86_data() {

  int16_t currentAccX, currentAccY, currentAccZ, currentGyroX, currentGyroY, currentGyroZ, currentMagnetX, currentMagnetY, currentMagnetZ;
  mpu.getMotion9(&currentAccX, &currentAccY, &currentAccZ, &currentGyroX, &currentGyroY, &currentGyroZ, &currentMagnetX, &currentMagnetY, &currentMagnetZ);

  vantoc.temperature = mpu.getTemperature();
  // int16_t currentAccX = mpu.getAccelerationX();
  // int16_t currentAccY = mpu.getAccelerationY();
  // int16_t currentAccZ = mpu.getAccelerationZ();
  unsigned long currentTime = millis();
  float deltaTime = 0.001;
  float elapsedTime = (currentTime - previousTime) / 1000.0;
  currentdisplacementX += ((currentAccX + previousAccX) / 2.0) * deltaTime - previousdisplacementX;
  currentdisplacementY += ((currentAccY + previousAccY) / 2.0) * deltaTime - previousdisplacementY;
  currentdisplacementZ += ((currentAccZ + previousAccZ) / 2.0) * deltaTime - previousdisplacementZ;
  //Tính toán vận tốc trên các trục
  // vantoc.velocityX = previousAccX + currentAccX * deltaTime;
  // vantoc.velocityY = previousAccY + currentAccY * deltaTime;
  // vantoc.velocityZ = previousAccZ + currentAccZ * deltaTime;
  vantoc.velocityX = abs(currentdisplacementX / elapsedTime - threshold_Vel_X);
  vantoc.velocityY = abs(currentdisplacementY / elapsedTime - threshold_Vel_Y);
  vantoc.velocityZ = abs(currentdisplacementZ / elapsedTime - threshold_Vel_Z);

  // Tính toán vị trí trên các trục
  vantoc.positionX = vantoc.velocityX * elapsedTime - prepositionX;
  vantoc.positionY = vantoc.velocityY * elapsedTime - prepositionY;
  vantoc.positionZ = vantoc.velocityZ * elapsedTime - prepositionZ;

  previousAccX = currentAccX;
  previousAccY = currentAccY;
  previousAccZ = currentAccZ;
  previousTime = currentTime;

  previousdisplacementX = currentdisplacementX;
  previousdisplacementY = currentdisplacementY;
  previousdisplacementZ = currentdisplacementZ;



  prepositionX = vantoc.positionX;
  prepositionY = vantoc.positionY;
  prepositionZ = vantoc.positionZ;
  // Serial.print("Position X: ");
  // Serial.print(velocityX);
  // Serial.print("  Position Y: ");
  // Serial.print(velocityY);
  // Serial.print("  Position Z: ");
  // Serial.println(velocityZ);

  previousTime = currentTime;
  // delay(100);
  // Serial.print("Gia toc (mg): ");
  // Serial.print("X = "); Serial.print(ax);
  // Serial.print(", Y = "); Serial.print(ay);
  // Serial.print(", Z = "); Serial.println(az);

  // Serial.print("Quay (°/s): ");
  // Serial.print("X = "); Serial.print(gx);
  // Serial.print(", Y = "); Serial.print(gy);
  // Serial.print(", Z = "); Serial.println(gz);

  // delay(1000); // Đợi 1 giây trước khi đọc lại dữ liệu

  return vantoc;
}
bool handleData() {
  GY_86_data();
  float delta = sqrt(pow(vantoc.velocityX, 2) + pow(vantoc.velocityY, 2) + pow(vantoc.velocityZ, 2));
    
  //Serial.println(delta);
  delay(100);
  if (delta >= threshold_gy_86) return true;
  else return false;
}