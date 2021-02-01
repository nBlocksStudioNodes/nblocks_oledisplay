#ifndef __NB_OLEDISPLAY
#define __NB_OLEDISPLAY

#include "mbed.h"
#include "nworkbench.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

class nBlock_OLEDisplay: public nBlockSimpleNode<1> {
public:
    nBlock_OLEDisplay(PinName pinSDA, PinName pinSCL, PinName pinRST);
    void triggerInput(nBlocks_Message message);
    void endFrame(void);
    char TextContent[257]; // Up to 64x4 + NULL	
    uint32_t inputNumber;

    
private:
	I2C _i2c;
    Adafruit_SSD1306_I2c * _gfx;
	int data_received;
    	// Buffer to store the string
	
};

#endif
