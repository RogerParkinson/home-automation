/*
 * Logger.h
 *
 *  Created on: 6/07/2015
 *      Author: roger
 */

#ifndef LOGGER_H_
#define LOGGER_H_


class Logger {
public:
	Logger(char const *logfile);
	virtual ~Logger();
	void logEntry(int id);
	void newLog();
	void logString(char *s);
private:
	char const  *m_logfile;
};

#endif /* LOGGER_H_ */
