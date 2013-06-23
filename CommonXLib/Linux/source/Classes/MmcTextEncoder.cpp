/*
 * MmcTextEncoder.cpp
 *
 *  Created on: 23.10.2012
 *      Author: MMAGDAWS
 */

#include <Classes/MmcTextEncoder.h>

CStdString Mbcs2Unicode(CStdStringA pszMBCS)
{
	CStdString unicode;

	unicode = pszMBCS;

	return unicode;
}

CStdStringA Unicode2Mbcs(CStdString strText)
{
	CStdStringA strMbcsText;

	strMbcsText = strText;

	return strMbcsText;
}


