#include "OLEDisplay.h"

nBlock_OLEDisplay::nBlock_OLEDisplay(PinName pinSDA, PinName pinSCL, PinName pinRST): _i2c(pinSDA, pinSCL) {		
	_i2c.frequency(400000);
	_i2c.start();
	_gfx = new Adafruit_SSD1306_I2c(_i2c, pinRST) ;
	_gfx->clearDisplay();			

	// Reset internal buffer and  flag
	for (uint32_t i=0; i<257; i++) TextContent[i] = 0;
	data_received = 0;
}

void nBlock_OLEDisplay::triggerInput(nBlocks_Message message) {
	//uint32_t inputNumber = message.inputNumber;
	//char * value = message.stringValue;
	//strcpy(TextContent, value ); // Copy string to internal buffer

    strcpy(TextContent, message.stringValue);
	data_received = 1;
}

void nBlock_OLEDisplay::endFrame(void) {
	if (data_received ) {
		data_received  = 0;
		_gfx->printf(TextContent);
        _gfx->display();
        _gfx->display();
	}
}

