/******************************************************************************

    File:   LVInterpreter.cpp
    Desc:   LVInterpreter is a subcomponent in Level 2 of the design that acts
            as a wrapper for base ASIC LabVIEW dlls.

******************************************************************************/
#include "LVInterpreter.h"

CLVInterpreter *CLVInterpreter::Instance = NULL;

/******************************************************************************
    Name:   CLVInterpreter
    Desc:   Default constructor
******************************************************************************/
CLVInterpreter::CLVInterpreter()
{
#ifdef _LV_COMM_
    //LabVIEW dll setup
    LPCWSTR dllName = L"LVInterpreter.dll";  // L coerces it into a long str
    hDLL = LoadLibrary(dllName);
    
    if (hDLL == NULL) // try relative path in software library
    {
        dllName = LV_INTERPRETER_PATH;
        hDLL = LoadLibrary(dllName);
    }
    
    if (hDLL == NULL)
        ERRLog("Did not load LVInterpreter.dll");
    else //get the dll functions from the interpreter dll
    {
        LVEnable = (dllEnableType)GetProcAddress(hDLL, "Enable");
        LVGetAccelOutputs = (dllGetOutType)GetProcAddress(hDLL, "ReadOutputs");
        LVGetByte = (dllGSByteType)GetProcAddress(hDLL, "ReadRegisters");
        LVSetByte = (dllGSByteType)GetProcAddress(hDLL, "WriteRegisters");
        LVGetRAM = (dllGetPageType)GetProcAddress(hDLL, "ReadRam");
        LVGetFakeRAM = (dllGetPageType)GetProcAddress(hDLL, "FakeReadRam");
    }
    
    //memory allocation from LabVIEW RT dll
    dllName = L"C:\\Program Files\\National Instruments\\Shared\\LabVIEW Run-Time\\2010\\lvrt.dll";
    hDLL2 = LoadLibrary(dllName);
    
    if (hDLL2 == NULL) //try for the location on a 64-bit machine
    {
        dllName = L"C:\\Program Files (x86)\\National Instruments\\Shared\\LabVIEW Run-Time\\2010\\lvrt.dll";
        hDLL2 = LoadLibrary(dllName);
    }
    
    if (hDLL2 == NULL)
        ERRLog("Did not load lvrt.dll");
    else
        DSNewHandle = (LVRTMem)GetProcAddress(hDLL2, "DSNewHandle");
#endif
    
}

/******************************************************************************
    Name:   ~CLVInterpreter
    Desc:   Default destructor
******************************************************************************/
CLVInterpreter::~CLVInterpreter(void)
{
#ifdef _LV_COMM_
    //free LV dlls from memory
    FreeLibrary(hDLL);
    FreeLibrary(hDLL2);
#endif
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
CLVInterpreter *CLVInterpreter::GetInstance(void)
{
    if (CLVInterpreter::Instance == NULL)
        CLVInterpreter::Instance = new CLVInterpreter();
    
    return CLVInterpreter::Instance;
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the LVInterpreter class and subclasses
******************************************************************************/
void CLVInterpreter::Init(double proto, double slave_addr, double freq, CTool* Tool, word* listDut)
{
    DBGTrace("--> CLVInterpreter::Init");
    
    DBGPrint("\tInitializing the LVInterpreter module");
    
    this->tool = Tool->GetToolName();
    
#ifdef _LV_COMM_
    this->SetLVComm(proto, slave_addr, freq);
#endif
}

/******************************************************************************
    Name:   SetLVComm
    Desc:   Sets up the communication settings array needed by the
            LabVIEW dll functions
******************************************************************************/
void CLVInterpreter::SetLVComm(double proto, double slave_addr, double freq)
{
    DBGTrace("--> CLVInterpreter::SetLVComm");
    
    this->CommSettings[0] = proto;
    this->CommSettings[1] = slave_addr;
    this->CommSettings[2] = freq;
    
    /****************************************
    * CommSettings is 3 element array of    *
    * CommSettings[0]: com protocol         *
    * CommSettings[1]: I2C slave_addr       *
    * CommSettings[2]: com frequency        *
    ****************************************/
}

/******************************************************************************
    Name:   Enable
    Desc:   Puts the parts into enabled mode
******************************************************************************/
int CLVInterpreter::Enable(word* listDut)
{
    DBGTrace("--> CLVInterpreter::Enable");
    
    LVEnable(listDut, this->tool, this->CommSettings, SHOW_FP, TOOL_MAX_DUT, COMM_SET_LEN);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   GetOutputs
    Desc:   Gets Outputs from the parts
******************************************************************************/
int CLVInterpreter::GetAccelOutputs(int *XOut, int *YOut, int *ZOut, word* listDut)
{
    DBGTrace("--> CLVInterpreter::SetLVComm");
    
    LVGetAccelOutputs(XOut, YOut, ZOut, listDut, this->tool, this->CommSettings, SHOW_FP, TOOL_MAX_DUT, COMM_SET_LEN);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   GetByte
    Desc:   Gets one or more bytes of data from the specified registers
******************************************************************************/
int CLVInterpreter::GetByte(byte page, byte loc, int count, byte *values, word* listDut)
{
    DBGTrace("--> CLVInterpreter::GetByte");
    
    ArrayHdl data = NULL;
    int correctDim[2];
    
    data = (LVArray**)DSNewHandle(sizeof(LVArray));
    
    correctDim[0] = count;
    correctDim[1] = TOOL_MAX_DUT;
    
    memset((*data)->Val, 0, sizeof((*data)->Val));
    
    LVGetByte(page, loc, count, &data, listDut, this->tool, this->CommSettings, SHOW_FP, TOOL_MAX_DUT, COMM_SET_LEN);

    //TODO add error if LV returns an index of 0
    if (correctDim != (*data)->dimSizes && (*data)->dimSizes[0] != 0 && (*data)->dimSizes[1] != 0)
        data = this->ReorderArray(data, correctDim);
    
    memcpy(values, (*data)->Val, correctDim[0]*correctDim[1]);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   SetByte
    Desc:   Sets one or more bytes of data to the specified registers
******************************************************************************/
int CLVInterpreter::SetByte(byte page, byte loc, int count, byte *values, word* listDut)
{
    DBGTrace("--> CLVInterpreter::SetByte");
    
    ArrayHdl data = NULL;
    
    data = (LVArray**)DSNewHandle(sizeof(LVArray));
    
    (*data)->dimSizes[0] = count;
    (*data)->dimSizes[1] = TOOL_MAX_DUT;
    
    //(*data)->Val = *values;
    memcpy((*data)->Val, values, count*TOOL_MAX_DUT);
    
    LVSetByte(page, loc, count, &data, listDut, this->tool, this->CommSettings, SHOW_FP, TOOL_MAX_DUT, COMM_SET_LEN);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   GetRAM
    Desc:   Gets all of the RAM registers
            values is of size MAX_RAM_SIZE by TOOL_MAX_DUT 
******************************************************************************/
int CLVInterpreter::GetRAM(byte *values, word* listDut)
{
    DBGTrace("--> CLVInterpreter::GetRAM");
    
    ArrayHdl data = NULL;
    int correctDim[2];
    
    data = (LVArray**)DSNewHandle(sizeof(LVArray));
    
    correctDim[0] = MAX_RAM_SIZE;
    correctDim[1] = TOOL_MAX_DUT;

    (*data)->dimSizes[0] = MAX_RAM_SIZE;
    (*data)->dimSizes[1] = TOOL_MAX_DUT;
    
    memset((*data)->Val, 0, sizeof((*data)->Val));
    
    if (DBGSimulate == YES)
    {
        LVGetFakeRAM(&data, listDut, this->tool, this->CommSettings, SHOW_FP, TOOL_MAX_DUT, COMM_SET_LEN);
    }
    else
    {
        LVGetRAM(&data, listDut, this->tool, this->CommSettings, SHOW_FP, TOOL_MAX_DUT, COMM_SET_LEN);
    }

    //TODO add error if LV returns an index of 0
    if (correctDim != (*data)->dimSizes && (*data)->dimSizes[0] != 0 && (*data)->dimSizes[1] != 0)
        data = this->ReorderArray(data, correctDim);
    
    memcpy(values, (*data)->Val, correctDim[0]*correctDim[1]);

    return SUCCESS;
}

/******************************************************************************
    Name:   ReorderArray
    Desc:   Reorders a 2D array recieved from LabVIEW dll into correct
            memory space.  Based on how many iterations LV uses in building
            array, it will not fill entire index before moving to next
            dimension, so the memory space will not be ordered correctly.
            
            LabVIEW will helpfully change data->dimSizes[] to what it ended
            up with, we can use those to know where the values have been put.
******************************************************************************/
ArrayHdl CLVInterpreter::ReorderArray(ArrayHdl data, int correctDim[2])
{
    DBGTrace("--> CLVInterpreter::ReorderArray");
    
    byte wrongArray[MAX_RAM_SIZE * TOOL_MAX_DUT]; //place to store the wrongly ordered array
    memcpy(wrongArray, (*data)->Val, correctDim[0]*correctDim[1]);
    
    for (int j = 0; j < correctDim[1]; j++)//dut
    {
        for (int i = 0; i < correctDim[0]; i++)//registers
        {
            if ((i < (*data)->dimSizes[0]) && (j < (*data)->dimSizes[1]))
                (*data)->Val[i * TOOL_MAX_DUT + j] = wrongArray[(i * (*data)->dimSizes[1] + j)];
            else
                (*data)->Val[i * TOOL_MAX_DUT + j] = 0;//outside of registers or duts that LV populated, put 0s
        }
    }
    return data;
}

