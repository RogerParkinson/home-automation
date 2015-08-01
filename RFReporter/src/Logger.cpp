/*
 * Logger.cpp
 *
 *  Created on: 6/07/2015
 *      Author: roger
 */

#include <fstream>
#include <ctime>

//using namespace std;

#include "Logger.h"

Logger::Logger(char const *logfile) {
	m_logfile = logfile;

}

void Logger::logEntry(int id) {
	std::time_t result = std::time(NULL);

	std::ofstream logStream(m_logfile, std::ofstream::app);
	logStream << id << "at " << std::asctime(std::localtime(&result))
			<< std::endl;
	logStream.close();
}

void Logger::logString(char *s) {
	std::time_t result = std::time(NULL);
	std::ofstream logStream(m_logfile, std::ofstream::app);
	logStream << s << " at " << std::asctime(std::localtime(&result)) << std::endl;
	logStream.close();
	printf(" %s",s);
}


	void Logger::newLog() {
		std::time_t result = std::time(NULL);

		std::ofstream logStream("/home/receiver/log.txt", std::ofstream::out);
		logStream << "New log " << std::asctime(std::localtime(&result))
				<< std::endl;
		logStream.close();
	}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

