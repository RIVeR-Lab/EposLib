// ParameterEntry.cpp: Implementierung der Klasse CParameterEntry.
//
//////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <Classes/MmcDataConversion.h>

#include <PersistenceReaderWriter/XmlWriter.h>
#include <PersistenceReaderWriter/XmlReader.h>
#include <ParameterSet/ParameterSetEntry.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CParameterSetEntry::CParameterSetEntry()
{
	m_Name = _T("");
	m_oPersistent = FALSE;
	m_pDefaultValueBuffer = NULL;
	m_ulDefaultValueBufferSize = 0;
	m_pValueBuffer = NULL;
	m_ulValueBufferSize = 0;
}

CParameterSetEntry::~CParameterSetEntry()
{
	DeleteDefaultValueBuffer();
	DeleteValueBuffer();
}

BOOL CParameterSetEntry::CreateDefaultValueBuffer(BYTE* p_pData, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	DeleteDefaultValueBuffer();
	if((!m_pDefaultValueBuffer && (m_ulDefaultValueBufferSize == 0)) && (p_pData && (p_ulSize != 0)))
	{
		m_pDefaultValueBuffer = (BYTE*)malloc(p_ulSize);
		memcpy(m_pDefaultValueBuffer, p_pData, p_ulSize);
		m_ulDefaultValueBufferSize = p_ulSize;
		oResult = TRUE;
	}

	return oResult;
}
	
BOOL CParameterSetEntry::CreateValueBuffer(BYTE* p_pData, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	DeleteValueBuffer();
	if((!m_pValueBuffer && (m_ulValueBufferSize == 0)) && (p_pData && (p_ulSize != 0)))
	{
		m_pValueBuffer = (BYTE*)malloc(p_ulSize);
		memcpy(m_pValueBuffer, p_pData, p_ulSize);
		m_ulValueBufferSize = p_ulSize;
		oResult = TRUE;
	}

	return oResult;
}

BOOL CParameterSetEntry::DeleteDefaultValueBuffer()
{
	BOOL oResult = FALSE;

	if(m_pDefaultValueBuffer)
	{
		free(m_pDefaultValueBuffer);
		m_pDefaultValueBuffer = NULL;
		m_ulDefaultValueBufferSize = 0;
		oResult = TRUE;
	}

	return oResult;
}
	
BOOL CParameterSetEntry::DeleteValueBuffer()
{
	BOOL oResult = FALSE;

	if(m_pValueBuffer)
	{
		free(m_pValueBuffer);
		m_pValueBuffer = NULL;
		m_ulValueBufferSize = 0;
		oResult = TRUE;
	}

	return oResult;
}

CParameterSetEntry& CParameterSetEntry::operator=(CParameterSetEntry& other)
{
	if(this != &other)
	{
		m_Name = other.m_Name;
		m_oPersistent = other.m_oPersistent;
		CreateDefaultValueBuffer(other.m_pDefaultValueBuffer,other.m_ulDefaultValueBufferSize);
		CreateValueBuffer(other.m_pValueBuffer,other.m_ulValueBufferSize);
	}

	return *this;
}

BOOL CParameterSetEntry::operator==(CParameterSetEntry& other)
{
	BOOL oResult = TRUE;
	
	if(this != &other)
	{
		if(oResult && (m_Name != other.m_Name)) oResult = FALSE;
		if(oResult && (m_ulValueBufferSize != other.m_ulValueBufferSize)) oResult = FALSE;
		if(oResult && (m_pValueBuffer == NULL) && (other.m_pValueBuffer != NULL)) oResult = FALSE;
		if(oResult && (m_pValueBuffer != NULL) && (other.m_pValueBuffer == NULL)) oResult = FALSE;
		if(oResult && (m_pValueBuffer != NULL) && (other.m_pValueBuffer != NULL))
		{
			if(oResult && (memcmp(m_pValueBuffer,other.m_pValueBuffer,m_ulValueBufferSize) != 0)) oResult = FALSE; 
		}
	}

	return oResult;
}

BOOL CParameterSetEntry::Init(CStdString p_Name, BYTE* p_pDefaultValue, DWORD p_ulSize, BOOL p_oPersistent)
{
	BOOL oResult = TRUE;

	m_Name = p_Name;
	m_oPersistent = p_oPersistent;
	if(oResult && !CreateDefaultValueBuffer(p_pDefaultValue,p_ulSize)) oResult = FALSE;
	
	return oResult;
}

BOOL CParameterSetEntry::Init(CStdString p_Name, CStdString p_DefaultValue, BOOL p_oPersistent)
{
	BOOL oResult = TRUE;
	TCHAR* pDefaultData = NULL;

	//General Attributes
	m_Name = p_Name;
	m_oPersistent = p_oPersistent;

	//Create Default Value
	pDefaultData = p_DefaultValue.GetBuffer();
	if(oResult && pDefaultData)
	{
		oResult = CreateDefaultValueBuffer((BYTE*)pDefaultData,strlen(pDefaultData)*sizeof(TCHAR));
	}
	p_DefaultValue.ReleaseBuffer();

	return oResult;
}

BOOL CParameterSetEntry::ResetValue()
{
	BOOL oResult = FALSE;

	DeleteValueBuffer();
	oResult = CreateValueBuffer(m_pDefaultValueBuffer,m_ulDefaultValueBufferSize);

	return oResult;
}
	
CStdString CParameterSetEntry::GetName()
{
	return m_Name;
}
	
BOOL CParameterSetEntry::IsPersistent()
{
	return m_oPersistent;
}

BOOL CParameterSetEntry::GetParameter(BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pValueBuffer && p_pValue && (m_ulValueBufferSize == p_ulSize))
	{
		memcpy(p_pValue, m_pValueBuffer,m_ulValueBufferSize);
		oResult = TRUE;
	}

	return oResult;
}

BOOL CParameterSetEntry::GetParameter(CStdString& p_rValue)
{
	BOOL oResult = FALSE;

	TCHAR* pBuffer = NULL;

	p_rValue = _T("");
	if(m_pValueBuffer)
	{
		pBuffer = (TCHAR*)m_pValueBuffer;
		for(DWORD i = 0; i< m_ulValueBufferSize/sizeof(TCHAR); i++)
		{
			p_rValue += *pBuffer++;
		}
		oResult = TRUE;
	}

	return oResult;
}

BOOL CParameterSetEntry::SetParameter(BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	oResult = CreateValueBuffer(p_pValue,p_ulSize);

	return oResult;
}

BOOL CParameterSetEntry::SetParameter(CStdString p_Value)
{
	BOOL oResult = FALSE;
	TCHAR* pData = NULL;

	pData = p_Value.GetBuffer();
	if(pData)
	{
		oResult = CreateValueBuffer((BYTE*)pData, strlen(pData)*sizeof(TCHAR));
	}
	p_Value.ReleaseBuffer();

	return oResult;
}

BOOL CParameterSetEntry::WriteTo(CXmlWriter* p_pXmlWriter)
{
	BOOL oResult = FALSE;

	CStdString valueString = _T("");

	if(p_pXmlWriter)
	{
		//Init
		oResult = TRUE;

		//Parameter
		if(FormatValueString(m_pValueBuffer,m_ulValueBufferSize,valueString))
		{
			if(oResult && !p_pXmlWriter->WriteElement(m_Name,valueString)) oResult = FALSE;
		}
	}

	return oResult;
}

BOOL CParameterSetEntry::ReadFrom(CXmlReader* p_pXmlReader)
{
	BOOL oResult = FALSE;

	CStdString valueString = _T("");

	if(p_pXmlReader)
	{
		//Init
		oResult = TRUE;
		//Parameter
		if(p_pXmlReader->ReadElement(m_Name,0,valueString))
		{
			oResult = ParseValueString(valueString,m_pValueBuffer,m_ulValueBufferSize);
		}
	}

	return oResult;
}

BOOL CParameterSetEntry::FormatValueString(BYTE* p_pBuffer, DWORD p_ulBufferSize, CStdString& p_rValueString)
{
	BOOL oResult = FALSE;
	CStdString resultString = _T("");
	CStdString digit = _T("");

	for(DWORD i = 0;i < p_ulBufferSize; i++)
	{
		digit.Format("0x%.2X ",*p_pBuffer++);
		resultString += digit;
	}
	resultString.TrimRight(" ");
	p_rValueString = resultString;
	oResult = TRUE;

	return oResult;
}

BOOL CParameterSetEntry::ParseValueString(CStdString p_ValueString, BYTE*& p_rpBuffer, DWORD& p_rulBufferSize)
{
	BOOL oResult = TRUE;
	CStdString resToken = _T("");
	CStdStringArray tokenArray;
	CMmcDataConversion  dataConversion;
	BYTE* pTargetBuffer = NULL;

	//Tokenize
	 char* str = p_ValueString.GetBuffer();

	 char* pch = strtok(str,"x ");

     while(pch != NULL)
	 {
    	 resToken = pch;
    	 resToken.Trim();
		 if(resToken.GetLength() == strlen("00"))
		 {
			resToken = "0x" + resToken;
			tokenArray.push_back(resToken);
		 }

		pch = strtok (NULL, "x ");
	  }
     p_ValueString.ReleaseBuffer();

	//Prepare Buffer
	if(p_rpBuffer)
	{
		free(p_rpBuffer);
		p_rpBuffer = (BYTE*)malloc(tokenArray.size());
		p_rulBufferSize = tokenArray.size();
	}

	//Convert Token to Byte
	pTargetBuffer  = p_rpBuffer;
	for(CStdStringArray::iterator it = tokenArray.begin(); it != tokenArray.end(); it++)
	{
		if(!dataConversion.HexByteStr2Byte((*it).c_str(),
											pTargetBuffer++,FALSE)) oResult = FALSE;
	}

	return oResult;
}
