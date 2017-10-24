/******************************************************************************

    File:   DeviceBase.h
    Desc:   DeviceBase is a subcomponent in Level 1 of the design that
            contains commonly used device functionality.

******************************************************************************/
#ifndef _DEVICE_BASE_H_
#define _DEVICE_BASE_H_

// ASICregister, RAMstruct, and Image
#include "RegisterTypeDefs.h"

#include "Tool.h"

#ifdef _LV_COMM_
    #include "LVInterpreter.h"
#endif

//-----------------------------------------------------------------------------
//  DeviceBase class definition
class CDeviceBase
{
protected:
    static byte SlaveAddr;
    static byte CurrPage;
    
    static CTool *Tool;
    
#ifdef _USE_FAKE_MEMORY_
    static byte FakeMemory[NUM_PAGES][MAX_PAGE_SIZE][TOOL_MAX_DUT];
#endif
    
#ifdef _LV_COMM_
    CLVInterpreter* LV;
#endif
    
    // All possible ASIC registers declared in RegisterMap header
    #include "RegisterMap.h"
    
    byte SingleEnded[MAX_NUM_BANKS][NUM_ROM_REG][TOOL_MAX_DUT];
    
    RAMstruct RAMregisters;
    
    Default DefaultImage;
    
    Image RAMImage;
    Image ROMImage;
    
    VolImage VolatileImage;
    
#if NUM_VOL_PAGES > 1
    VolImage VolatileImage_2;
#endif
    
    void SetPage(byte page, word* listDut, byte slave_addr = ADDR_INVALID);
    
    void GetRegister(ASICregister reg, byte* output, word* listDut);
    void GetRegister(ASICregister reg, int* output, word* listDut);
    
    void SetRegister(ASICregister reg, byte* input, word* listDut);
    void SetRegister(ASICregister reg, int* input, word* listDut);
    
    // TODO: Eliminate these when ^those are finished
    void SetRegister(ASICregister reg, byte input, word* listDut);
    void SetRegister(ASICregister reg, int input, word* listDut);
    
    void CheckRange(ASICregister reg, int* input, word* listDut);

public:
    CDeviceBase(void);
    ~CDeviceBase(void);
    
    //RAMValueStruct RAMValues;
    
    void GetByte(byte page, byte reg_loc, byte* values, word* listDut, string label = "");
    void GetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label = "");
    
    void SetByte(byte page, byte reg_loc, byte value, word* listDut, string label = "");
    void SetByte(byte page, byte reg_loc, byte* values, word* listDut, string label = "");
    void SetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label = "");
    
    // verify communication
    void VerifySetByte(byte page, byte reg_loc, byte value, word* listDut, string label = "");
    void VerifySetByte(byte page, byte reg_loc, byte* values, word* listDut, string label = "");
    void VerifySetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label = "");
    
    void FakeGetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label);
    void FakeSetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label);
};

#endif