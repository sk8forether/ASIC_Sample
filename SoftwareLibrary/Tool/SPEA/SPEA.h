/******************************************************************************

    File:   SPEA.h
    Desc:   SPEA is a component in Level 2 of the design that controls all
            the interaction with the physical SPEA tool that contains and
            tests DUTs.  The SPEA tools include 6, 7, and 10-series.  For
            more information on the SPEA tools, see the wiki page:
            https://kionix.sharepoint.com/sites/swdevtest/tools/spea

******************************************************************************/
#include "ToolInfo.h"

#ifdef _SPEA_
#ifndef _SPEA_H_
#define _SPEA_H_

#include "Tool.h"

//-----------------------------------------------------------------------------
//  SPEA class definition
class CSPEA : public CTool
{
private:
    CSPEA(void);
    static CSPEA* Instance;

public:
    ~CSPEA(void);
    static CSPEA* GetInstance(void);
    
    int Init(PinUsageStruct pinUsage, int series, int unit, word* listDut);
    //
};

#endif
#endif