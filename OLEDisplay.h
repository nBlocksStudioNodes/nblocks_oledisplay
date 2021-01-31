#ifndef __NB_OLEDISPLAY
#define __NB_OLEDISPLAY

#include "mbed.h"
#include "nworkbench.h"

// OLEDisplay constants

#define __OLEDisplay_ADDRESS_BASE = 0x90

// All registers but config are 16-bit wide temperature values
// where MSB is temperature in Celsius and only bit 7 of LSB
// is used (represents 0.5 Celsius)
#define __OLEDisplay_TEMPERATURE   0x00
#define __OLEDisplay_CONFIG        0x01
#define __OLEDisplay_ALARM_LOW     0x02
#define __OLEDisplay_ALARM_HIGH    0x03

#define __OLEDisplay_CONF_FAULTQUEUE_4  0b00010000
#define __OLEDisplay_CONF_POLARITY_Pos  2


/**
 *  \brief Driver to read data from OLEDisplay temperature
 *  sensors. It has a trigger input and 2 outputs: temperature and alarm
 *  The trigger is used to synchronise the output - as OLEDisplay has no internal
 *  event system for temperature
 */
class nBlock_OLEDisplay: public nBlockSimpleNode<1> {
public:
    /**
     *  \brief Constructor is invoked at node creation, and arguments
     *  are configuration parameters assigned in the IDE
     *  
     *  \param [in] pinSDA Pin connected to the sensor's SDA line
     *  \param [in] pinSCL Pin connected to the sensor's SCL line
     *  \param [in] address I2C address
     *  \param [in] alarmMin Lower threshold for temperature alarm
     *  \param [in] alarmMax Upper threshold for temperature alarm
     *  \param [in] comparatorPolarity Output value for the OS output when active
     *  \param [in] scale Multiplier for temperature value as integer (scale=100 means 2.5C will be represented as 250)
     *  
     */
    nBlock_OLEDisplay(PinName pinSDA, PinName pinSCL, int address, float alarmMin, float alarmMax, int comparatorPolarity, int scale);
	
    /**
     *  \brief Called whenever a data packet is sent to this node in one
     *  of the inputs. In this case, sets a flag indicating a read must be 
     *  performed in the end of this frame, so data is available in the next.
     *  Does not engage in bus communication yet.
     *  
     *  \param [in] inputNumber Number identifying the input. This node has only one.
     *  \param [in] value The value sent to this node. In this node, any value triggers.
     */
    void triggerInput(nBlocks_Message message);
	
    /**
     *  \brief Called in the end of every frame.
     *  In this case, reads the sensor if a read was requested and prepares
     *  data in the output for the next frame.
     */
    void endFrame(void);
	
	/**
	 *  \brief Writes a 8-bit or 16-bit value into a register over I2C
	 *  
	 *  \param [in] regAddress Register address (range 0x00 - 0x03)
	 *  \param [in] value Value to be written
	 *  \param [in] num_bytes Number of bytes to send (1 or 2)
	 */
	void i2cWrite(uint8_t regAddress, uint16_t value, uint32_t num_bytes);
	
	/**
	 *  \brief Reads a 16-bit register over I2C
	 *  
	 *  \param [in] regAddress Register address (range 0x00 - 0x03)
	 *  \param [in] num_bytes Number of bytes to read (1 or 2)
	 *  \return Register value as 16-bit integer
	 *  
	 */
	uint16_t i2cRead(uint8_t regAddress, uint32_t num_bytes);
	
	
private:
	// I2C Object
	I2C _i2c;
	// I2C Address
	int _address;
	// Scale multiplier for output
	int _scale;
	// Flag indicating a read was requested
	int _flagReadRequested;
	
};

#endif
