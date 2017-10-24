/******************************************************************************

    File:   BF.h
    Desc:   The BF ASIC is an accelerometer and pedometer with 3 pages.
            Page 0x00 contains the Volatile registers, Page 0x01 contains the
            RAM, and Page 0x02 contains the ROM (OTP).

******************************************************************************/
#ifndef _BF_H_
#define _BF_H_

#include "ASIC.h"

//-----------------------------------------------------------------------------
//  ASIC class definition
class CBF : public CASIC
{
private:
    static CBF* Instance;
    CBF(void);
    
    void SetupRegisterMap(void);
    void SetupPage0(void);
    void SetupPage1(void);
    void SetupPage2(void);

public:
    ~CBF(void);
    static CBF* GetInstance(void);
    
    void Init(String DefaultImg, String DefaultMask, CTool* Tool, word* listDut);
};

#endif