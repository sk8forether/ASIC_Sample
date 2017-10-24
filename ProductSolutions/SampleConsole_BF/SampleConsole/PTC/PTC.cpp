/******************************************************************************

    File:   PTC.cpp
    Desc:   The brains of the operation

******************************************************************************/
#include "PTC.h"

PTC *PTC::Instance = NULL;

/******************************************************************************
    Name:   Main
    Desc:   Where all the action happens
******************************************************************************/
int PTC::Main(void)
{
    //-------------------------------------------------------------
    // PTC Initialization
    try
    {
        this->Init();
    }
    catch(const exception&)
    {
        return ERROR_INIT;
    }
    
    //-------------------------------------------------------------
    // Run Tests
    try
    {
        this->Run_Tests();
    }
    catch(const exception&)
    {
        return ERROR_RUN;
    }
    
    //-------------------------------------------------------------
    // Engineering testing area
    try
    {
        this->Currently_Testing();
    }
    catch(const exception&)
    {
        return ERROR_UNIMPLEMENTED;
    }
    
    //-------------------------------------------------------------
    // End
    
    // Close connection to Tool
    this->Tool->Close();
    
    CTestbench::End();
    
    return SUCCESS;
}

/******************************************************************************
    Name:   PTC
    Desc:   Default constructor
******************************************************************************/
PTC::PTC()
{
    PTCBase();
}

/******************************************************************************
    Name:   ~PTC
    Desc:   Default destructor
******************************************************************************/
PTC::~PTC(void)
{
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the PTC
******************************************************************************/
void PTC::Init(void)
{
    CUtilities::Init();
    CTestbench::Init(this->TestModuleDebug);
    
    // This has to be done after Testbench is enabled
    DBGTrace("-> PTC::Init");
    
    this->PTCInfo.testFlow = READONLY;
    
    int specIndex = SPEC_INDEX;
    String productName = PROD_NAME;
    int BO = BUILD_ORDER;
    int reel = REEL;
    int TIndex = TINDEX;
    String mode = "Engineering";
    
    PTCBase::Init();
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
PTC *PTC::GetInstance(void)
{
    if (PTC::Instance == NULL)
        PTC::Instance = new PTC();
    
    return PTC::Instance;
}

/******************************************************************************
    Name:   Run_Tests
    Desc:   Testflow execution
******************************************************************************/
int PTC::Run_Tests(void)
{
    DBGTrace("-> PTC::Run_Tests");
    
    // Tests normally run here (removed)
    
    return SUCCESS;
}
    
/******************************************************************************
    Name:   Currently_Testing
    Desc:   Put Engineering tests here
******************************************************************************/
int PTC::Currently_Testing(void)
{
    DBGTrace("-> PTC::Currently_Testing");
    
    //-------------------------------------------------------------
    // must connect power and comm to do anything with part
    this->Tool->PowerOnPart(OPERATING_VOLTAGE, this->listDut);
    this->Tool->ConnectComm(COM_I2C, this->listDut);
    this->ASIC->SetSlaveAddr(PTCInfo.Spec.slave_addr, listDut);
    //-------------------------------------------------------------
    
    //TESTING always display
    bool was_on = DBGVerboseEnabled;
    //DBGVerify = YES;
    DBGVerboseEnabled = YES;
    
    // Engineering tests can be performed here
    
    //TESTING
    DBGVerboseEnabled = was_on;
    //-------------------------------------------------------------
    // disconnect and power off after testing
    this->Tool->DisconnectComm(this->listDut);
    this->Tool->PowerOffPart(this->listDut);
    //-------------------------------------------------------------
    
    return SUCCESS;
}

/******************************************************************************
    Name:   Init_ASIC
    Desc:   Initializes the ASIC module, which controls all interaction with
            the device
******************************************************************************/
int PTC::Init_ASIC(void)
{
    DBGTrace("-> PTC::Init_ASIC");
    
    CBF *BF = CBF::GetInstance();
    BF->Init(DEFAULT_IMAGE, DEFAULT_MASK, this->Tool, this->listDut);
    this->ASIC = (CASIC *)BF;
    
    return SUCCESS;
}

/******************************************************************************
    Name:   Init_Tool
    Desc:   Initializes the Tool module, which controls all interaction with
            the tool
******************************************************************************/
int PTC::Init_Tool(void)
{
    DBGTrace("-> PTC::Init_Tool");
    
    ISMECA = CISMECA::GetInstance();
    
#ifdef _ISMECA_4_
    word listDut_array[TOOL_MAX_DUT + 1] = {1, 2, 3, 0};
    
    // copy initialized listDut_array to PTC class listDut array
    memcpy(this->listDut, listDut_array, sizeof(listDut_array));
#endif

    this->Tool = (CTool *)ISMECA;
    ISMECA->Init(this->PTCInfo.PinUsage, 1, 15, this->PTCInfo.PinUsage.pin_out.hasVPP, this->listDut);
    
    this->Init_Generic_Tool();
    
    return SUCCESS;
}

/******************************************************************************
    Name:   Add_Tests
    Desc:   Adds the tests to the Active Tests list in the Test module, so they
            will be run in Run_Tests
******************************************************************************/
int PTC::Add_Tests(void)
{
    DBGTrace("-> PTC::Add_Tests");
    
    return SUCCESS;
}
