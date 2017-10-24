/******************************************************************************

    File:   ISMECA.cpp
    Desc:   ISMECA is a component in Level 2 of the design that controls all
            the interaction with the physical ISMECA tool that contains and
            tests DUTs.  The ISMECA tools include 8, 9, and 13-series.  For
            more information on the ISMECA tools, see the wiki page:
            https://kionix.sharepoint.com/sites/swdevtest/tools/ismeca

******************************************************************************/
#include "ISMECA.h"

#ifdef _ISMECA_

CISMECA *CISMECA::Instance = NULL;

/******************************************************************************
    Name:   CISMECA
    Desc:   Default constructor
******************************************************************************/
CISMECA::CISMECA()
{
    #if defined(_8SERIES_x16_) || defined(_13SERIES_)
        #include "PXICardSetup_8_13.h"
    #endif
    
    #ifdef _9SERIES_x12_
        #include "PXICardSetup_9.h"
    #endif
}

/******************************************************************************
    Name:   ~CISMECA
    Desc:   Default destructor
******************************************************************************/
CISMECA::~CISMECA(void)
{
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
CISMECA *CISMECA::GetInstance(void)
{
    if (CISMECA::Instance == NULL)
        CISMECA::Instance = new CISMECA();
    
    return CISMECA::Instance;
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the ISMECA class and subclasses
******************************************************************************/
int CISMECA::Init(PinUsageStruct pinUsage, int series, int unit, int uses_VPP, word* listDut)
{
    DBGTrace("--> CISMECA::Init");
    DBGPrint("Initializing the ISMECA module...");
    
    CTool::Init(pinUsage, series, unit, ismeca_type, uses_VPP, listDut);
    
    //memcpy(this->PSSessions, PSSessions, sizeof(PSSessions));
    //this->PowerSupply->Init(this->PSSessions, uses_VPP);
    
    //this->Pin->Init(pinUsage.pin_out, this->DutCntl, this->AnalogIn);
    //this->Pin->SetPowerSupply(this->PowerSupply);
    //CPatternHS::GetInstance()->CommIn(this->CommGroup);
    
    return SUCCESS;
}

/******************************************************************************
    Name:   Close
    Desc:   Closes the Tool module, which entails turning off the power supply
            and diconnecting the pins for the session.
******************************************************************************/
int CISMECA::Close(void)
{
    DBGTrace("--> CISMECA::Close");
    
    this->PowerSupply->Disconnect();
    
    // Clear Relay tasks
    this->Pin->Disconnect();
    
    return SUCCESS;
}

/******************************************************************************
    Name:   PowerOnPart
    Desc:   
******************************************************************************/
int CISMECA::PowerOnPart(double vdd, word* listDut)
{
    DBGTrace("--> CISMECA::PowerOnPart");
    
    return this->PowerSupply->PowerOnPart(vdd, listDut);
}

/******************************************************************************
    Name:   PowerOffPart
    Desc:   
******************************************************************************/
int CISMECA::PowerOffPart(word* listDut)
{
    DBGTrace("--> CISMECA::PowerOffPart");
    
    return this->PowerSupply->PowerOffPart(listDut);
}

/******************************************************************************
    Name:   SetBurnVoltage
    Desc:   
******************************************************************************/
int CISMECA::SetBurnVoltage(double vdd) 
{
    
    return SUCCESS;
}

#endif