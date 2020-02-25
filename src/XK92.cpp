#include "XK92.h"

hw_timer_t *timer = NULL;

struct sensor_acks sensor_ready, data_ready;
struct network_vars network_data;
struct sg33_vars sg33_data;
struct sg34_vars sg34_data;
struct si01_vars si01_data;
struct sn01_vars sn01_data;
struct sw02_vars sw02_data;
struct sw03_vars sw03_data;
struct sd_vars sd_data;
uint8_t xk92_mode[2];

void rgb_setup();
void rgb(uint8_t colour, bool state);

const long wdtTimeout = 10000000; //us
void IRAM_ATTR resetModule()
{
  ets_printf("reboot\n");
  esp_restart();
}
void watchdog_setup();

XinaBox_xk92::XinaBox_xk92()
{
  sensor_ready.sg33 = data_ready.sg33 = false;
  sensor_ready.sg34 = data_ready.sg34 = false;
  sensor_ready.si01 = data_ready.si01 = false;
  sensor_ready.sn01 = data_ready.sn01 = false;
  sensor_ready.sw02 = data_ready.sw02 = false;
  sensor_ready.sw03 = data_ready.sw03 = false;
  network_data.wifiStatus = false;
  uint8_t xk92_mode[2] = {0};
}

void XinaBox_xk92::begin()
{
  xProv.setVariables();
  xProv.getVariables();
  watchdog_setup();
  xWifi.begin();
  timerWrite(timer, 0);

  if (xWifi.connect())
  {
    dateTime_setup();
    xMqtt.begin();
    timerWrite(timer, 0);
    if (xMqtt.connect())
    {
      timerWrite(timer, 0);
      xk92_mode[DATA_VISUALIZER] = XK92_SUCCESS;
      debug("\nData Visualizer mode Successful.");
    }
    else
    {
      timerWrite(timer, 0);
      xk92_mode[DATA_VISUALIZER] = XK92_FAIL;
      debug("\nMQTT connection Failed. Data Visualizer mode Disabled.");
    }
  }
  else
  {
    sd_data.headerrecord += "CPU_RUNTIME,";
#ifdef DEBUG
    debug("\nWiFi connection Failed. Data Visualizer mode Disabled.");
#endif
  }

  if (pingSlave())
  {
    timerWrite(timer, 0);
    if (testSlave())
    {
      timerWrite(timer, 0);
      debug("\nData Logger mode Successful.");
      xk92_mode[DATA_LOGGER] = XK92_SUCCESS;
    }
    else
    {
      debug("\nFailed to test slave. Data logger mode disabled.");
      xk92_mode[DATA_LOGGER] = XK92_FAIL;
    }
  }
  else
  {
    debug("\nFailed to ping slave. Data logger mode disabled.");
    xk92_mode[DATA_LOGGER] = XK92_FAIL;
  }

  xData.begin();
  // if (xk92_mode[DATA_LOGGER] == XK92_SUCCESS)
  // {
  //   send_header();
  // }
  timerWrite(timer, 0);
}

void XinaBox_xk92::logData()
{
  if (send_header())
  {
    send_data();
  }
  timerWrite(timer, 0);
}

void XinaBox_xk92::getData()
{
  sd_data.datarecord = "";
  if (xk92_mode[DATA_VISUALIZER] == XK92_SUCCESS)
  {
    getDateTime();
  }
  else
  {
    sd_data.datarecord += String(millis() / 1000) + ",";
  }
  xData.getData();
  timerWrite(timer, 0);
}

void XinaBox_xk92::publishData()
{

  if (xk92_mode[DATA_VISUALIZER] == XK92_SUCCESS)
  {
    rgb(LED_GREEN, LED_ON);
    buildJson();
    if (xMqtt.connect())
    {
      if (xMqtt.publish(finalJson, finalJsonLen))
      {
        rgb(LED_GREEN, LED_OFF);
      }
    }
  }
  timerWrite(timer, 0);
}

void XinaBox_xk92::buildJson()
{
  DynamicJsonBuffer xk92Buffer;
  JsonObject &root = xk92Buffer.createObject();
  JsonObject &XK92 = root.createNestedObject("XK92");

  if (data_ready.sg33)
  {
    JsonObject &SG33 = XK92.createNestedObject("SG33");
    SG33["TVOC"] = sg33_data.tvoc;
    SG33["CO2"] = sg33_data.co2;
  }

  if (data_ready.sg34)
  {
    JsonObject &SG34 = XK92.createNestedObject("SG34");
    SG34["Infrared"] = sg34_data.ir;
    SG34["Red"] = sg34_data.red;
    SG34["Green"] = sg34_data.green;
    SG34["Temperature(C)"] = sg34_data.temperature;
  }

  if (data_ready.si01)
  {
    JsonObject &SI01 = XK92.createNestedObject("SI01");
    SI01["Gyro_mean"] = si01_data.g_mean;
    SI01["Gyro_min"] = si01_data.g_min;
    SI01["Gyro_max"] = si01_data.g_max;
    SI01["Gyro_stddev"] = si01_data.g_stddev;
    SI01["Acce_mean"] = si01_data.a_mean;
    SI01["Acce_min"] = si01_data.a_min;
    SI01["Acce_max"] = si01_data.a_max;
    SI01["Acce_stddev"] = si01_data.a_stddev;
    SI01["Magn_mean"] = si01_data.m_mean;
    SI01["Magn_min"] = si01_data.m_min;
    SI01["Magn_max"] = si01_data.m_max;
    SI01["Magn_stddev"] = si01_data.m_stddev;
  }

  if (data_ready.sw02)
  {
    JsonObject &SW02 = XK92.createNestedObject("SW02");
    SW02["Temperature(C)"] = sw02_data.temperature;
    SW02["Humidity(%)"] = sw02_data.humidity;
    SW02["Pressure(hPa)"] = sw02_data.rawPress;
    SW02["GasResistance(ohm)"] = sw02_data.gasRes;
    SW02["IAQ"] = sw02_data.iaq;
    SW02["IAQAccuracy"] = sw02_data.iaqAccuracy;
    SW02["Co2Equivalent"] = sw02_data.co2Equivalent;
    SW02["BreathVoCEquivalent"] = sw02_data.breathVOCEquivalent;
  }
  if (data_ready.sn01)
  {
    JsonObject &SN01 = XK92.createNestedObject("SN01");
    SN01["Hdop"] = sn01_data.hdop;
    SN01["Satellites"] = sn01_data.noOfSat;
    SN01["Date"] = sn01_data.date;
    SN01["Time"] = sn01_data.time;
    if ((sn01_data.hdop < 10) && (sn01_data.hdop > 0))
    {
      SN01["Latitude"] = sn01_data.latitude;
      SN01["Longitude"] = sn01_data.longitude;
      SN01["Altitude"] = sn01_data.altitude;
      SN01["CourseOverGround"] = sn01_data.cog;
      SN01["SpeedOverGround"] = sn01_data.sog;
    }
  }

  if (data_ready.sw03)
  {
    JsonObject &SW03 = XK92.createNestedObject("SW03");
    SW03["Temperature(C)"] = sw03_data.temperature;
    SW03["Pressure(hPa"] = sw03_data.pressure;
    SW03["Altitude"] = sw03_data.altitude;
  }

  JsonObject &Input = XK92.createNestedObject("Input");
  Input["Unit Name"] = network_data.unitName;
  Input["Organisation"] = "XinaBox";

  JsonObject &CW02 = XK92.createNestedObject("CW02");
  CW02["RSSI(dB)"] = WiFi.RSSI();
  CW02["Firmware"] = FIRMWARE_VERSION;
  CW02["MAC"] = network_data.macAddress;
  CW02["Chip ID"] = network_data.clientId;

  root.printTo(finalJson, sizeof(finalJson));
  uint16_t finalJsonLen = strlen(finalJson);
  debug("\n");
  root.prettyPrintTo(Serial);
  timerWrite(timer, 0);
}

void XinaBox_xk92::dateTime_setup()
{
  sd_data.headerrecord +=
      "DATE,"
      "TIME,";
  timerWrite(timer, 0);
}

void XinaBox_xk92::getDateTime()
{
  dateTime = NTPch.getNTPtime(0.0, 1);
  while (!dateTime.valid)
  {
    dateTime = NTPch.getNTPtime(0.0, 1);
  }
  sd_data.datarecord += String(dateTime.year) + "-" + String(dateTime.month) +
                        "-" + String(dateTime.day) + "," +
                        String(dateTime.hour) + ":" + String(dateTime.minute) +
                        ":" + String(dateTime.second) + ",";
  timerWrite(timer, 0);
}

bool XinaBox_xk92::testSlave()
{
  Wire.beginTransmission(CS11_I2C_ADDRESS);
  for (uint8_t i = 0; i < 128; i++)
  {
    timerWrite(timer, 0);
    Wire.write(i);
  }
  if (Wire.endTransmission() != 0)
  {
#ifdef DEBUG
    debug("\nFailed to send test bytes to slave. Data Logger mode disabled.");
#endif
    timerWrite(timer, 0);
    return false;
  }
  Wire.requestFrom(CS11_I2C_ADDRESS, 1);
  if (Wire.available() == 1)
  {
    if (Wire.read() != 99)
    {
#ifdef DEBUG
      debug(
          "\nFailed to read test byte from slave. Data Logger mode disabled.");
#endif
      timerWrite(timer, 0);
      return false;
    }
  }
  timerWrite(timer, 0);
  return true;
}

bool XinaBox_xk92::pingSlave()
{
  debug("\nATTEMPTING TO PING SLAVE");
  uint32_t period = 1 * 2000L;
  for (uint32_t tStart = millis(); (millis() - tStart) < period;)
  {
    if (xCore.ping(CS11_I2C_ADDRESS))
    {
      timerWrite(timer, 0);
      return true;
    }
    else
    {
      debug(".");
      delay(250);
    }
  }
  timerWrite(timer, 0);
  return false;
}

bool XinaBox_xk92::send_data()
{
  uint16_t error_status;
#ifdef DEBUG
  debug("\nData record : ");
  debug(sd_data.datarecord);
#endif
  uint16_t data_size = strlen(sd_data.datarecord.c_str());
  debug("\nData size: ");
  debug(data_size);
  Wire.beginTransmission(CS11_I2C_ADDRESS);
  Wire.write('b');
  uint8_t k = (data_size >> 8) & 0xFF;
  uint8_t j = (uint8_t)data_size & 0xFF;
  Wire.write(k);
  Wire.write(j);
  error_status = Wire.endTransmission(false);
  if (error_status != 0)
  {
    timerWrite(timer, 0);
    return false;
  }

  for (uint16_t i = 0; i < data_size; i++)
  {
    Wire.beginTransmission(CS11_I2C_ADDRESS);
    Wire.write(sd_data.datarecord[i]);
    if (i == data_size)
      error_status = Wire.endTransmission(false);
    else
      error_status = Wire.endTransmission(true);
    if (error_status != 0)
    {
      timerWrite(timer, 0);
      return false;
    }
  }
  timerWrite(timer, 0);
  return true;
}

bool XinaBox_xk92::send_header()
{
  uint16_t error_status;
#ifdef DEBUG
  debug("\n" + sd_data.headerrecord);
#endif
  uint16_t header_size = strlen(sd_data.headerrecord.c_str());
  Wire.beginTransmission(CS11_I2C_ADDRESS);
  Wire.write('a');
  uint8_t k = (header_size >> 8) & 0xFF;
  uint8_t j = (uint8_t)header_size & 0xFF;
  Wire.write(k);
  Wire.write(j);
  error_status = Wire.endTransmission(false);
  if (error_status != 0)
  {
    timerWrite(timer, 0);
    return false;
  }
  for (uint16_t i = 0; i < header_size; i++)
  {
    Wire.beginTransmission(CS11_I2C_ADDRESS);
    // debug(headerrecord[i]);
    Wire.write(sd_data.headerrecord[i]);
    if (i == header_size)
      error_status = Wire.endTransmission(false);
    else
      error_status = Wire.endTransmission(true);
    if (error_status != 0)
    {
      timerWrite(timer, 0);
      return false;
    }
  }
  timerWrite(timer, 0);
  return true;
}

void rgb_setup()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  rgb(LED_RGB, LED_OFF);
}

void rgb(uint8_t colour, bool state)
{
  if (colour == LED_RED || colour == LED_GREEN || colour == LED_BLUE)
  {
    digitalWrite(colour, state);
  }
  else
  {
    digitalWrite(LED_RED, state);
    digitalWrite(LED_GREEN, state);
    digitalWrite(LED_BLUE, state);
  }
}

void watchdog_setup()
{
  timer = timerBegin(0, 80, true);                 //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true); //attach callback
  timerAlarmWrite(timer, wdtTimeout, false);       //set time in us
  timerAlarmEnable(timer);
}