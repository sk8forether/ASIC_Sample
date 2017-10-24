/******************************************************************************

    File:   PTC.h
    Desc:   The brains of the operation

******************************************************************************/
#ifndef _PTC_H_
#define _PTC_H_

// always include
#include "PTCBase.h"

// product specific
#include "BF.h"

//-----------------------------------------------------------------------------
//  PTC class definition
class PTC : public PTCBase
{
private:
    static PTC* Instance;
    PTC(void);
    
    int Init_ASIC(void);
    int Init_Tool(void);
    int Add_Tests(void);
    
    CISMECA* ISMECA;
    
public:
    ~PTC(void);
    static PTC* GetInstance(void);
    
    void Init(void);
    int Main(void);
    int Run_Tests(void);
    
    int Currently_Testing(void);
};

#endif
