/////////////////////////////////////////////////////////////////////////
// DcsCanOpenPlcDef.h
//
// Project:             Dtm
// Author:              Christian Herger
// Date of Creation:    17.12.2007
//
// Contains all PLC related CANopen definitions for the DCS.
//
// Copyright © 2007, maxon motor ag.
// All rights reserved.
/////////////////////////////////////////////////////////////////////////

#if !defined(DcsCanOpenPlcDef_H__20071217__INCLUDED_)
#define DcsCanOpenPlcDef_H__20071217__INCLUDED_

//Run commands enumeration
enum EDeviceControl
{
	DC_STOP             = 0x00,
	DC_COLD_START       = 0x01,
	DC_WARM_START       = 0x02,
	DC_HOT_START        = 0x03,
	DC_SINGLE_CYCLE     = 0x04,
	DC_CLEAR_SYSTEM     = 0x05,
	DC_RESTORE_DS       = 0xFF

};
//Definition of mandatory PLC objects (index/subindex)
const WORD		INDEX_PROCESS_INPUT_INT8            = 0xA000;

const WORD		INDEX_PROCESS_INPUT_INT16           = 0xA0C0;

const WORD		INDEX_PROCESS_INPUT_INT32           = 0xA1C0;

const WORD		INDEX_PROCESS_INPUT_INT64           = 0xA400;

const WORD		INDEX_PROCESS_INPUT_UINT8           = 0xA040;

const WORD		INDEX_PROCESS_INPUT_UINT16          = 0xA100;

const WORD		INDEX_PROCESS_INPUT_UINT32          = 0xA200;

const WORD		INDEX_PROCESS_INPUT_UINT64          = 0xA440;

const WORD		INDEX_PROCESS_INPUT_END             = 0xA47F;

const WORD		INDEX_PROCESS_OUTPUT_INT8           = 0xA480;

const WORD		INDEX_PROCESS_OUTPUT_INT16          = 0xA540;

const WORD		INDEX_PROCESS_OUTPUT_INT32          = 0xA640;

const WORD		INDEX_PROCESS_OUTPUT_INT64          = 0xA880;

const WORD		INDEX_PROCESS_OUTPUT_UINT8          = 0xA4C0;

const WORD		INDEX_PROCESS_OUTPUT_UINT16         = 0xA580;

const WORD		INDEX_PROCESS_OUTPUT_UINT32         = 0xA680;

const WORD		INDEX_PROCESS_OUTPUT_UINT64         = 0xA8C0;

const WORD		INDEX_PROCESS_OUTPUT_END            = 0xA8FF;

const WORD		INDEX_TX_PDO1_DEFAULT_VALUE         = 0xA580;
const BYTE		SUBINDEX_TX_PDO1_DEFAULT_VALUE      = 0x01;

#endif // !defined(DcsCanOpenPlcDef_H__20071217__INCLUDED_)
