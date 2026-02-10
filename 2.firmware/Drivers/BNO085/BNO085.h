/*
 * BNO085.h
 *
 *  Created on: 2026. 2. 10.
 *      Author: jojae
 */

#ifndef BNO085_BNO085_H_
#define BNO085_BNO085_H_

#include "hw_def.h"

#ifndef _USE_BNO080

#include "cli.h"

//////////////////////////////////////////////////////////////////////////
#define printf(...) cliPrintf(__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////

#define BNO085_SPI_CHANNEL    SPI3

#define BNO085_SPI_SCLK_PIN   LL_GPIO_PIN_10
#define BNO085_SPI_SCLK_PORT  GPIOC
#define BNO085_SPI_SCLK_CLK   LL_AHB1_GRP1_PERIPH_GPIOC

#define BNO085_SPI_MISO_PIN   LL_GPIO_PIN_11
#define BNO085_SPI_MISO_PORT  GPIOC
#define BNO085_SPI_MISO_CLK   LL_AHB1_GRP1_PERIPH_GPIOC

#define BNO085_SPI_MOSI_PIN   LL_GPIO_PIN_12
#define BNO085_SPI_MOSI_PORT  GPIOC
#define BNO085_SPI_MOSI_CLK   LL_AHB1_GRP1_PERIPH_GPIOC

#define BNO085_SPI_CS_PIN     SPI3_NSS_Pin
#define BNO085_SPI_CS_PORT    SPI3_NSS_GPIO_Port
#define BNO085_SPI_CS_CLK     LL_AHB1_GRP1_PERIPH_GPIOC

#define BNO085_PS0_WAKE_PIN   BNO085_PS0_Pin
#define BNO085_PS0_WAKE_PORT  BNO085_PS0_GPIO_Port
#define BNO085_PS0_WAKE_CLK   LL_AHB1_GRP1_PERIPH_GPIOG

#define BNO085_PS1_PIN      BNO085_PS1_Pin
#define BNO085_PS1_PORT     BNO085_PS1_GPIO_Port
#define BNO085_PS1_CLK      LL_AHB1_GRP1_PERIPH_GPIOG

#define BNO085_RST_PIN      BNO085_RST_Pin
#define BNO085_RST_PORT     BNO085_RST_GPIO_Port
#define BNO085_RST_CLK      LL_AHB1_GRP1_PERIPH_GPIOC

#define BNO085_INT_PIN      BNO085_INT_Pin
#define BNO085_INT_PORT     BNO085_INT_GPIO_Port
#define BNO085_INT_CLK      LL_AHB1_GRP1_PERIPH_GPIOD

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define CHIP_SELECT(BNO085)   LL_GPIO_ResetOutputPin(BNO085_SPI_CS_PORT, BNO085_SPI_CS_PIN)
#define CHIP_DESELECT(BNO085) LL_GPIO_SetOutputPin(BNO085_SPI_CS_PORT, BNO085_SPI_CS_PIN)

#define WAKE_HIGH()       LL_GPIO_SetOutputPin(BNO085_PS0_WAKE_PORT, BNO085_PS0_WAKE_PIN)
#define WAKE_LOW()        LL_GPIO_ResetOutputPin(BNO085_PS0_WAKE_PORT, BNO085_PS0_WAKE_PIN)

#define PS1_HIGH()       LL_GPIO_SetOutputPin(BNO085_PS1_PORT, BNO085_PS1_PIN)
#define PS1_LOW()        LL_GPIO_ResetOutputPin(BNO085_PS1_PORT, BNO085_PS1_PIN)

#define RESET_HIGH()      LL_GPIO_SetOutputPin(BNO085_RST_PORT, BNO085_RST_PIN)
#define RESET_LOW()       LL_GPIO_ResetOutputPin(BNO085_RST_PORT, BNO085_RST_PIN)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define IS_INT_HIGH()   LL_GPIO_IsInputPinSet(BNO085_INT_PORT, BNO085_INT_PIN)



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    SPI_INIT,
    SPI_DUMMY,
    SPI_DFU,
    SPI_IDLE,
    SPI_RD_HDR,
    SPI_RD_BODY,
    SPI_WRITE
} SpiState_t;

//Registers
enum Registers
{
  CHANNEL_COMMAND = 0,
  CHANNEL_EXECUTABLE = 1,
  CHANNEL_CONTROL = 2,
  CHANNEL_REPORTS = 3,
  CHANNEL_WAKE_REPORTS = 4,
  CHANNEL_GYRO = 5
};

//All the ways we can configure or talk to the BNO080, figure 34, page 36 reference manual
//These are used for low level communication with the sensor, on channel 2
#define SHTP_REPORT_COMMAND_RESPONSE 0xF1
#define SHTP_REPORT_COMMAND_REQUEST 0xF2
#define SHTP_REPORT_FRS_READ_RESPONSE 0xF3
#define SHTP_REPORT_FRS_READ_REQUEST 0xF4
#define SHTP_REPORT_PRODUCT_ID_RESPONSE 0xF8
#define SHTP_REPORT_PRODUCT_ID_REQUEST 0xF9
#define SHTP_REPORT_BASE_TIMESTAMP 0xFB
#define SHTP_REPORT_SET_FEATURE_COMMAND 0xFD

//All the different sensors and features we can get reports from
//These are used when enabling a given sensor
#define SENSOR_REPORTID_ACCELEROMETER 0x01
#define SENSOR_REPORTID_GYROSCOPE 0x02
#define SENSOR_REPORTID_MAGNETIC_FIELD 0x03
#define SENSOR_REPORTID_LINEAR_ACCELERATION 0x04
#define SENSOR_REPORTID_ROTATION_VECTOR 0x05
#define SENSOR_REPORTID_GRAVITY 0x06
#define SENSOR_REPORTID_GAME_ROTATION_VECTOR 0x08
#define SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR 0x09
#define SENSOR_REPORTID_TAP_DETECTOR 0x10
#define SENSOR_REPORTID_STEP_COUNTER 0x11
#define SENSOR_REPORTID_STABILITY_CLASSIFIER 0x13
#define SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER 0x1E

//Record IDs from figure 29, page 29 reference manual
//These are used to read the metadata for each sensor type
#define FRS_RECORDID_ACCELEROMETER 0xE302
#define FRS_RECORDID_GYROSCOPE_CALIBRATED 0xE306
#define FRS_RECORDID_MAGNETIC_FIELD_CALIBRATED 0xE309
#define FRS_RECORDID_ROTATION_VECTOR 0xE30B

//Command IDs from section 6.4, page 42
//These are used to calibrate, initialize, set orientation, tare etc the sensor
#define COMMAND_ERRORS 1
#define COMMAND_COUNTER 2
#define COMMAND_TARE 3
#define COMMAND_INITIALIZE 4
#define COMMAND_DCD 6
#define COMMAND_ME_CALIBRATE 7
#define COMMAND_DCD_PERIOD_SAVE 9
#define COMMAND_OSCILLATOR 10
#define COMMAND_CLEAR_DCD 11

#define CALIBRATE_ACCEL 0
#define CALIBRATE_GYRO 1
#define CALIBRATE_MAG 2
#define CALIBRATE_PLANAR_ACCEL 3
#define CALIBRATE_ACCEL_GYRO_MAG 4
#define CALIBRATE_STOP 5

#define MAX_PACKET_SIZE 128 //Packets can be up to 32k but we don't have that much RAM.
#define MAX_METADATA_SIZE 9 //This is in words. There can be many but we mostly only care about the first 9 (Qs, range, etc)

void BNO085_GPIO_SPI_Initialization(void);
int BNO085_Initialization(void);
unsigned char SPI_SendByte(unsigned char data);

int BNO085_dataAvailable(void);
void BNO085_parseCommandReport(void);
void BNO085_parseInputReport(void);

float BNO085_getQuatI();
float BNO085_getQuatJ();
float BNO085_getQuatK();
float BNO085_getQuatReal();
float BNO085_getQuatRadianAccuracy();
uint8_t BNO085_getQuatAccuracy();
float BNO085_getAccelX();
float BNO085_getAccelY();
float BNO085_getAccelZ();
uint8_t BNO085_getAccelAccuracy();
float BNO085_getLinAccelX();
float BNO085_getLinAccelY();
float BNO085_getLinAccelZ();
uint8_t BNO085_getLinAccelAccuracy();
float BNO085_getGyroX();
float BNO085_getGyroY();
float BNO085_getGyroZ();
uint8_t BNO085_getGyroAccuracy();
float BNO085_getMagX();
float BNO085_getMagY();
float BNO085_getMagZ();
uint8_t BNO085_getMagAccuracy();
uint16_t BNO085_getStepCount();
uint8_t BNO085_getStabilityClassifier();
uint8_t BNO085_getActivityClassifier();
uint32_t BNO085_getTimeStamp();
int16_t BNO085_getQ1(uint16_t recordID);
int16_t BNO085_getQ2(uint16_t recordID);
int16_t BNO085_getQ3(uint16_t recordID);
float BNO085_getResolution(uint16_t recordID);
float BNO085_getRange(uint16_t recordID);

uint32_t BNO085_readFRSword(uint16_t recordID, uint8_t wordNumber);
void BNO085_frsReadRequest(uint16_t recordID, uint16_t readOffset, uint16_t blockSize);
int BNO085_readFRSdata(uint16_t recordID, uint8_t startLocation, uint8_t wordsToRead);
void BNO085_softReset(void);
uint8_t BNO085_resetReason();

float BNO085_qToFloat(int16_t fixedPointValue, uint8_t qPoint);

void BNO085_enableRotationVector(uint16_t timeBetweenReports);
void BNO085_enableGameRotationVector(uint16_t timeBetweenReports);
void BNO085_enableAccelerometer(uint16_t timeBetweenReports);
void BNO085_enableLinearAccelerometer(uint16_t timeBetweenReports);
void BNO085_enableGyro(uint16_t timeBetweenReports);
void BNO085_enableMagnetometer(uint16_t timeBetweenReports);
void BNO085_enableStepCounter(uint16_t timeBetweenReports);
void BNO085_enableStabilityClassifier(uint16_t timeBetweenReports);

void BNO085_calibrateAccelerometer();
void BNO085_calibrateGyro();
void BNO085_calibrateMagnetometer();
void BNO085_calibratePlanarAccelerometer();
void BNO085_calibrateAll();
void BNO085_endCalibration();
int BNO085_calibrationComplete();

void BNO085_setFeatureCommand(uint8_t reportID, uint32_t microsBetweenReports, uint32_t specificConfig);
void BNO085_sendCommand(uint8_t command);
void BNO085_sendCalibrateCommand(uint8_t thingToCalibrate);
void BNO085_requestCalibrationStatus();
void BNO085_saveCalibration();

int BNO085_waitForSPI(void);
int BNO085_receivePacket(void);
int BNO085_sendPacket(uint8_t channelNumber, uint8_t dataLength);

void BNO085_Main(void);


#endif


#endif /* BNO085_BNO085_H_ */
