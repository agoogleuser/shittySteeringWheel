#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MPU_biasing.hpp"
#include <Wire.h>
#include <BleKeyboard.h>

#define FILTER_DEFAULT MPU6050_BAND_21_HZ
Adafruit_MPU6050 mpu;
sensors_event_t a,g,temp;
Position P;
float time2;
float ax_avg=0, ay_avg=0, gz_avg=0;

BleKeyboard bleKeyboard;

//===============uController configs===============//
#define PIN_W				5
#define PIN_S				15
#define PIN_CALIBRATE		4
#define TOGGLE_STREERING	18
#ifndef LED_BUILTIN
	#define LED_BUILTIN		2
#endif

void setup(void) {

	pinMode (PIN_W, INPUT_PULLUP);
	pinMode (PIN_S, INPUT_PULLUP);
	pinMode (PIN_CALIBRATE, INPUT_PULLUP);
	pinMode (TOGGLE_STREERING, INPUT_PULLUP);
	pinMode (LED_BUILTIN, OUTPUT);
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // will pause Zero, Leonardo, etc until serial console opens
	}

	// Try to initialize!
	if (!mpu.begin()) {
		// Serial.println("Failed to find MPU6050 chip");
		while (1);
	}

	mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);

	// getting the most accurate DC Value for the filter
	setOffset();
	//Serial.println("");
	bleKeyboard.begin();
}

bool b1,b2,bCal,b4;
uint8_t buf[8] = {0};
int8_t weight = 0; // Variable gets its value from -60 to 60 degress as 10 steps


void loop()
{
	//Steering
	mpu.getEvent(&a, &g, &temp);
	time2=millis();
	float ax = a.acceleration.x - ax_avg;
	float ay = a.acceleration.y - ay_avg;
	float gz = g.gyro.z - gz_avg;
	if (fabs(ax) <= 0.05) (ax=0);
	if (fabs(ay) <= 0.05) (ay=0);
	if (fabs(gz) <= 0.05) (ay=0);
	P.update(time2,ax,ay,gz);
	//Buttons Logic
	b1 = digitalRead(PIN_W);
	b2 = digitalRead(PIN_S);
	bCal = digitalRead(PIN_CALIBRATE);
	b4 = digitalRead(TOGGLE_STREERING);
	if (bCal)
	{
		setOffset();
		P.clearAll();
	}

	if (b4)
	{
		Serial.println(P.angZ);
		if (P.angZ<-20)
		{
			bleKeyboard.release('a');
			bleKeyboard.press('d');
		}
		else if (P.angZ>20)
		{
			bleKeyboard.release('d');
			bleKeyboard.press('a');
		}
		else
		{
			bleKeyboard.release('d');
			bleKeyboard.release('a');
		}
	}
	else
	{
		bleKeyboard.release('d');
		bleKeyboard.release('a');
		P.clearAll();
	}
	if (!b1) 	bleKeyboard.press('w');
	else		bleKeyboard.release('w');

	if (!b2) 	bleKeyboard.press('s');
	else		bleKeyboard.release('s');


}

void setOffset()
{
	mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
	for (int i=0; i<500; i++){
		mpu.getEvent(&a, &g, &temp);
		ax_avg= (ax_avg + a.acceleration.x)/2;
		ay_avg= (ay_avg + a.acceleration.y)/2;
		gz_avg= (gz_avg + g.gyro.z)/2;
	}
	mpu.setFilterBandwidth(FILTER_DEFAULT);
}

