// Command_I_USB.cpp: Implementierung der Klasse CCommand_I_USB.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_USB.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_I_USB::CCommand_I_USB()
{	
}

CCommand_I_USB::CCommand_I_USB(DWORD dCommandId)
{
	InitCommand(dCommandId);
}

CCommand_I_USB::~CCommand_I_USB()
{	
}

//******************************************************************
CCommandRoot* CCommand_I_USB::CloneCommand()
{
	CCommand_I_USB* pNewCommand;

	pNewCommand = new CCommand_I_USB();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_I_USB& CCommand_I_USB::operator=(CCommand_I_USB& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}
//******************************************************************
BOOL CCommand_I_USB::InitCommand(DWORD dCommandId)
{
	ResetCommand();
	switch(dCommandId)
	{
        case USB_WRITE_DATA:
			{
                CCommand_I::InitCommand("USB WriteData", USB_WRITE_DATA);
				AddParameter(0,"data",ODT_MEMORY_BLOCK);
				AddReturnParameter(0,"nbOfBytesWritten",ODT_UINT32,FALSE);
				SetDefaultParameter_WriteData();
				return TRUE;
			};
        case USB_READ_DATA:
			{
                CCommand_I::InitCommand("USB ReadData", USB_READ_DATA);
				AddParameter(0,"nbOfBytesToRead",ODT_UINT32,FALSE,TRUE);
				AddReturnParameter(0,"data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_ReadData();
				return TRUE;
			};

        case USB_EE_READ:
			{
                CCommand_I::InitCommand("USB_EE_Read", USB_EE_READ);
				AddReturnParameter(0,"vendorId",ODT_UINT16);
				AddReturnParameter(1,"productId",ODT_UINT16);
				AddReturnParameter(2,"manufacturer",ODT_MEMORY_BLOCK);
				AddReturnParameter(3,"description",ODT_MEMORY_BLOCK);
				AddReturnParameter(4,"serialNumber",ODT_MEMORY_BLOCK);
				AddReturnParameter(5,"useVcpDriver",ODT_BOOLEAN);
				SetDefaultParameter_EE_Read();
				return TRUE;
			};

        case USB_EE_PROGRAM:
			{
                CCommand_I::InitCommand("USB_EE_Program", USB_EE_PROGRAM);
				AddParameter(0,"vendorId",ODT_UINT16);
				AddParameter(1,"productId",ODT_UINT16);
				AddParameter(2,"manufacturer",ODT_MEMORY_BLOCK);
				AddParameter(3,"description",ODT_MEMORY_BLOCK);
				AddParameter(4,"serialNumber",ODT_MEMORY_BLOCK);
				AddParameter(5,"useVcpDriver",ODT_BOOLEAN);
				SetDefaultParameter_EE_Program();
				return TRUE;
			};

        case USB_RESCAN:
			{
                CCommand_I::InitCommand("USB_Rescan", USB_RESCAN);
				SetDefaultParameter_Rescan();
				return TRUE;
			};

        case USB_RELOAD:
			{
                CCommand_I::InitCommand("USB_Reload", USB_RELOAD);
				AddParameter(0,"vendorId",ODT_UINT16);
				AddParameter(1,"productId",ODT_UINT16);
				SetDefaultParameter_Reload();
				return TRUE;
			};

	}

	return FALSE;
}

//**************************************************************************
void CCommand_I_USB::SetDefaultParameter_ReadData()
{
	DWORD dNbOfBytesToRead = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&dNbOfBytesToRead,sizeof(dNbOfBytesToRead));

	//ReturnParameter
	SetReturnParameterData(0,pData,0);
}

//**************************************************************************
void CCommand_I_USB::SetDefaultParameter_WriteData()
{
	void* pData = NULL;
	DWORD dNbOfBytesWritten = 0;

	//Parameter
	SetParameterData(0,pData,0);

	//ReturnParameter
    SetReturnParameterData(0, &dNbOfBytesWritten, sizeof(dNbOfBytesWritten));
}

void CCommand_I_USB::SetDefaultParameter_EE_Read()
{
	WORD usVendorId = 0;
	WORD usProductId = 0;
	void* pManufacturer = NULL;
	void* pDescription = NULL;
	void* pSerialNumber = NULL;
	BOOL oUseVcpDriver = FALSE;

	//ReturnParameter
	SetReturnParameterData(0,&usVendorId,sizeof(usVendorId));
	SetReturnParameterData(1,&usProductId,sizeof(usVendorId));
	SetReturnParameterData(2,&pManufacturer,0);
	SetReturnParameterData(3,&pDescription,0);
	SetReturnParameterData(4,&pSerialNumber,0);
	SetReturnParameterData(5,&oUseVcpDriver,0);
}

void CCommand_I_USB::SetDefaultParameter_EE_Program()
{
	WORD usVendorId = 0;
	WORD usProductId = 0;
	void* pManufacturer = NULL;
	void* pDescription = NULL;
	void* pSerialNumber = NULL;
	BOOL oUseVcpDriver = FALSE;

	//Parameter
	SetParameterData(0,&usVendorId,sizeof(usVendorId));
	SetParameterData(1,&usProductId,sizeof(usVendorId));
	SetParameterData(2,&pManufacturer,0);
	SetParameterData(3,&pDescription,0);
	SetParameterData(4,&pSerialNumber,0);
	SetParameterData(5,&oUseVcpDriver,0);
}

void CCommand_I_USB::SetDefaultParameter_Rescan()
{
	//No Parameters
}

void CCommand_I_USB::SetDefaultParameter_Reload()
{
	WORD usVendorId = 0;
	WORD usProductId = 0;

	//Parameter
	SetParameterData(0,&usVendorId,sizeof(usVendorId));
	SetParameterData(1,&usProductId,sizeof(usVendorId));
}

