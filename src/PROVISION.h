/**
 * @file PROVISION.h
 * @author Luqmaan Baboo (lb@xinabox.cc)
 * @brief XK92 Firmware - Quest for Space ISS Mission Kit.
 *        This file contains the provision functionality of
 *        the XK92 Firmware.
 * @version 0.1
 * @date 2020-02-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef PROVISION_H
#define PROVISION_H

#include <xProvision.h>
#include "global_defs.h"

/**
 * @brief XK92 Provision class
 * 
 */
class provision
{
public:
    provision();
    //~provision();
    /**
     * @brief Store the device name label and wifi credentials label in SPIFF memory.
     * 
     */
    void setVariables();
    /**
     * @brief Gets the variables that were set from SPIFF.
     * 
     * @return true if variables were retreived successfully.
     * @return false if variables were unsuccessfully retrieved or if provision failed.
     */
    bool getVariables();

private:
    xProvision prv;
    const String ss = "WiFi_Network";
    const String pp = "WiFi_Password";
    const String deviceName = "Device_Name";
    String _ss;
    String _pp;
    String _deviceName;
};
#endif