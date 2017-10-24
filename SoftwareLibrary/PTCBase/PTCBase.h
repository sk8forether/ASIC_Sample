/******************************************************************************

    File:   PTCBase.h
    Desc:   PTCBase is a subcomponent in Level 3 of the design that contains
            the generic functionality necessary for the Product Test Control 
            class for any Product Solution.

******************************************************************************/
#ifndef _PTC_BASE_H_
#define _PTC_BASE_H_

#include "Defines.h"

#include "ASIC.h"

#include "ISMECA.h"
#include "SPEA.h"

// TESTING
#include "TestingSpec.h"

//-----------------------------------------------------------------------------
//  PTCBase class definition
class PTCBase
{
protected:
    PTCBase(void);
    static PTCBase* Instance;
    
    bool TestModuleDebug;
    
    word listDut[TOOL_MAX_DUT + 1];
    CASIC *ASIC;
    CTool *Tool;
    
    // Test module removed
    
    // Tool info
    int series;
    int unit;
    char toolName;
    double toolNameD;
    
    PTCInfoStruct PTCInfo;
    
    void Init(void);
    void CheckSpec(void);
    int Init_listDut(void);
    int Init_Generic_Tool(void);
    int Init_Test(void);
    
    // redefine in PTC
    virtual int Init_ASIC(void) { return ERROR_INIT; }
    virtual int Init_Tool(void) { return ERROR_INIT; }
    virtual int Add_Tests(void) { return ERROR_INIT; }
    
    String DisplayProductToTest(void);
    
public:
    ~PTCBase(void);
    static PTCBase* GetInstance(void);
};

#endif