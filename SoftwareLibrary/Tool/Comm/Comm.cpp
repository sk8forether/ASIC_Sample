/******************************************************************************

    File:   Comm.cpp
    Desc:   Comm is a component in Level 2 of the design that controls all 
            of the communciation that occurs between the tool and DUTs, on a
            variety of protocols including: I2C, I3C, SPI3, and SPI4.  The PLU
            method is handled within the SPEA class.

******************************************************************************/
#include "Comm.h"

Comm *Comm::Instance = NULL;

/******************************************************************************
    Name:   Comm
    Desc:   Default constructor
******************************************************************************/
Comm::Comm()
{
    this->Pin = CPin::GetInstance();
    this->CurrCom = COM_INVALID;
    this->SetCommMethod(COM_METHOD_INVALID);
    this->I2CRelayState = 0;
    
    this->Pattern = NULL;
    
    #ifdef _SPEA_
        this->PLU = NULL;
    #endif
}

/******************************************************************************
    Name:   ~Comm
    Desc:   Default destructor
******************************************************************************/
Comm::~Comm(void)
{
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
Comm *Comm::GetInstance(void)
{
    if (Comm::Instance == NULL)
        Comm::Instance = new Comm();
    
    return Comm::Instance;
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the Comm class and subclasses
******************************************************************************/
void Comm::Init(PinUsageStruct pinUsage, word* listDut)
{
    DBGTrace("---> Comm::Init");
    
    DBGPrint("\tInitializing the Comm module");
    
    memcpy(&this->pinUsage, &pinUsage, sizeof(pinUsage));
    
    #ifdef _SPEA_
        // TODO: Some logic to determine whether or now we default to PLU
        this->SetCommMethod(COM_METHOD_PLU);
        return;
    #endif
    
    forISMECASetThirdLineHigh = LOW;
    
    this->SetCommMethod(COM_METHOD_PATT);
    
    // TODO: pick speed based on static or dynamic site
    this->Pattern = new CPattern();
    this->Pattern->Init(PATT_HIGH_SPEED);

    #ifdef _SPEA_
        this->PLU = new CPLU();
    #endif
}

/******************************************************************************
    Name:   ConnectComm
    Desc:   
******************************************************************************/
int Comm::ConnectComm(word com, word* listDut, int method)
{
    DBGTrace("---> Comm::ConnectComm");
    
    // set communication method
    if (method == COM_METHOD_PLU)
    {
        #ifndef _SPEA_
            ERRLog("Attempting to use PLU when _SPEA_ is not defined!!");
            return ERROR_UNDEFINED;
        #else
            this->SetCommMethod(COM_METHOD_PLU);
        #endif
    }
    else if (method == COM_METHOD_PATT)
    {
        this->SetCommMethod(COM_METHOD_PATT);
    }
    else
    {
        ERRLog("Incorrect Communication Method attempted in Comm::Read");
        return ERROR_COMMUNICATION;
    }
    
    // initialize variable
    forISMECASetThirdLineHigh = LOW;
    
    // set communication protocol
    
    word prevCom = GetCom();
    if (prevCom != com)
    {
        // TODO: test this
        // if previously was in I2C, save the I2C Pin Relay state
        if ((prevCom == COM_I2C) || (prevCom == COM_I3C))
            this->I2CRelayState = this->Pin->GetRelayState();
        
        // if going into I2C, restore the I2C Pin Relay state
        if ((com == COM_I2C) || (com == COM_I3C))
            this->Pin->SetAllRelays(this->I2CRelayState, listDut);
    }
    
    switch(com)
    {
        case COM_INVALID:
        {
            ERRLog("Invalid communication protocol selected in ConnectComm");
            return ERROR_COMMUNICATION;
        }
        case COM_I2C:
        {
            this->SetCom(com);
            this->Pattern->Connect(com, listDut);
            
#ifdef _ISMECA_
            // Select I2C mode on socket board
            this->Pin->SetSBControl(LOW, listDut);
            
            if (this->pinUsage.pin_out.nCSConnectedInI2C == TRUE)
                forISMECASetThirdLineHigh = HIGH;
#endif
            
            return SUCCESS;
        }
        case COM_I3C:
        {
            this->SetCom(com);
            
#ifdef _ISMECA_
            // Select I2C mode on socket board
            this->Pin->SetSBControl(LOW, listDut);
            
            if (this->pinUsage.pin_out.nCSConnectedInI2C == TRUE)
                forISMECASetThirdLineHigh = HIGH;
#endif
            
            return SUCCESS;
            
            // TODO: Connect to appropriate pins here?
        }
        case COM_SPI3:
        {
#ifdef _ISMECA_
            // Select SPI mode on socket board
            this->Pin->SetSBControl(HIGH, listDut);
            
            // Select SPI3 mode on socket board
            this->Pin->SetAddrPin(LOW, listDut);
#endif
            
            this->SetCom(com);
            return SUCCESS;
            
            // TODO: Connect to appropriate pins here?
        }
        case COM_SPI4:
        {
            
#ifdef _ISMECA_
            // Select SPI mode on socket board
            this->Pin->SetSBControl(HIGH, listDut);
            
            // Select SPI4 mode on socket board
            this->Pin->SetAddrPin(HIGH, listDut);
#endif
            
            this->SetCom(com);
            return SUCCESS;
            
            // TODO: Connect to appropriate pins here?
        }
        default:
        {
            ERRLog("Invalid communication protocol selected in ConnectComm");
            return ERROR_COMMUNICATION;
        }
    }
    
    return ERROR_UNIMPLEMENTED;
}

/******************************************************************************
    Name:   DisconnectComm
    Desc:   
******************************************************************************/
int Comm::DisconnectComm(word* listDut)
{
    DBGTrace("---> Comm::DisconnectComm");
    
    // TODO: disconnect from appropriate pins here based on CurrCom
    
    this->SetCom(COM_INVALID);
    this->SetCommMethod(COM_METHOD_INVALID);
    
    return ERROR_UNIMPLEMENTED;
}

/******************************************************************************
    Name:   ResolveAddr
    Desc:   For SPI3 and SPI4, asserts or clears the read bit for the read and
            write actions respectively
******************************************************************************/
void Comm::ResolveAddr(byte reg_addr, int action)
{
    DBGTrace("---> Comm::ResolveAddr");
    
    if ( (this->CurrCom == COM_SPI3) || (this->CurrCom == COM_SPI4) )
    {
        if (action == WRITE)
            reg_addr &= 0x7F;
        else if (action == READ)
            reg_addr |= 0x80;
        else
            ERRLog("Incorrect action selected for ResoveAddr for SPI3 or SPI4");
    }
}/******************************************************************************
    Name:   Read
    Desc:   Depending on the current communication method, reads bytes from the
            DUTs at the specified address or addresses.  If PLU is the current
            method, the PLU class is utilized (SPEA only).  If PATT is the
            current method, the Pattern class is utilized for all protocols.
******************************************************************************/
int Comm::Read(byte slave_addr, byte reg_addr, int count, byte* data, word* listDut)
{
    DBGTrace("---> Comm::Read");
    
    if (this->GetCommMethod() == COM_METHOD_PLU)
    {
        #ifndef _SPEA_
            ERRLog("Attempting to use PLU when _SPEA_ is not defined!!");
            return ERROR_UNDEFINED;
        #else
            this->ResolveAddr(reg_addr, READ);
            this->PLU->Read(slave_addr, reg_addr, count, data, listDut);
        #endif
    }
    else if (this->GetCommMethod() == COM_METHOD_PATT)
    {
        #ifdef _SPI_PAGE_TRANSLATION_
            if ( (this->GetCom() == COM_I2C) || (this->GetCom() == COM_I3C) )
            {
                byte temp_addr = this->TranslateAddr(reg_addr);
                reg_addr = temp_addr;
            }
        #endif
        
        int dut, index;
        PatternInfo patt_info_data;
        PatternInfo patt_info_sad;
        byte toSet[TOOL_MAX_DUT];
        byte toGet[TOOL_MAX_DUT];
        
        this->ResolveAddr(reg_addr, READ);
        
        // TODO: Don't hardcode pattern name when pattern list is ready
        char* pattern_name = "GetByteI2C";
        this->Pattern->GetInfo(pattern_name, &patt_info_data);
        this->Pattern->GetInfo(pattern_name, &patt_info_sad, true);
        
        // modify the pattern for the specified slave address
        this->Pattern->ModifySad(pattern_name, slave_addr, listDut, forISMECASetThirdLineHigh);
        
        // for each byte
        for (int i = 0; i < count; i++)
        {
            // set location to read
            memset(toSet, 0, sizeof(toSet));
            for (int d = 0; listDut[d] != 0; d++)
            {
                dut = listDut[d] - 1;
                toSet[dut] = reg_addr + (byte)i;
            }
            
            // send
            // TODO: pick speed based on static or dynamic site
            // 
            
            this->Pattern->Send(patt_info_data, slave_addr, &toSet[0], listDut, forISMECASetThirdLineHigh);
            
            // receive
            memset(toGet, 0, sizeof(toGet));
            this->Pattern->Receive(patt_info_data, slave_addr, &toGet[0], 0, listDut, patt_info_sad);
            
            // store data
            for (int d = 0; listDut[d] != 0; d++)
            {
                dut = listDut[d] - 1;
                index = (i * TOOL_MAX_DUT) + dut;
                data[index] = toGet[dut];
            }
        }
    }
    else
    {
        ERRLog("Incorrect Communication Method attempted in Comm::Read");
        return ERROR_COMMUNICATION;
    }
    
    return SUCCESS;
}

/******************************************************************************
    Name:   Write
    Desc:   Depending on the current communication method, writes bytes to the
            DUTs at the specified address or addresses.  If PLU is the current
            method, the PLU class is utilized (SPEA only).  If PATT is the
            current method, the Pattern class is utilized for all protocols.
******************************************************************************/
int Comm::Write(byte slave_addr, byte reg_addr, int count, byte* data, word* listDut)
{
    DBGTrace("---> Comm::Write");
    
    if (this->GetCommMethod() == COM_METHOD_PLU)
    {
        #ifndef _SPEA_
            ERRLog("Attempting to use PLU when _SPEA_ is not defined!!");
            return ERROR_UNDEFINED;
        #else
            this->ResolveAddr(reg_addr, WRITE);
            this->PLU->Write(slave_addr, reg_addr, count, data, listDut);
        #endif
    }
    else if (this->GetCommMethod() == COM_METHOD_PATT)
    {
        #ifdef _SPI_PAGE_TRANSLATION_
            if ( (this->GetCom() == COM_I2C) || (this->GetCom() == COM_I3C) )
            {
                byte temp_addr = this->TranslateAddr(reg_addr);
                reg_addr = temp_addr;
            }
        #endif
        
        int dut, index;
        PatternInfo patt_info_data;
        PatternInfo patt_info_sad;
        byte toSet[2][TOOL_MAX_DUT];
        
        this->ResolveAddr(reg_addr, WRITE);
        
        // TODO: Don't hardcode pattern name when pattern list is ready
        char* pattern_name = "SetByteI2C";
        this->Pattern->GetInfo(pattern_name, &patt_info_data);
        this->Pattern->GetInfo(pattern_name, &patt_info_sad, true);
        
        // modify the pattern for the specified slave address
        this->Pattern->ModifySad(pattern_name, slave_addr, listDut, forISMECASetThirdLineHigh);
        
        // for each byte
        for (int i = 0; i < count; i++)
        {        
            memset(toSet, 0, sizeof(toSet));
            for (int d = 0; listDut[d] != 0; d++)
            {
                dut = listDut[d] - 1;
                index = (i * TOOL_MAX_DUT) + dut;
                toSet[0][dut] = reg_addr + (byte)i;
                toSet[1][dut] = data[index];
            }
            
            this->Pattern->Send(patt_info_data, slave_addr, &toSet[0][0], listDut, forISMECASetThirdLineHigh);
        }
    }
    else
    {
        ERRLog("Incorrect Communication Method attempted in Comm::Write");
        return ERROR_COMMUNICATION;
    }
    
    return SUCCESS;
}

/******************************************************************************
    Name:   TestMode
    Desc:   Sends the TestModeEnable pattern, which contains an ASIC-specific
            command to enable the test mode of the part.
******************************************************************************/
int Comm::TestMode(word* listDut)
{
    DBGTrace("---> Comm::TestMode");
    
    PatternInfo patt_info;
    
    char* pattern_name = "TestModeEnable";
    this->Pattern->GetInfo(pattern_name, &patt_info);
    
    // special pattern is SPI-like (no slave address)
    this->Pattern->Send(patt_info, NULL, NULL, listDut);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   WritePage
    Desc:   If using the I2C protocol and page translation is necessary (such
            as for BB ASIC), translates from the SPI page (which is what the
            ASICregisters are setup in) to the I2C page before setting the
            page.  Otherwise, just sets the page that was input.
******************************************************************************/
int Comm::WritePage(byte slave_addr, byte mempage, byte page, word* listDut)
{
    DBGTrace("---> Comm::WritePage");
    
    byte pageset[TOOL_MAX_DUT];
    
    #ifdef _SPI_PAGE_TRANSLATION_
        this->CurrPage_SPI = page;
        
        if ( (this->GetCom() == COM_I2C) || (this->GetCom() == COM_I3C) )
            memset(pageset,this->TranslatePage(),sizeof(pageset));
        else
            memset(pageset,page,sizeof(pageset));
    #else
        memset(pageset,page,sizeof(pageset));
    #endif

    this->Write(slave_addr, mempage, 1, &pageset[0], listDut);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   TranslatePage and TranslateAddr
    Desc:   
******************************************************************************/
#ifdef _SPI_PAGE_TRANSLATION_
byte Comm::TranslatePage(void)
{
    DBGTrace("---> Comm::TranslatePage");
    
    double SPI_page, I2C_page;
    
    SPI_page = (double)this->CurrPage_SPI;
    I2C_page = floor( SPI_page/2 );
    
    this->CurrPage_I2C = (byte)I2C_page;
    
    return this->CurrPage_I2C;
}

byte Comm::TranslateAddr(byte addr_SPI)
{
    DBGTrace("---> Comm::TranslateAddr");
    
    byte addr_I2C = addr_SPI;
    
    // one I2C page is equal to 2 SPI pages
    if (this->CurrPage_SPI % 2 == 1)
        addr_I2C += MAX_PAGE_SIZE;
    
    return addr_I2C;
}

#endif