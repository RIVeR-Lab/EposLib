// DeviceErrorHandling.cpp: Implementierung der Klasse CDeviceErrorHandling.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <CommunicationModel/CommonLayer/ErrorHandling/DeviceErrorHandling.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CDeviceErrorHandling::CDeviceErrorHandling()
{
    
}

CDeviceErrorHandling::~CDeviceErrorHandling()
{
    
}

BOOL CDeviceErrorHandling::GetErrorDescription(DWORD dErrorCode, CStdString *pDescription)
{
    WORD usErrorCode(0);
	BOOL oResult(FALSE);

	if(pDescription)
    {
		//Init
		usErrorCode = (WORD)(dErrorCode & ERROR_MASK);
        
		//Description
		if(!oResult && GetErrorDescription_EPOS2(usErrorCode, pDescription)) oResult = TRUE;
		if(!oResult && GetErrorDescription_EPOS2_P(usErrorCode, pDescription)) oResult = TRUE;
		if(!oResult && GetErrorDescription_EPOS(usErrorCode, pDescription)) oResult = TRUE;
		if(!oResult && GetErrorDescription_EPOS_P(usErrorCode, pDescription)) oResult = TRUE;
        
        if(!oResult && pDescription) *pDescription = "Unknown Error";
    }

    return oResult;
}

BOOL CDeviceErrorHandling::GetErrorDescription_EPOS(WORD usErrorCode, CStdString *pDescription)
{
    BOOL oResult(FALSE);

	switch(usErrorCode)
    {
        case ERROR_EPOS_GENERIC:					*pDescription = "Generic Error"; oResult = TRUE; break;
		case ERROR_EPOS_OVER_CURRENT:				*pDescription = "Over Current"; oResult = TRUE; break;
		case ERROR_EPOS_OVER_VOLTAGE:				*pDescription = "Over Voltage"; oResult = TRUE; break;
		case ERROR_EPOS_UNDER_VOLTAGE:				*pDescription = "Under Voltage"; oResult = TRUE; break;
		case ERROR_EPOS_OVER_TEMPERATURE:			*pDescription = "Over Temperature"; oResult = TRUE; break;
		case ERROR_EPOS_SUPPLY_VOLTAGE_TOO_LOW:		*pDescription = "Supply Voltage is too low"; oResult = TRUE; break;
		case ERROR_EPOS_INTERNAL_SOFTWARE:			*pDescription = "Internal Software Error"; oResult = TRUE; break;
		case ERROR_EPOS_SOFTWARE_PARAMETER:			*pDescription = "Software Parameter Error"; oResult = TRUE; break;
		case ERROR_EPOS_SENSOR_POSITION:			*pDescription = "Sensor Position Error"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_OVERRUN_OBJECT_LOST:    *pDescription = "CAN Overrun (Objects lost)"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_OVERRUN:				*pDescription = "CAN Overrun "; oResult = TRUE; break;
		case ERROR_EPOS_CAN_PASSIVE:				*pDescription = "CAN Passive Mode"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_LIFE_GUARD:				*pDescription = "CAN Lifeguard Error"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_TX_COB_ID_COLLISION:    *pDescription = "CAN Transmit COB-ID Collision"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_BUS_OFF:				*pDescription = "CAN Bus Off"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_RX_QUEUE_OVERRUN:		*pDescription = "CAN Rx Queue Overrun"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_TX_QUEUE_OVERRUN:		*pDescription = "CAN Tx Queue Overrun"; oResult = TRUE; break;
		case ERROR_EPOS_CAN_PDO_LENGTH:				*pDescription = "CAN PDO Length Error"; oResult = TRUE; break;
		case ERROR_EPOS_FOLLOWING_ERROR:			*pDescription = "Following Error"; oResult = TRUE; break;
		case ERROR_EPOS_HALL_SENSOR:				*pDescription = "Hall Sensor Error"; oResult = TRUE; break;
		case ERROR_EPOS_INDEX_PROCESSING:			*pDescription = "Index Processing Error"; oResult = TRUE; break;
		case ERROR_EPOS_ENCODER_RESOLUTION:			*pDescription = "Encoder Resolution Error"; oResult = TRUE; break;
		case ERROR_EPOS_HALL_SENSOR_NOT_FOUND:		*pDescription = "Hallsensor not found"; oResult = TRUE; break;
		case ERROR_EPOS_NEGATIVE_LIMIT:				*pDescription = "Negative Limit Switch Error"; oResult = TRUE; break;
		case ERROR_EPOS_POSITIVE_LIMIT:				*pDescription = "Positive Limit Switch Error"; oResult = TRUE; break;
		case ERROR_EPOS_HALL_ANGLE_DETECTION:		*pDescription = "Hall Angle Detection Error"; oResult = TRUE; break;
		case ERROR_EPOS_SOFTWARE_POSITION_LIMIT:    *pDescription = "Software Position Limit Error"; oResult = TRUE; break;
		case ERROR_EPOS_POSITION_SENSOR_BREACH:		*pDescription = "Position Sensor Breach"; oResult = TRUE; break;
		case ERROR_EPOS_SYSTEM_OVERLOADED:			*pDescription = "System Overloaded"; oResult = TRUE; break;
    }

    return oResult;
}

BOOL CDeviceErrorHandling::GetErrorDescription_EPOS2(WORD usErrorCode, CStdString *pDescription)
{
    BOOL oResult(FALSE);

	switch(usErrorCode)
    {
        case ERROR_EPOS2_GENERIC:								*pDescription = "Generic Error"; oResult = TRUE; break;
		case ERROR_EPOS2_OVER_CURRENT:							*pDescription = "Over Current"; oResult = TRUE; break;
		case ERROR_EPOS2_OVER_VOLTAGE:							*pDescription = "Over Voltage"; oResult = TRUE; break;
		case ERROR_EPOS2_UNDER_VOLTAGE:							*pDescription = "Under Voltage"; oResult = TRUE; break;
		case ERROR_EPOS2_OVER_TEMPERATURE:						*pDescription = "Over Temperature"; oResult = TRUE; break;
		case ERROR_EPOS2_SUPPLY_VOLTAGE_TOO_LOW:				*pDescription = "Supply Voltage is too low"; oResult = TRUE; break;
		case ERROR_EPOS2_SUPPLY_VOLTAGE_OUTPUT_STAGE_TOO_LOW:	*pDescription = "Supply Voltage Output Stage is too low"; oResult = TRUE; break;
		case ERROR_EPOS2_INTERNAL_SOFTWARE:						*pDescription = "Internal Software Error"; oResult = TRUE; break;
		case ERROR_EPOS2_SOFTWARE_PARAMETER:					*pDescription = "Software Parameter Error"; oResult = TRUE; break;
		case ERROR_EPOS2_SENSOR_POSITION:						*pDescription = "Sensor Position Error"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_OVERRUN_OBJECT_LOST:				*pDescription = "CAN Overrun (Objects lost)"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_OVERRUN:							*pDescription = "CAN Overrun"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_PASSIVE:							*pDescription = "CAN Passive Mode"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_LIFE_GUARD:						*pDescription = "CAN Lifeguard Error"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_TX_COB_ID_COLLISION:				*pDescription = "CAN Transmit COB-ID Collision"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_BUS_OFF:							*pDescription = "CAN Bus Off"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_RX_QUEUE_OVERRUN:					*pDescription = "CAN Rx Queue Overrun"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_TX_QUEUE_OVERRUN:					*pDescription = "CAN Tx Queue Overrun"; oResult = TRUE; break;
		case ERROR_EPOS2_CAN_PDO_LENGTH:						*pDescription = "CAN PDO Length Error"; oResult = TRUE; break;
		case ERROR_EPOS2_FOLLOWING_ERROR:						*pDescription = "Following Error"; oResult = TRUE; break;
		case ERROR_EPOS2_HALL_SENSOR:							*pDescription = "Hall Sensor Error"; oResult = TRUE; break;
		case ERROR_EPOS2_INDEX_PROCESSING:						*pDescription = "Index Processing Error"; oResult = TRUE; break;
		case ERROR_EPOS2_ENCODER_RESOLUTION:					*pDescription = "Encoder Resolution Error"; oResult = TRUE; break;
		case ERROR_EPOS2_HALL_SENSOR_NOT_FOUND:					*pDescription = "Hallsensor not found"; oResult = TRUE; break;
		case ERROR_EPOS2_NEGATIVE_LIMIT:						*pDescription = "Negative Limit Switch Error"; oResult = TRUE; break;
		case ERROR_EPOS2_POSITIVE_LIMIT:						*pDescription = "Positive Limit Switch Error"; oResult = TRUE; break;
		case ERROR_EPOS2_HALL_ANGLE_DETECTION:					*pDescription = "Hall Angle Detection Error"; oResult = TRUE; break;
		case ERROR_EPOS2_SOFTWARE_POSITION_LIMIT:				*pDescription = "Software Position Limit Error"; oResult = TRUE; break;
		case ERROR_EPOS2_POSITION_SENSOR_BREACH:				*pDescription = "Position Sensor Breach"; oResult = TRUE; break;
		case ERROR_EPOS2_SYSTEM_OVERLOADED:						*pDescription = "System Overloaded"; oResult = TRUE; break;
		case ERROR_EPOS2_IPM_MODE:								*pDescription = "Interpolated Position Mode Error"; oResult = TRUE; break;
		case ERROR_EPOS2_AUTOTUNING_IDENTIFICATION:				*pDescription = "Auto Tuning Identification Error"; oResult = TRUE; break;
    }

    return oResult;
}

BOOL CDeviceErrorHandling::GetErrorDescription_EPOS_P(WORD usErrorCode, CStdString *pDescription)
{
    BOOL oResult(FALSE);

	switch(usErrorCode)
    {
        case ERROR_EPOS_P_EEPROM:								*pDescription = "Hardware Error by accessing nonvolatile memory"; oResult = TRUE; break;
		case ERROR_EPOS_P_INTERPRETER:							*pDescription = "Error occurred at interpreting code"; oResult = TRUE; break;
		case ERROR_EPOS_P_NATIVE_CODE:							*pDescription = "Error occured at executing native code"; oResult = TRUE; break;
		case ERROR_EPOS_P_TIMER_TASK:							*pDescription = "Error occured at starting a timer task"; oResult = TRUE; break;
		case ERROR_EPOS_P_INTERRUPT_TASK:						*pDescription = "Error occured at starting interrupt/event task"; oResult = TRUE; break;
		case ERROR_EPOS_P_DATA_ACCESS:							*pDescription = "Data or program access exception detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_RETAIN_VARIABLE:						*pDescription = "Error occured at saving or restoring retain variables"; oResult = TRUE; break;
		case ERROR_EPOS_P_DEVICE_TYPE:							*pDescription = "Failed network bootup! Bad device type detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_VENDOR_ID:							*pDescription = "Failed network bootup! Bad vendor-Id detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_PRODUCT_CODE:							*pDescription = "Failed network bootup! Bad product code detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_REVISION_NUMBER:						*pDescription = "Failed network bootup! Bad revision number detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_SERIAL_NUMBER:						*pDescription = "Failed network bootup! Bad serial number detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_CONFIGURATION_TIME_DATE:				*pDescription = "Failed network bootup! Bad configuration date or time detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_ERROR_CONTROL:						*pDescription = "Failed network bootup! No heartbeat received"; oResult = TRUE; break;
		case ERROR_EPOS_P_NODE_IS_NOT_SLAVE:					*pDescription = "Failed network bootup! Mandatory Node is not defined as slave"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_OVERRUN:							*pDescription = "CAN Overrun"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_PASSIVE:							*pDescription = "CAN Passive Mode"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_LIFQGUARD:						*pDescription = "CAN Lifeguard Error"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_CONTROLLER:						*pDescription = "Error occured at configuring CAN controller"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_NO_ANSWER:						*pDescription = "Configured Node does not answer or is not present"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_WARNING:							*pDescription = "CAN Frame Error warning level detected"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_BUS_OFF:							*pDescription = "CAN Bus Off Error"; oResult = TRUE; break;
		case ERROR_EPOS_P_CAN_PDO_LENGTH:						*pDescription = "CAN PDO Length Error"; oResult = TRUE; break;
    }

    return oResult;
}

BOOL CDeviceErrorHandling::GetErrorDescription_EPOS2_P(WORD usErrorCode, CStdString *pDescription)
{
    BOOL oResult(FALSE);

	switch(usErrorCode)
    {
        case ERROR_EPOS2_P_EEPROM:								*pDescription = "Hardware Error by accessing nonvolatile memory"; oResult = TRUE; break;
		case ERROR_EPOS2_P_INTERNAL_SOFTWARE:					*pDescription = "Internal Software error occurred"; oResult = TRUE; break;
		case ERROR_EPOS2_P_INTERPRETER:							*pDescription = "Error occurred at interpreting code"; oResult = TRUE; break;
		case ERROR_EPOS2_P_NATIVE_CODE:							*pDescription = "Error occured at executing native code"; oResult = TRUE; break;
		case ERROR_EPOS2_P_TIMER_TASK:							*pDescription = "Error occured at starting a timer task"; oResult = TRUE; break;
		case ERROR_EPOS2_P_INTERRUPT_TASK:						*pDescription = "Error occured at starting interrupt/event task"; oResult = TRUE; break;
		case ERROR_EPOS2_P_DATA_ACCESS:							*pDescription = "Data or program access exception detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_STACK_OVERFLOW:						*pDescription = "Stack overflow in application task detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_RETAIN_VARIABLE:						*pDescription = "Error occured at saving or restoring retain variables"; oResult = TRUE; break;
		case ERROR_EPOS2_P_DEVICE_TYPE:							*pDescription = "Failed network bootup! Bad device type detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_VENDOR_ID:							*pDescription = "Failed network bootup! Bad vendor-Id detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_PRODUCT_CODE:						*pDescription = "Failed network bootup! Bad product code detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_REVISION_NUMBER:						*pDescription = "Failed network bootup! Bad revision number detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_SERIAL_NUMBER:						*pDescription = "Failed network bootup! Bad serial number detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CONFIGURATION_TIME_DATE:				*pDescription = "Failed network bootup! Bad configuration date or time detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_ERROR_CONTROL:						*pDescription = "Failed network bootup! No heartbeat received"; oResult = TRUE; break;
		case ERROR_EPOS2_P_NODE_IS_NOT_SLAVE:					*pDescription = "Failed network bootup! Mandatory Node is not defined as slave"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_OVERRUN:							*pDescription = "CAN Overrun"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_PASSIVE:							*pDescription = "CAN Passive Mode"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_LIFQGUARD:						*pDescription = "CAN Lifeguard Error"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_CONTROLLER:						*pDescription = "Error occured at configuring CAN controller"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_NO_ANSWER:						*pDescription = "Configured Node does not answer or is not present"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_WARNING:							*pDescription = "CAN Frame Error warning level detected"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_BUS_OFF:							*pDescription = "CAN Bus Off Error"; oResult = TRUE; break;
		case ERROR_EPOS2_P_CAN_PDO_LENGTH:						*pDescription = "CAN PDO Length Error"; oResult = TRUE; break;
		case ERROR_EPOS2_P_SERIAL_OVERRUN:						*pDescription = "Serial Communication interface buffer overflow"; oResult = TRUE; break;
    }

    return oResult;
}