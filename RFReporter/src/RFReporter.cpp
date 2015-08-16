//============================================================================
// Name        : RFReporter.cpp
// Author      : RJP
// Version     :
// Copyright   : (C) 2015 Prometheus Consulting
// Description : Reports RF signals to a logfile on a Beaglebone
//============================================================================

#include <unistd.h>
#include <fstream>
#include <ctime>

#include "I2CHandler.h"
#include "Logger.h"

#define HOME 1
#define LOW_POWER 2
#define EVENT 1


#define BUFFER_LENGTH 100

int main(int argc, char* argv[]) {

	unsigned int delay = 4;
	int triggerHour = 18;
//	unsigned char rByte = 0;
	unsigned char addr = 0x26;
	unsigned char bus = 2;
	int lastHour = -1;
	int i;
	unsigned char buf[BUFFER_LENGTH];
	unsigned char buflen = 0;
	char s[100];

	Logger logger("/home/receiver/log.txt");
	logger.newLog();
	I2CHandler i2cHandler(bus, addr);

	while (1) {
		if (i2cHandler.write_byte(0x01) == -1) {
			printf("write failed\n");
			sleep(delay);
			continue;
		}
		buflen = i2cHandler.read_byte();
		if (buflen != 0 && buflen != 255) {
			std::time_t result = std::time(NULL);
//			std::asctime(std::localtime(&result));
			printf("%s buflen=%d ",std::asctime(std::localtime(&result)),buflen);
			for (i = 0; i < buflen && i < BUFFER_LENGTH; i++) {
				buf[i] = i2cHandler.read_byte();
				printf(" %d ",buf[i]);
			}
			if (buf[0] == HOME) {
				if (buf[2] == LOW_POWER) {
					sprintf(s, "low power for device %d ", buf[1]);
					logger.logString(s);
				} else if (buf[2] == EVENT) {
					sprintf(s, "event for device %d ", buf[1]);
					logger.logString(s);
				} else {
					sprintf(s, "unknown event %d for device %d ", buf[2],
							buf[1]);
					logger.logString(s);
				}
			}
			printf("\n");
		}
		time_t now = time(0);
		tm *ltm = localtime(&now);
		int hour = ltm->tm_hour;
		if (hour == triggerHour && lastHour == (triggerHour - 1)) {
			logger.newLog();
		}
		lastHour = hour;
		sleep(delay);
	}
}

