#include "OLEDisplay.h"


nBlock_OLEDisplay::nBlock_OLEDisplay(PinName pinSDA, PinName pinSCL, int address, 
		float alarmMin = 75.0, float alarmMax = 80.0, 
		int comparatorPolarity = 0, int scale = 1):
			_i2c(pinSDA, pinSCL) {
		
	_i2c.frequency(400000);
		
	// Store the I2C address
	_address = address;
	
	// Store scale multiplier
	_scale = scale;
	
	// Reset the flag, we are not requesting a read yet
	_flagReadRequested = 0;
	
	// Configures to a minimum of 4 consecutive readings beyond threshold 
	// to qualify as valid comparison
	// And the specified comparator polarity
	uint8_t config_value = __OLEDisplay_CONF_FAULTQUEUE_4 | ((comparatorPolarity & 0b00000001) << __OLEDisplay_CONF_POLARITY_Pos);
	i2cWrite(__OLEDisplay_CONFIG, config_value, 1);

	// Configures low temperature threshold for comparator
	uint16_t t_hyst = ((uint16_t)round(alarmMin * 2.0)) << 7;
	i2cWrite(__OLEDisplay_ALARM_LOW, t_hyst, 2);
	
	// Configures high temperature threshold for comparator
	uint16_t t_os = ((uint16_t)round(alarmMax * 2.0)) << 7;
	i2cWrite(__OLEDisplay_ALARM_HIGH, t_os, 2);
	
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
		
		// Perform the I2C read
		
		// Reads register and casts to float
		float temperature_shifted = i2cRead(__OLEDisplay_TEMPERATURE, 2) * 1.0; // temperature is *signed* int
		// Register value (temperature_shifted) is shifted 7 to left, 0.5 resolution
		// That is, MSB is round Celsius
		// temperature is Celsius as float
		float temperature = temperature_shifted / 256; 
		
        output[0] = temperature*_scale;
		available[0] = 1;
				
	}
    return;
}


void nBlock_OLEDisplay::i2cWrite(uint8_t regAddress, uint16_t value, uint32_t num_bytes) {
	char buf[3]; // [reg. number, MSB, LSB]
	
	buf[0] = regAddress;
	
	// 2-byte transmission
	if (num_bytes == 2) {
		buf[1] = (value >> 8) & 0xFF;
		buf[2] = (value     ) & 0xFF;
		_i2c.write(_address, buf, 3);
	}
	// 1-byte transmission
	else {
		buf[1] = (value     ) & 0xFF;
		_i2c.write(_address, buf, 2);
	}
}

uint16_t nBlock_OLEDisplay::i2cRead(uint8_t regAddress, uint32_t num_bytes) {
	char buf[2]; // [reg. number, unused] or [MSB, LSB]

	// Sets the register address
	buf[0] = regAddress;
	buf[1] = 0xFF;// dummy
	
	_i2c.write(_address, buf, 1);
	
	// Clears the buffer and result variable
	buf[0] = 0;
	buf[1] = 0;
	uint16_t result = 0;
	
	// 2-byte reception
	if (num_bytes == 2) {
		_i2c.read(_address, buf, 2);
		// Cast to uint16_t is important otherwise the value disappears on overflow
		result = (((uint16_t)buf[0]) << 8) | buf[1];
	}
	// 1-byte reception
	else {
		_i2c.read(_address, buf, 1);
		result = buf[0] & 0xFF;
	}
	
	return result;
	
}
