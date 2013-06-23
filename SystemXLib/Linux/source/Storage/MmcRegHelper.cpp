/*
 * MmcRegHelper.cpp
 *
 *  Created on: Oct 21, 2010
 *      Author: dawid
 */
#include <Storage/MmcRegHelper.h>

long CMmcRegHelper::GetValue(CStdString strSubKey,CStdString valueName,CStdString* pStrValue)
{

	return 0;
}
long CMmcRegHelper::GetSubKeys(CStdString strKey, tRegStringList *pListSubKeys)
{
	/*lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKey,0,KEY_READ,&hKey);
	if(lResult == ERROR_SUCCESS)
	{
		do
		{
			char bName[256] = "";
			DWORD dName = 256;

			lResult = RegEnumKeyEx(hKey,l,bName,&dName,0,0,0,0);
			if(lResult != ERROR_NO_MORE_ITEMS)
			{
				CStdString* pStr = new CStdString();
				*pStr = bName;
				listSubKeys.push_back(pStr);
			}
			l++;
		}while(lResult != ERROR_NO_MORE_ITEMS);

		lResult = RegCloseKey(hKey);
		*/
	return 0;
}
