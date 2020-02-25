#include "WIFI.h"

wifi::wifi()
{
}

wifi::~wifi() 
{
} 
void wifi::begin()
{
    network_data.macAddress = WiFi.macAddress();
    network_data.clientId = String((long)ESP.getEfuseMac());
    wifiMulti.addAP(network_data.userSSID.c_str(), network_data.userPASS.c_str());
    wifiMulti.addAP(network_data.xinaboxSSID, network_data.xinaboxPASS);
}

bool wifi::connect()
{
    uint32_t period = 1 * 10000L;
    debug("\nConnecting to WiFi");
    for (uint32_t tStart = millis(); (millis() - tStart) < period;)
    {
        timerWrite(timer, 0);
        if (wifiMulti.run() == WL_CONNECTED)
        {
            network_data.wifiStatus = true;
#ifdef DEBUG
            debug("\nWiFi connected\n");
            debug("IP address: ");
            debug(WiFi.localIP());
            debug("\n");
#endif
            return true;
        }
        else
        {
            debug(".");
            delay(250);
            timerWrite(timer, 0);
        }
    }
    network_data.wifiStatus = false;
    return false;
}