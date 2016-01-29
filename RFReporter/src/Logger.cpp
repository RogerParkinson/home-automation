/*
 * Logger.cpp
 *
 *  Created on: 6/07/2015
 *      Author: roger
 */

#include <fstream>
#include <ctime>
#include <sys/stat.h>

//using namespace std;

#include "Logger.h"

Logger::Logger(char const *logfile) {
	m_logfile = logfile;
	m_mode = S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH;
}

void Logger::logEntry(int id) {
	std::time_t result = std::time(NULL);

	std::ofstream logStream(m_logfile, std::ofstream::app);
	logStream << id << "at " << std::asctime(std::localtime(&result))
			<< std::endl;
	logStream.close();
	chmod(m_logfile,m_mode);
}

void Logger::logString(char *s) {
	std::time_t result = std::time(NULL);
	std::ofstream logStream(m_logfile, std::ofstream::app);
	logStream << s << " at " << std::asctime(std::localtime(&result)) << std::endl;
	logStream.close();
	chmod(m_logfile,m_mode);
}


void Logger::newLog() {
	std::time_t result = std::time(NULL);

	std::ofstream logStream(m_logfile, std::ofstream::app);
	logStream << "New log " << std::asctime(std::localtime(&result))
			<< std::endl;
	logStream.close();
	chmod(m_logfile,m_mode);
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

