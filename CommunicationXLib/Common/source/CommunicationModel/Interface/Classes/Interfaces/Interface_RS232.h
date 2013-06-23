// Interface_RS232.h: Schnittstelle f�r die Klasse CInterface_RS232.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACE_RS232_H__27D9CCA8_43A0_4813_8EF9_7B2298F7567B__INCLUDED_)
#define AFX_INTERFACE_RS232_H__27D9CCA8_43A0_4813_8EF9_7B2298F7567B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "BaseClasses/InterfaceBase.h"

class CInterface_RS232 : public CInterfaceBase
{
public:
	void Init() {m_strClassType="CInterface_RS232";}
//Initialisation
	BOOL InitInterface(WORD wBoardNumber,WORD wNbBoardWithOldDriver);

	BOOL I_OpenInterface(CErrorInfo* pErrorInfo = NULL);
	BOOL I_OpenInterfacePort(CPortBase* pPort,CStdString strPortName,CErrorInfo* pErrorInfo = NULL);

	BOOL I_CloseInterface(CErrorInfo* pErrorInfo = NULL);
	BOOL I_CloseInterfacePort(CPortBase* pPort,CErrorInfo* pErrorInfo = NULL);

	BOOL I_GetInterfaceSettings(CPortBase* pPort,DWORD* pdBaudrate,DWORD* pdTimeout,CErrorInfo* pErrorInfo = NULL);
	BOOL I_SetInterfaceSettings(CPortBase* pPort,DWORD dBaudrate,DWORD dTimeout,BOOL oChangeOnly,CErrorInfo* pErrorInfo = NULL);

	BOOL I_GetInterfaceMode(WORD* pwModeIndex,CErrorInfo* pErrorInfo = NULL);
	BOOL I_SetInterfaceMode(WORD wModeIndex,CErrorInfo* pErrorInfo = NULL);

	BOOL I_ResetInterface(CPortBase* pPort,CErrorInfo* pErrorInfo = NULL);

	CInterface_RS232();
	CInterface_RS232(const CInterface_RS232& rObject);
	virtual ~CInterface_RS232();
	CInterfaceBase* Clone();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

private:
	BOOL InitErrorHandling();
	BOOL InitGateway();

	//ParameterSet
	BOOL InitParameterSet();

};


#endif // !defined(AFX_INTERFACE_RS232_H__27D9CCA8_43A0_4813_8EF9_7B2298F7567B__INCLUDED_)
