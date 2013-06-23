/*
 * MmcLogger.cpp
 *
 *  Created on: Oct 20, 2010
 *      Author: dawid
 */
#include <Log/MmcLogger.h>
#include <time.h>
#include <Process/MmcProcess.h>

bool CMmcLogger::Commit(int p_lInstance, CStdString p_Action,
					    CStdString p_Infos/*=""*/)
{
	std::ofstream file;
	CStdString lineStr;
	CStdString date;
	CStdString process;
	CStdString infos;
	bool result = false;

	file.open(LOGFILE_NAME);

	if(!file.fail())
	{
		//Format Line
		time_t rawtime;
		struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		date = asctime(timeinfo);

		process.Format("PID %i Module %s Instance %i",
				MmcGetCurrentProcessId(),
				MmcGetModuleName(), p_lInstance);

		if( !p_Infos.IsEmpty() )
			lineStr = date + process + " " + p_Action + " Infos: " + p_Infos;
		else
			lineStr = date + process + " " + p_Action;

		//Write Line
		result = CMmcLogger::WriteLineToFile(&file, lineStr);

		file.close();
	}

	return result;
}

bool CMmcLogger::WriteLineToFile(std::ofstream *file, CStdString lineStr)
{
    char lineFeed = '\x0A';
    char carriageReturn = '\r';

    if(file)
    {
		*file << lineStr << carriageReturn << lineFeed;

        return true;
    }

    return false;
}

