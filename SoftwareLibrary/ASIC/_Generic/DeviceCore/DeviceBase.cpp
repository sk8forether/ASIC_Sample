/******************************************************************************

    File:   DeviceBase.cpp
    Desc:   DeviceBase is a subcomponent in Level 1 of the design that
            contains commonly used device functionality.

******************************************************************************/
#include "DeviceBase.h"
#include "DeviceBase_test.h"

#include "SEregisters.h"

byte CDeviceBase::SlaveAddr = NULL;
byte CDeviceBase::CurrPage = PAGE_INVALID;
CTool* CDeviceBase::Tool = NULL;

/******************************************************************************
    Name:   CDeviceBase
    Desc:   Default constructor
******************************************************************************/
CDeviceBase::CDeviceBase(void)
{
    CDeviceBase::SlaveAddr = NULL;
    CDeviceBase::CurrPage = PAGE_INVALID;
    
#ifdef _USE_FAKE_MEMORY_
    memset(FakeMemory, 0, sizeof(FakeMemory));
#elif defined(_LV_COMM_)
    this->LV = CLVInterpreter::GetInstance();
#endif
}

/******************************************************************************
    Name:   ~CDeviceBase
    Desc:   Default destructor
******************************************************************************/
CDeviceBase::~CDeviceBase(void)
{
}

/******************************************************************************
    Name:   SetPage
    Desc:   
******************************************************************************/
void CDeviceBase::SetPage(byte page, word* listDut, byte slave_addr)
{
#ifdef _LV_COMM_
    return;  // LV sets it's own page
#elif defined(_HAS_PAGES_)
    DBGTrace("---> CDeviceBase::SetPage");
    
    // check for nonsensical page number
    if ( (page == PAGE_INVALID) || (page >= NUM_PAGES) )
    {
        ERRLog("Attempted to set an invalid page to the part's MEMPAGE register.\n\
            No page was set.");
        return;
    }
    
    // set page if it's not already set
    if (CDeviceBase::CurrPage != page)
    {
        if (slave_addr == ADDR_INVALID)
        {
            Tool->WritePage(CDeviceBase::SlaveAddr, CDeviceBase::REG_MEMPAGE.addr[0], page, listDut);
        }
        else
        {
            Tool->WritePage(slave_addr, CDeviceBase::REG_MEMPAGE.addr[0], page, listDut);
        }
        
        CDeviceBase::CurrPage = page;
    }
#endif
}

/******************************************************************************
    Name:   GetByte
    Desc:   Read from a single device register using Tool
******************************************************************************/
void CDeviceBase::GetByte(byte page, byte reg_loc, byte* values, word* listDut, string label)
{
    DBGTrace("---> CDeviceBase::GetByte");
    
    this->GetByte(page, reg_loc, 1, values, listDut, label);
}

/******************************************************************************
    Name:   GetByte
    Desc:   Read from device registers using Tool
******************************************************************************/
void CDeviceBase::GetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label)
{
    DBGTrace("---> CDeviceBase::GetByte_array");
    
#ifdef _USE_FAKE_MEMORY_
    this->FakeGetByte(page, reg_loc, count, values, listDut, label);
#elif defined(_LV_COMM_)
    this->LV->GetByte(page, reg_loc, count, values, listDut);
#else
    this->SetPage(page, listDut);
    Tool->Read(CDeviceBase::SlaveAddr, reg_loc, count, values, listDut);
#endif
}

/******************************************************************************
    Name:   SetByte
    Desc:   Write to device registers using Tool
******************************************************************************/
void CDeviceBase::SetByte(byte page, byte reg_loc, byte value, word* listDut, string label)
{
    DBGTrace("---> CDeviceBase::SetByte");
    
    byte values[TOOL_MAX_DUT];
    memset(values, value, sizeof(values));
    
    this->SetByte(page, reg_loc, 1, values, listDut, label);
}

/******************************************************************************
    Name:   SetByte
    Desc:   Write to device registers using Tool
******************************************************************************/
void CDeviceBase::SetByte(byte page, byte reg_loc, byte* values, word* listDut, string label)
{
    DBGTrace("---> CDeviceBase::SetByte");
    
    this->SetByte(page, reg_loc, 1, values, listDut, label);
}

/******************************************************************************
    Name:   SetByte
    Desc:   Write to device registers using Tool
******************************************************************************/
void CDeviceBase::SetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label)
{
    DBGTrace("---> CDeviceBase::SetByte_array");
    
#ifdef _USE_FAKE_MEMORY_
    this->FakeSetByte(page, reg_loc, count, values, listDut, label);
#elif defined(_LV_COMM_)
    this->LV->SetByte(page, reg_loc, count, values, listDut);
#else
    this->SetPage(page, listDut);
    Tool->Write(CDeviceBase::SlaveAddr, reg_loc, count, values, listDut);
#endif
}

/******************************************************************************
    Name:   GetRegister
    Desc:   Reads output from an ASIC register using GetByte for a particular
            register or registers of an ASIC and modifies into readable form
            based on the ASICregister it is from.
******************************************************************************/
void CDeviceBase::GetRegister(ASICregister reg, byte* output, word* listDut)
{
    DBGTrace("---> CDeviceBase::GetRegister (byte)");
    
    // Multiple addresses
    if (reg.addr[1] != ADDR_INVALID)
    {
        int index;
        
        for (int a = 0; a < reg.num_registers; a++)
        {
            if (reg.addr[a] != ADDR_INVALID)
            {
                String reg_name;
                sprintf(reg_name,"%s(%1i)", reg.name.c_str(), a);
                
                index = (a * TOOL_MAX_DUT);
                this->GetByte(reg.page, reg.addr[a], 1, &output[index], listDut, string(reg_name));
            }
        }
        reg.MaskAndShift(output, listDut);
    }
    // Single address but multiple consecutive bytes (assume mask = 0xFF)
    else if (reg.num_registers > 1)
    {
        this->GetByte(reg.page, reg.addr[0], reg.num_registers, output, listDut, reg.name);
    }
    // Single byte
    else
    {
        this->GetByte(reg.page, reg.addr[0], output, listDut, reg.name);
        reg.MaskAndShift(output, listDut);
    }
}

/******************************************************************************
    Name:   GetRegister
    Desc:   Reads output from an ASIC register using GetByte and converts it
            into information in an expected format for a particular register
            or registers of an ASIC
******************************************************************************/
void CDeviceBase::GetRegister(ASICregister reg, int* output, word* listDut)
{
    DBGTrace("---> CDeviceBase::GetRegister (int)");
    
    byte fromDut[APP_MAX_ADDR][TOOL_MAX_DUT];
    memset(fromDut, 0, sizeof(fromDut));
    
    // get byte(s)
    this->GetRegister(reg, &fromDut[0][0], listDut);
    
    // convert
    reg.ConvertToInt(&fromDut[0][0], output, listDut);
}

/******************************************************************************
    Name:   SetRegister
    Desc:   Write input to an ASIC register using SetByte for a particular
            register or registers of an ASIC after modifying it to fit into the
            designated register(s).
******************************************************************************/
void CDeviceBase::SetRegister(ASICregister reg, byte* input, word* listDut)
{
    DBGTrace("---> CDeviceBase::SetRegister (byte)");
    
    string origName = "orig: " + reg.name;
    
    byte original[APP_MAX_ADDR][TOOL_MAX_DUT];
    memset(original, 0, sizeof(original));
    
    // Multiple addresses
    if (reg.addr[1] != ADDR_INVALID)
    {
        int index;
        
        for (int a = 0; a < reg.num_registers; a++)
        {
            // if mask = 0xFF, then we're overwriting anyway
            if ((reg.addr[a] != ADDR_INVALID) && (reg.mask[a] != 0xFF))
                this->GetByte(reg.page, reg.addr[a], &original[a][0], listDut, origName);
        }
        reg.ShiftAndMask(&original[0][0], input, listDut);
        
        for (int a = 0; a < reg.num_registers; a++)
        {
            if (reg.addr[a] != ADDR_INVALID)
            {
                index = (a * TOOL_MAX_DUT);
                
                String reg_name;
                sprintf(reg_name,"%s(%1i)", reg.name.c_str(), a);
                if (DBGVerify >= ENG_LEVEL_2)
                    this->VerifySetByte(reg.page, reg.addr[a], 1, &input[index], listDut, string(reg_name));
                else
                    this->SetByte(reg.page, reg.addr[a], 1, &input[index], listDut, string(reg_name));
            }
        }
    }
    // Single address but multiple consecutive bytes (assume mask = 0xFF)
    else if (reg.num_registers > 1)
    {
        if (DBGVerify >= ENG_LEVEL_2)
            this->VerifySetByte(reg.page, reg.addr[0], reg.num_registers, input, listDut, reg.name);
        else
            this->SetByte(reg.page, reg.addr[0], reg.num_registers, input, listDut, reg.name);
    }
    // Single byte
    else
    {
        // if mask = 0xFF, then we're overwritting anyway
        if (reg.mask[0] != 0xFF)
        {
            byte original[TOOL_MAX_DUT];
            memset(original, 0, sizeof(original));
            
            this->GetByte(reg.page, reg.addr[0], &original[0], listDut, origName);
            
            reg.ShiftAndMask(&original[0], input, listDut);
        }
        
        if (DBGVerify)
            this->VerifySetByte(reg.page, reg.addr[0], input, listDut, reg.name);
        else
            this->SetByte(reg.page, reg.addr[0], input, listDut, reg.name);
    }
}

/******************************************************************************
    Name:   SetRegister
    Desc:   Organizes values into particular bits or a byte within an ASIC
            register, and then sets the info using SetByte
******************************************************************************/
void CDeviceBase::SetRegister(ASICregister reg, int* input, word* listDut)
{
    DBGTrace("---> CDeviceBase::SetRegister (int)");
    
    byte toDut[APP_MAX_ADDR][TOOL_MAX_DUT];
    memset(toDut, 0, sizeof(toDut));
    
#ifdef _EXTRA_CHECKS_ENABLED_
    this->CheckRange(reg, input, listDut);
#endif
    
    // convert
    reg.ConvertFromInt(input, &toDut[0][0], listDut);
    
    // set byte(s)
    this->SetRegister(reg, &toDut[0][0], listDut);
}

/******************************************************************************
    Name:   SetRegister
    Desc:   Takes a single byte input and sets it to the specified register for
            all DUTs
******************************************************************************/
void CDeviceBase::SetRegister(ASICregister reg, byte inputIn, word* listDut)
{
    if (reg.num_registers > 1)
    {
        ERRLog(ERROR_ASIC_SPECIFIC, "Too many addresses in this register to use the single byte prototype");
        return;
    }
    
    int dut;
    byte input[TOOL_MAX_DUT];
    memset(input,0,sizeof(input));
    for (int d = 0; listDut[d] != 0; d++)
    {
        dut = listDut[d] - 1;
        input[dut] = inputIn;
    }
    
    SetRegister(reg, input, listDut);
}

/******************************************************************************
    Name:   SetRegister
    Desc:   Takes a single integer input and sets it to the specified register
            for all DUTs
******************************************************************************/
void CDeviceBase::SetRegister(ASICregister reg, int inputIn, word* listDut)
{
    int dut;
    int input[TOOL_MAX_DUT];
    for (int d = 0; listDut[d] != 0; d++)
    {
        dut = listDut[d] - 1;
        input[dut] = inputIn;
    }
    
    SetRegister(reg, input, listDut);
}

/******************************************************************************
    Name:   CheckRange
    Desc:   Verifies input will fit within specified range for ASICregister.
            Usese Range function to determine min and max based on conversion.
******************************************************************************/
void CDeviceBase::CheckRange(ASICregister reg, int* input, word* listDut)
{
    String msg;
    int dut, min, max;
    
    reg.Range(&min, &max);
    
    for (int d = 0; listDut[d] != 0; d++)
    {
        dut = listDut[d] - 1;
        if ((input[dut] < min) || (max < input[dut]))
        {
            sprintf(msg, "Input %i for %s is outside range (%i, %i) for site %i.", input[dut], reg.name, min, max, dut+1);
            ERRChk(ERROR_ASIC_SPECIFIC, msg);
        }
    }
}

/******************************************************************************
    Name:   VerifySetByte
    Desc:   Debug SetByte function
******************************************************************************/
void CDeviceBase::VerifySetByte(byte page, byte reg_loc, byte value, word* listDut, string label)
{
    //DBGTrace("---> CDeviceBase::VerifySetByte");
    
    byte values[TOOL_MAX_DUT];
    memset(values, value, sizeof(values));
    
    this->VerifySetByte(page, reg_loc, 1, values, listDut, label);
}
/******************************************************************************
    Name:   VerifySetByte
    Desc:   Debug SetByte function
******************************************************************************/
void CDeviceBase::VerifySetByte(byte page, byte reg_loc, byte* values, word* listDut, string label)
{
    //DBGTrace("---> CDeviceBase::VerifySetByte");
    
    this->VerifySetByte(page, reg_loc, 1, values, listDut, label);
}
/******************************************************************************
    Name:   VerifySetByte
    Desc:   Debug SetByte function
******************************************************************************/
void CDeviceBase::VerifySetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label)
{
    DBGTrace("---> CDeviceBase::VerifySetByte");
    
    String msg;
    
    // always display
    int was_on = DBGVerify;
    DBGVerify = max(YES,DBGVerify);
    
    byte output[MAX_RAM_SIZE][TOOL_MAX_DUT];
    this->GetByte(page, reg_loc, count, &output[0][0], listDut, label);
    if (count > 1)
        DBGVerbose("");
    
    this->SetByte(page, reg_loc, count, values, listDut, label);
    
    if (count > 1)
        DBGVerbose("");
    memset(output, 0, sizeof(output));
    this->GetByte(page, reg_loc, count, &output[0][0], listDut, label);
    
    if (DBGVerify >= ENG_LEVEL_2)
        DBGPrint("");
    
    DBGVerify = was_on;
}
