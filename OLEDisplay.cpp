#include "OLEDisplay.h"


nBlock_OLEDisplay::nBlock_OLEDisplay(PinName pinSDA, PinName pinSCL, int address, 
		float alarmMin = 75.0, float alarmMax = 80.0, 
		int comparatorPolarity = 0, int scale = 1):
			_i2c(pinSDA, pinSCL) {
		
	_i2c.frequency(400000);
		
//*******************************************
	class I2CPreInit : public I2C
	{
	public:
		I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
		{
			frequency(400000);
			start();
		};
	};
	
	I2CPreInit gI2C(P0_7,P0_30);
	Adafruit_SSD1306_I2c gOled(gI2C,P0_31);

	gOled.printf("%ux%u OLED Display\r\n", gOled.width(), gOled.height());
//*************************************************
	
	}

void nBlock_OLEDisplay::triggerInput(nBlocks_Message message) {
	// inputNumber is ignored as we only have one input
	// value is ignored as any value is considered a trigger for reading
	// If we received ANY input, means we should read the sensor
	_flagReadRequested = 1;

}

void nBlock_OLEDisplay::endFrame(void) {
	// If a read has been requested
	if (_flagReadRequested) {
		// Clear the flag
		_flagReadRequested = 0;
		
        //***************************
		gOled.printf("%u\r",x);
        gOled.display();
		//*****************************

        output[0] = temperature*_scale;
		available[0] = 1;
				
	}
    return;
}

