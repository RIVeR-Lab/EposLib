/*
 * MmcLogger.h
 *
 *  Created on: Oct 20, 2010
 *      Author: dawid
 */

#ifndef MMCLOGGER_H_
#define MMCLOGGER_H_

#include <fstream>
#include <StdString.h>

const char LOGFILE_NAME[] = "MmcLogFile.log";

class CMmcLogger
{
public:
	static bool Commit(int p_lInstance, CStdString p_Action,
			    	   CStdString p_Infos="");

private:
	static bool WriteLineToFile(std::ofstream *file,
							CStdString lineStr);
};


#endif /* MMCLOGGER_H_ */
