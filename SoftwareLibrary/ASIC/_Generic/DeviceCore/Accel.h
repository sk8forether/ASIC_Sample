/******************************************************************************

    File:   Accel.h
    Desc:   Accel is a subcomponent in Level 1 of the design that contains
            specific functions/functionality for interacting with the
            accelerometer portion(s) of an ASIC

******************************************************************************/
#ifndef _ACCEL_H_
#define _ACCEL_H_

#include "SenseBase.h"

//-----------------------------------------------------------------------------
//  Accel class definition
class CAccel : public CSenseBase
{
private:
    static CAccel* Instance;

public:
    CAccel(void);
    ~CAccel(void);
    static CAccel* GetInstance(void);
    
    void SampleOutputs(int count, double* avg, double* st_dev, word* listDut);
    void NoFlipSample(double odr, int axis, int num_samples, word* listDut);
    
    int NumberOfOutputBits(int dim);
    void GetOffsetRange(int dim, int* min, int*max);
    void GetOffset(int* code, word* listDut);
    void SetOffset(int* code, word* listDut);
    
#ifdef _DIGITAL_OFFSET_
    void GetDigitalOffset(byte reg_addr, int num_registers, int* code, word* listDut);
    void SetDigitalOffset(byte reg_addr, int num_registers, int* code, word* listDut);
#endif
    
    void GetGain(byte reg_addr, int num_registers, int* code, word* listDut);
    void SetGain(byte reg_addr, int num_registers, int* code, word* listDut);
    
    void GetST(byte reg_addr, int num_registers, int* code, word* listDut);
    void SetST(byte reg_addr, int num_registers, int* code, word* listDut);
    
    void GetPolarity(byte reg_addr, int num_registers, double* values, word* listDut);
};

#endif