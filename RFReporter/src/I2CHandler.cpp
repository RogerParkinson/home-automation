/*
 * I2CHandler.cpp
 *
 *  Created on: 16/07/2015
 *      Author: roger
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

#include "I2CHandler.h"

I2CHandler::I2CHandler(unsigned char bus, unsigned char addr) {
	char filename[16];
	sprintf(filename, "/dev/i2c-%d", bus);
//	printf("%s\n", filename);
	if ((fd = open(filename, O_RDWR)) < 0) {
		perror("Failed to open the i2c bus");
		exit(1);
	}
	if (ioctl(fd, I2C_SLAVE, addr) < 0) {
		fprintf(stderr, "i2c_open ioctl error: %s\n", strerror(errno));
		exit(1);
	}
//	signal(SIGINT, signal_handler);
}

I2CHandler::~I2CHandler() {
	if ((close(fd)) != 0) {
		fprintf(stderr, "i2c_close error: %s\n", strerror(errno));
	}
}

int I2CHandler::write_byte(unsigned char val) {
	if (write(fd, &val, 1) != 1) {
		fprintf(stderr, "i2c_write_byte error: %s\n", strerror(errno));
		return (-1);
	}
	return (1);

}
unsigned char I2CHandler::read_byte() {
	unsigned char val;
	if (read(fd, &val, 1) != 1) {
		fprintf(stderr, "i2c_read_byte error: %s\n", strerror(errno));
		return (-1);
	}
	return val;

}

