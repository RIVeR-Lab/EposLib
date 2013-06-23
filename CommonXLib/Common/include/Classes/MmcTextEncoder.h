#pragma once

#include <StdString.h>

#ifndef _RT
#define _RT
#endif

CStdString Mbcs2Unicode(CStdStringA pszMBCS);
CStdStringA Unicode2Mbcs(CStdString strText);