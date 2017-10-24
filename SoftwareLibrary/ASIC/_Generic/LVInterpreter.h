/******************************************************************************

    File:   LVInterpreter.h
    Desc:   LVInterpreter is a subcomponent in Level 2 of the design that acts
    as a wrapper for base ASIC LabVIEW dlls.

******************************************************************************/
#ifndef _LV_INTERPRETER_H_
#define _LV_INTERPRETER_H_

#include "Defines.h"
#include "Tool.h"

#define COMM_SET_LEN    3
#define SHOW_FP         NO

//-----------------------------------------------------------------------------
//    dll setup

//2D array type def for read/write registers
typedef struct {
    int dimSizes[2];//number of indexes is number of dimensions
    byte Val[MAX_RAM_SIZE * TOOL_MAX_DUT]; //max registers in any given page * max duts
    } LVArray;
typedef LVArray **ArrayHdl;

//DSNewHandle from lvrt.dll to allocate memory as LV wants it for the 2D arrays
typedef LVArray**(*LVRTMem) (size_t size);


//-----------------------------------------------------------------------------
//    interpreter dll functions

//Enable
typedef void (*dllEnableType) (word listDut[], double Tool,
    double CommSettings[], byte show_fp, int MAX_DUT, int len);

//GetOutputs
typedef void (*dllGetOutType) (int *XOut, int *YOut, int *ZOut, word listDut[], 
    double Tool, double CommSettings[], byte show_fp, int MAX_DUT, int len);

//GetByte and SetByte
typedef void (*dllGSByteType) (byte page, byte reg_addr, int count, 
    ArrayHdl *data, word listDut[], double Tool,
    double CommSettings[], byte show_fp, int MAX_DUT, int len);

//Get RAM    
typedef void (*dllGetPageType) (ArrayHdl *data, word listDut[], double Tool,
    double CommSettings[], byte show_fp, int MAX_DUT, int len);


//-----------------------------------------------------------------------------
//  LVInterpreter class definition
class CLVInterpreter
{
private:
    CLVInterpreter(void);
    static CLVInterpreter* Instance;
    
    double CommSettings[3];// {protocol, I2C slave address, comm frequency}
    double tool;
    
    ArrayHdl ReorderArray(ArrayHdl data, int correctDim[2]);
    
    HINSTANCE hDLL; //LVInterpreter.dll
    HINSTANCE hDLL2; //lvrt.dll
    
    LVRTMem DSNewHandle;
    
    dllEnableType LVEnable;
    dllGetOutType LVGetAccelOutputs;
    dllGSByteType LVGetByte;
    dllGSByteType LVSetByte;
    dllGetPageType LVGetRAM;
    dllGetPageType LVGetFakeRAM;

public:
    ~CLVInterpreter(void);
    static CLVInterpreter* GetInstance(void);
    
    void Init(double proto, double slave_addr, double freq, CTool* Tool, word* listDut);
    
    int Enable(word* listDut);
    int GetAccelOutputs(int *XOut, int *YOut, int *ZOut, word* listDut);
    int GetByte(byte page, byte loc, int count, byte *values, word* listDut);
    int SetByte(byte page, byte loc, int count, byte *values, word* listDut);
    int GetRAM(byte *values, word* listDut);
    void SetLVComm(double proto, double slave_addr, double freq);
};

#endif