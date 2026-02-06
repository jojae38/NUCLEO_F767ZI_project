/*
 * bno085_common.h
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#ifndef BNO085_BNO085_COMMON_H_
#define BNO085_BNO085_COMMON_H_

#include "hw_def.h"
#include "cli.h"

//COMM TYPE
#define BNO_SPI
//#define BNO_I2C
//#define BNO_UART
//#define BNO_UARTRVC

//PIN ANNOUNCE
#define BNO_RST_PORT    BNO085_RST_GPIO_Port
#define BNO_RST_PIN     BNO085_RST_Pin
#define BNO_PS0_PORT    BNO085_PS0_GPIO_Port
#define BNO_PS0_PIN     BNO085_PS0_Pin
#define BNO_PS1_PORT    BNO085_PS1_GPIO_Port
#define BNO_PS1_PIN     BNO085_PS1_Pin
#define BNO_INT_PORT    BNO085_INT_GPIO_Port
#define BNO_INT_PIN     BNO085_INT_Pin

#ifdef BNO_SPI
extern SPI_HandleTypeDef hspi3;
#define BNO_SPI_HANDLER   hspi3
#endif

#ifdef BNO_I2C
#define BNO_ADDR7  0x4A
#define BNO_ADDR8  (BNO_ADDR7 << 1)
#define I2C_BUFFER_MAX 1024
#endif

#ifdef BNO_UART

#endif

#ifdef BNO_UARTRVC

#endif

//COMMON DEFINE
#define BNO_BUFFER_LIMIT        0x7FFF
#define BNO_DEFAULT_WAIT_TIME   500
#define BNO_TX_BUFFER_LIMIT     21
#define BNO_RX_BUFFER_LIMIT     284

typedef struct{
  uint32_t raw_acc;

}bno085_sensor_t;

typedef enum{
  BNO_OK = 0,
  BNO_ERROR,
  BNO_TIMEOUT,
}BNO_STATE_E;

void PS0_wake(bool state);
void PS1(bool state);
void RSTN(bool state);
bool getIntState(void);


// For telescope use we need to be able to calibrate and change the orientation
#ifdef USE_FOR_TELESCOPE
  #define ACCELEROMETER                 0X01
  #define GYROSCOPE_CALIBRATED          0X02
  #define MAGNETIC_FIELD_CALIBRATED     0X03
  #define ROTATION_VECTOR               0X05
  #define GAME_ROTATION_VECTOR          0X08
  #define USE_REORIENT // Used to set X to North, Y to East and Z down
#else
  //Features that can be used. Play with the conditions above to activate them
  #define RAW_ACCELEROMETER             0X14
  #define ACCELEROMETER                 0X01
  #define LINEAR_ACCELERATION           0X04
  #define GRAVITY                       0X06
  #define RAW_GYROSCOPE                 0X15
  #define GYROSCOPE_CALIBRATED          0X02
  #define GYROSCOPE_UNCALIBRATED        0X07
  #define RAW_MAGNETOMETER              0X16
  #define MAGNETIC_FIELD_CALIBRATED     0X03
  #define MAGNETIC_FIELD_UNCALIBRATED   0X0F
  #define ROTATION_VECTOR               0X05
  #define GAME_ROTATION_VECTOR          0X08
  #define GEOMAGNETIC_ROTATION_VECTOR   0X09
  #define PRESSURE                      0X0A
  #define AMBIENT_LIGHT                 0X0B
  #define HUMIDITY                      0X0C
  #define PROXIMITY                     0X0D
  #define TEMPERATURE                   0X0E
  #define RESERVED                      0X17
  #define TAP_DETECTOR                  0X10
  #define STEP_DETECTOR                 0X18
  #define STEP_COUNTER                  0X11
  #define SIGNIFICANT_MOTION            0X12
  #define STABILITY_CLASSIFIER          0X13
  #define SHAKE_DETECTOR                0X19
  #define FLIP_DETECTOR                 0X1A
  #define PICKUP_DETECTOR               0X1B
  #define STABILITY_DETECTOR            0X1C
  #define PERSONAL_ACTIVITY_CLASSIFIER  0X1E
  #define SLEEP_DETECTOR                0X1F
  #define TILT_DETECTOR                 0X20
  #define POCKET_DETECTOR               0X21
  #define CIRCLE_DETECTOR               0X22
  #define HEART_RATE_MONITOR            0X23
  #define ARVR_STABILIZED_RV            0X28
  #define ARVR_STABILIZED_GRV           0X29
  #define GYRO_INTEGRATED_RV            0X2A
  #define IZRO_MOTION_REQUEST           0X2B
  #define RAW_OPTICAL_FLOW              0X2C
  #define DEAD_RECKONING_POSE           0X2D
  #define WHEEL_ENCODER                 0X2E
#endif

// Address
#define BNO_W_ADDR                      0x96
#define BNO_R_ADDR                      0x97
#define ADDR_BNO_DFU_W_ADDR             0x52
#define ADDR_BNO_DFU_R_ADDR             0x53

// Delays
#define RESET_DELAY                     200
#define PORT_TIMEOUT                    500
#define Q_FLIP                          0.70710678118654752440084436210485
#define _180_DIV_PI                     57.295779515f // = 180 / PI

// Array sizes
#define SEQUENCE_SIZE                   6
#define HEADER_SIZE                     4
#define RX_PACKET_SIZE                  284 // For BNO086 276 for BNO080
#define TX_PACKET_SIZE                  21

// Channels
#define CHANNEL_COMMAND                 0
#define CHANNEL_EXECUTABLE              1
#define CHANNEL_CONTROL                 2
#define CHANNEL_REPORTS                 3
#define CHANNEL_WAKE_REPORTS            4
#define CHANNEL_GYRO                                5

// Reports
#define REPORT_UNSOLICITED_RESPONSE     0x00
#define REPORT_UNSOLICITED_RESPONSE1    0x01

#define REPORT_SENSOR_FLUSH_REQUEST     0xF0
#define REPORT_SENSOR_FLUSH_RESPONSE    0xEF

#define REPORT_COMMAND_REQUEST          0xF2
#define REPORT_COMMAND_RESPONSE         0xF1

#define REPORT_FRS_READ_REQUEST         0xF4
#define REPORT_FRS_READ_RESPONSE        0xF3

#define REPORT_FRS_WRITE_REQUEST        0xF7
#define REPORT_FRS_WRITE_DATA_REQUEST   0xF6
#define REPORT_FRS_WRITE_RESPONSE       0xF5

#define REPORT_PRODUCT_ID_REQUEST       0xF9
#define REPORT_PRODUCT_ID_RESPONSE      0xF8


#define REPORT_TIMESTAMP_REBASE         0xFA
#define REPORT_BASE_TIMESTAMP_REF       0xFB

#define REPORT_GET_FEATURE_REQUEST      0xFE
#define REPORT_GET_FEATURE_RESPONSE     0xFC

#define REPORT_SET_FEATURE_COMMAND      0xFD

// Command and Subcommand values
#define COMMAND_ERRORS                  1
#define COMMAND_COUNTER                 2

#define COMMAND_TARE                    3
#define COMMAND_TARE_NOW                0
#define COMMAND_TARE_PERSIST            1
#define COMMAND_TARE_REORIENT           2

#define COMMAND_INITIALIZE              4
#define COMMAND_INITIALIZE_RESET        1
#define COMMAND_INITIALIZE_ON           2
#define COMMAND_INITIALIZE_SLEEP        3

#define COMMAND_SAVE_DCD                6

#define COMMAND_ME_CALIBRATE            7
#define COMMAND_ME_CALIBRATE_CONFIG     0
#define COMMAND_ME_CALIBRATE_GET        1

#define COMMAND_DCD_PERIOD_SAVE         9

#define COMMAND_OSCILLATOR              0x0A
#define COMMAND_CLEAR_DCD_RESET         0x0B
#define COMMAND_TURNTABLE_CAL           0x0C

#define COMMAND_BOOTLOADER              0x0D
#define COMMAND_BOOTLOADER_MODE_REQ     0x00
#define COMMAND_BOOTLOADER_STATUS_REQ   0x01

#define COMMAND_INTERACTIVE_CAL_REQ     0x0E
#define COMMAND_WHEEL_REQ               0x0F

#define COMMAND_UNSOLICITED_INITIALIZE  0x84

#define CALIBRATE_ACCEL                 0
#define CALIBRATE_GYRO                  1
#define CALIBRATE_MAG                   2
#define CALIBRATE_PLANAR_ACCEL          3
#define CALIBRATE_ON_TABLE              4
#define CALIBRATE_ACCEL_GYRO_MAG        5
#define CALIBRATE_ALL                   6

#define CALIBRATE_STOP                  5


//Record IDs from SH-2 figure 28
//These are used to read and set various configuration options
#define FRS_RECORDID_SERIAL_NUMBER                0x4B4B
#define FRS_RECORDID_SYSTEM_ORIENTATION           0x2D3E

// FRS write status values
#define FRS_WRITE_STATUS_RECEIVED                 0
#define FRS_WRITE_STATUS_UNRECOGNIZED_FRS_TYPE    1
#define FRS_WRITE_STATUS_BUSY                     2
#define FRS_WRITE_STATUS_WRITE_COMPLETED          3
#define FRS_WRITE_STATUS_READY                    4
#define FRS_WRITE_STATUS_FAILED                   5
#define FRS_WRITE_STATUS_NOT_READY                6 // data received when not in write mode
#define FRS_WRITE_STATUS_INVALID_LENGTH           7
#define FRS_WRITE_STATUS_RECORD_VALID             8
#define FRS_WRITE_STATUS_INVALID_RECORD           9
#define FRS_WRITE_STATUS_DEVICE_ERROR             10
#define FRS_WRITE_STATUS_READ_ONLY                11

// Status values
#define FRS_READ_STATUS_NO_ERROR                        0
#define FRS_READ_STATUS_UNRECOGNIZED_FRS_TYPE           1
#define FRS_READ_STATUS_BUSY                            2
#define FRS_READ_STATUS_READ_RECORD_COMPLETED           3
#define FRS_READ_STATUS_OFFSET_OUT_OF_RANGE             4
#define FRS_READ_STATUS_RECORD_EMPTY                    5
#define FRS_READ_STATUS_READ_BLOCK_COMPLETED            6
#define FRS_READ_STATUS_READ_BLOCK_AND_RECORD_COMPLETED 7
#define FRS_READ_STATUS_DEVICE_ERROR                    8

// The below values are pre-calculated using SCALE_Q(n) = (1.0f / (1 << n))
#define SCALE_Q4      0.06250000000000000000
#define SCALE_Q7      0.00781250000000000000
#define SCALE_Q8      0.00390625000000000000
#define SCALE_Q9      0.00195312500000000000
#define SCALE_Q10     0.00097656250000000000
#define SCALE_Q12     0.00024414062500000000
#define SCALE_Q14     0.00006103515625000000
#define SCALE_Q17     0.00000762939453125000
#define SCALE_Q20     0.00000095367431640625
#define SCALE_Q25     0.00000002980232238770
#define SCALE_Q30     0.00000000093132257462
#define SCALE_TO_Q14  16384.0

// General Enums
typedef enum {
  NA = 0,
  PowerOnReset,
  InternalSystemReset,
  WatchdogTimeout,
  ExternalReset,
  Other,
} BNO_ResetCause_t;

typedef enum {
  InternalOscillator = 0,
  ExternalCrystal,
  ExternalClock,
  OscillatorError
} BNO_Oscillator_t;

typedef enum {
  ResetToBootloader = 0, // Reset to bootloader Mode
  UpgradeApp, //1 – Upgrade Application Mode; upgrade the application image in flash.
  ValidateImage, //2 – Validate Image Mode; validate an application image without updating the flash
  LauchApp, //3 – Launch Application; launch the application image in flash
  BootInvalid
} BNO_BootMode_t;

typedef enum {
  MOTION_INTENT_UNKNOWN = 0, // FME_MOBILE_MOTION_INTENT_UNKNOWN – this is the initial state assumed by the sensor hub
  INTENT_STATIONARY_WITHOUT_VIBRATION, //
  MOTION_INTENT_STATIONARY_WITH_VIBRATION, //
  MOTION_INTENT_IN_MOTION, //
  MOTION_INTENT_IN_MOTION_ACCELERATING
} BNO_MotionIntent_t;

// General structures

typedef struct __attribute__((packed)) {
  BNO_ResetCause_t  rst_Cause;  // report 0xF8 byte 1 Reset Cause
  uint8_t           sw_Maj;     // report 0xF8 byte 2 SW Version Major
  uint8_t           sw_Min;     // report 0xF8 byte 3 SW Version Minor
  uint32_t          sw_PN;      // report 0xF8 byte 4-7 SW Part Number
  uint32_t          sw_BN;      // report 0xF8 byte 8-11 SW Build Number
  uint16_t          sw_VP;      // report 0xF8 byte 12-13 SW Version Patch
} BNO_productID_t;

typedef struct __attribute__((packed)) {
  uint8_t status;
  uint16_t wordOffset;
} BNO_FrsWriteResp_t;

typedef struct __attribute__((packed)) {
  uint8_t len_status;
  uint16_t wordOffset;
  uint32_t data0;
  uint32_t data1;
  uint16_t frsType;
} BNO_FrsReadResp_t;

#define RESPONSE_VALUES 11
typedef struct __attribute__((packed)) {
  uint8_t seq;
  uint8_t command;
  uint8_t commandSeq;
  uint8_t respSeq;
  uint8_t r[RESPONSE_VALUES];
} BNO_CommandResp_t;

#define COMMAND_PARAMS  9
typedef struct __attribute__((packed)) {
    uint8_t reportId;
    uint8_t seq;
    uint8_t command;
    uint8_t p[COMMAND_PARAMS];
} BNO_CommandReq_t;

#ifdef USE_ERROR_REPORT
typedef struct __attribute__((packed)) {
  uint8_t Severity; // Severity – the severity of the error currently being reported
  uint8_t SeqNo;    // Error sequence number. A monotonically incrementing uin8_t that counts all the errors generated for the reported severity. It may rollover.
  uint8_t Source;   // Error source. 0 – reserved, 1 – MotionEngine, 2 – MotionHub, 3 – SensorHub, 4 – Chip level executable, 5-254 reserved. 255 – no error to report.
  uint8_t Error;    // Error. See library API
  uint8_t Module;   // Error module. See library API
  uint8_t Code;     // Error code. See library API
} BNO_Error_t;
#endif

#ifdef USE_COUNTER_REPORT
typedef struct __attribute__((packed)) {
  uint32_t offered;   // [events]
  uint32_t accepted;  // [events]
  uint32_t on;        // [events]
  uint32_t attempted; // [events]
} BNO_Counts_t;
#endif

#ifdef USE_REORIENT
typedef enum {
  TARE_Z = 4,
  TARE_ALL = 7,
} BNO_TareAxis_t;

typedef enum {
  RotationVector = 0,
  GamingRotationVector = 1,
  GeomagneticRotationVector = 2,
  GyroIntegratedRotationVector = 3,
  ArVrStabilizedRotationVector = 4,
  ArVrStabilizedGameRotationVector =5,
} BNO_TareRV_t;

#endif
typedef struct __attribute__((packed)) {
  uint8_t Status;         // Status (0 – success. Non Zero – Operation failed)
  uint8_t AccCalEnable;   // Accel Cal Enable (1 – enabled, 0 – disabled)
  uint8_t GyroCalEnable;  // Gyro Cal Enable (1 – enabled, 0 – disabled)
  uint8_t MagCalEnable;   // Mag Cal Enable (1 – enabled, 0 – disabled)
  uint8_t PlanCalEnable;  // Planar Accel Cal Enable (1 – enabled, 0 – disabled)
  uint8_t OnTableEnable;  // On Table Cal Enable (1 – enabled, 0 – disabled)
} BNO_calibrationStat_t;

typedef struct __attribute__((packed)) {
  uint8_t OperationMode;
  uint8_t reserved;
  uint32_t Status;
  uint32_t Errors;
} BNO_Boot_t;

typedef struct __attribute__((packed)) {
  uint8_t sensorID; // sensor id
  uint8_t flags; // FEAT_... values
  uint16_t changeSensitivity;
  uint32_t reportInterval_uS;
  uint32_t batchInterval_uS;
  uint32_t sensorSpecific;
} BNO_Feature_t;

// Sensor values

typedef struct __attribute__((packed)) {
  float Roll;
  float Pitch;
  float Yaw;
} BNO_RollPitchYaw_t;

#ifdef RAW_ACCELEROMETER
// Raw Accelerometer
typedef struct __attribute__((packed)) {
  // Units are ADC counts
  int16_t X; // [ADC counts]
  int16_t Y; // [ADC counts]
  int16_t Z; // [ADC counts]
  uint32_t TimeStamp; // [uS]
} BNO_RawAccelerometer_t;
#endif

#if defined(ACCELEROMETER) || defined(LINEAR_ACCELERATION) || defined(GRAVITY)
// Accelerometer
typedef struct __attribute__((packed)) {
  float X;
  float Y;
  float Z;
} BNO_Accelerometer_t;
#endif

#ifdef RAW_GYROSCOPE
// Raw Gyroscope
typedef struct __attribute__((packed)) {
  // Units are ADC counts
  int16_t X; // [ADC Counts]
  int16_t Y; // [ADC Counts]
  int16_t Z; // [ADC Counts]
  int16_t Temperature; // [ADC Counts]
  uint32_t TimeStamp; // [uS]
} BNO_RawGyroscope_t;
#endif

#ifdef GYROSCOPE_CALIBRATED
// Gyroscope
typedef struct __attribute__((packed)) {
  // Units are rad/s
  float X;
  float Y;
  float Z;
} BNO_Gyroscope_t;
#endif

#ifdef GYROSCOPE_UNCALIBRATED
// Uncalibrated Gyroscope
typedef struct __attribute__((packed)) {
  // Units are rad/s
  float X; // [rad/s]
  float Y; // [rad/s]
  float Z; // [rad/s]
  float BiasX; // [rad/s]
  float BiasY; // [rad/s]
  float BiasZ; // [rad/s]
} BNO_GyroscopeUncalibrated_t;
#endif

#ifdef RAW_MAGNETOMETER
// Raw Magnetometer
typedef struct __attribute__((packed)) {
  // Units are ADC counts
  int16_t X; // [ADC Counts]
  int16_t Y; // [ADC Counts]
  int16_t Z; // [ADC Counts]
  uint32_t TimeStamp; // [uS]
} BNO_RawMagnetometer_t;
#endif

#ifdef MAGNETIC_FIELD_CALIBRATED
// Magnetic Field
typedef struct __attribute__((packed)) {
  // Units are uTesla
  float X; // [uTesla]
  float Y; // [uTesla]
  float Z; // [uTesla]
} BNO_MagneticField_t;
#endif

#ifdef MAGNETIC_FIELD_UNCALIBRATED
// Uncalibrated Magnetic Field
typedef struct __attribute__((packed)) {
  // Units are uTesla
  float X; // [uTesla]
  float Y; // [uTesla]
  float Z; // [uTesla]
  float BiasX; // [uTesla]
  float BiasY; // [uTesla]
  float BiasZ; // [uTesla]
} BNO_MagneticFieldUncalibrated_t;
#endif

#if defined(ROTATION_VECTOR) || defined(GEOMAGNETIC_ROTATION_VECTOR) || defined(ARVR_STABILIZED_RV)
// Rotation Vector with Accuracy
typedef struct __attribute__((packed)) {
  float I; // Quaternion component i
  float J; // Quaternion component j
  float K; // Quaternion component k
  float Real; // Quaternion component, real
  float Accuracy; // Accuracy estimate [radians]
} BNO_RotationVectorWAcc_t;
#endif

#if defined(GAME_ROTATION_VECTOR) || defined(ARVR_STABILIZED_GRV)
// Rotation Vector
typedef struct __attribute__((packed)) {
  float I; // Quaternion component i
  float J; // Quaternion component j
  float K; // Quaternion component k
  float Real; // Quaternion component real
} BNO_RotationVector_t;
#endif

#ifdef TAP_DETECTOR
typedef enum {
  TAPDET_X      = 1,
  TAPDET_X_POS  = 2,
  TAPDET_Y      = 4,
  TAPDET_Y_POS  = 8,
  TAPDET_Z      = 16,
  TAPDET_Z_POS  = 32,
  TAPDET_DOUBLE = 64,
} BNO_Tap_t;

#endif

#ifdef STEP_COUNTER
typedef struct __attribute__((packed)) {
    uint32_t Latency; // Step counter latency [uS].
    uint16_t Steps; // Steps counted.
} BNO_StepCounter_t;
#endif

#ifdef STABILITY_CLASSIFIER
typedef enum {
  STABILITY_CLASSIFIER_UNKNOWN = 0,
  STABILITY_CLASSIFIER_ON_TABLE,
  STABILITY_CLASSIFIER_STATIONARY,
  STABILITY_CLASSIFIER_STABLE,
  STABILITY_CLASSIFIER_MOTION,
} BNO_Stability_t;
#endif

#ifdef SHAKE_DETECTOR
typedef enum {
  SHAKE_X = 1,
  SHAKE_Y = 2,
  SHAKE_Z = 4,
} BNO_Shake_t;
#endif

#ifdef PICKUP_DETECTOR
typedef enum {
  PICKUP_LEVEL_TO_NOT_LEVEL = 1,
  PICKUP_STOP_WITHIN_REGION = 2,
} BNO_Pickup_t;
#endif

#ifdef STABILITY_DETECTOR
typedef enum {
  STABILITY_ENTERED = 1,
  STABILITY_EXITED  = 2,
} BNO_StabilityDetector_t;
#endif

#ifdef PERSONAL_ACTIVITY_CLASSIFIER
typedef enum {
  PAC_UNKNOWN = 0,
  PAC_IN_VEHICLE,
  PAC_ON_BICYCLE,
  PAC_ON_FOOT,
  PAC_STILL,
  PAC_TILTING,
  PAC_WALKING,
  PAC_RUNNING,
} BNO_PAC_t;
// Personal Activity Classifier
typedef struct __attribute__((packed)) {
  uint8_t Page;
  uint8_t LastPage;
  BNO_PAC_t MostLikelyState;
  uint8_t Confidence[10];
} BNO_PersonalActivityClassifier_t;
#endif

#ifdef GYRO_INTEGRATED_RV
// Gyro Integrated Rotation Vector
typedef struct __attribute__((packed)) {
  float I;   // Quaternion component i
  float J;   // Quaternion component j
  float K;   // Quaternion component k
  float Real;// Quaternion component real
  float AngleVelX; // Angular velocity about x [rad/s]
  float AngleVelY; // Angular velocity about y [rad/s]
  float AngleVelZ; // Angular velocity about z [rad/s]
} BNO_GyroIntegratedRV_t;
#endif

#ifdef IZRO_MOTION_REQUEST
// Interactive ZRO Motion Intent. See the SH-2 Reference Manual, 6.4.13
typedef enum {
  IZRO_MI_UNKNOWN = 0,
  IZRO_MI_STATIONARY_NO_VIBRATION,
  IZRO_MI_STATIONARY_WITH_VIBRATION,
  IZRO_MI_IN_MOTION,
  IZRO_MI_ACCELERATING,
} BNO_IZroMotionIntent_t;

typedef enum {
  IZRO_MR_NO_REQUEST = 0,
  IZRO_MR_STAY_STATIONARY,
  IZRO_MR_STATIONARY_NON_URGENT,
  IZRO_MR_STATIONARY_URGENT,
} BNO_IZroMotionRequest_t;

typedef struct __attribute__((packed)) {
  BNO_IZroMotionIntent_t Intent;
  BNO_IZroMotionRequest_t Request;
} BNO_IZroRequest_t;
#endif

#ifdef RAW_OPTICAL_FLOW
typedef struct __attribute__((packed)) {
  uint32_t TimeStamp;
  int16_t Dt;
  int16_t Dx;
  int16_t Dy;
  int16_t Iq;
  uint8_t ResX;
  uint8_t ResY;
  uint8_t Shutter;
  uint8_t FrameMax;
  uint8_t FrameAvg;
  uint8_t FrameMin;
  uint8_t LaserOn;
} BNO_RawOptFlow_t;
#endif

#ifdef DEAD_RECKONING_POSE
typedef struct __attribute__((packed)) {
  uint32_t TimeStamp;
  float LinPosX;
  float LinPosY;
  float LinPosZ;
  float I;
  float J;
  float K;
  float Real;
  float LinVelX;
  float LinVelY;
  float LinVelZ;
  float AngleVelX;
  float AngleVelY;
  float AngleVelZ;
} BNO_DeadReckoningPose_t;
#endif

#ifdef WHEEL_ENCODER
typedef struct __attribute__((packed)) {
  uint32_t TimeStamp;
  uint8_t WheelIndex;
  uint8_t DataType;
  uint16_t Data;
} BNO_WheelEncoder_t;
#endif

typedef struct __attribute__((packed)) {
  uint8_t sensorId; // Which sensor produced this event.
  uint8_t sequence; // The sequence number increments once for each report sent. Gaps in the sequence numbers indicate missing or dropped reports.
  uint8_t status; // bits 7-5: reserved, 4-2: exponent delay, 1-0: Accuracy 0 - Unreliable 1 - Accuracy low 2 - Accuracy medium 3 - Accuracy high
  uint64_t timestamp;  // [uS]
  uint32_t delay; /// [uS] value is delay * 2^exponent (see status)
  union {
    #ifdef RAW_ACCELEROMETER
      BNO_RawAccelerometer_t RawAccelerometer;
    #endif
    #ifdef ACCELEROMETER
      BNO_Accelerometer_t Accelerometer;
    #endif
    #ifdef LINEAR_ACCELERATION
      BNO_Accelerometer_t LinearAcceleration;
    #endif
    #ifdef GRAVITY
      BNO_Accelerometer_t Gravity;
    #endif
    #ifdef RAW_GYROSCOPE
      BNO_RawGyroscope_t RawGyroscope;
    #endif
    #ifdef GYROSCOPE_CALIBRATED
      BNO_Gyroscope_t Gyroscope;
    #endif
    #ifdef GYROSCOPE_UNCALIBRATED
      BNO_GyroscopeUncalibrated_t GyroscopeUncal;
    #endif
    #ifdef RAW_MAGNETOMETER
      BNO_RawMagnetometer_t RawMagnetometer;
    #endif
    #ifdef MAGNETIC_FIELD_CALIBRATED
      BNO_MagneticField_t MagneticField;
    #endif
    #ifdef MAGNETIC_FIELD_UNCALIBRATED
      BNO_MagneticFieldUncalibrated_t MagneticFieldUncal;
    #endif
    #ifdef ROTATION_VECTOR
      BNO_RotationVectorWAcc_t RotationVector;
    #endif
    #ifdef GAME_ROTATION_VECTOR
      BNO_RotationVector_t GameRotationVector;
    #endif
    #ifdef GEOMAGNETIC_ROTATION_VECTOR
      BNO_RotationVectorWAcc_t GeoMagRotationVector;
    #endif
    #ifdef PRESSURE
      float Pressure; // Atmospheric Pressure.  [hectopascals]
    #endif
    #ifdef AMBIENT_LIGHT
      float AmbientLight; // Ambient Light.  [lux]
    #endif
    #ifdef HUMIDITY
      float Humidity; // Relative Humidity.  [percent]
    #endif
    #ifdef PROXIMITY
      float Proximity; // Proximity.  [cm]
    #endif
    #ifdef TEMPERATURE
      float Temperature; // Temperature.  [C]
    #endif
    #ifdef RESERVED
      float Reserved;  // Reserved
    #endif
    #ifdef TAP_DETECTOR
      BNO_Tap_t TapDetectorFlag;
    #endif
    #ifdef STEP_DETECTOR
      uint32_t StepDetectorLatency; // Step detect latency [uS]
    #endif
    #ifdef STEP_COUNTER
      BNO_StepCounter_t StepCounter;
    #endif
    #ifdef SIGNIFICANT_MOTION
      uint16_t SignificantMotion;
    #endif
    #ifdef STABILITY_CLASSIFIER
      BNO_Stability_t StabilityClassifier;
    #endif
    #ifdef SHAKE_DETECTOR
      BNO_Shake_t ShakeDetector;
    #endif
    #ifdef FLIP_DETECTOR
      uint16_t FlipDetector;
    #endif
    #ifdef PICKUP_DETECTOR
      BNO_Pickup_t PickupDetector;
    #endif
    #ifdef STABILITY_DETECTOR
      BNO_StabilityDetector_t StabilityDetector;
    #endif
    #ifdef PERSONAL_ACTIVITY_CLASSIFIER
      BNO_PersonalActivityClassifier_t PersonalActivityClassifier;
    #endif
    #ifdef SLEEP_DETECTOR
      uint8_t SleepDetector;
    #endif
    #ifdef TILT_DETECTOR
      uint16_t TiltDetector;
    #endif
    #ifdef POCKET_DETECTOR
      uint16_t PocketDetector;
    #endif
    #ifdef CIRCLE_DETECTOR
      uint16_t CircleDetector;
    #endif
    #ifdef HEART_RATE_MONITOR
      uint16_t HeartRateMonitor; // Heart rate in beats per minute.
    #endif
    #ifdef ARVR_STABILIZED_RV
      BNO_RotationVectorWAcc_t ArVrStabilizedRV;
    #endif
    #ifdef ARVR_STABILIZED_GRV
      BNO_RotationVector_t ArVrStabilizedGRV;
    #endif
    #ifdef GYRO_INTEGRATED_RV
      BNO_GyroIntegratedRV_t GyroIntegratedRV;
    #endif
    #ifdef IZRO_MOTION_REQUEST
      BNO_IZroRequest_t IzroRequest;
    #endif
    #ifdef RAW_OPTICAL_FLOW
      BNO_RawOptFlow_t RawOptFlow;
    #endif
    #ifdef DEAD_RECKONING_POSE
      BNO_DeadReckoningPose_t DeadReckoningPose;
    #endif
    #ifdef WHEEL_ENCODER
      BNO_WheelEncoder_t WheelEncoder;
    #endif
  } SenVal;
} BNO_SensorValue_t;



#endif /* BNO085_BNO085_COMMON_H_ */
