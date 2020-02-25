/**
 * @file MQTT.h
 * @author Luqmaan Baboo (lb@xinabox.cc)
 * @brief XK92 Firmware - Quest for Space ISS Mission Kit.
 *        This file contains the MQTT functionality of the
 *        XK92 Firmware.
 * @version 0.1
 * @date 2020-02-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include "WIFI.h"
#include "global_defs.h"

/**
 * @brief Stucture containing MQTT credentials.
 * 
 */
struct mqtt_vars
{
    const char *xinaboxIndex = "xk92";
    const char *xinaboxServer = "mqtt.xinabox.cc";
    const char *xinaboxUsername = "xchip";
    const char *xinaboxPassword = "Vy3t2VwhhuKMLSa26PnJ4eQa";
    const uint16_t xinaboxServerPort1 = 80;
    const uint16_t xinaboxServerPort2 = 1883;
    char topic[40];
};

/**
 * @brief MQTT class
 * 
 */
class mqtt
{
public:
    ~mqtt();
    /**
     * @brief Construct a new mqtt object
     * 
     */
    mqtt();
    /**
     * @brief 
     * 
     */
    void begin();
    /**
     * @brief Connects to the XinaBox MQTT server.
     *        Tests both ports if either fails.
     * 
     * @return true if connection successful.
     * @return false if connection fails.
     */
    bool connect();
    /**
     * @brief Publishes the data to the XinaBox server.
     * 
     * @param finalJson json to be sent to the MQTT server.
     * @param finalJsonLen size of json document.
     */
    bool publish(char *finalJson, uint16_t finalJsonLen);

private:
    WiFiClient wifiClient = WiFiClient();
    PubSubClient xinaboxMqtt = PubSubClient(wifiClient);
    struct mqtt_vars mqtt_data;
    /**
     * @brief Concatenates an MQTT topic using mqtt_vars.xinaboxIndex and
     *        mac address of the CW02.
     * 
     * @return true if topic successfully created.
     * @return false if topic creation failed.
     */
    bool generate_topic();
};

#endif