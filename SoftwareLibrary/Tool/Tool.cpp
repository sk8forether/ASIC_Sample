/******************************************************************************

    File:   Tool.cpp
    Desc:   Tool is a component in Level 2 of the design that controls all the
            interaction with the physical tool that contains and tests DUTs.
            This class is generalized to work with any tool, and the specific
            tool-types (ISMECA, SPEA, NI, TESEC, etc.) expand on the
            functionality of this class.  For more information on tools or
            tool-types, see https://kionix.sharepoint.com/sites/swdevtest/tools

******************************************************************************/
#include "Tool.h"

CTool *CTool::Instance = NULL;

/******************************************************************************
    Name:   CTool
    Desc:   Default constructor
******************************************************************************/
CTool::CTool()
{
    this->PowerSupply = CPowerSupply::GetInstance();
    this->series = 0;
    this->unit = 0;
    this->op_vdd = 0.0;
}

/******************************************************************************
    Name:   ~CTool
    Desc:   Default destructor
******************************************************************************/
CTool::~CTool(void)
{
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
CTool *CTool::GetInstance(void)
{
    if (CTool::Instance == NULL)
        CTool::Instance = new CTool();
    
    return CTool::Instance;
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the Tool class and subclasses
******************************************************************************/
int CTool::Init(PinUsageStruct pinUsage, int series, int unit, tool_type type, int burn_with_vpp, word* listDut)
{
    DBGTrace("--> CTool::Init");
    
    DBGPrint("\tInitializing the Tool module");
    
    if (series != TOOL_SERIES)
        CUtilities::Error.Add("The Tool series you are trying to initialize does not match the Tool you are on.");
    
    this->series = series;
    this->unit = unit;
    this->type = type;
    this->burn_with_vpp = burn_with_vpp;
    
    String msg;
    sprintf(msg, "\tTool series: %i, unit: %i", series, unit);
    DBGPrint(msg);
    
    this->GetToolName(this->toolName);
    
    Comm::Init(pinUsage, listDut);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   GetToolName
    Desc:   
******************************************************************************/
void CTool::GetToolName(char* toolName)
{
    sprintf(toolName, "%i.%02i", series, unit);
}

/******************************************************************************
    Name:   GetToolName
    Desc:   
******************************************************************************/
double CTool::GetToolName(void)
{
    return atof(this->toolName);
}

/******************************************************************************
    Name:   GetSeries
    Desc:   
******************************************************************************/
int CTool::GetSeries(void)
{
    return this->series;
}

/******************************************************************************
    Name:   GetUnit
    Desc:   
******************************************************************************/
int CTool::GetUnit(void)
{
    return this->unit;
}
