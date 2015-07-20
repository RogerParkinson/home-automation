/*
 * I2CHandler.h
 *
 *  Created on: 16/07/2015
 *      Author: roger
 */

#ifndef SRC_I2CHANDLER_H_
#define SRC_I2CHANDLER_H_

class I2CHandler {
public:
	I2CHandler(unsigned char bus, unsigned char addr);
	virtual ~I2CHandler();
	int write_byte(unsigned char val);
	unsigned char read_byte();
private:
	int fd;
};

#endif /* SRC_I2CHANDLER_H_ */
