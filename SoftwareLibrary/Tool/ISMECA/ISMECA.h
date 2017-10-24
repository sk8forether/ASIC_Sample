/******************************************************************************

    File:   ISMECA.h
    Desc:   ISMECA is a component in Level 2 of the design that controls all
            the interaction with the physical ISMECA tool that contains and
            tests DUTs.  The ISMECA tools include 8, 9, and 13-series.  For
            more information on the ISMECA tools, see the wiki page:
            https://kionix.sharepoint.com/sites/swdevtest/tools/ismeca

******************************************************************************/
#ifndef _ISMECA_H_
#define _ISMECA_H_

#include "Defines.h"
#include "Tool.h"

#ifdef _ISMECA_

//-----------------------------------------------------------------------------
//  ISMECA class definition
class CISMECA : public CTool
{
private:
    CISMECA(void);
    static CISMECA* Instance;

public:
    ~CISMECA(void);
    static CISMECA* GetInstance(void);
    
    int Init(PinUsageStruct pinUsage, int series, int unit, int uses_VPP, word* listDut);
    int Close(void);
    
    int PowerOnPart(double vdd, word* listDut);
    int PowerOffPart(word* listDut);
    int SetBurnVoltage(double vdd);
    
    //DCPowerCard PSSessions[TOOL_MAX_PWRSUPP];
    //HSDIOCard CommGroup[TOOL_MAX_COM_GROUP]; // For all duts on the same HSDIO card
    #ifdef _9SERIES_x12_
        DAQmxCard LSComm[TOOL_MAX_LSCOMM];
    #endif
    DAQmxCard DutCntl[TOOL_MAX_DUT]; // 4 Dut control pins for each site
    DAQmxCard AnalogIn[TOOL_MAX_DUT]; // 4 analog in pins for each site (only 1 for LS)
};

#endif

#endif