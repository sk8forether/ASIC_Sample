/******************************************************************************

    File:   DeviceCore.h
    Desc:   DeviceCore is a component in Level 1 of the design that
            contains all functionality for interacting with a device of
            any type (including combo parts)

******************************************************************************/
#ifndef _DEVICE_CORE_H_
#define _DEVICE_CORE_H_

#include "DeviceBase.h"

#ifdef _ACCEL_ENABLED_
    #include "Accel.h"
#endif

//-----------------------------------------------------------------------------
//  DeviceCore class definition
class CDeviceCore : public CDeviceBase
{
protected:
    CDeviceCore(void);
    
    byte device_types;
    byte comm_types;
    int CurrState;
    
    void TestModeEnable(word* listDut);

private:
    void Setlsb(byte slave_addr, word* listDut);
    void Burn(byte reg_loc, int num_registers, word* listDut);

public:
    ~CDeviceCore(void);
    
#ifdef _ACCEL_ENABLED_
    CAccel* Accel;
#endif
    
    void Init(char* DefaultImg, char* DefaultMask);
    
    virtual void SetState(int state, word* listDut) {}  // Implemented in ASIC
    
    void HardwareReset(word* listDut, double vdd = NULL);
    void SoftwareReset(word* listDut);
    
    void SaveSlaveAddr(byte slave_addr, word* listDut);
    void SetSlaveAddr(byte slave_addr, word* listDut);
    void HighSpeedMode(byte reg_addr);
    
    void SetDefaultRAM(bool* result, byte* difference, word* listDut, RAMrailway* RAMValues = NULL);
    void GetRAM(byte* values, word* listDut);
    void GetRAM(int* values, word* listDut, RAMrailway* RAMValues = NULL);
    void SetRAM(byte* values, word* listDut);
    
    void GetROM(MEMstring* ROMstr, word* listDut);
    void GetROM(byte* values, word* listDut);
    void SetROM(int* values, word* listDut);
    void VerifyROM(int burned, bool* result, byte* difference, word* listDut);
    
    void GetSN(int* values, word* listDut);
    void SetSN(int* values, word* listDut);
    void VfySN(int* values, word* listDut);
    
    void SaveVolatile(void);
    void RestoreVolatile(void);
    bool CompareVolatile(word* listDut);
    
    void GetRegulator(int* codes, word* listDut);
    void SetRegulator(int* codes, word* listDut);
    void MeasRegulator(double* values, word* listDut);
    void GetOscillator(int* codes, word* listDut);
    void SetOscillator(int* codes, word* listDut);
    void MeasOscillator(double* values, word* listDut);
    void GetWatchdog(int* codes, word* listDut);
    void SetWatchdog(int* codes, word* listDut);
    void MeasWatchdog(double* values, word* listDut);
    
    void MeasCurrent(double* values, word* listDut);
    void GetTempComp(int* codes, word* listDut);
    void SetTempComp(int* codes, word* listDut);
};

#endif