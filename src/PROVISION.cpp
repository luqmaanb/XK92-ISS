#include "PROVISION.h"

provision::provision()
{
}
// provision::~provision()
// {
// }
void provision::setVariables()
{
    prv.begin();
    prv.addWiFi();
    prv.addVariable(deviceName, "YourDeviceName");
    prv.transmit();
}

bool provision::getVariables()
{
    if (!prv.receive())
    {
        prv.getVariable(ss, _ss);
        prv.getVariable(pp, _pp);
        prv.getVariable(deviceName, _deviceName);
    }
    if (prv.success())
    {
        prv.getWiFi(network_data.userSSID, network_data.userPASS);
        prv.getVariable(deviceName, _deviceName);
        if (network_data.userSSID == '\0' || network_data.userPASS == '\0')
        {
            network_data.userSSID = _ss;
            network_data.userPASS = _pp;
        }
        network_data.unitName = _deviceName;
        // network_data.userSSID = _ss;
        // network_data.userPASS = _pp;
#ifdef DEBUG
        debug("\n" + network_data.unitName);
        debug("\n" + network_data.userSSID);
        debug("\n" + network_data.userPASS);
#endif
        return true;
    }
    else
    {
        prv.fail();
        return false;
    }
}