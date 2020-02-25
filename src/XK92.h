/**
 * @file XK92.h
 * @author Luqmaan Baboo (lb@xinabox.cc)
 * @brief XK92 Firmware - Quest for Space ISS Mission Kit.
 *        This file contains kit specific functionality
 * @version 0.1
 * @date 2020-02-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef XK92_H
#define XK92_H

#include <Arduino.h>
#include "global_defs.h"
#include "WIFI.h"
#include "PROVISION.h"
#include "MQTT.h"
#include "DATA.h"
#include <ArduinoJson.h>
#include <NTPtimeESP.h>

/**
 * @brief XK92 main class.
 *        Consists of members specific to the XK92 - Quest for Space ISS Mission Kit Firmware.
 * 
 */
class XinaBox_xk92
{
public:
    /**
    * @brief Construct a new XinaBox_xk92 object
    * 
    */
    XinaBox_xk92();
    /**
     * @brief Initializes the firmware.
     *        Sets provisioning, wifi, mqtt, CS11 and connected sensors.
     * 
     */
    void begin();
    /**
     * @brief Sends a String of data one byte at a time to I2C slave (CS11).
     *        CS11 will log the data received to SD card.
     * 
     */
    void logData();
    /**
     * @brief Publishes data in json format to XinaBox MQTT server
     * 
     */
    void publishData();
    /**
     * @brief Reads connected sensors' variables and stores the data in network_vars structure.
     * 
     */
    void getData();

private:
    /**
    * @brief Pings the I2C address 0x04 (CS11 Slave).
    * 
    * @return true if successfully pinged.
    * @return false if ping fails.
    */
    bool pingSlave();
    /**
     * @brief Sends integers 0 - 128 one byte at a time to I2C Slave.
     * 
     * @return true if I2C Slave sends (uint8_t)99 to Master
     * @return false if I2C Slave does not return (uint8_t)99 to Master 
     */
    bool testSlave();
    /**
     * @brief Sends the data header consisting of connected sensor names and their variables to I2C Slave.
     * 
     * @return true if header sent successfully.
     * @return false if header failed.
     */
    bool send_header();
    /**
     * @brief Sends the data consisting of connected sensors to I2C Slave
     * 
     * @return true if data sent successfully.
     * @return false if data sent successfully.
     */
    bool send_data();
    /**
     * @brief Adds date/time header to headerrecord.
     * 
     */
    void dateTime_setup();
    /**
     * @brief Retrieves data and time from NTP Timer Server. 
     * 
     */
    void getDateTime();
    /**
     * @brief Builds the json file to send to XinaBox MQTT Server.
     * 
     */
    void buildJson();
    char finalJson[2048];

    uint16_t finalJsonLen;
    provision xProv;
    wifi xWifi;
    strDateTime dateTime;

    mqtt xMqtt;
    data xData;
    NTPtime NTPch = NTPtime("pool.ntp.org"); // Choose server pool as required
};
#endif