/////////////////////////////////////////////////////////////////////////
// DcsCanOpenCommunicationDef.h
//
// Project:             Dtm
// Author:              Christian Herger
// Date of Creation:    17.12.2007
//
// Contains all Communication related CANopen definitions for the DCS.
//
// Copyright © 2007, maxon motor ag.
// All rights reserved.
/////////////////////////////////////////////////////////////////////////

#if !defined(DcsCanOpenCommunicationDef_H__20071217__INCLUDED_)
#define DcsCanOpenCommunicationDef_H__20071217__INCLUDED_

//CommunicationModel(GatewayVCStoEpos)
enum ENmtServiceIdentifier
{
	NSI_START_REMOTE_NODE           = 1,
	NSI_STOP_REMOTE_NODE            = 2,
	NSI_ENTER_PRE_OPERATIONAL       = 128,
	NSI_RESET_NODE                  = 129,
	NSI_RESET_COMMUNICATION         = 130

};

enum ERequestNmtWriteAccess
{
	RNW_REQUEST_NMT_STOP					= 0x04,
	RNW_REQUEST_NMT_START					= 0x05,
	RNW_REQUEST_NMT_RESET					= 0x06,
	RNW_REQUEST_NMT_RESET_COMMUNICATION     = 0x07,
	RNW_REQUEST_NMT_ENTER_PRE_OPERATIONAL   = 0x7F
};

//Definition of mandatory communication objects (index/subindex)
const WORD		INDEX_DEVICE_TYPE								= 0x1000;
const BYTE		SUBINDEX_DEVICE_TYPE							= 0x00;

const WORD		INDEX_ERROR_REGISTER							= 0x1001;
const BYTE		SUBINDEX_ERROR_REGISTER							= 0x00;

const WORD		INDEX_MANUFACTURER_ERROR_REGISTER				= 0x1002;
const BYTE		SUBINDEX_MANUFACTURER_ERROR_REGISTER			= 0x00;

const WORD		INDEX_ERROR_HISTORY								= 0x1003;
const BYTE		SUBINDEX_NB_OF_ERRORS							= 0x00;
const BYTE		SUBINDEX_ERROR_HISTORY1							= 0x01;
const BYTE		SUBINDEX_ERROR_HISTORY2							= 0x02;
const BYTE		SUBINDEX_ERROR_HISTORY3							= 0x03;
const BYTE		SUBINDEX_ERROR_HISTORY4							= 0x04;
const BYTE		SUBINDEX_ERROR_HISTORY5							= 0x05;

const WORD		INDEX_COB_ID_SYNC_MESSAGE						= 0x1005;
const BYTE		SUBINDEX_COB_ID_SYNC_MESSAGE					= 0x00;

const WORD		INDEX_COMM_CYCLE_TIME							= 0x1006;
const BYTE		SUBINDEX_COMM_CYCLE_TIME						= 0x00;

const WORD		INDEX_SYNCH_WINDOW_LENGTH						= 0x1007;
const BYTE		SUBINDEX_SYNCH_WINDOW_LENGTH					= 0x00;

const WORD		INDEX_DEVICE_NAME								= 0x1008;
const BYTE		SUBINDEX_DEVICE_NAME							= 0x00;

const WORD		INDEX_GUARD_TIME								= 0x100C;
const BYTE		SUBINDEX_GUARD_TIME								= 0x00;

const WORD		INDEX_LIFE_TIME_FACTOR							= 0x100D;
const BYTE		SUBINDEX_LIFE_TIME_FACTOR						= 0x00;

const WORD		INDEX_STORE_PARAMETERS							= 0x1010;
const BYTE		SUBINDEX_SAVE_ALL_PARAMETERS					= 0x01;

const WORD      INDEX_RESTORE_DEFAULT_PARAMETERS				= 0x1011;
const BYTE		SUBINDEX_RESTORE_ALL_DEFAULT_PARAMETERS			= 0x01;
const BYTE		SUBINDEX_RESTORE_FACTORY_DEFAULT_PARAMETERS		= 0x04;
const BYTE		SUBINDEX_RESTORE_DEFAULT_PDO_IDS				= 0x05;

const WORD      INDEX_CONSUMER_HEARTBEAT_TIME					= 0x1016;
const BYTE		SUBINDEX_CONSUMER1_HEARTBEAT_TIME				= 0x01;
const BYTE		SUBINDEX_CONSUMER2_HEARTBEAT_TIME				= 0x02;
const BYTE		SUBINDEX_CONSUMER127_HEARTBEAT_TIME				= 0x7F;

const WORD      INDEX_PRODUCER_HEARTBEAT_TIME					= 0x1017;
const BYTE		SUBINDEX_PRODUCER_HEARTBEAT_TIME				= 0x00;

const WORD      INDEX_DEVICE_IDENTIFICATION						= 0x1018;
const BYTE		SUBINDEX_VENDOR_ID								= 0x01;
const BYTE		SUBINDEX_PRODUCT_CODE							= 0x02;
const BYTE		SUBINDEX_REVISION_NO							= 0x03;
const BYTE		SUBINDEX_SERIAL_NO								= 0x04;

const WORD      INDEX_SYNC_COUNTER								= 0x1019;
const BYTE		SUBINDEX_SYNC_COUNTER							= 0x00;

const WORD      INDEX_VERIFY_CONFIG								= 0x1020;
const BYTE		SUBINDEX_CONFIG_DATE							= 0x01;
const BYTE		SUBINDEX_CONFIG_TIME							= 0x02;

const WORD      INDEX_SERVER_SDO_PARAMETER						= 0x1200;
const WORD      INDEX_CLIENT_SDO_PARAMETER						= 0x1280;
const BYTE		SUBINDEX_SDO_NUMBER_OF_ENTRIES					= 0x00;
const BYTE		SUBINDEX_SDO_COB_ID_SERVER_TO_CLIENT			= 0x01;
const BYTE		SUBINDEX_SDO_COB_ID_CLIENT_TO_SERVER			= 0x02;
const BYTE      SUBINDEX_SDO_NODE_ID							= 0x03;

const WORD      INDEX_SDO_CLIENT_PARAMETER_MAX					= 0x129F;

const WORD      INDEX_RX_PDO1_PARAMETER							= 0x1400;
const BYTE		SUBINDEX_COB_ID_RX_PDO							= 0x01;
const BYTE		SUBINDEX_TRANSMISSION_TYPE_RX_PDO				= 0x02;

const WORD      INDEX_RX_PDO_001_PARAMETER						= 0x1500;

const WORD		INDEX_RX_PDO_004_PARAMETER						= 0x1503;

const WORD      INDEX_RX_PDO_512_PARAMETER						= 0x15FF;

const WORD      INDEX_RX_PDO1_MAPPING							= 0x1600;
const BYTE		SUBINDEX_RX_PDO1_MAPPING						= 0x00;

const WORD      INDEX_RX_PDO_001_MAPPING						= 0x1700;

const WORD      INDEX_RX_PDO_004_MAPPING						= 0x1703;

const WORD      INDEX_RX_PDO_512_MAPPING						= 0x17FF;

const WORD      INDEX_TX_PDO1_PARAMETER							= 0x1800;
const BYTE		SUBINDEX_COB_ID_TX_PDO							= 0x01;
const BYTE		SUBINDEX_TRANSMISSION_TYPE_TX_PDO				= 0x02;
const BYTE		SUBINDEX_INHIBIT_TIME							= 0x03;
const BYTE		SUBINDEX_EVENT_TIMER							= 0x05;

const WORD      INDEX_TX_PDO_001_PARAMETER						= 0x1900;

const WORD      INDEX_TX_PDO_004_PARAMETER						= 0x1903;

const WORD      INDEX_TX_PDO_512_PARAMETER						= 0x19FF;

const WORD      INDEX_TX_PDO1_MAPPING							= 0x1A00;
const BYTE		SUBINDEX_TX_PDO1_MAPPING						= 0x00;
const BYTE		SUBINDEX_TX_PDO_FIRST_MAPPED_OBJECT				= 0x01;

const WORD      INDEX_TX_PDO_001_MAPPING						= 0x1B00;

const WORD      INDEX_TX_PDO_004_MAPPING						= 0x1B03;

const WORD      INDEX_TX_PDO_512_MAPPING						= 0x1BFF;

const WORD      INDEX_EXPECTED_CONFIG_DATE						= 0x1F26;

const WORD      INDEX_EXPECTED_CONFIG_TIME						= 0x1F27;

const WORD      INDEX_REMOTE_NETWORK_ROUTING_LIST               = 0x1F2C;
const BYTE      SUBINDEX_REMOTE_NETWORK_ROUTING                 = 0x00;

const WORD      INDEX_NMT_STARTUP								= 0x1F80;
const BYTE		SUBINDEX_NMT_STARTUP							= 0x00;

const WORD      INDEX_SLAVE_ASSIGNEMENT							= 0x1F81;
const BYTE		SUBINDEX_SLAVE_ASSIGNEMENT						= 0x00;

const WORD      INDEX_REQUEST_NMT								= 0x1F82;
const BYTE		SUBINDEX_REQUEST_NMT_NODE1						= 0x01;
const BYTE		SUBINDEX_REQUEST_NMT_ALL_SLAVES					= 0x80;

const WORD      INDEX_EXPECTED_DEVICE_TYPE						= 0x1F84;

const WORD      INDEX_EXPECTED_VENDOR_ID						= 0x1F85;

const WORD      INDEX_EXPECTED_PRODUCT_CODE						= 0x1F86;

const WORD      INDEX_EXPECTED_REVISION_NO						= 0x1F87;

const WORD      INDEX_EXPECTED_SERIAL_NO						= 0x1F88;

const WORD      INDEX_BOOT_TIME									= 0x1F89;
const BYTE		SUBINDEX_BOOT_TIME								= 0x00;

const WORD      INDEX_RECORDER_CONTROL                          = 0x2010;
const BYTE      SUBINDEX_RECORDER_CONTROL                       = 0x00;

const WORD      INDEX_RECORDER_CONFIGURATION                    = 0x2011;
const BYTE      SUBINDEX_RECORDER_CONFIGURATION                 = 0x00;

const WORD      INDEX_SAMPLING_PERIOD                           = 0x2012;
const BYTE      SUBINDEX_SAMPLING_PERIOD                        = 0x00;

const WORD      INDEX_NB_OF_PRECEDING_SAMPLES                   = 0x2013;
const BYTE      SUBINDEX_NB_OF_PRECEDING_SAMPLES                = 0x00;

const WORD      INDEX_NB_OF_SAMPLING_VARIABLES                  = 0x2014;
const BYTE      SUBINDEX_NB_OF_SAMPLING_VARIABLES               = 0x00;

const WORD      INDEX_VARIABLE_INDEX                            = 0x2015;
const BYTE      SUBINDEX_VARIABLE_INDEX                         = 0x01;

const WORD      INDEX_VARIABLE_SUB_INDEX                        = 0x2016;
const BYTE      SUBINDEX_VARIABLE_SUB_INDEX                     = 0x01;

const WORD      INDEX_RECORDER_STATUS                           = 0x2017;
const BYTE      SUBINDEX_RECORDER_STATUS                        = 0x00;

const WORD      INDEX_MAX_NB_OF_SAMPLES                         = 0x2018;
const BYTE      SUBINDEX_MAX_NB_OF_SAMPLES                      = 0x00;

const WORD      INDEX_NB_OF_RECORDED_SAMPLES                    = 0x2019;
const BYTE      SUBINDEX_NB_OF_RECORDED_SAMPLES                 = 0x00;

const WORD      INDEX_VECTOR_START_OFFSET                       = 0x201A;
const BYTE      SUBINDEX_VECTOR_START_OFFSET                    = 0x00;

const WORD      INDEX_DATA_BUFFER                               = 0x201B;
const BYTE      SUBINDEX_DATA_BUFFER                            = 0x00;

#endif // !defined(DcsCanOpenCommunicationDef_H__20071217__INCLUDED_)
