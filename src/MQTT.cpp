#include "MQTT.h"

mqtt::mqtt()
{
}

mqtt::~mqtt() 
{
} 
void mqtt::begin()
{
    generate_topic();
}
bool mqtt::generate_topic()
{
    if (sprintf(mqtt_data.topic, "x/data/%s/%s", mqtt_data.xinaboxIndex, network_data.clientId.c_str()) < 0)
    {
#ifdef DEBUG
        debug("\nFailed to generate topic.");
#endif
        return false;
    }
#ifdef DEBUG
    debug("\nTopic: " + String(mqtt_data.topic));
#endif
    return true;
}

bool mqtt::connect()
{
    uint8_t tick = 5;
    if (!xinaboxMqtt.connected())
    {
        xinaboxMqtt.setServer(mqtt_data.xinaboxServer, mqtt_data.xinaboxServerPort1);
        do
        {
            timerWrite(timer, 0);
            xinaboxMqtt.connect(network_data.clientId.c_str(), mqtt_data.xinaboxUsername, mqtt_data.xinaboxPassword);
        } while (!xinaboxMqtt.connected() && tick-- == 0);
    }
    tick = 5;
    if (!xinaboxMqtt.connected())
    {
        xinaboxMqtt.setServer(mqtt_data.xinaboxServer, mqtt_data.xinaboxServerPort2);
        do
        {
            timerWrite(timer, 0);
            xinaboxMqtt.connect(network_data.clientId.c_str(), mqtt_data.xinaboxUsername, mqtt_data.xinaboxPassword);
        } while (!xinaboxMqtt.connected() && tick-- == 0);
    }

    if (!xinaboxMqtt.connected())
    {
        debug("\nCONNECTION TO MQTT SERVER FAILED.");
        return false;
    }
    else
    {
        debug("\nCONNECTION TO MQTT SERVER SUCCESSFUL.");
        xinaboxMqtt.loop();
        return true;
    }
}

bool mqtt::publish(char *finalJson, uint16_t finalJsonLen)
{
    if (!xinaboxMqtt.publish(mqtt_data.topic, finalJson, finalJsonLen))
    {
        debug("\nPUBLISH FAILED, rc = " + String(xinaboxMqtt.state()));
        return false;
    }
    debug("\nPUBLISH SUCCESSFUL.");
    return true;
    xinaboxMqtt.loop();
}