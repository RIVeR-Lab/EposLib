// DataConversionHelper.cpp: Implementierung der Klasse CDataConversionHelper.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include <Classes/MmcDataConversion.h>
#include <Ui/MmcMessageDialog.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMmcDataConversion::CMmcDataConversion()
{
}

CMmcDataConversion::~CMmcDataConversion()
{
}

bool CMmcDataConversion::Byte2HexByteStr(BYTE hexValue, CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format(_T("0x%.2X"), hexValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::HexWordStr2Word(CStdString hexValueStr,WORD* hexValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 4)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result > USHRT_MAX)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*hexValue = (WORD)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::HexByteStr2Byte(CStdString hexValueStr,BYTE* hexValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 2)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result > UCHAR_MAX)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*hexValue = (BYTE)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::HexCharStr2Char(CStdString hexValueStr, char *hexValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(result > SCHAR_MAX)
		{
			negative = TRUE;
			result -= UCHAR_MAX;
			result -= 1;
		}

		if(nbOfDigits > 2)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(negative)
		{
			if(result < ((__int64)SCHAR_MIN))
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > ((__int64)SCHAR_MAX))
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		*hexValue = (char)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::HexFloatStr2Float(CStdString hexValueStr,float* hexValue,bool showMsg)
{
	return HexDWordStr2DWord(hexValueStr,(DWORD*)hexValue,showMsg);
}

bool CMmcDataConversion::HexDWordStr2DWord(CStdString hexValueStr,DWORD* hexValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 8)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result > ULONG_MAX)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*hexValue = (DWORD)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::DecShortStr2Short(CStdString decValueStr,short* decValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int nbOfDigits = 0;
	int startChar = 0;
	short resultDummy;
	unsigned __int64 result = 0;

	if(decValue)
	{
		if(decValueStr.GetLength()>0)
		{
			if(decValueStr[0] == '-')
			{
				startChar = 1;
				negative = TRUE;
			}
		}

		for(startChar = startChar; startChar<decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 5)
		{
			if(showMsg)
			{
				if(negative) MmcMessageDialog(MT_INFO,"Value is too small!");else MmcMessageDialog(MT_INFO,"Value is too large!");
			}
			return FALSE;
		}

		if(negative)
		{
			if(result > ((__int64)SHRT_MAX)+1)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > ((__int64)SHRT_MAX))
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		resultDummy = short(result);
		if(negative) resultDummy *= -1;
		*decValue = resultDummy;

		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::DecByteStr2Byte(CStdString decValueStr,BYTE* decValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
	unsigned __int64 result = 0;

	if(decValue)
	{
		for(int startChar = 0; startChar<decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i=0; i<decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 3)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result > UCHAR_MAX)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*decValue = (BYTE)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::DecCharStr2Char(CStdString decValueStr,char* decValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int nbOfDigits = 0;
	int startChar = 0;
	char resultDummy;
	unsigned __int64 result = 0;

	if(decValue)
	{
		if(decValueStr.GetLength()>0)
		{
			if(decValueStr[0] == '-')
			{
				startChar = 1;
				negative = TRUE;
			}
		}

		for(startChar = startChar; startChar<decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 3)
		{
			if(showMsg)
			{
				if(negative) MmcMessageDialog(MT_INFO,"Value is too small!");else MmcMessageDialog(MT_INFO,"Value is too large!");
			}
			return FALSE;
		}

		if(negative)
		{
			if(result > ((__int64)SCHAR_MAX)+1)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > ((__int64)SCHAR_MAX))
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		resultDummy = char(result);
		if(negative) resultDummy *= -1;
		*decValue = resultDummy;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::DecWordStr2Word(CStdString decValueStr,WORD* decValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
	unsigned __int64 result = 0;

	if(decValue)
	{
		for(int startChar = 0; startChar<decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i=0; i<decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 5)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result > USHRT_MAX)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*decValue = (WORD)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::DecIntStr2Int(CStdString decValueStr,int* decValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int resultDummy;
	int startChar = 0;
	int nbOfDigits = 0;
	unsigned __int64 result = 0;

	if(decValue)
	{
		if(decValueStr.GetLength()>0)
		{
			if(decValueStr[0] == '-')
			{
				startChar = 1;
				negative = TRUE;
			}
		}

		for(startChar = startChar; startChar<decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 10)
		{
			if(showMsg)
			{
				if(negative) MmcMessageDialog(MT_INFO,"Value is too small!");else MmcMessageDialog(MT_INFO,"Value is too large!");
			}
			return FALSE;
		}

		if(negative)
		{
			if(result > ((__int64)LONG_MAX)+1)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > ((__int64)LONG_MAX))
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		resultDummy = int(result);
		if(negative) resultDummy *= -1;
		*decValue = resultDummy;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::DecDWordStr2DWord(CStdString decValueStr,DWORD* decValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
    int startChar = 0;
	unsigned __int64 result = 0;

	if(decValue)
	{
		for(startChar = 0; startChar < decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i = startChar; i < decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i), &digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if((nbOfDigits > 10) || (result > ULONG_MAX))
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*decValue = (DWORD)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::DecFloatStr2Float(CStdString decValueStr, float *decValue, bool showMsg)
{
	BYTE digit;
	int startChar = 0;

	if(decValueStr.GetLength() > 0)
	{
		if(decValueStr[0] == '-')
		{
			startChar = 1;
		}

		for(int i = startChar; i < decValueStr.GetLength(); i++)
		{
			if(!Char2DecDigit(decValueStr.GetAt(i), &digit))
			{
				if(decValueStr.GetAt(i) == '.')
				{
				}
				else
				{
					if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
					return FALSE;
				}
			}
		}
	}
	*decValue = (float)atof(decValueStr);

	return TRUE;
}

bool CMmcDataConversion::Char2HexDigit(TCHAR data, BYTE *digit)
{
	switch(data)
	{
		case '0': *digit = 0;  break;
		case '1': *digit = 1;  break;
		case '2': *digit = 2;  break;
		case '3': *digit = 3;  break;
		case '4': *digit = 4;  break;
		case '5': *digit = 5;  break;
		case '6': *digit = 6;  break;
		case '7': *digit = 7;  break;
		case '8': *digit = 8;  break;
		case '9': *digit = 9;  break;
		case 'A': *digit = 10; break;
		case 'a': *digit = 10; break;
		case 'B': *digit = 11; break;
		case 'b': *digit = 11; break;
		case 'C': *digit = 12; break;
		case 'c': *digit = 12; break;
		case 'D': *digit = 13; break;
		case 'd': *digit = 13; break;
		case 'E': *digit = 14; break;
		case 'e': *digit = 14; break;
		case 'F': *digit = 15; break;
		case 'f': *digit = 15; break;
		default: return FALSE;
	}
	return TRUE;
}

bool CMmcDataConversion::IsDigit(TCHAR data)
{
	switch(data)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': return TRUE;
		default: return FALSE;
	}
}

bool CMmcDataConversion::Char2DecDigit(TCHAR data, BYTE *digit)
{
	switch(data)
	{
		case '0': *digit = 0;break;
		case '1': *digit = 1;break;
		case '2': *digit = 2;break;
		case '3': *digit = 3;break;
		case '4': *digit = 4;break;
		case '5': *digit = 5;break;
		case '6': *digit = 6;break;
		case '7': *digit = 7;break;
		case '8': *digit = 8;break;
		case '9': *digit = 9;break;
		default: return FALSE;
	}
	return TRUE;
}

bool CMmcDataConversion::Short2DecShortStr(short decValue, CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%hi",decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Long2DecLongStr(long decValue, CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%i",decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Float2DecFloatStr(float decValue, CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%.3f", decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Float2HexFloatStr(float hexValue,CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		return DWord2HexDWordStr(*((DWORD*)&hexValue),hexValueStr);
	}

	return FALSE;
}

bool CMmcDataConversion::Word2DecWordStr(WORD decValue, CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%hu", decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Char2DecCharStr(BYTE decValue, CStdString *decValueStr)
{
	short buffer;

	if(decValueStr)
	{
		buffer = (char)decValue;
		decValueStr->Format("%i", buffer);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::UChar2DecUCharStr(BYTE decValue, CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%hu", decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Char2HexCharStr(BYTE hexValue, CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.2X", hexValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::UChar2HexUCharStr(BYTE hexValue, CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.2X", hexValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Int2DecIntStr(int decValue, CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%i", decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::DWord2DecDWordStr(DWORD decValue, CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%u", decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Word2HexWordStr(WORD hexValue, CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.4X", hexValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::DWord2HexDWordStr(DWORD hexValue, CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.8X", hexValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Time2TimeStr(time_t time, CStdString *timeStr)
{
	if(timeStr)
	{
		tm* atm = localtime ( &time );

		(*timeStr).Format("%02d.%02d.%04d %02d:%02d",
				atm->tm_wday,
				atm->tm_mon,
				atm->tm_year,
				atm->tm_hour, atm->tm_min);

		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::TimeStr2Time(CStdString timeStr, time_t* time, bool showMsg)
{
	CStdString subString;
	int nYear,nMonth,nDay,nHour,nMin;

	if(timeStr.GetLength() < 16) return FALSE;

	//Day
	subString = timeStr.Left(2);
	if(!DecIntStr2Int(subString, &nDay,FALSE) && showMsg)
	{
		MmcMessageDialog(MT_INFO,"Time format is not correct!\n Use the format DD.MM.YYYY HH.MM");
		return FALSE;
	}

	//Month
	subString = timeStr.Mid(3,2);
	if(!DecIntStr2Int(subString, &nMonth,FALSE) && showMsg)
	{
		MmcMessageDialog(MT_INFO,"Time format is not correct!\n Use the format DD.MM.YYYY HH.MM");
		return FALSE;
	}

	//Year
	subString = timeStr.Mid(6,4);
	if(!DecIntStr2Int(subString, &nYear, FALSE) && showMsg)
	{
		MmcMessageDialog(MT_INFO,"Time format is not correct!\n Use the format DD.MM.YYYY HH.MM");
		return FALSE;
	}

	//Hour
	subString = timeStr.Mid(11,2);
	if(!DecIntStr2Int(subString, &nHour,FALSE) && showMsg)
	{
		MmcMessageDialog(MT_INFO,"Time format is not correct!\n Use the format DD.MM.YYYY HH.MM");
		return FALSE;
	}

	//Minute
	subString = timeStr.Mid(14,2);
	if(!DecIntStr2Int(subString, &nMin,FALSE) && showMsg)
	{
		MmcMessageDialog(MT_INFO,"Time format is not correct!\n Use the format DD.MM.YYYY HH.MM");
		return FALSE;
	}

	struct tm atm;

	atm.tm_sec = 0;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	atm.tm_mday = nDay;
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = -1;

	*time = mktime(&atm);

	return true;
}

bool CMmcDataConversion::DecLongStr2Long(CStdString decValueStr,long *decValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int nbOfDigits = 0;
	int startChar = 0;
	long resultDummy;
	__int64 result = 0;

	if(decValue)
	{
		if(decValueStr.GetLength()>0)
		{
			if(decValueStr[0] == '-')
			{
				startChar = 1;
				negative = TRUE;
			}
		}

		for(startChar = startChar; startChar<decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 10)
		{
			if(showMsg)
			{
				if(negative) MmcMessageDialog(MT_INFO,"Value is too small!");else MmcMessageDialog(MT_INFO,"Value is too large!");
			}
			return FALSE;
		}

		if(negative)
		{
			if(result < LONG_MIN)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > LONG_MAX)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		resultDummy = long(result);
		if(negative) resultDummy *= -1;
		*decValue = resultDummy;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::Long2HexLongStr(long hexValue,CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.8X",hexValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Short2HexShortStr(short hexValue,CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.4X",hexValue);
		if(hexValue < 0)
		{
			hexValueStr->Delete(2,4);
		}
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::HexShortStr2Short(CStdString hexValueStr,short *hexValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(result > SHRT_MAX)
		{
			negative = TRUE;
			result -= USHRT_MAX;
			result -= 1;
		}

		if(nbOfDigits > 4)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(negative)
		{
			if(result < SHRT_MIN)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > SHRT_MAX)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		*hexValue = (short)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::HexLongStr2Long(CStdString hexValueStr,long *hexValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(result > LONG_MAX)
		{
			negative = TRUE;
			result -= ULONG_MAX;
			result -= 1;
		}

		if(nbOfDigits > 8)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(negative)
		{
			if(result < ((__int64)LONG_MIN))
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > ((__int64)LONG_MAX))
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		*hexValue = (long)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::Fractional2DecFloatStr(WORD fractionNum,WORD fractionDen,CStdString *pDecValueStr)
{
	CStdString strValue;
	float fValue;
	WORD nbOfEndingZeros;
	int iIndex;

	//Check range
	if(fractionDen == 0) return FALSE;

	//Calculate float value
	fValue = float(fractionNum)/float(fractionDen);

	//Format String
	strValue.Format("%.6f",fValue);

	//Remove Ending zeros
	iIndex = strValue.ReverseFind('.');
	if(iIndex != -1)
	{
		nbOfEndingZeros = 0;
		for(int i=strValue.GetLength()-1;i>iIndex;i--)
		{
			if(strValue.GetAt(i) != '0') break;
			nbOfEndingZeros++;
		}
		strValue = strValue.Left(strValue.GetLength()-nbOfEndingZeros);
		if((strValue.GetLength() > 0) && (strValue.GetAt(strValue.GetLength()-1) == '.')) strValue += "0";
	}

	if(pDecValueStr) *pDecValueStr = strValue;
	return TRUE;
}

bool CMmcDataConversion::DecFloatStr2Fractional(CStdString decValueStr,WORD *pFractionNum,WORD *pFractionDen,bool showMsg)
{
	const BYTE baseValue = 10;
	const unsigned __int64 uint64_max = ULLONG_MAX;

	BYTE digit;
	int startChar = 0;
	unsigned __int64 num,den;

	bool oDecimalPlaces = FALSE;

	if(decValueStr.GetLength() > 0)
	{
		if(decValueStr[0] == '-')
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Negative Value is not allowed!");
			return FALSE;
		}

		num = 0;
		den = 1;
		for(int i=startChar;i<decValueStr.GetLength();i++)
		{
			if(!Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				if(decValueStr.GetAt(i) == '.')
				{
					oDecimalPlaces = TRUE;
				}
				else
				{
					if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
					return FALSE;
				}
			}
			else
			{
				num *= baseValue;
				num += digit;

				if(oDecimalPlaces)
				{
					den *= baseValue;
				}
			}

			//Check Range to UINT64
			if(num > uint64_max)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large or has to many decimal places!");
				return FALSE;
			}

			if(den > uint64_max)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large or has to many decimal places!");
				return FALSE;
			}
		}

		//Shorten the fractional Number
		while(((num % baseValue) == 0) && ((den % baseValue) == 0))
		{
			num /= baseValue;
			den /= baseValue;
		}

		//Check Range to UINT16
		if(num > ((__int64)USHRT_MAX))
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large or has to many decimal places!");
			return FALSE;
		}

		if(den > ((__int64)USHRT_MAX))
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large or has to many decimal places!");
			return FALSE;
		}

		//Copy return values
		if(pFractionNum) *pFractionNum = (WORD)num;
		if(pFractionDen) *pFractionDen = (WORD)den;
		return TRUE;
	}
	else
	{
		if(pFractionNum) *pFractionNum = 1;
		if(pFractionDen) *pFractionDen = 1;
		return TRUE;
	}
}

bool CMmcDataConversion::DecInt64Str2Int64(CStdString decValueStr,__int64* decValue,bool showMsg)
{
	bool negative = FALSE;
	BYTE digit;
	int nbOfDigits = 0;
	int startChar = 0;
	__int64 resultDummy;
	__int64 result = 0;

	if(decValue)
	{
		if(decValueStr.GetLength()>0)
		{
			if(decValueStr[0] == '-')
			{
				startChar = 1;
				negative = TRUE;
			}
		}

		for(startChar = startChar; startChar<decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 19)
		{
			if(showMsg)
			{
				if(negative) MmcMessageDialog(MT_INFO,"Value is too small!");else MmcMessageDialog(MT_INFO,"Value is too large!");
			}
			return FALSE;
		}

		if(negative)
		{
			if(result < _I64_MIN)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
				return FALSE;
			}
		}
		else
		{
			if(result > _I64_MAX)
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
				return FALSE;
			}
		}

		resultDummy = (__int64)result;
		if(negative) resultDummy *= -1;
		*decValue = resultDummy;
		return TRUE;
	}
	return FALSE;

}

bool CMmcDataConversion::DecUInt64Str2UInt64(CStdString decValueStr,unsigned __int64* decValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
    int startChar = 0;
	unsigned __int64 result = 0;

	if(decValue)
	{
		for(startChar = 0; startChar < decValueStr.GetLength(); startChar++)
		{
			if(decValueStr[startChar] != '0') break;
		}

		for(int i = startChar; i < decValueStr.GetLength(); i++)
		{
			if(Char2DecDigit(decValueStr.GetAt(i),&digit))
			{
				result *= 10;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a decimal number!");
				return FALSE;
			}
		}

		if((nbOfDigits > 20) || (result > _UI64_MAX))
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*decValue = (unsigned __int64)result;
		return TRUE;
	}
	return FALSE;

}

bool CMmcDataConversion::HexInt64Str2Int64(CStdString hexValueStr, __int64* hexValue, bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 16)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result > _UI64_MAX)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*hexValue = (__int64)result;
		return TRUE;
	}
	return FALSE;
}

bool CMmcDataConversion::HexUInt64Str2UInt64(CStdString hexValueStr,unsigned __int64* hexValue,bool showMsg)
{
	BYTE digit;
	int nbOfDigits = 0;
	int startChar;
	unsigned __int64 result = 0;

	if(hexValue)
	{
		if((hexValueStr.GetLength() >= 2) && (hexValueStr[0] == '0') && (hexValueStr[1] == 'x')) startChar = 2;else startChar = 0;

		for(startChar = startChar; startChar<hexValueStr.GetLength(); startChar++)
		{
			if(hexValueStr[startChar] != '0') break;
		}

		for(int i=startChar; i<hexValueStr.GetLength(); i++)
		{
			if(Char2HexDigit(hexValueStr.GetAt(i),&digit))
			{
				result *= 16;
				result += digit;
				nbOfDigits++;
			}
			else
			{
				if(showMsg) MmcMessageDialog(MT_INFO,"Value is not a hexadecimal number!");
				return FALSE;
			}
		}

		if(nbOfDigits > 16)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result > _UI64_MAX)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too large!");
			return FALSE;
		}

		if(result < 0)
		{
			if(showMsg) MmcMessageDialog(MT_INFO,"Value is too small!");
			return FALSE;
		}

		*hexValue = (unsigned __int64)result;
		return TRUE;
	}
	return FALSE;

}

bool CMmcDataConversion::Int64_2DecInt64Str(__int64 decValue,CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%I64i",decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::UInt64_2DecUInt64Str(unsigned __int64 decValue,CStdString *decValueStr)
{
	if(decValueStr)
	{
		decValueStr->Format("%I64u",decValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::Int64_2HexInt64Str(__int64 hexValue,CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.16I64X",hexValue);
		return TRUE;
	}

	return FALSE;
}

bool CMmcDataConversion::UInt64_2HexUInt64Str(unsigned __int64 hexValue,CStdString *hexValueStr)
{
	if(hexValueStr)
	{
		hexValueStr->Format("0x%.16I64X",hexValue);
		return TRUE;
	}

	return FALSE;
}

CMmcDataConversion::FormatType CMmcDataConversion::GetFormatType(CStdString valueStr)
{
	if((valueStr.IsEmpty()) || (valueStr.GetLength() == 1))
	{
		return DecFormat;
	}
	else
	{
		if(valueStr.Left(2) == "0x") return HexFormat;
		else return DecFormat;
	}
}

bool CMmcDataConversion::ByteStr2Byte(CStdString valueStr,BYTE* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecByteStr2Byte(valueStr,pValue,showMsg);
		case HexFormat: return HexByteStr2Byte(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::CharStr2Char(CStdString valueStr,char* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecCharStr2Char(valueStr,pValue,showMsg);
		case HexFormat: return HexCharStr2Char(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::DWordStr2DWord(CStdString valueStr,DWORD* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecDWordStr2DWord(valueStr,pValue,showMsg);
		case HexFormat: return HexDWordStr2DWord(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::FloatStr2Float(CStdString valueStr,float* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecFloatStr2Float(valueStr,pValue,showMsg);
		case HexFormat: return HexFloatStr2Float(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::LongStr2Long(CStdString valueStr,long* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecLongStr2Long(valueStr,pValue,showMsg);
		case HexFormat: return HexLongStr2Long(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::ShortStr2Short(CStdString valueStr,short* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecShortStr2Short(valueStr,pValue,showMsg);
		case HexFormat: return HexShortStr2Short(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::WordStr2Word(CStdString valueStr,WORD* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecWordStr2Word(valueStr,pValue,showMsg);
		case HexFormat: return HexWordStr2Word(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::Int64Str2Int64(CStdString valueStr,__int64* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecInt64Str2Int64(valueStr,pValue,showMsg);
		case HexFormat: return HexInt64Str2Int64(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
	
bool CMmcDataConversion::UInt64Str2UInt64(CStdString valueStr,unsigned __int64* pValue,bool showMsg)
{
	switch(GetFormatType(valueStr))
	{
		case DecFormat: return DecUInt64Str2UInt64(valueStr,pValue,showMsg);
		case HexFormat: return HexUInt64Str2UInt64(valueStr,pValue,showMsg);
		default: return FALSE;
	}
}
