//GND - GND
//VCC - VCC
//SDA - Pin 2
//SCL - Pin 14

#include <Wire.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// definir registros de la función "wake up motion"
#define PWR_MGMT_1      0x6B

//Funcion auxiliar lectura
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.endTransmission();

   Wire.requestFrom(Address, Nbytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}


// Funcion auxiliar de escritura
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.write(Data);
   Wire.endTransmission();
}


void acelconfig()
{
   Wire.begin();
   Serial.begin(9600);
  Serial.println("Inicializando...");
  Serial.println("Configurando acelerónetro...");
   // Configurar acelerometro
   I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);

   // Configurar giroscopio
     Serial.println("Configurando giroscopio...");
   I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
   // Configurar magnetometro

    Serial.println("Configurando magnetómetro...");
   I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);
   I2CwriteByte(MAG_ADDRESS, 0x0A, 0x01);

// -----------------------------------------------------------------------------
// In PWR_MGMT_1 (0x6B) make CYCLE =0, SLEEP = 0 and STANDBY = 0
I2CwriteByte(0x68, 0x6B, 0);
// -----------------------------------------------------------------------------
// In PWR_MGMT_2 (0x6C) set DIS_XA, DIS_YA, DIS_ZA = 0 and DIS_XG, DIS_YG, DIS_ZG = 1
I2CwriteByte (0x68, 0x6C, 7);
// -----------------------------------------------------------------------------
// In ACCEL_CONFIG 2 (0x1D) set ACCEL_FCHOICE_B = 1 and A_DLPFCFG[2:]=1(b001)
I2CwriteByte (0x68, 0x1D, 9);
// -----------------------------------------------------------------------------
// In INT_ENABLE (0x38), set the whole register to 0x40 to enable motion interrupt only.
I2CwriteByte (0x68, 0x38, 64);
// -----------------------------------------------------------------------------
// In MOT_DETECT_CTRL (0x69), set ACCEL_INTEL_EN = 1 and ACCEL_INTEL_MODE = 1
I2CwriteByte (0x68, 0x69, 192);
// -----------------------------------------------------------------------------
// In WOM_THR (0x1F), set the WOM_Threshold[7:0] to 1~255 LSBs (0~1020mg)
I2CwriteByte (0x68, 0x1F, 10); // umbral sensibilidad
// -----------------------------------------------------------------------------
// In LP_ACCEL_ODR (0x1E), set Lposc_clksel[3:0] = 0.24Hz ~ 500Hz
I2CwriteByte (0x68, 0x1E, 100); //
// -----------------------------------------------------------------------------
// In PWR_MGMT_1 (0x6B) make CYCLE =1
I2CwriteByte (0x68, 0x6B, 32);
// definir pin interrupcion 4
// devolver por pantalla algo al hacer interrupcion
//
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

}


void medidas()
{




   // ---  Lectura acelerometro y giroscopio ---
   uint8_t Buf[14];
   int FS_ACC = 16;
   int FS_GYRO = 2000;



   I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);

//   // Convertir registros acelerometro
   float ax = (Buf[0] << 8 | Buf[1]);
   float ay = (Buf[2] << 8 | Buf[3]);
   float az = Buf[4] << 8 | Buf[5];

   ax = ax*FS_ACC/32768 ;
   ay = ay*FS_ACC/32768 ;
   az = az*FS_ACC/32768 ;

//   // Convertir registros giroscopio
   float gx = (Buf[8] << 8 | Buf[9]);
   float gy = (Buf[10] << 8 | Buf[11]);
   float gz = Buf[12] << 8 | Buf[13];

   gx = gx*FS_GYRO/32768;
   gy = gy*FS_GYRO/32768;
   gz = gz*FS_GYRO/32768;
}
