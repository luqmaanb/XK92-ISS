/**
 * @file DATA.h
 * @author Luqmaan Baboo (lb@xinabox.cc)
 * @brief XK92 Firmware - Quest for Space ISS Mission Kit.
 *        This file contains the sensor functionality of the 
 *        XK92 Firmware.
 * @version 0.1
 * @date 2020-02-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef DATA_H
#define DATA_H

#include <Wire.h>
#include "global_defs.h"
#include <MAX30105.h>
#include <xSN01.h>
#include <xSI01.h>
#include <xSW02.h>
#include <xSG33.h>
#include <Adafruit_MPL3115A2.h>
#include <Statistic.h>
#include <HTTPClient.h>
#include <SPIFFS.h>

#define OVER_I2C 1
#define OVER_UART 2

#define SN01_I2C_ADDRESS 0x42
#define SG33_I2C_ADDRESS 0x5A
#define SG34_I2C_ADDRESS 0x57
#define SI01_I2C_ADDRESS 0x42
#define SI01_I2C_ADDRESS_AG 0x68
#define SI01_I2C_ADDRESS_M 0x3D
#define SW02_I2C_ADDRESS 0x76
#define SW03_I2C_ADDRESS 0x60

/**
 * @brief Data class
 * 
 */
class data
{
public:
    /**
     * @brief Construct a new data object
     * 
     */
    data();
    /**
     * @brief 
     * 
     */
    void begin();
    /**
     * @brief Reads the data from all sensors that were pinged
     *        during setup.
     * 
     */
    void getData();

    // public so it can be polled in main sketch

private:
    xSG33 SG33;
    xSI01 SI01;

    Adafruit_MPL3115A2 SW03 = Adafruit_MPL3115A2();
    MAX30105 SG34;

    /**
    * @brief Initializes SG34 configuration registers.
    *        Sets sensor_ready.sg34 = true if successful
    *        and appends variables to sd_data.headerrecord.
    * 
    */
    void SG34_setup();
    /**
    * @brief Initializes SI01 configuration registers.
    *        Sets sensor_ready.si01 = true if successful
    *        and appends variables to sd_data.headerrecord.
    * 
    */
    void SI01_setup();
    /**
    * @brief Initializes SW02 configuration registers.
    *        Sets sensor_ready.sw02 = true if successful
    *        and appends variables to sd_data.headerrecord.
    * 
    */
    void SW02_setup();
    /**
    * @brief Initializes SW03 configuration registers.
    *        Sets sensor_ready.sw03 = true if successful
    *        and appends variables to sd_data.headerrecord.
    * 
    */
    void SW03_setup();
    /**
    * @brief Initializes SN01 configuration registers.
    *        Sets sensor_ready.sn01 = true if successful
    *        and appends variables to sd_data.headerrecord.
    * 
    */
    void SN01_setup();
    /**
    * @brief Initializes SG33 configuration registers.
    *        Sets sensor_ready.sg33 = true if successful
    *        and appends variables to sd_data.headerrecord.
    * 
    */
    void SG33_setup();
    /**
     * @brief Reads SG34 data if sensor_ready.sg34 = true.
     *        Appends the data to sd_data.datarecord and sets
     *        data_ready.sg34 = true if data is ready successfully.
     * 
     */
    void SG34_data();
    /**
     * @brief Reads SI01 data if sensor_ready.si01 = true.
     *        Appends the data to sd_data.datarecord and sets
     *        data_ready.si01 = true if data is ready successfully.
     * 
     */
    void SI01_data();
    /**
     * @brief Reads SW02 data if sensor_ready.sw02 = true.
     *        Appends the data to sd_data.datarecord and sets
     *        data_ready.sw02 = true if data is ready successfully.
     * 
     */
    void SW02_data();
    /**
     * @brief Reads SW03 data if sensor_ready.sw03 = true.
     *        Appends the data to sd_data.datarecord and sets
     *        data_ready.sw03 = true if data is ready successfully.
     * 
     */
    void SW03_data();
    /**
     * @brief Reads SN01 data if sensor_ready.sn01 = true.
     *        Appends the data to sd_data.datarecord and sets
     *        data_ready.sn01 = true if data is ready successfully.
     * 
     */
    void SN01_data();
    /**
     * @brief Reads SG33 data if sensor_ready.sg33 = true.
     *        Appends the data to sd_data.datarecord and sets
     *        data_ready.sg33 = true if data is ready successfully.
     * 
     */
    void SG33_data();

    /**
     * @brief Downloads the almanac using HTTP and stores it
     *        in SPIFFS
     * 
     * @return true if almanac successfully downloaded.
     * @return false if http returns an error.
     */
    bool getAlmanac();
    /**
     * @brief Transmits the almanac to the SN01
     * 
     * @param protocol OVER_I2C transmits the almanac via I2C.
     *                 OVER_UART transmits the almanac via Serial.
     * @return true if tranfer successfully.
     * @return false if I2C Wire library retruns an error.
     */
    bool sendAlmanac(uint8_t protocol);

    uint8_t count = 0;
    Statistic statsa;
    Statistic statsg;
    Statistic statsm;
    bool stats_complete;
    const uint8_t STAT_COUNT = 10;
    String almanac_string;
};

#endif