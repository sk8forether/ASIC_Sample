/******************************************************************************

    File:   ASIC.h
    Desc:   ASIC is a component in Level 2 of the design that controls all 
            interaction with any ASIC of any type, and thus is generic.  Unique
            ASIC classes inherit this class.

******************************************************************************/
#ifndef _ASIC_H_
#define _ASIC_H_

#include "DeviceCore.h"

//-----------------------------------------------------------------------------
//  ASIC class definition
class CASIC : public CDeviceCore
{
protected:
    CASIC(void);
    static CASIC* Instance;
    
    byte ROMtype;
    int HasDigOffCorr;
    
    void SetDeviceTypes(void);
    void SetCommTypes(void);

public:
    ~CASIC(void);
    static CASIC* GetInstance(void);
    String Name;
    
    void Init(char* DefaultImg, char* DefaultMask, word* listDut);
    byte GetDeviceTypes(void) { return this->device_types; }
    
    void SetState(int state, word* listDut);
    
    void GetResponse(byte* output, word* listDut);
    
    void RegressionTestForASIC(int mode, word* listDut);
};

#endif