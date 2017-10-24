/******************************************************************************

    File:   Tool.h
    Desc:   Tool is a component in Level 2 of the design that controls all the
            interaction with the physical tool that contains and tests DUTs.
            This class is generalized to work with any tool, and the specific
            tool-types (ISMECA, SPEA, NI, TESEC, etc.) expand on the
            functionality of this class.  For more information on tools or
            tool-types, see https://kionix.sharepoint.com/sites/swdevtest/tools

******************************************************************************/
#ifndef _TOOL_H_
#define _TOOL_H_

#include "Defines.h"
#include "Comm.h"
#include "PowerSupply.h"

//-----------------------------------------------------------------------------
//  Tool class definition
class CTool : public Comm
{
protected:
    CTool(void);
    static CTool* Instance;
    
    int series;
    int unit;
    
    double op_vdd;
    int burn_with_vpp;

private:
    String toolName;

public:
    ~CTool(void);
    static CTool* GetInstance(void);
    
    tool_type type;
    CPowerSupply* PowerSupply;
    
    int Init(PinUsageStruct pinUsage, int series, int unit, tool_type type, int burn_with_vpp, word* listDut);
    
    double GetToolName(void);
    void GetToolName(char* toolName);
    int GetSeries(void);
    int GetUnit(void);
    
    double GetOpVDD(void) { return this->op_vdd; }
    void SetOpVDD(double new_vdd) { this->op_vdd = new_vdd; }
    
    // These functions are implemented by ISMECA, which inherits Tool
    virtual int PowerOnPart(double vdd, word* listDut) { return ERROR_UNIMPLEMENTED; };
    virtual int PowerOffPart(word* listDut) { return ERROR_UNIMPLEMENTED; };
    virtual int SetBurnVoltage(double vdd) { return ERROR_UNIMPLEMENTED; };
    virtual int GetTemperature(double* temp, word* listDut) { return ERROR_UNIMPLEMENTED; };
    virtual int Close(void) { return ERROR_UNIMPLEMENTED; };
};

#endif