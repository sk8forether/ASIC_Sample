/******************************************************************************

    File:   SPEA.cpp
    Desc:   SPEA is a component in Level 2 of the design that controls all
            the interaction with the physical SPEA tool that contains and
            tests DUTs.  The SPEA tools include 6, 7, and 10-series.  For
            more information on the SPEA tools, see the wiki page:
            https://kionix.sharepoint.com/sites/swdevtest/tools/spea

******************************************************************************/
#include "ToolInfo.h"

#ifdef _SPEA_
#include "SPEA.h"

CSPEA *CSPEA::Instance = NULL;

/******************************************************************************
    Name:   CSPEA
    Desc:   Default constructor
******************************************************************************/
CSPEA::CSPEA()
{
}

/******************************************************************************
    Name:   ~CSPEA
    Desc:   Default destructor
******************************************************************************/
CSPEA::~CSPEA(void)
{
    // 
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
CSPEA *CSPEA::GetInstance(void)
{
    if (CSPEA::Instance == NULL)
        CSPEA::Instance = new CSPEA();

    return CSPEA::Instance;
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the SPEA class and subclasses
******************************************************************************/
int CSPEA::Init(PinUsageStruct pinUsage, int series, int unit, word* listDut)
{
    DBGTrace("--> CSPEA::Init");

    CTool::Init(pinUsage, series, unit, spea_type, listDut);
    DBGPrint("\tInitializing the SPEA module");

    return SUCCESS;
}

#endif