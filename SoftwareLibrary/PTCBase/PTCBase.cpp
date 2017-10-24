/******************************************************************************

    File:   PTCBase.cpp
    Desc:   PTCBase is a subcomponent in Level 3 of the design that contains
            the generic functionality necessary for the Product Test Control
            class for any Product Solution.

******************************************************************************/
#include "PTCBase.h"

PTCBase *PTCBase::Instance = NULL;

/******************************************************************************
    Name:   PTCBase
    Desc:   Default constructor
******************************************************************************/
PTCBase::PTCBase()
{
    memset(this->listDut, 0, sizeof(this->listDut));
    this->ASIC = NULL;
    this->Tool = NULL;
}

/******************************************************************************
    Name:   ~PTCBase
    Desc:   Default destructor
******************************************************************************/
PTCBase::~PTCBase(void)
{
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
PTCBase *PTCBase::GetInstance(void)
{
    if (PTCBase::Instance == NULL)
        PTCBase::Instance = new PTCBase();
    
    return PTCBase::Instance;
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the PTCBase class
******************************************************************************/
void PTCBase::Init(void)
{
    DBGTrace("-> PTCBase::Init");

    this->PTCInfo.Spec.Name = SPEC_NAME;
    this->PTCInfo.Spec.Default_Image = DEFAULT_IMAGE;
    this->PTCInfo.Spec.Default_Mask = DEFAULT_MASK;
    this->PTCInfo.Spec.slave_addr = (byte) CUtilities::BinaryStringToLong(SLAVE_ADDRESS,8);
    this->PTCInfo.Spec.op_vdd = OPERATING_VOLTAGE;
    
    int BO = BUILD_ORDER;
    int reel = REEL;
    int TIndex = TINDEX;
    String mode = "Engineering";
    
    strcpy_s((char *) PTCInfo.productName, APP_MAX_CHAR, (char *) this->PTCInfo.Spec.Name);
    PTCInfo.productName.TrimToLength(5);
    
#ifdef _ACCEL_ENABLED_
    this->PTCInfo.Spec.Accel_Max_Count          = SPEC_ACCEL_MAXCOUNT;
    
#endif
    
    // Initialize listDut, Tool, ASIC, and Test (removed)
    this->Init_listDut();
    this->Init_Tool();
    this->Init_ASIC();
}

/******************************************************************************
    Name:   Init_listDut
    Desc:   Initializes the listDut array, which keeps track of the active DUTs
******************************************************************************/
int PTCBase::Init_listDut(void)
{
    DBGTrace("-> PTCBase::Init_listDut");
    
    word listDut_array[TOOL_MAX_DUT + 1];// = {1, 3, 5, 7, 9, 11, 0};
    memset(listDut_array, 0, sizeof(listDut_array));
    
    for (int i = 0; i < TOOL_MAX_DUT; i++)
        listDut_array[i] = i + 1;
    listDut_array[3] = 0;
    
    memcpy(this->listDut, listDut_array, TOOL_MAX_DUT + 1);
    
    if (this->listDut[0] != 0)
        return SUCCESS;
    else
    {
        DBGPrint("WARNING: There are no duts enabled in the listDut array.");
        return ERROR_INIT;
    }
}

/******************************************************************************
    Name:   Init_Generic_Tool
    Desc:   Initializes the generic feature of the Tool module, which controls
            all interaction with the tool
******************************************************************************/
int PTCBase::Init_Generic_Tool(void)
{
    DBGTrace("-> PTCBase::Init_Tool");
    
    if (DBGSimulate == YES)
        ERRChk(SIMULATE_HARDWARE, "", "PTC::Init_Tool");
    
#ifdef _BURN_WITH_VPP_
    if ((this->PTCInfo.testFlow == PROGRAMMING) &&
        (this->PTCInfo.PinUsage.pin_out.hasVPP == false))
    {
        ERRLog("Cannot burn with VPP with a socket board does not support VPP");
        return ERROR_INIT;
    }
#endif
    
    String toolName;
    this->Tool->GetToolName(toolName);
    toolNameD = this->Tool->GetToolName();
    series = this->Tool->GetSeries();
    unit = this->Tool->GetUnit();

    this->Tool->PowerSupply->Connect();
    
    // TODO: Get operating voltage from Spec
    this->Tool->SetOpVDD(this->PTCInfo.Spec.op_vdd);
    
    Tool->Pin->SetRelays(listDut);
    
    DBGPrint("Connect Comm for I2C: (This takes longer the first time.)");
    Tool->ConnectComm(COM_I2C, listDut);
    
    Tool->LoadAllPatterns(this->PTCInfo.Spec.slave_addr, listDut);
    
    DBGPrint("Initialization done.");

    return SUCCESS;
}

/******************************************************************************
    Name:   DisplayProductToTest
    Desc:   Returns a string to display which product is now being tested
******************************************************************************/
String PTCBase::DisplayProductToTest(void)
{
    String msg;
    sprintf(msg,"Now testing the %s(%s) with Spec %s(%i), with the %s testflow, on the %s socket board.\n", PROD_NAME, (char *) ASIC->Name, SPEC_NAME, SPEC_INDEX, (char *) CTestbench::TestFlowToString(this->PTCInfo.testFlow), this->PTCInfo.PinUsage.pin_out.name);
    
    return msg;
}