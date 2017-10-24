/******************************************************************************

    File:   ToolInfo.h
    Desc:   Contains specific tool configuration information needed by other modules
    (Like the TOOL_MAX_DUT (number of sites), needed by ASIC to dimensions data arrays)

******************************************************************************/
#ifndef _TOOL_INFO_H_
#define _TOOL_INFO_H_

#include "PTCSpecific.h"

//-----------------------------------------------------------------------------
#ifdef _ISMECA_4_
    #define _ISMECA_
    #define TOOL_SERIES         1
    #define TOOL_MAX_DUT        12
#endif

#ifdef _ISMECA_
    #define INVALID_COM_GROUP   255
    #define TOOL_RESET_DELAY    40
    #define TOOL_BURN_DELAY     20
    #define TOOL_MAX_RELAY      4
    #define TOOL_IFC_BD_VDD     2.6        // tool interface board vdd
    #define TOOL_MAX_VDD        5
    #define TOOL_NUM_PATT_HS_LINES 16      // max number of lines to show on txtScope
#endif

//-----------------------------------------------------------------------------
#endif