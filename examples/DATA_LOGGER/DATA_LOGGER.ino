#include <XK92.h>
#include <DATA.h>
XinaBox_xk92 xk92;

unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;

const long interval = 10000;
const long interval1 = 300;
void setup()
{
	Serial.begin(115200);
	Wire.begin();
	xk92.begin();
}

void loop()
{
	SW02.run();
	SN01.poll();
		
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= interval)
	{
		previousMillis = currentMillis;
		xk92.getData();
		if (xk92_mode[DATA_VISUALIZER] == XK92_SUCCESS)
		{
			xk92.publishData();
		}
		if (xk92_mode[DATA_LOGGER] == XK92_SUCCESS)
		{
			xk92.logData();
		}
	}
	timerWrite(timer, 0);
}