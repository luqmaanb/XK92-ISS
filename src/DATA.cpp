#include "DATA.h"

xSN01 SN01;
xSW02 SW02;

data::data()
{
}
void data::begin()
{
    SG33_setup();
    SG34_setup();
    SI01_setup();
    SW02_setup();
    SW03_setup();
    SN01_setup();
}

void data::getData()
{
    if (sensor_ready.sg33)
        SG33_data();
    if (sensor_ready.sg34)
        SG34_data();
    if (sensor_ready.si01)
        SI01_data();
    if (sensor_ready.sw02)
        SW02_data();
    if (sensor_ready.sw03)
        SW03_data();
    if (sensor_ready.sn01)
        SN01_data();
}

void data::SG33_setup()
{
    if (SG33.begin())
    {
        sensor_ready.sg33 = true;
        sd_data.headerrecord += "SG33-TVOC,"
                                "SG33-CO2,";
    }
    else
    {
        debug("\nNo SG33 present.");
        sensor_ready.sg33 = false;
    }
    timerWrite(timer, 0);
}

void data::SG34_setup()
{
    if (SG34.begin())
    {
        /*
      byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
      byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
      byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
      byte sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
      int pulseWidth = 411; //Options: 69, 118, 215, 411
      int adcRange = 2048; //Options: 2048, 4096, 8192, 16384
      SG34.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
    */
        //SG34.setup();
        //particleSensor.setPulseAmplitudeRed(0); //Turn off Red LED
        //particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
        sensor_ready.sg34 = true;
        sd_data.headerrecord += "SG34-IR,"
                                "SG34-Green,"
                                "SG34-Red,"
                                "SG34-Temperature,";
    }
    else
    {
        debug("\nNo SG34 present.");
        sensor_ready.sg34 = false;
    }
    timerWrite(timer, 0);
}

void data::SI01_setup()
{
    if (SI01.begin())
    {
        sensor_ready.si01 = true;
        sd_data.headerrecord += "SI01-G-MEAN,"
                                "SI01-G-MIN,"
                                "SI01-G-MAX,"
                                "SI01-G-STDDEV,"
                                "SI01-A-MEAN,"
                                "SI01-A-MIN,"
                                "SI01-A-MAX,"
                                "SI01-A-STDDEV,"
                                "SI01-M-MEAN,"
                                "SI01-M-MIN,"
                                "SI01-M-MAX,"
                                "SI01-M-STDDEV,";
    }
    else
    {
        debug("\nNo SI01 present.");
        sensor_ready.si01 = false;
    }
    timerWrite(timer, 0);
}

void data::SN01_setup()
{
    if (SN01.begin())
    {
        sensor_ready.sn01 = true;
        sd_data.headerrecord += "SN01-HDOP,"
                                "SN01-SatellitesConnected,"
                                "SN01-Date,"
                                "SN01-Time,"
                                "SN01-Latitude,"
                                "SN01-Longitude,"
                                "SN01-Altitude,"
                                "SN01-CoG,"
                                "SN01-SoG,";
        if (network_data.wifiStatus)
        {
            if (!getAlmanac())
            {
#ifdef DEBUG
                debug("\nFailed to download almanac...");
#endif
                if (!sendAlmanac(OVER_I2C))
                {
#ifdef DEBUG
                    debug("\nFailed to send almanac to SN01...");
#endif
                }
            }
        }
    }
    else
    {
        debug("\nNo SN01 present.");
        sensor_ready.sn01 = false;
    }
    timerWrite(timer, 0);
}

void data::SW02_setup()
{
    if (xCore.ping(0x76))
    {
        SW02.begin();
        sd_data.headerrecord += "SW02-RawTemp,"
                                "SW02-RawPress,"
                                "SW02-RawHumi,"
                                "SW02-GasResistance,"
                                "SW02-IAQ,"
                                "SW02-IAQAccuracy,"
                                "SW02-Temperature,"
                                "SW02-Humidity,"
                                "SW02-CO2Equivalent,"
                                "SW02-BreathVOCEquivalent,";
        sensor_ready.sw02 = true;
    }
    else
    {
        debug("\nNo SW02 present.");
        sensor_ready.sw02 = false;
    }
    timerWrite(timer, 0);
}

void data::SW03_setup()
{
    if (SW03.begin())
    {
        sensor_ready.sw03 = true;
        sd_data.headerrecord += "SW03-Temperature,"
                                "SW03-Pressure,"
                                "SW03-Altitude,";
    }
    else
    {
        sensor_ready.sw03 = false;
        debug("\nNo SW03 present.");
    }
    timerWrite(timer, 0);
}

void data::SN01_data()
{
    data_ready.sn01 = XK92_FAIL;
    SN01.poll();

    sn01_data.date = SN01.getDate();
    sn01_data.time = SN01.getTime();
    sn01_data.hdop = SN01.getHDOP();
    sn01_data.noOfSat = SN01.getSatelitesConnected();

    if ((sn01_data.hdop < 10) && (sn01_data.hdop > 0))
    {
        sn01_data.latitude = SN01.getLatitude();
        sn01_data.longitude = SN01.getLongitude();
        sn01_data.altitude = SN01.getAltitude();
        sn01_data.cog = SN01.getCOG();
        sn01_data.sog = SN01.getSOG();

        sd_data.datarecord += String(sn01_data.hdop) + "," +
                              String(sn01_data.noOfSat) + "," +
                              sn01_data.date + "," +
                              sn01_data.time + "," +
                              String(sn01_data.latitude, 6) + "," +
                              String(sn01_data.longitude, 6) + "," +
                              String(sn01_data.altitude) + "," +
                              String(sn01_data.cog) + "," +
                              String(sn01_data.sog) + ",";
    }
    else
    {
        sd_data.datarecord += String(sn01_data.hdop) + "," +
                              String(sn01_data.noOfSat) + "," +
                              sn01_data.date + "," +
                              sn01_data.time + "," +
                              ",,,,,";
    }
    data_ready.sn01 = XK92_SUCCESS;
    timerWrite(timer, 0);
}

void data::SW02_data()
{
    data_ready.sw02 = XK92_FAIL;
    while (!SW02.run())
    {
        data_ready.sw02 = XK92_FAIL;
        timerWrite(timer, 0);
    }
    data_ready.sw02 = XK92_SUCCESS;
    sd_data.datarecord += String(sw02_data.rawTemp = SW02.getRawTemperature()) + "," +
                          String(sw02_data.rawPress = SW02.getPressure() / 100) + "," +
                          String(sw02_data.rawHumi = SW02.getRawHumidity()) + "," +
                          String(sw02_data.gasRes = SW02.getGasResistance()) + "," +
                          String(sw02_data.iaq = SW02.getIAQ()) + "," +
                          String(sw02_data.iaqAccuracy = SW02.getIAQAccuracy()) + "," +
                          String(sw02_data.temperature = SW02.getTemperature()) + "," +
                          String(sw02_data.humidity = SW02.getHumidity()) + "," +
                          String(sw02_data.co2Equivalent = SW02.getCO2Equivalent()) + "," +
                          String(sw02_data.breathVOCEquivalent = SW02.getBreathVOCEquivalent()) + ",";
    timerWrite(timer, 0);
}

void data::SW03_data()
{
    data_ready.sw03 = XK92_FAIL;
    SW03.poll();
    // changing order of get functions yields incorrect data
    sw03_data.pressure = SW03.getPressure() / 100;
    sw03_data.altitude = SW03.getAltitude();
    sw03_data.temperature = SW03.getTemperature();

    data_ready.sw03 = XK92_SUCCESS;
    sd_data.datarecord += String(sw03_data.temperature) + "," +
                          String(sw03_data.pressure) + "," +
                          String(sw03_data.altitude) + ",";
    timerWrite(timer, 0);
}

void data::SG33_data()
{
    data_ready.sg33 = XK92_FAIL;
    if (SG33.dataAvailable())
    {
        SG33.getAlgorithmResults();
        if (sw02_data.temperature != 0 || sw02_data.humidity != 0)
        {
            SG33.setEnvironmentData(sw02_data.humidity, sw02_data.temperature);
        }
        sg33_data.tvoc = SG33.getTVOC();
        sg33_data.co2 = SG33.getCO2();
        data_ready.sg33 = XK92_SUCCESS;
        sd_data.datarecord += String(sg33_data.tvoc) + "," +
                              String(sg33_data.co2) + ",";
    }
    else
    {
        data_ready.sg33 = XK92_FAIL;
        sd_data.datarecord += ",,";
    }
    timerWrite(timer, 0);
}

void data::SG34_data()
{
    data_ready.sg34 = XK92_FAIL;
    enum ledstates
    {
        R,
        IRR,
        IRGR
    };
    timerWrite(timer, 0);
    switch (count)
    {
    case R:
        SG34.setup(0xFF, 4, 1, 400, 411, 2048);
        delay(10);
        sg34_data.red = SG34.getRed();
        sd_data.datarecord += ",," + String(sg34_data.red) + ",";
        break;
    case IRR:
        SG34.setup(0xFF, 4, 2, 400, 411, 2048);
        delay(10);
        sg34_data.ir = SG34.getIR();
        sg34_data.red = SG34.getRed();
        sd_data.datarecord += String(sg34_data.ir) + ",," + String(sg34_data.red) + ",";
        break;
    case IRGR:
        SG34.setup(0xFF, 4, 3, 400, 411, 2048);
        delay(10);
        sg34_data.ir = SG34.getIR();
        sg34_data.green = SG34.getGreen();
        sg34_data.red = SG34.getRed();
        sd_data.datarecord += String(sg34_data.ir) + "," + String(sg34_data.green) + "," + String(sg34_data.red) + ",";
        break;
    default:
        sd_data.datarecord += ",,,";
        break;
    }
    data_ready.sg34 = XK92_SUCCESS;
    sg34_data.temperature = SG34.readTemperature();
    sd_data.datarecord += String(sg34_data.temperature) + ",";
    count++;
    if (count == 3)
        count = 0;
    timerWrite(timer, 0);
}

void data::SI01_data()
{
    data_ready.si01 = XK92_FAIL;
    float ax_, ay_, az_, aVector;
    float mx_, my_, mz_, mVector;
    float gx_, gy_, gz_, gVector;

    for (int i = 0; i < STAT_COUNT; i++)
    {
        SI01.poll();
        ax_ = SI01.getAX();
        ay_ = SI01.getAY();
        az_ = SI01.getAZ();
        gx_ = SI01.getGX();
        gy_ = SI01.getGY();
        gz_ = SI01.getGZ();
        mx_ = SI01.getMX();
        my_ = SI01.getMY();
        mz_ = SI01.getMZ();
        aVector = sqrt((ax_) * (ax_) + (ay_) * (ay_) + (az_) * (az_));
        gVector = sqrt((gx_) * (gx_) + (gy_) * (gy_) + (gz_) * (gz_));
        mVector = sqrt((mx_) * (mx_) + (my_) * (my_) + (mz_) * (mz_));

        statsa.add(aVector);
        statsg.add(gVector);
        statsm.add(mVector);
    }

    int counta = statsa.count();
    if (counta == 10)
    {
        si01_data.a_mean = statsa.average();
        si01_data.a_min = statsa.minimum();
        si01_data.a_max = statsa.maximum();
        si01_data.a_stddev = statsa.pop_stdev();
        statsa.clear();
        counta = 0;
    }

    int countg = statsg.count();
    if (countg == STAT_COUNT)
    {
        si01_data.g_mean = statsg.average();
        si01_data.g_min = statsg.minimum();
        si01_data.g_max = statsg.maximum();
        si01_data.g_stddev = statsg.pop_stdev();
        statsg.clear();
        countg = 0;
    }

    int countm = statsm.count();
    if (countm == STAT_COUNT)
    {
        si01_data.m_mean = statsm.average();
        si01_data.m_min = statsm.minimum();
        si01_data.m_max = statsm.maximum();
        si01_data.m_stddev = statsm.pop_stdev();
        statsm.clear();
        countm = 0;
    }

    if (counta == 0 && countg == 0 && countm == 0)
    {
        stats_complete = true;
    }
    if (stats_complete)
    {
        sd_data.datarecord += String(si01_data.g_mean, 4) + "," +
                              String(si01_data.g_min, 4) + "," +
                              String(si01_data.g_max, 4) + "," +
                              String(si01_data.g_stddev, 4) + "," +
                              String(si01_data.a_mean, 4) + "," +
                              String(si01_data.a_min, 4) + "," +
                              String(si01_data.a_max, 4) + "," +
                              String(si01_data.a_stddev, 4) + "," +
                              String(si01_data.m_mean, 4) + "," +
                              String(si01_data.m_min, 4) + "," +
                              String(si01_data.m_max, 4) + "," +
                              String(si01_data.m_stddev, 4) + ",";
        data_ready.si01 = XK92_SUCCESS;
        stats_complete = false;
    }
    else
    {
        data_ready.si01 = XK92_FAIL;
        sd_data.datarecord += ",,,,,,,,,,,,";
    }
    timerWrite(timer, 0);
}

bool data::getAlmanac()
{
    HTTPClient http;
#ifdef DEBUG
    debug("\n[HTTP] begin...");
#endif
    // configure traged server and url
    http.begin("http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=60MHKSUo10q7ydnB5I9GgQ;gnss=gps,glo;datatype=eph,alm,aux;"); //HTTP
#ifdef DEBUG
    debug("\n[HTTP] GET...");
#endif
    // start connection and send HTTP header
    int httpCode = http.GET();
    timerWrite(timer, 0);
    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
#ifdef DEBUG
        debug("\n[HTTP] GET... code: ");
        debug(httpCode);
#endif

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            almanac_string = http.getString();
#ifdef DEBUG
            //debug(almanac_string + "\n");
#endif
        }
    }
    else
    {
#ifdef DEBUG
        debug("\n[HTTP] GET... failed, error: ");
        debug(http.errorToString(httpCode).c_str());
#endif
        timerWrite(timer, 0);
        return false;
    }
    http.end();
    timerWrite(timer, 0);
    return true;
}

bool data::sendAlmanac(uint8_t protocol)
{
    uint16_t error_status;
    if (!SPIFFS.begin(true))
    {
#ifdef DEBUG
        debug("\nAn Error has occurred while mounting SPIFFS.");
#endif
        return false;
    }
    File file = SPIFFS.open("/almanac.ubx", FILE_WRITE);
    if (!file)
    {
#ifdef DEBUG
        debug("\nThere was an error opening the file for writing.");
#endif
        return false;
    }
    if (file.print(almanac_string))
    {
#ifdef DEBUG
        debug("\nFile was written.");
#endif
    }
    else
    {
#ifdef DEBUG
        debug("\nFile write failed.");
#endif
    }
    file.close();
    file = SPIFFS.open("/almanac.ubx", FILE_READ);
    switch (protocol)
    {
    case OVER_I2C:
        while (file.available())
        {
            timerWrite(timer, 0);
            Wire.beginTransmission(SN01_I2C_ADDRESS);
            Wire.write((uint8_t)file.read());
            if (file.available() == 0)
                error_status = Wire.endTransmission(false);
            else
                error_status = Wire.endTransmission(true);
            if (!error_status)
                return false;
        }
        file.close();
        break;
    case OVER_UART:
        Serial.begin(9600);
        while (file.available())
        {
            Serial.print((uint8_t)file.read());
        }
        Serial.end();
        break;
    default:
        break;
    }
    timerWrite(timer, 0);
    return true;
}