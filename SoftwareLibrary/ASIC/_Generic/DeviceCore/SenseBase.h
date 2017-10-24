/******************************************************************************

    File:   SenseBase.h
    Desc:   SenseBase is a subcomponent in Level 1 of the design that
            contains some commonly used sense element functionality

******************************************************************************/
#ifndef _SENSE_BASE_H_
#define _SENSE_BASE_H_

#include "DeviceBase.h"

//-----------------------------------------------------------------------------
//  SenseBase class definition
class CSenseBase : public CDeviceBase
{
protected:
    double CurrODR;
    
    //void ReadOutputs(ASICregister reg, int output_type);  // TODO: determine inputs
    //void Decode(byte* raw, double* out, word* listDut);

public:
    CSenseBase(void);
    ~CSenseBase(void);
    
    void SampleOutputs(int count, int timeout, double* avg, double* st_dev, int output_type, word* listDut);
};

#endif