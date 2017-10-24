/******************************************************************************

    File:   DeviceCore.cpp
    Desc:   DeviceCore is a component in Level 1 of the design that
            contains all functionality for interacting with a device of
            any type (including combo parts)

******************************************************************************/
#include "DeviceCore.h"

/******************************************************************************
    Name:   CDeviceCore
    Desc:   Default constructor
******************************************************************************/
CDeviceCore::CDeviceCore(void)
{
    memset(this->SingleEnded, 0x00, sizeof(this->SingleEnded));
    this->RAMImage = Image(RAM, RAM_PG);
    this->ROMImage = Image(ROM, ROM_PG);
    
    this->VolatileImage = VolImage(VOL_PG1, NUM_VOL_REG);
    
#if NUM_VOL_PAGES > 1
    this->VolatileImage_2 = VolImage(VOL_PG2, NUM_VOL_REG_2);
#endif
    
#ifdef _ACCEL_ENABLED_
    this->Accel = CAccel::GetInstance();
#endif
}

/******************************************************************************
    Name:   ~CDeviceCore
    Desc:   Default destructor
******************************************************************************/
CDeviceCore::~CDeviceCore(void)
{
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the DeviceCore class
******************************************************************************/
void CDeviceCore::Init(char* DefaultImg, char* DefaultMask)
{
    DBGTrace("--> CDeviceCore::Init");
    DBGPrint("\tInitializing DeviceCore");
    
#ifdef _USE_FAKE_MEMORY_
    DBGPrint("*** Warning: Using fake memory to simulate several parts. ***");
#endif
    
    this->DefaultImage = Default(DefaultImg, DefaultMask);
    
    // check proper setup of RAMregisters
    this->RAMregisters.Check();
}

/******************************************************************************
    Name:   HardwareReset
    Desc:   Resets the device (hardware reset)
******************************************************************************/
void CDeviceCore::HardwareReset(word* listDut, double vdd)
{
    DBGTrace("--> CDeviceCore::HardwareReset");
    
    word com = this->Tool->GetCom();
    if (vdd == NULL)
        vdd = Tool->GetOpVDD();
    
    this->SetState(STATE_DEVICE_DISABLE, listDut);
    this->Tool->DisconnectComm(listDut);
    this->Tool->PowerOffPart(listDut);
    
    this->Tool->ConnectComm(com, listDut);
    this->Tool->PowerOnPart(0.0, listDut);
    //TIMEDelay(TOOL_RESET_DELAY);
    
    this->Tool->PowerOnPart(vdd, listDut);
    this->Tool->ConnectComm(com, listDut);
    
    //TIMEDelay(RESET_DELAY);
    
    this->SetState(STATE_DEVICE_UNKNOWN, listDut);
    
#ifdef _TEST_MODE_
    this->TestModeEnable(listDut);
#endif
    
    this->SetState(STATE_DEVICE_ENABLE, listDut);
    
    // set page if it has pages to ensure known state of parts
#ifdef _HAS_PAGES_
    this->SetPage(PAGE_00, listDut);
#endif
}

/******************************************************************************
    Name:   SoftwareReset
    Desc:   Resets the device (software reset) and sets state to ENABLE
            Includes a RESET_DELAY
******************************************************************************/
void CDeviceCore::SoftwareReset(word* listDut)
{
    DBGTrace("--> CDeviceCore::SoftwareReset");
    
    this->SetState(STATE_DEVICE_DISABLE, listDut);
    this->SetRegister(this->REG_SRST, this->REG_SRST.mask, listDut);
    
    //TIMEDelay(RESET_DELAY);
    
#ifdef _TEST_MODE_
    this->TestModeEnable(listDut);
#endif
    
    this->SetState(STATE_DEVICE_ENABLE, listDut);
    
    // set page if it has pages to ensure known state of parts
#ifdef _HAS_PAGES_
    this->SetPage(PAGE_00, listDut);
#endif
}

/******************************************************************************
    Name:   SaveSlaveAddr
    Desc:   Saves the value of SlaveAddr in DeviceBase, which keeps track of
            the current slave address of DUTs for patterns.  Also sends Test
            Mode Enable pattern if necessary.
******************************************************************************/
void CDeviceCore::SaveSlaveAddr(byte slave_addr, word* listDut)
{
    DBGTrace("--> CDeviceCore::SaveSlaveAddr");
    
#ifdef _TEST_MODE_
    this->TestModeEnable(listDut);
#endif
    // must do first
    this->Setlsb(slave_addr, listDut);
    
    this->SlaveAddr = slave_addr;
}

/******************************************************************************
    Name:   SetSlaveAddr
    Desc:   Sets the slave address of the DUTs to the specified hex address
            input.  Also sets the value of SlaveAddr in DeviceBase, which keeps
            track of the current slave address of DUTs for patterns.
******************************************************************************/
void CDeviceCore::SetSlaveAddr(byte slave_addr, word* listDut)
{
    DBGTrace("--> CDeviceCore::SetSlaveAddr");
    
#ifdef _TEST_MODE_
    this->TestModeEnable(listDut);
#endif
    
#ifdef _LV_COMM_
    // LV sets it's own page
    return;
#else
    #ifdef _I2C_SLAVE_CONFIG_
        byte new_sad = slave_addr;
        new_sad &= this->REG_SAD1_6.mask[0];
        
        byte new_sads[TOOL_MAX_DUT];
        memset(new_sads,new_sad,TOOL_MAX_DUT);
        
        // TODO: Uncomment when SPI communication has been implemented
        //Tool->Write(new_sad, this->REG_SAD1_6.addr[0], this->REG_SAD1_6.num_registers, new_sads, listDut);
        
        // must do last since addr pin affects SPI3/4 selection
        this->Setlsb(slave_addr, listDut);
    #elif defined(_NO_SAD1_BIT_)
        // bit 1 isn't configurable, only set lsb (bit 0) by setting Address pin
        this->Setlsb(slave_addr, listDut);
    #else
        // must do first
        this->Setlsb(slave_addr, listDut);
        
        byte bit2 = 0x02;
        
        byte alternate = 0x00;
        byte data = 0x00;
        
        // Set second bit
        if ( (slave_addr & bit2) == bit2 )           // second bit high
        {
            alternate = slave_addr & ~bit2;
            data |= REG_SAD1.mask[0];
        }
        else                                         // second bit low
        {
            alternate = slave_addr | bit2;
            data &= REG_SAD1.mask[0];
        }
        
        #ifdef _HAS_PAGES_
            // set page at either slave address
            this->SetPage(this->REG_SAD1.page, listDut, slave_addr);
            
            this->CurrPage = ADDR_INVALID;
            this->SetPage(this->REG_SAD1.page, listDut, alternate);
        #endif
        
        byte dataArray[TOOL_MAX_DUT];
        memset(dataArray,data,TOOL_MAX_DUT);
        
        // if at alternate slave address, force to correct one
        Tool->Write(alternate, this->REG_SAD1.addr[0], this->REG_SAD1.num_registers, dataArray, listDut);
    #endif
    
    this->SlaveAddr = slave_addr;
#endif
}

/******************************************************************************
    Name:   Setlsb
    Desc:   Set lsb of slave addr using addr pin
******************************************************************************/
void CDeviceCore::Setlsb(byte slave_addr, word* listDut)
{
    DBGTrace("--> CDeviceCore::Setlsb");
    
    if ( IsOdd(slave_addr) )
    {
        Tool->Pin->SetAddrPin(HIGH, listDut);
    }
    else
    {
        Tool->Pin->SetAddrPin(LOW, listDut);
    }
}

/******************************************************************************
    Name:   TestModeEnable
    Desc:   Enables the Test Mode by sending a specific pattern to the DUTs
            For parts with _TEST_MODE_ this function should be called after
            connecting communication but before reading/writing bytes.
******************************************************************************/
void CDeviceCore::TestModeEnable(word* listDut)
{
    DBGTrace("--> CDeviceCore::TestModeEnable");
    
    Tool->TestMode(listDut);
}

/******************************************************************************
    Name:   SetDefaultRAM
    Desc:   Sets the Default RAM Image (from the Spec) to the DUTs
            difference - XOR between RAM to set and RAM after set
            results - T/F of compare Default RAM Image and RAM after set
******************************************************************************/
void CDeviceCore::SetDefaultRAM(bool* result, byte* difference, word* listDut, RAMrailway* RAMValues)
{
    DBGTrace("--> CDeviceCore::SetDefaultRAM");
    if (DBGVerboseEnabled)
        this->DefaultImage.Print();
    
    // set Default RAM Image
    int dut;
    byte target[NUM_RAM_REG][TOOL_MAX_DUT];
    memset(target,0x00,sizeof(target));
    for (int count = 0; count < NUM_RAM_REG; count++)
    {
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            target[count][dut] = this->DefaultImage.SpecImage[count];
        }
    }
    this->SetRAM(&target[0][0], listDut);
    
    // get RAM after setting Default
    this->GetRAM(&this->RAMImage.Raw[0][0], listDut);
    
    // TODO: Switch to using int function
    //this->GetRAM(&this->RAMImage.Raw[0][0], listDut, RAMValues);
    
    // compare RAM to Default
    this->RAMImage.CompareDefault(this->DefaultImage, difference, result, listDut);
}

/******************************************************************************
    Name:   GetRAM
    Desc:   Obtains the entire RAM image and returns it as raw hex bytes
******************************************************************************/
void CDeviceCore::GetRAM(byte* values, word* listDut)
{
    DBGTrace("--> CDeviceCore::GetRAM (raw)");
    
#ifdef _LV_COMM_
    this->LV->GetRAM(values, listDut);
#else
    this->GetRegister(this->RawRAM, values, listDut);
#endif
    
#ifndef _USE_FAKE_MEMORY_
    if (DBGSimulate)
    {
        for (int i = 0; i < NUM_RAM_REG; i++)
        {
            memset(&values[i * TOOL_MAX_DUT], i+1, TOOL_MAX_DUT);
        }
    }
#endif
}

/******************************************************************************
    Name:   GetRAM
    Desc:   Obtains the entire RAM image as raw hex bytes, converts the values
            of each ASICregister (based on RAMregisters) into integers, and
            returns an int array of the values
******************************************************************************/
void CDeviceCore::GetRAM(int* values, word* listDut, RAMrailway* RAMValues)
{
    DBGTrace("--> CDeviceCore::GetRAM (conversion)");
    
    // get raw values first (faster)
    this->GetRAM(&this->RAMImage.Raw[0][0], listDut);
    
    // convert
    this->RAMImage.Convert(values, this->RAMregisters, listDut);
    
    // TODO: pass converted values back to Test via RAMValues
    //RAMValues.Add(...); <- Probably inside convert
    
    if (DBGVerboseEnabled)
    {
        this->RAMImage.PrintConverted(listDut);
        this->RAMImage.View(0);
    }
}

/******************************************************************************
    Name:   SetRAM
    Desc:   
******************************************************************************/
void CDeviceCore::SetRAM(byte* values, word* listDut)
{
    DBGTrace("--> CDeviceCore::SetRAM");
    
    this->SetRegister(this->RawRAM, values, listDut);
}

/******************************************************************************
    Name:   GetROM
    Desc:   Obtains the entire ROM and stores it as a string for each DUT
******************************************************************************/
void CDeviceCore::GetROM(MEMstring* ROMstr, word* listDut)
{
    DBGTrace("--> CDeviceCore::GetROM (str)");
    
    byte values[NUM_ROM_REG][TOOL_MAX_DUT];
    memset(values, 0, sizeof(values));
    
    this->GetROM(&values[0][0], listDut);
    
    // Store ROM images as strings
    B2SArray(&ROMstr->Raw[0], &values[0][0], NUM_RAM_REG, listDut);
}

/******************************************************************************
    Name:   GetROM
    Desc:   Obtains the entire ROM and returns it as raw hex bytes
******************************************************************************/
void CDeviceCore::GetROM(byte* values, word* listDut)
{
    DBGTrace("--> CDeviceCore::GetROM (raw)");
    
#ifdef _LV_COMM_
    //this->LV->GetROM(values, listDut);
    ERRChk(ERROR_UNIMPLEMENTED, "GetROM doesn't exist in LabVIEW", "CDeviceCore::GetROM", YES);
#elif defined(_OTP_)
    // put the parts into redundant read mode
    this->SetState(STATE_REDUNDANT_READ_MODE, listDut);
    
    this->GetRegister(this->RawROM, values, listDut);
    
    // lock memory access
    this->SetState(STATE_LOCK_MEMORY_ACCESS, listDut);
#else
    // TODO: for _EEPROM_
#endif
    
    if (DBGSimulate)
    {
        for (int i = 0; i < NUM_RAM_REG; i++)
        {
            memset(&values[i * TOOL_MAX_DUT], i+1, TOOL_MAX_DUT);
        }
    }
}

/******************************************************************************
    Name:   VerifyROM
    Desc:   
******************************************************************************/
void CDeviceCore::VerifyROM(int burned, bool* result, byte* difference, word* listDut)
{
    DBGTrace("--> CDeviceCore::VfyPROM");
    
    int dut;
    byte mask, bank0, bank1, bank2, bank3;
    byte check[NUM_ROM_REG][TOOL_MAX_DUT];
    byte CombinedRead[NUM_ROM_REG][TOOL_MAX_DUT];
    Image ROMcheck = Image(ROM, ROM_PG);
    
    if (burned)
        memset(check, 0xFF, sizeof(check));
    else
        memset(check, 0x00, sizeof(check));
    
    ROMcheck.SetRaw(&check[0][0]);
    
    // put the parts in single-ended read mode
    this->SetState(STATE_SINGLE_ENDED_READ_MODE, listDut);
    
    // read each of the banks
    for (int bank = 0; bank < MAX_NUM_BANKS; bank++)
    {
        this->SetRegister(this->REG_OTPAC, (byte)bank, listDut);
        
        this->GetRegister(this->RawROM, &this->SingleEnded[bank][0][0], listDut);
    }
    
    // combine the banks together
    memset(CombinedRead, 0, sizeof(CombinedRead));
    
    for (int reg = 0; reg < NUM_ROM_REG; reg++)
    {
        for (int bit = 0; bit < 8; bit++)
        {
            mask = (byte)(0x01 << bit);
            
            for (int d = 0; listDut[d] != 0; d++)
            {
                dut = listDut[d] - 1;
                
                bank0 = SingleEnded[0][reg][dut] & mask;
                bank1 = SingleEnded[1][reg][dut] & mask;
                bank2 = SingleEnded[2][reg][dut] & mask;
                bank3 = SingleEnded[3][reg][dut] & mask;
                
                if ( (bank0 == bank1) && (bank2 == bank3) && (bank0 != bank3) )
                    CombinedRead[reg][dut] |= mask;
            }
        }
    }
    
    // do actual check if burned or not
    //TODO: Ignore SN registers?
    ROMcheck.Compare(&CombinedRead[0][0], difference, result, listDut);
    
    // put the parts back into redundant read mode
    this->SetState(STATE_REDUNDANT_READ_MODE, listDut);
    
    // lock memory access
    this->SetState(STATE_LOCK_MEMORY_ACCESS, listDut);
}
