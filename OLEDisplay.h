#ifndef __NB_OLEDISPLAY
#define __NB_OLEDISPLAY

#include "mbed.h"
#include "nworkbench.h"

class nBlock_OLEDisplay: public nBlockSimpleNode<1> {
public:

    nBlock_OLEDisplay(PinName pinSDA, PinName pinSCL);
    void triggerInput(nBlocks_Message message);
    void endFrame(void);

private:
	I2C _i2c;
	int _flagReadRequested;
	
};

#endif
