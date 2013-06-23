// DataConversionHelper.h: Schnittstelle fr die Klasse CDataConversionHelper.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MmcDataConversionHelper_H__C5CD9B35_F4CA_11D3_9E76_00A02436C4EF__INCLUDED_)
#define AFX_MmcDataConversionHelper_H__C5CD9B35_F4CA_11D3_9E76_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <StdString.h>
#include <MmcTypeDefinition.h>
#include <time.h>

class CMmcDataConversion
{
public:
	CMmcDataConversion();
	virtual ~CMmcDataConversion();

	bool HexByteStr2Byte(CStdString hexValueStr,BYTE* hexValue,bool showMsg = TRUE);

	bool IsDigit(TCHAR data);

	bool DecByteStr2Byte(CStdString decValueStr,BYTE* decValue,bool showMsg = TRUE);
	bool DecCharStr2Char(CStdString decValueStr,char* decValue,bool showMsg = TRUE);
	bool DecDWordStr2DWord(CStdString decValueStr,DWORD* decValue,bool showMsg = TRUE);
	bool DecFloatStr2Float(CStdString decValueStr,float *decValue,bool showMsg = TRUE);
	bool DecFloatStr2Fractional(CStdString decValueStr,WORD* pFractionNum,WORD* pFractionDen,bool showMsg = TRUE);
	bool DecIntStr2Int(CStdString decValueStr,int* decValue,bool showMsg = TRUE);
	bool DecLongStr2Long(CStdString decValueStr,long* decValue,bool showMsg = TRUE);
	bool DecShortStr2Short(CStdString decValueStr,short* decValue,bool showMsg = TRUE);
	bool DecWordStr2Word(CStdString decValueStr,WORD* decValue,bool showMsg = TRUE);
	bool DecInt64Str2Int64(CStdString decValueStr,__int64* decValue,bool showMsg = TRUE);

	bool HexCharStr2Char(CStdString hexValueStr,char* hexValue,bool showMsg = TRUE);
	bool HexDWordStr2DWord(CStdString hexValueStr,DWORD* hexValue,bool showMsg = TRUE);
	bool HexFloatStr2Float(CStdString hexValueStr,float* hexValue,bool showMsg = TRUE);
	bool HexLongStr2Long(CStdString hexValueStr,long *hexValue,bool showMsg = TRUE);
	bool HexShortStr2Short(CStdString hexValueStr,short* hexValue,bool showMsg = TRUE);
	bool HexWordStr2Word(CStdString hexValueStr,WORD* hexValue,bool showMsg = TRUE);
	bool HexInt64Str2Int64(CStdString hexValueStr,__int64* hexValue,bool showMsg = TRUE);

	bool ByteStr2Byte(CStdString valueStr,BYTE* pValue,bool showMsg = TRUE);
	bool Byte2HexByteStr(BYTE hexValue, CStdString *hexValueStr);

	bool CharStr2Char(CStdString valueStr,char* pValue,bool showMsg = TRUE);
	bool DWordStr2DWord(CStdString valueStr,DWORD* pValue,bool showMsg = TRUE);
	bool FloatStr2Float(CStdString valueStr,float* pValue,bool showMsg = TRUE);
	bool LongStr2Long(CStdString valueStr,long* pValue,bool showMsg = TRUE);
	bool ShortStr2Short(CStdString valueStr,short* pValue,bool showMsg = TRUE);
	bool WordStr2Word(CStdString valueStr,WORD* pValue,bool showMsg = TRUE);
	bool Int64Str2Int64(CStdString valueStr,__int64* pValue,bool showMsg = TRUE);

	bool TimeStr2Time(CStdString timeStr,time_t* time,bool showMsg = TRUE);

	bool Char2DecCharStr(BYTE decValue,CStdString *decValueStr);
	bool Char2HexCharStr(BYTE hexValue,CStdString *hexValueStr);
	bool DWord2DecDWordStr(DWORD decValue,CStdString *decValueStr);
	bool DWord2HexDWordStr(DWORD hexValue,CStdString *hexValueStr);
	bool Float2DecFloatStr(float decValue,CStdString *decValueStr);
	bool Float2DecFloatStr(float decValue, DWORD ulDecimalPlaces, CStdString *decValueStr);
	bool Float2HexFloatStr(float hexValue,CStdString *hexValueStr);
	bool Fractional2DecFloatStr(WORD fractionNum,WORD fractionDen,CStdString *pDecValueStr);
	bool Int2DecIntStr(int decValue,CStdString *decValueStr);
	bool Long2DecLongStr(long decValue,CStdString *decValueStr);
	bool Long2HexLongStr(long hexValue,CStdString *hexValueStr);
	bool Short2DecShortStr(short decValue,CStdString* decValueStr);
	bool Short2HexShortStr(short hexValue,CStdString* hexValueStr);

	bool Time2TimeStr(time_t time,CStdString* timeStr);

	bool UChar2DecUCharStr(BYTE decValue,CStdString *decValueStr);
	bool UChar2HexUCharStr(BYTE hexValue,CStdString *hexValueStr);
	bool Word2DecWordStr(WORD decValue,CStdString *decValueStr);
	bool Word2HexWordStr(WORD hexValue,CStdString *hexValueStr);

	bool Int64_2DecInt64Str(__int64 decValue,CStdString *decValueStr);
	bool Int64_2HexInt64Str(__int64 hexValue,CStdString *hexValueStr);

	bool UInt64_2HexUInt64Str(unsigned __int64 hexValue,CStdString *hexValueStr);
	bool UInt64_2DecUInt64Str(unsigned __int64 decValue,CStdString *decValueStr);
	bool UInt64Str2UInt64(CStdString valueStr,unsigned __int64* pValue,bool showMsg = TRUE);
	bool HexUInt64Str2UInt64(CStdString hexValueStr,unsigned __int64* hexValue,bool showMsg = TRUE);
	bool DecUInt64Str2UInt64(CStdString decValueStr,unsigned __int64* decValue,bool showMsg = TRUE);

private:
	enum FormatType{UnknownFormat,DecFormat,HexFormat};

	bool Char2HexDigit(TCHAR data,BYTE *digit);
	bool Char2DecDigit(TCHAR data,BYTE *digit);
	FormatType GetFormatType(CStdString valueStr);
};

#endif // !defined(AFX_MmcDataConversionHelper_H__C5CD9B35_F4CA_11D3_9E76_00A02436C4EF__INCLUDED_)
