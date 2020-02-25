/**
 * @file WIFI.h
 * @author Luqmaan Baboo (lb@xinabox.cc)
 * @brief XK92 Firmware - Quest for Space ISS Mission Kit.
 *        This file contains the wifi functionality of the XK92
 *        Firmware.
 * @version 0.1
 * @date 2020-02-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <WiFiMulti.h>
#include "global_defs.h"

/**
 * @brief 
 * 
 */
class wifi
{
private:
    WiFiMulti wifiMulti;

public:
    /**
     * @brief Construct a new wifi object
     * 
     */
    wifi();
    ~wifi();
    /**
     * @brief Gets the mac address and adds user defined and XinaBox
     *        wifi credentials to WiFiMulti list.
     * 
     */
    void begin();
    /**
     * @brief Connects to the user defined wifi credentials or
     *        defaults to XinaBox wifi credentials using WiFiMulti.
     * 
     * @return true if wifi connection successful.
     * @return false if wifi connection failed.
     */
    bool connect();
};
#endif