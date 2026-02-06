/*
 * bno085.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */


#include "bno085.h"


sh2_ProductIds_t prodIds;
sh2_Hal_t* pSh2Hal = NULL;
uint8_t status;
bool _resetOccurred;
sh2_SensorValue_t sensorValue;
static sh2_SensorValue_t *_sensor_value = NULL;

sh2_Hal_t bno085Open(void);
static void hal_callback(void *cookie, sh2_AsyncEvent_t *pEvent);
static void sensorHandler(void *cookie, sh2_SensorEvent_t *event);

static void startReports();
static void reportProdIds(void);

void bno085Init(void)
{
#ifdef BNO_SPI
  _resetOccurred = false;
  sh2Spi_init();
#endif

#ifdef BNO_I2C

#endif

#ifdef BNO_UARTRVC

#endif

#ifdef BNO_UART

#endif
  status = sh2_open(pSh2Hal, hal_callback, NULL);
  if(status != SH2_OK)
    return;

  memset(&prodIds, 0, sizeof(prodIds));
  status = sh2_getProdIds(&prodIds);
  if(status != SH2_OK)
    return;

  sh2_setSensorCallback(sensorHandler, NULL);
//  bnoSpiReset();

  reportProdIds();
  _resetOccurred = false;

  startReports();
}

void bno085Main(void)
{
#ifdef BNO_SPI
  pSh2Hal = sh2Spi_init();
//  bnoSpiSeq();
#endif
  if (_resetOccurred) {
      // Restart the flow of sensor reports
      _resetOccurred = false;
      startReports();
  }
  sh2_service();
}

static void hal_callback(void *cookie, sh2_AsyncEvent_t *pEvent) {
  // If we see a reset, set a flag so that sensors will be reconfigured.
  if (pEvent->eventId == SH2_RESET) {
      _resetOccurred = true;
  }
  else if (pEvent->eventId == SH2_SHTP_EVENT) {
      cliPrintf("EventHandler  id:SHTP, %d\n", pEvent->shtpEvent);
  }
  else if (pEvent->eventId == SH2_GET_FEATURE_RESP) {
      // printf("EventHandler Sensor Config, %d\n", pEvent->sh2SensorConfigResp.sensorId);
  }
  else {
      cliPrintf("EventHandler, unknown event Id: %d\n", pEvent->eventId);
  }
}

// Handle sensor events.
//static void sensorHandler(void *cookie, sh2_SensorEvent_t *event) {
//  int rc;
//
//  // Serial.println("Got an event!");
//
//  rc = sh2_decodeSensorEvent(_sensor_value, event);
//  if (rc != SH2_OK) {
//    //Serial.println("BNO08x - Error decoding sensor event");
//    _sensor_value->timestamp = 0;
//    return;
//  }
//}

static void startReports()
{
    int status;

    // Each entry of sensorConfig[] represents one sensor to be configured in the loop below
    static const struct {
        int sensorId;
        sh2_SensorConfig_t config;
    } sensorConfig[] =
    {
        // Game Rotation Vector, 100Hz
        {SH2_GAME_ROTATION_VECTOR, {.reportInterval_us = 10000}},

        // Stability Detector, 100 Hz, changeSensitivityEnabled
        // {SH2_STABILITY_DETECTOR, {.reportInterval_us = 10000, .changeSensitivityEnabled = true}},

        // Raw accel, 100 Hz
        // {SH2_RAW_ACCELEROMETER, {.reportInterval_us = 10000}},

        // Raw gyroscope, 100 Hz
        // {SH2_RAW_GYROSCOPE, {.reportInterval_us = 10000}},

        // Rotation Vector, 100 Hz
        // {SH2_ROTATION_VECTOR, {.reportInterval_us = 10000}},

        // Gyro Integrated Rotation Vector, 100 Hz
        // {SH2_GYRO_INTEGRATED_RV, {.reportInterval_us = 10000}},

        // Motion requests for Interactive Zero Reference Offset cal
        // {SH2_IZRO_MOTION_REQUEST, {.reportInterval_us = 10000}},

        // Shake detector
        // {SH2_SHAKE_DETECTOR, {.reportInterval_us = 10000}},
    };

    for (int n = 0; n < ARRAY_LEN(sensorConfig); n++)
    {
        int sensorId = sensorConfig[n].sensorId;

        status = sh2_setSensorConfig(sensorId, &sensorConfig[n].config);
        if (status != 0) {
            cliPrintf("Error while enabling sensor %d\n", sensorId);
        }
    }
}

static void reportProdIds(void)
{
    int status;

    memset(&prodIds, 0, sizeof(prodIds));
    status = sh2_getProdIds(&prodIds);

    if (status < 0) {
        cliPrintf("Error from sh2_getProdIds.\n");
        return;
    }

    // Report the results
    for (int n = 0; n < prodIds.numEntries; n++) {
        cliPrintf("Part %d : Version %d.%d.%d Build %d\n",
               prodIds.entry[n].swPartNumber,
               prodIds.entry[n].swVersionMajor, prodIds.entry[n].swVersionMinor,
               prodIds.entry[n].swVersionPatch, prodIds.entry[n].swBuildNumber);

        // Wait a bit so we don't overflow the console output.
        delay(10);
    }
}

static void printEvent(const sh2_SensorEvent_t * event)
{
    int rc;
    sh2_SensorValue_t value;
    float scaleRadToDeg = 180.0 / 3.14159265358;
    float r, i, j, k, acc_deg, x, y, z;
    float t;
    static int skip = 0;

    rc = sh2_decodeSensorEvent(&value, event);
    if (rc != SH2_OK) {
        cliPrintf("Error decoding sensor event: %d\n", rc);
        return;
    }

    t = value.timestamp / 1000000.0;  // time in seconds.
    switch (value.sensorId) {
        case SH2_RAW_ACCELEROMETER:
            cliPrintf("%8.4f Raw acc: %d %d %d time_us:%d\n",
                   (double)t,
                   value.un.rawAccelerometer.x,
                   value.un.rawAccelerometer.y,
                   value.un.rawAccelerometer.z,
                   value.un.rawAccelerometer.timestamp);
            break;

        case SH2_ACCELEROMETER:
          cliPrintf("%8.4f Acc: %f %f %f\n",
                   (double)t,
                   (double)value.un.accelerometer.x,
                   (double)value.un.accelerometer.y,
                   (double)value.un.accelerometer.z);
            break;

        case SH2_RAW_GYROSCOPE:
          cliPrintf("%8.4f Raw gyro: x:%d y:%d z:%d temp:%d time_us:%d\n",
                   (double)t,
                   value.un.rawGyroscope.x,
                   value.un.rawGyroscope.y,
                   value.un.rawGyroscope.z,
                   value.un.rawGyroscope.temperature,
                   value.un.rawGyroscope.timestamp);
            break;

        case SH2_ROTATION_VECTOR:
            r = value.un.rotationVector.real;
            i = value.un.rotationVector.i;
            j = value.un.rotationVector.j;
            k = value.un.rotationVector.k;
            acc_deg = scaleRadToDeg *
                value.un.rotationVector.accuracy;
            cliPrintf("%8.4f Rotation Vector: "
                   "r:%0.6f i:%0.6f j:%0.6f k:%0.6f (acc: %0.6f deg)\n",
                   (double)t,
                   (double)r, (double)i, (double)j, (double)k, (double)acc_deg);
            break;
        case SH2_GAME_ROTATION_VECTOR:
            r = value.un.gameRotationVector.real;
            i = value.un.gameRotationVector.i;
            j = value.un.gameRotationVector.j;
            k = value.un.gameRotationVector.k;
            cliPrintf("%8.4f GRV: "
                   "r:%0.6f i:%0.6f j:%0.6f k:%0.6f\n",
                   (double)t,
                   (double)r, (double)i, (double)j, (double)k);
            break;
        case SH2_GYROSCOPE_CALIBRATED:
            x = value.un.gyroscope.x;
            y = value.un.gyroscope.y;
            z = value.un.gyroscope.z;
            cliPrintf("%8.4f GYRO: "
                   "x:%0.6f y:%0.6f z:%0.6f\n",
                   (double)t,
                   (double)x, (double)y, (double)z);
            break;
        case SH2_GYROSCOPE_UNCALIBRATED:
            x = value.un.gyroscopeUncal.x;
            y = value.un.gyroscopeUncal.y;
            z = value.un.gyroscopeUncal.z;
            cliPrintf("%8.4f GYRO_UNCAL: "
                   "x:%0.6f y:%0.6f z:%0.6f\n",
                   (double)t,
                   (double)x, (double)y, (double)z);
            break;
        case SH2_GYRO_INTEGRATED_RV:
            // These come at 1kHz, too fast to print all of them.
            // So only print every 10th one
            skip++;
            if (skip == 10) {
                skip = 0;
                r = value.un.gyroIntegratedRV.real;
                i = value.un.gyroIntegratedRV.i;
                j = value.un.gyroIntegratedRV.j;
                k = value.un.gyroIntegratedRV.k;
                x = value.un.gyroIntegratedRV.angVelX;
                y = value.un.gyroIntegratedRV.angVelY;
                z = value.un.gyroIntegratedRV.angVelZ;
                cliPrintf("%8.4f Gyro Integrated RV: "
                       "r:%0.6f i:%0.6f j:%0.6f k:%0.6f x:%0.6f y:%0.6f z:%0.6f\n",
                       (double)t,
                       (double)r, (double)i, (double)j, (double)k,
                       (double)x, (double)y, (double)z);
            }
            break;
        case SH2_IZRO_MOTION_REQUEST:
          cliPrintf("IZRO Request: intent:%d, request:%d\n",
                   value.un.izroRequest.intent,
                   value.un.izroRequest.request);
            break;
        case SH2_SHAKE_DETECTOR:
          cliPrintf("Shake Axis: %c%c%c\n",
                   (value.un.shakeDetector.shake & SHAKE_X) ? 'X' : '.',
                   (value.un.shakeDetector.shake & SHAKE_Y) ? 'Y' : '.',
                   (value.un.shakeDetector.shake & SHAKE_Z) ? 'Z' : '.');

            break;
        case SH2_STABILITY_CLASSIFIER:
          cliPrintf("Stability Classification: %d\n",
                   value.un.stabilityClassifier.classification);
            break;
        case SH2_STABILITY_DETECTOR:
          cliPrintf("Stability Detector: %d\n",
                   value.un.stabilityDetector.stability);
            break;
        default:
          cliPrintf("Unknown sensor: %d\n", value.sensorId);
            break;
    }
}

static void sensorHandler(void * cookie, sh2_SensorEvent_t *pEvent)
{
#ifdef DSF_OUTPUT
    printDsf(pEvent);
#else
    printEvent(pEvent);
#endif
}

