/******************************************************************************

    File:   Accel.cpp
    Desc:   Accel is a subcomponent in Level 2 of the design that contains
            specific functions/functionality for interacting with the
            accelerometer portion(s) of an ASIC

******************************************************************************/
#include "Accel.h"

CAccel *CAccel::Instance = NULL;

/******************************************************************************
    Name:   CAccel
    Desc:   Default constructor
******************************************************************************/
CAccel::CAccel(void)
{
}

/******************************************************************************
    Name:   ~CAccel
    Desc:   Default destructor
******************************************************************************/
CAccel::~CAccel(void)
{
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
CAccel *CAccel::GetInstance(void)
{
    if (CAccel::Instance == NULL)
        CAccel::Instance = new CAccel();
    
    return CAccel::Instance;
}

/******************************************************************************
    Name:   SampleOutputs
    Desc:   Passthrough to SenseBase SampleOutputs function, which is sense
            element generic for getting 
******************************************************************************/
void CAccel::SampleOutputs(int count, double* avg, double* st_dev, word* listDut)
{
    DBGTrace("---> CAccel::SampleOutputs");
    
    // TODO: update this after testing
    count = 1;
    int timeout = 0;
    
    // Passthrough to SenseBase
    CSenseBase::SampleOutputs(count, timeout, avg, st_dev, SENSE_ACCEL, listDut);
}

/******************************************************************************
    Name:   NoFlipSample
    Desc:   
******************************************************************************/
void CAccel::NoFlipSample(double odr, int axis, int num_samples, word* listDut)
{
    DBGTrace("--> CAccel::NoFlipSample");
    
    //
}

/******************************************************************************
    Name:   NumberOfOutputBits
    Desc:   Get number of accelerometer output bits for X, Y, and Z
******************************************************************************/
int CAccel::NumberOfOutputBits(int dim)
{
    DBGTrace("--> CAccel::NumberOfOutputBits");
    
    switch(dim)
    {
        case X:
            return this->REG_ACCEL_OUT_X.TotalBits();
        case Y:
            return this->REG_ACCEL_OUT_X.TotalBits();
        case Z:
            return this->REG_ACCEL_OUT_X.TotalBits();
        default:
            ERRCrit(ERROR_UNDEFINED, "Unknown axis");
            break;
    }
    
    return 0;
}

/******************************************************************************
    Name:   GetOffsetRange
    Desc:   Get accelerometer offset codes ranges for X, Y, and Z
******************************************************************************/
void CAccel::GetOffsetRange(int dim, int* min, int* max)
{
    DBGTrace("--> CAccel::GetOffsetRange");
    
    switch(dim)
    {
        case X:
            this->REG_ACCEL_FOX.Range(min, max);
            break;
        case Y:
            this->REG_ACCEL_FOY.Range(min, max);
            break;
        case Z:
            this->REG_ACCEL_FOX.Range(min, max);
            break;
        default:
            ERRCrit(ERROR_UNDEFINED, "Unknown axis");
            break;
    }
}

/******************************************************************************
    Name:   GetOffset
    Desc:   Get accelerometer offset codes for X, Y, and Z
******************************************************************************/
void CAccel::GetOffset(int* code, word* listDut)
{
    DBGTrace("--> CAccel::GetOffset");
    
    int xcodes[TOOL_MAX_DUT];
    int ycodes[TOOL_MAX_DUT];
    int zcodes[TOOL_MAX_DUT];
    memset(xcodes, 0, sizeof(xcodes));
    memset(ycodes, 0, sizeof(ycodes));
    memset(zcodes, 0, sizeof(zcodes));
    
    // TODO: use a more efficient method
    this->GetRegister(this->REG_ACCEL_FOX, &xcodes[0], listDut);
    this->GetRegister(this->REG_ACCEL_FOY, &ycodes[0], listDut);
    this->GetRegister(this->REG_ACCEL_FOZ, &zcodes[0], listDut);
    
    for (int d = 0; listDut[d] != 0; d++)
    {
        code[(X * TOOL_MAX_DUT) + _dut] = xcodes[_dut];
        code[(Y * TOOL_MAX_DUT) + _dut] = ycodes[_dut];
        code[(Z * TOOL_MAX_DUT) + _dut] = zcodes[_dut];
    }
}

/******************************************************************************
    Name:   SetOffset
    Desc:   Get accelerometer offset codes for X, Y, and Z
******************************************************************************/
void CAccel::SetOffset(int* code, word* listDut)
{
    DBGTrace("--> CAccel::SetOffset");
    
    int dut;
    int xcodes[TOOL_MAX_DUT];
    int ycodes[TOOL_MAX_DUT];
    int zcodes[TOOL_MAX_DUT];
    memset(xcodes, 0, sizeof(xcodes));
    memset(ycodes, 0, sizeof(ycodes));
    memset(zcodes, 0, sizeof(zcodes));
    
    for (int d = 0; listDut[d] != 0; d++)
    {
        dut = listDut[d] - 1;
        xcodes[dut] = code[(X * TOOL_MAX_DUT) + dut];
        ycodes[dut] = code[(Y * TOOL_MAX_DUT) + dut];
        zcodes[dut] = code[(Z * TOOL_MAX_DUT) + dut];
    }
    
    // TODO: use a more efficient method
    this->SetRegister(this->REG_ACCEL_FOX, &xcodes[0], listDut);
    this->SetRegister(this->REG_ACCEL_FOY, &ycodes[0], listDut);
    this->SetRegister(this->REG_ACCEL_FOZ, &zcodes[0], listDut);
    
    // TODO do this properly, once SetODR is implemented
    this->CurrODR = 50;
    
    int delay = (int)ceil(7000.0 / (double)this->CurrODR);
    //TIMEDelay(delay);
}

/******************************************************************************
    Name:   GetDigitalOffset
    Desc:   
******************************************************************************/
#ifdef _DIGITAL_OFFSET_
void CAccel::GetDigitalOffset(byte reg_addr, int num_registers, int* code, word* listDut)
{
    DBGTrace("--> CAccel::GetDigitalOffset");
    
    //
}
#endif

/******************************************************************************
    Name:   SetDigitalOffset
    Desc:   
******************************************************************************/
#ifdef _DIGITAL_OFFSET_
void CAccel::SetDigitalOffset(byte reg_addr, int num_registers, int* code, word* listDut)
{
    DBGTrace("--> CAccel::SetDigitalOffset");
    
    //
}
#endif

/******************************************************************************
    Name:   GetGain
    Desc:   
******************************************************************************/
void CAccel::GetGain(byte reg_addr, int num_registers, int* code, word* listDut)
{
    DBGTrace("--> CAccel::GetGain");
    
    //
}

/******************************************************************************
    Name:   SetGain
    Desc:   
******************************************************************************/
void CAccel::SetGain(byte reg_addr, int num_registers, int* code, word* listDut)
{
    DBGTrace("--> CAccel::SetGain");
    
    //
}

/******************************************************************************
    Name:   GetST
    Desc:   
******************************************************************************/
void CAccel::GetST(byte reg_addr, int num_registers, int* code, word* listDut)
{
    DBGTrace("--> CAccel::GetST");
    
    //
}

/******************************************************************************
    Name:   SetST
    Desc:   
******************************************************************************/
void CAccel::SetST(byte reg_addr, int num_registers, int* code, word* listDut)
{
    DBGTrace("--> CAccel::SetST");
    
    //
}

/******************************************************************************
    Name:   GetPolarity
    Desc:   
******************************************************************************/
void CAccel::GetPolarity(byte reg_addr, int num_registers, double* values, word* listDut)
{
    DBGTrace("--> CAccel::GetPolarity");
    
    //
}
