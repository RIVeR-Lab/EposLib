/*
 * MmcRegHelper.h
 *
 *  Created on: Oct 21, 2010
 *      Author: dawid
 */

#ifndef MMCREGHELPER_H_
#define MMCREGHELPER_H_

#include <list>
#include <StdString.h>

typedef std::list<CStdString*> tRegStringList;

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS                    0L
#endif

class CMmcRegHelper
{
public:
	static long GetValue(CStdString strSubKey,CStdString valueName,CStdString* pStrValue);
	static long GetSubKeys(CStdString strKey, tRegStringList *pListSubKeys);
	static long GetCurrentUserSubKeys(CStdString strKey, tRegStringList *pListSubKeys);
};

#endif /* MMCREGHELPER_H_ */
