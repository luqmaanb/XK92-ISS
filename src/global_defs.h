/**
 * @file global_defs.h
 * @author Luqmaan Baboo (lb@xinabox.cc)
 * @brief XK92 Firmware - Quest for Space ISS Mission Kit.
 *        This file defines the global variables associated with
 *        the software.
 * @version 0.1
 * @date 2020-02-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef global_defs_h
#define global_defs_h

#include <Arduino.h>
#include <esp_system.h>
#include <xSW02.h>
#include <xSN01.h>
#pragma once
// uncomment to view debug information
#define DEBUG 1
#define debug(x) Serial.print(x)
extern hw_timer_t *timer;
extern xSN01 SN01;
extern xSW02 SW02;

/**
 * @brief RGB LED pin numbers
 * 
 */
#define LED_RED 25
#define LED_GREEN 26
#define LED_BLUE 27
#define LED_RGB 255
#define LED_ON HIGH
#define LED_OFF LOW

#define CS11_I2C_ADDRESS 0x04
extern uint8_t xk92_mode[2];
/**
 * @brief String literal of software version
 * 
 */
#define FIRMWARE_VERSION "0.0.1"

/**
 * @brief 
 * 
 */
enum XK92_STATUS {
  XK92_FAIL,
  XK92_SUCCESS
};

/**
 * @brief Operation mode of XK92
 * 
 */
enum XK92_MODE {
  DATA_LOGGER,
  DATA_VISUALIZER
};

/**
 * @brief SG33 data variables
 * 
 */
typedef struct sg33_vars {
  float tvoc;
  float co2;
};

/**
 * @brief SG34 data variables
 * 
 */
typedef struct sg34_vars {
  uint32_t ir;
  uint32_t red;
  uint32_t green;
  float temperature;
};

/**
 * @brief SI01 data variables
 * 
 */
typedef struct si01_vars {
  float g_mean;
  float g_min;
  float g_max;
  float g_stddev;
  float a_mean;
  float a_min;
  float a_max;
  float a_stddev;
  float m_mean;
  float m_min;
  float m_max;
  float m_stddev;
};

/**
 * @brief SN02 data variables
 * 
 */
typedef struct sn01_vars {
  uint16_t hdop;
  String date;
  String time;
  float latitude;
  float longitude;
  float altitude;
  int noOfSat;
  float cog;
  float sog;
};

/**
 * @brief SW02 data variables
 * 
 */

typedef  struct sw02_vars {
  float rawTemp;
  float rawPress;
  float rawHumi;
  float gasRes;
  float iaq;
  float iaqAccuracy;
  float temperature;
  float humidity;
  float co2Equivalent;
  float breathVOCEquivalent;
};

/**
 * @brief SW03 data variables
 * 
 */
typedef struct sw03_vars {
  float temperature;
  float pressure;
  float altitude;
};

/**
 * @brief Network variables and literals
 * 
 */
typedef struct network_vars {
  const char*     xinaboxSSID = "XinaBox";
  const char*     xinaboxPASS = "RapidIoT";
  String          macAddress;
  String          clientId;
  String          userSSID;
  String          userPASS;
  String          unitName;
  bool            wifiStatus;
};

/**
 * @brief xChip sensor status
 * 
 */
typedef struct sensor_acks {
  bool sg33;
  bool sg34;
  bool si01;
  bool sn01;
  bool sw02;
  bool sw03;
};

/**
 * @brief Records to log to SD card
 * 
 */
typedef struct sd_vars {
  String headerrecord;
  String datarecord;
};

extern sd_vars sd_data;
extern sensor_acks sensor_ready, data_ready;
extern network_vars network_data;
extern sg33_vars sg33_data;
extern sg34_vars sg34_data;
extern si01_vars si01_data;
extern sn01_vars sn01_data;
extern sw02_vars sw02_data;
extern sw03_vars sw03_data;

#endif