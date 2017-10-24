/******************************************************************************

    File:   BF.cpp
    Desc:   The BF ASIC is an accelerometer and pedometer with 3 pages.
            Page 0x00 contains the Volatile registers, Page 0x01 contains the
            RAM, and Page 0x02 contains the ROM (OTP).

******************************************************************************/
#include "BF.h"

CBF *CBF::Instance = NULL;

/******************************************************************************
    Name:   CBF
    Desc:   Default constructor
******************************************************************************/
CBF::CBF()
{
    this->Name = STR(BF);
    this->SetupRegisterMap();
}

/******************************************************************************
    Name:   ~CBF
    Desc:   Default destructor
******************************************************************************/
CBF::~CBF(void)
{
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the ASIC class and subclasses
******************************************************************************/
void CBF::Init(String DefaultImg, String DefaultMask, CTool* Tool, word* listDut)
{
    DBGTrace("--> CBF::Init");
    
    DBGPrint("Initializing the BF ASIC...");
    this->Tool = Tool;
    CASIC::Init(DefaultImg, DefaultMask, listDut);
    
    DBGPrint("Initialization done.");
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
CBF *CBF::GetInstance(void)
{
    if (CBF::Instance == NULL)
        CBF::Instance = new CBF();
    
    return CBF::Instance;
}

/******************************************************************************
    Name:   SetupRegisterMap
    Desc:   Defines the registers specifically for the BF ASIC
******************************************************************************/
void CBF::SetupRegisterMap(void)
{
    this->SetupPage0();
    this->SetupPage1();
    this->SetupPage2();
}

/******************************************************************************
    Name:   SetupPage0
    Desc:   Defines the registers on Page 00 of the BF ASIC
******************************************************************************/
void CBF::SetupPage0(void)
{
    byte parent_addr;
    byte special_mask;
    byte p = PAGE_00;
    byte all = 0xFF;
    memory_type mem = Volatile;
    
    /*********************************************************
    * Register setup format: Include all useful registers    *
    * this->REG_NAME = ASICregister(                         *
    *   name, page, addr, mask, conversion, memory, number); *
    *********************************************************/
    
    // outputs
    this->REG_ACCEL_OUT_X = ASICregister(
        "ACCEL_OUT_X", p, 0x18, all, sense_output, mem, NUM_ACCEL_OUT);
    this->REG_ACCEL_OUT_Y = ASICregister(
        "ACCEL_OUT_Y", p, 0x1A, all, sense_output, mem, NUM_ACCEL_OUT);
    this->REG_ACCEL_OUT_Z = ASICregister(
        "ACCEL_OUT_Z", p, 0x1C, all, sense_output, mem, NUM_ACCEL_OUT);
    
    // response (cotr)
    this->REG_RESP = ASICregister(
        "RESP", p, 0x20, all, convert_byte, mem, 1);
    
    // settings
    this->REG_CNTL1 = ASICregister(
        "CNTL1", p, 0x2A, all, convert_byte, mem, 1);
    this->REG_CNTL2 = ASICregister(
        "CNTL2", p, 0x2B, all, convert_byte, mem, 1);
    this->REG_ACCEL_ODR = ASICregister(
        "ACCEL_ODR", p, 0x2F, 0x0F, convert_uint, mem, 1);
    this->REG_INC1 = ASICregister(
        "INC1", p, 0x30, all, convert_byte, mem, 1);
    this->REG_INC4 = ASICregister(
        "INC4", p, 0x33, all, convert_byte, mem, 1);
    this->REG_TSC = ASICregister(
        "TSC", p, 0x37, all, convert_byte, mem, 1);
    this->REG_EXTRA_MIRROR =  ASICregister(
        "EXTRA_MIRROR", p, 0x43, all, convert_byte, mem, 1);
    
    // set key to turn on selftest (send mask)
    this->KEY_SELFTEST = 0x88;
    this->REG_SELFTEST = ASICregister(
        "SELFTEST", p, 0x5D, KEY_SELFTEST, convert_uint, mem, 1);
    
    this->REG_BUF_CNTL1 = ASICregister(
        "BUF_CNTL1", p, 0x6A, all, convert_byte, mem, 1);
    
    // bit4 is reserved
    this->REG_BUF_CNTL2 = ASICregister(
        "BUF_CNTL2", p, 0x6B, 0xEF, convert_byte, mem, 1);
    
    this->REG_BUF_STATUS1 = ASICregister(
        "BUF_STATUS1", p, 0x6C, all, convert_byte, mem, 1);
    
    // mr sets read modes for OTP
    this->SINGLE_ENDED = 0x00;
    this->REDUNDANT = 0x11;
    this->REG_MR = ASICregister(
        "MR", p, 0x73, all, convert_byte, mem, 1);
    
    // bits 2-5 are reserved
    this->REG_OTPAC = ASICregister(
        "OTPAC", p, 0x74, 0xC3, convert_byte, mem, 1);
    
    // bit7 is tm_steps_wr and bits 4-6 are empty
    this->REG_MUXC = ASICregister(
        "MUXC", p, 0x75, 0x0F, convert_byte, mem, 1);
    
    this->REG_BIST_CNTL = ASICregister(
        "BIST_CNTL", p, 0x76, all, convert_byte, mem, 1);
    this->REG_TM_DTB = ASICregister(
        "TM_DTB", p, 0x7D, all, convert_byte, mem, 1);
    
    // access memory for OTP requires keys be sent before burn (send mask)
    this->KEY_AMEM2 = 0x8B;
    this->REG_AMEM2 = ASICregister(
        "AMEM2", p, 0x72, all, convert_byte, mem, 1);
    this->KEY_AMEM1 = 0x55;
    this->REG_AMEM1 = ASICregister(
        "AMEM1", p, 0x7E, all, convert_byte, mem, 1);
    
    // only setup mempage on page 0
    this->REG_MEMPAGE = ASICregister(
        "MEMPAGE", p, 0x7F, 0x03, convert_byte, mem, 1);
    
    /****************************************************************
    * Special Register (single bit) setup format: special case only *
    * parent_addr = this->REG_PARENT.addr[0];                       *
    * special_mask = 0x55;                                          *
    * this->REG_NAME = ASICregister(                                *
    *   name, page, parent_addr, mask, conversion, memory, number); *
    ****************************************************************/
    
    parent_addr = this->REG_CNTL1.addr[0];
    special_mask = 0x80;
    this->REG_PC1 = ASICregister(
        "PC1", p, parent_addr, special_mask, convert_bit, mem, 1);
    
    parent_addr = this->REG_CNTL1.addr[0];
    special_mask = 0x40;
    this->REG_RES = ASICregister(
        "RES", p, parent_addr, special_mask, convert_bit, mem, 1);
    
    parent_addr = this->REG_CNTL2.addr[0];
    special_mask = 0x80;
    this->REG_SRST = ASICregister(
        "SRST", p, parent_addr, special_mask, convert_byte, mem, 1);
    
    parent_addr = this->REG_CNTL2.addr[0];
    special_mask = 0x40;
    this->REG_COTC = ASICregister(
        "COTC", p, parent_addr, special_mask, convert_bit, mem, 1);
    
    /*********************************************************************
    * RawVOLATILE setup format: start address and number of bytes        *
    * this->RawVOLATILE = ASICregister(                                  *
    *   "RawVOLATILE", p, VOL_start, all, conversion, mem, NUM_VOL_REG); *
    *********************************************************************/
    
    this->RawVOLATILE = ASICregister(
        "RawVOLATILE", p, 0x00, all, convert_byte, mem, NUM_VOL_REG);
}

/******************************************************************************
    Name:   SetupPage1
    Desc:   Defines the registers on Page 01 of the BF ASIC
******************************************************************************/
void CBF::SetupPage1(void)
{
    byte parent_addr;
    byte special_mask;
    byte p = PAGE_01;
    byte all = 0xFF;
    memory_type mem = RAM;
    
    /*********************************************************
    * Register setup format: Include all RAM registers       *
    * this->REG_NAME = ASICregister(                         *
    *   name, page, addr, mask, conversion, memory, number); *
    *********************************************************/
    
    byte mask[APP_MAX_ADDR];
    memset(mask, 0x00, sizeof(mask));
    
    // Oscillator Trim Code
    this->REG_OSC = ASICregister(
        "OSC", p, 0x19, 0x3F, convert_uint, mem, 1);
    
    // Spare_1
    this->REG_SPARE_1 = ASICregister(
        "SPARE1", p, 0x1A, all, convert_byte, mem, 1);
    
    // Watchdog Trim Code
    this->REG_WDT = ASICregister(
        "WDT", p, 0x1B, all, convert_uint, mem, 1);
    
    // Selftest Trim Codes
    this->REG_ACCEL_STX = ASICregister(
        "ACCEL_STX", p, 0x1C, 0x0F, convert_uint, mem, 1);
    this->REG_ACCEL_STY = ASICregister(
        "ACCEL_STY", p, 0x1C, 0xF0, convert_uint, mem, 1);
    this->REG_ACCEL_STZ = ASICregister(
        "ACCEL_STZ", p, 0x1D, 0x0F, convert_uint, mem, 1);
    
    // Who Am I
    this->REG_WAI = ASICregister(
        "WAI", p, 0x1E, all, convert_byte, mem, 1);
    
    // Extra contains part_en, hs, sad1, ssb, stnf, ppod, and tcen
    this->REG_EXTRA = ASICregister(
        "EXTRA", p, 0x1F, 0xFE, convert_byte, mem, 1);
    
    // Regulator (Bandgap) Trim Code
    this->REG_BGT = ASICregister(
        "BGT", p, 0x20, 0x7F, convert_uint, mem, 1);
    
    // Bandgap chopping
    this->REG_BGCH = ASICregister(
        "BGCH", p, 0x20, 0x80, convert_bit, mem, 1);
    
    // Temperature Compensation Codes
    this->REG_ACCEL_TCX = ASICregister(
        "ACCEL_TCX", p, 0x21, all, convert_uint, mem, 1);
    this->REG_ACCEL_TCY = ASICregister(
        "ACCEL_TCY", p, 0x22, all, convert_uint, mem, 1);
    this->REG_ACCEL_TCZ = ASICregister(
        "ACCEL_TCZ", p, 0x23, all, convert_uint, mem, 1);
    
    // Tilt Position Threshold and Gain
    this->REG_LL = ASICregister(
        "LL", p, 0x28, all, convert_uint, mem, 1);
    this->REG_HL = ASICregister(
        "HL", p, 0x29, all, convert_uint, mem, 1);
    this->REG_TPGC = ASICregister(
        "TPGC", p, 0x2A, 0x3F, convert_byte, mem, 1);
    
    // Low Power Control
    this->REG_LP_CNTL = ASICregister(
        "LP_CNTL", p, 0x2B, all, convert_byte, mem, 1);
    
    // More Accel Output Data Control (OUT_CNTL)
    this->REG_ACCEL_CNTL = ASICregister(
        "ACCEL_CNTL", p, 0x2C, 0xF3, convert_byte, mem, 1);
    
    // Offset Calibration
    this->REG_XCAL = ASICregister(
        "XCAL", p, 0x2D, all, convert_uint, mem, 1);
    this->REG_YCAL = ASICregister(
        "YCAL", p, 0x2E, all, convert_uint, mem, 1);
    this->REG_ZCAL = ASICregister(
        "ZCAL", p, 0x2F, all, convert_uint, mem, 1);
    
    // Output Data Rate
    this->REG_ACCEL_ODR = ASICregister(
        "ACCEL_ODR", p, 0x2F, 0x0F, convert_uint, mem, 1);
    
    // Spare registers for Analog
    this->REG_REV_1 = ASICregister(
        "REV_1", p, 0x30, all, convert_byte, mem, 1);
    this->REG_REV_2 = ASICregister(
        "REV_2", p, 0x31, all, convert_byte, mem, 1);
    
    // WUF and BTS
    this->REG_WUFTH = ASICregister(
        "WUFTH", p, 0x32, all, convert_byte, mem, 1);
    this->REG_BTSTH_WUFTH = ASICregister(
        "BTSTH_WUFTH", p, 0x33, 0x77, convert_byte, mem, 1);
    this->REG_BTSTH = ASICregister(
        "BTSTH", p, 0x34, all, convert_byte, mem, 1);
    this->REG_BTSC = ASICregister(
        "BTSC", p, 0x35, all, convert_byte, mem, 1);
    this->REG_WUFC = ASICregister(
        "WUFC", p, 0x36, all, convert_byte, mem, 1);
    
    // Spare registers
    this->REG_SPARE_2 = ASICregister(
        "SPARE_2", p, 0x37, all, convert_byte, mem, 1);
    this->REG_SPARE_3 = ASICregister(
        "SPARE_3", p, 0x38, all, convert_byte, mem, 1);
    this->REG_SPARE_4 = ASICregister(
        "SPARE_4", p, 0x39, all, convert_byte, mem, 1);
    this->REG_SPARE_5 = ASICregister(
        "SPARE_5", p, 0x3A, all, convert_byte, mem, 1);
    this->REG_SPARE_6 = ASICregister(
        "SPARE_6", p, 0x3B, all, convert_byte, mem, 1);
    this->REG_SPARE_7 = ASICregister(
        "SPARE_7", p, 0x3C, all, convert_byte, mem, 1);
    this->REG_SPARE_8 = ASICregister(
        "SPARE_8", p, 0x3D, all, convert_byte, mem, 1);
    this->REG_SPARE_9 = ASICregister(
        "SPARE_9", p, 0x3E, all, convert_byte, mem, 1);
    this->REG_SPARE_10 = ASICregister(
        "SPARE_10", p, 0x3F, all, convert_byte, mem, 1);
    
    /****************************************************************
    * Special Register (single bit) setup format: special case only *
    * parent_addr = this->REG_PARENT.addr[0];                       *
    * special_mask = 0x55;                                          *
    * this->REG_NAME = ASICregister(                                *
    *   name, page, parent_addr, mask, conversion, memory, number); *
    ****************************************************************/
    
    parent_addr = this->REG_EXTRA.addr[0];
    special_mask = 0x20;
    this->REG_SAD1 = ASICregister(
        "SAD1", p, parent_addr, special_mask, convert_byte, mem, 1);
    
    parent_addr = this->REG_ACCEL_CNTL.addr[0];
    special_mask = 0x03;
    this->REG_ACCEL_SWAP = ASICregister(
        "ACCEL_SWAP", p, parent_addr, special_mask, convert_byte, mem, 1);
    
    byte snArray[APP_MAX_ADDR];
    memset(snArray, ADDR_INVALID, sizeof(snArray));
    memset(mask, 0xFF, sizeof(mask));
    snArray[0] = 0x14;
    snArray[1] = 0x15;
    snArray[2] = 0x16;
    snArray[3] = 0x17;
    snArray[4] = this->REG_SPARE_2.addr[0];
    mask[5] = 0x00;
    this->REG_SN = ASICregister(
        "SN_RAM", p, snArray, mask, convert_SN, mem, MAX_SN_SIZE);
    
    // special test register for development: must be read/write and 2 bytes
    // for the BF, using the ACCEL_FOX bytes
    byte testRegArray[APP_MAX_ADDR];
    byte testMask[APP_MAX_ADDR];
    memset(testRegArray, ADDR_INVALID, sizeof(testRegArray));
    memset(testMask, 0x00, sizeof(testMask));
    testRegArray[0] = 0x03;
    testRegArray[1] = 0x06;
    testMask[0] = 0xFF;
    testMask[1] = 0xFF;
    this->REG_TEST = ASICregister(
        "TEST", p, testRegArray, testMask, convert_uint, mem, 0, 1);
    
    /******************************************************************
    * RawRAM Register setup format: start address and number of bytes *
    * this->RawRAM = ASICregister(                                    *
    *   "RawRAM", p, RAM_start, all, conversion, mem, NUM_RAM_REG);   *
    ******************************************************************/
    
    this->RawRAM = ASICregister(
        "RawRAM", p, 0x00, all, convert_byte, mem, NUM_RAM_REG);
    
    /******************************************************************
    * RAMregisters struct setup format: Include all RAM registers     *
    * that are a full byte and ensure all registers accounted for.    *
    * Order does not matter since Add sorts them each time.           *
    * this->RAMregisters.Add(this->REG_RAM_STARTING_BYTE);            *
    * this->RAMregisters.Add(this->REG_RAM_SECOND_BYTE);              *
    * ...                                                             *
    * this->RAMregisters.Add(this->REG_RAM_ENDING_BYTE);              *
    * if (this->RAMregisters.Size != NUM_RAM_REG)                     *
    *   ERRCrit(ERROR_INIT, "RAMregisters is not the proper size!"); *
    ******************************************************************/
    
    this->RAMregisters.Add(this->REG_OSC);
    this->RAMregisters.Add(this->REG_SPARE_1);
    this->RAMregisters.Add(this->REG_WDT);
    this->RAMregisters.Add(this->REG_ACCEL_STX);
    this->RAMregisters.Add(this->REG_ACCEL_STY);
    this->RAMregisters.Add(this->REG_ACCEL_STZ);
    this->RAMregisters.Add(this->REG_WAI);
    this->RAMregisters.Add(this->REG_EXTRA);
    this->RAMregisters.Add(this->REG_BGT);
    this->RAMregisters.Add(this->REG_BGCH);
    this->RAMregisters.Add(this->REG_ACCEL_TCX);
    this->RAMregisters.Add(this->REG_ACCEL_TCY);
    this->RAMregisters.Add(this->REG_ACCEL_TCZ);
    this->RAMregisters.Add(this->REG_SN);
    this->RAMregisters.Add(this->REG_LL);
    this->RAMregisters.Add(this->REG_HL);
    this->RAMregisters.Add(this->REG_TPGC);
    this->RAMregisters.Add(this->REG_LP_CNTL);
    this->RAMregisters.Add(this->REG_ACCEL_CNTL);
    this->RAMregisters.Add(this->REG_XCAL);
    this->RAMregisters.Add(this->REG_YCAL);
    this->RAMregisters.Add(this->REG_ZCAL);
    this->RAMregisters.Add(this->REG_REV_1);
    this->RAMregisters.Add(this->REG_REV_2);
    this->RAMregisters.Add(this->REG_WUFC);
    this->RAMregisters.Add(this->REG_WUFTH);
    this->RAMregisters.Add(this->REG_BTSC);
    this->RAMregisters.Add(this->REG_BTSTH);
    this->RAMregisters.Add(this->REG_BTSTH_WUFTH);
    this->RAMregisters.Add(this->REG_SPARE_3);
    this->RAMregisters.Add(this->REG_SPARE_4);
    this->RAMregisters.Add(this->REG_SPARE_5);
    this->RAMregisters.Add(this->REG_SPARE_6);
    this->RAMregisters.Add(this->REG_SPARE_7);
    this->RAMregisters.Add(this->REG_SPARE_8);
    this->RAMregisters.Add(this->REG_SPARE_9);
    this->RAMregisters.Add(this->REG_SPARE_10);
    this->RAMregisters.Add(this->REG_SAD1);
}

/******************************************************************************
    Name:   SetupPage2
    Desc:   Defines the registers on Page 10 of the BF ASIC
******************************************************************************/
void CBF::SetupPage2(void)
{
    byte p = PAGE_02;
    byte all = 0xFF;
    memory_type mem = ROM;
    
    /*********************************************************
    * Register setup format: Include all useful registers    *
    * this->REG_NAME = ASICregister(                         *
    *   name, page, addr, mask, conversion, memory, number); *
    *********************************************************/
    
    this->RawROM = ASICregister(
        "RawROM", p, 0x00, all, convert_byte, mem, NUM_ROM_REG);
    
    byte snArray[APP_MAX_ADDR];
    byte mask[APP_MAX_ADDR];
    memset(snArray, ADDR_INVALID, sizeof(snArray));
    memset(mask, 0xFF, sizeof(mask));
    snArray[0] = 0x10;
    snArray[1] = 0x11;
    snArray[2] = 0x12;
    snArray[3] = 0x13;
    snArray[4] = 0x24;
    mask[5] = 0x00;
    this->REG_SN_ROM = ASICregister(
        "SN_ROM", p, snArray, mask, convert_SN, mem, MAX_SN_SIZE);
}

