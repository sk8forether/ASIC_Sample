/******************************************************************************

    File:   Testbench.cpp
    Desc:   Testbench is a component in Level 2 of the design that contains a 
            set of useful debugging tools.

******************************************************************************/
#include "Testbench.h"
#include "FileIO.h"

HINSTANCE CTestbench::hDLL = NULL;
dllRegViewType CTestbench::ImageToPage00 = NULL;
dllRegViewType CTestbench::ImageToPage01 = NULL;

/******************************************************************************
    Name:   CTestbench
    Desc:   Default constructor
******************************************************************************/
CTestbench::CTestbench(void)
{
}

/******************************************************************************
    Name:   ~CTestbench
    Desc:   Default destructor
******************************************************************************/
CTestbench::~CTestbench(void)
{
    //free LV dlls from memory
    FreeLibrary(CTestbench::hDLL);
}

/******************************************************************************
    Name:   Init
    Desc:   Initialize the testbench
******************************************************************************/
void CTestbench::Init(int TestModuleDebug)
{
    DBGTestModule = TestModuleDebug;

    CFileIO* WriteFileTo = CFileIO::GetInstance();
    WriteFileTo->Open(FILEIO_WRITE,"Output.txt");

    String enabledStr;
    String msg;
    String time;
    CUtilities::GetTime(time);
    sprintf(msg, "Output started at: %s\n", (char *)time);
    WriteFileTo->WriteLine(msg);

    if (DBGTestModule)
    {
        String trace = "--> CTestbench::Init";
        CEngTools::DebugTrace(trace);
        CEngTools::DisplayToOutputWindow("",trace);
        WriteFileTo->WriteLine(trace);
    }

    // Enable testbench
    CTestbench::SetEnabledState(ENABLED);

    String init = "Initializing Testbench";
    CEngTools::DebugPrint(init);

    // Determine if real hardware available
    std::string fileName("C:\\KionixProductionTest\\Global\\RealHardwareAvailable.txt" );
    ifstream fin( fileName.c_str() );

    int RealHardwareAvailable = !fin.fail();
    DBGPrint(msg);
    DBGSimulate = !RealHardwareAvailable;

    // Enable Visual Studio Output window
    enabledStr = (CTestbench::outputWindowEnabled == ENABLED)? "enabled" : "disabled";
    sprintf(msg, "\tVisual Studio Output window now %s", (char *) enabledStr);
    
    // Enable DBGTrace
    enabledStr = (CTestbench::DebugTraceEnabled == ENABLED)? "enabled" : "disabled";
    sprintf(msg, "%s, DBGTrace now %s, \n\t", (char *)msg, (char *) enabledStr);

    // Enable DBGPrint
    enabledStr = (CTestbench::DebugPrintEnabled == ENABLED)? "enabled" : "disabled";
    sprintf(msg, "%sDBGPrint now %s", (char *)msg, (char *) enabledStr);

    // Enable DBGVerbose
    enabledStr = (CTestbench::DebugVerboseEnabled == ENABLED)? "enabled" : "disabled";
    sprintf(msg, "%s, DBGVerbose now %s", (char *)msg, (char *) enabledStr);

    // Enable Log file
    enabledStr = (CTestbench::logFileEnabled == ENABLED)? "enabled" : "disabled";
    sprintf(msg, "%s,\n\tOutput to log file now %s.", (char *)msg, (char *) enabledStr);
    DBGPrint(msg);

    //DBGTrace("Start Dll init");
    
    // LabVIEW dll setup
    /*LPCWSTR dllName = L"LVViewRegisters.dll";  // L coerces it into a long str
    CTestbench::hDLL = LoadLibrary(dllName);
    
    if (CTestbench::hDLL == NULL) // try relative path in software library
    {
        dllName = LV_REGVIEW_PATH;
        CTestbench::hDLL = LoadLibrary(dllName);
    }
    
    if (CTestbench::hDLL == NULL)
        ERRLog("Did not load LVViewRegisters.dll");
    else //get the dll functions from the dll
    {
        CTestbench::ImageToPage00 = (dllRegViewType)GetProcAddress(CTestbench::hDLL, "ImageToPage00");
        CTestbench::ImageToPage01 = (dllRegViewType)GetProcAddress(CTestbench::hDLL, "ImageToPage01");
    }*/

    //DBGTrace("After LV Dll stuff");
    
    DBGPrint("Initialization done.");
}

/******************************************************************************
    Name:   End
    Desc:   End the TestBench
******************************************************************************/
void CTestbench::End(void)
{
    //free LV dlls from memory
    FreeLibrary(CTestbench::hDLL);
    
    DBGTrace("--> CTestbench::End");
    CFileIO* WriteFileTo = CFileIO::GetInstance();
    
    String msg;
    String time;
    CUtilities::GetTime(time);
    sprintf(msg, "Output ended at: %s", (char *)time);
    WriteFileTo->Write(msg);
    
    WriteFileTo->Close();
}

/******************************************************************************
    Name:   SetEnabledState
    Desc:   Enable or disable Testbench
******************************************************************************/
void CTestbench::SetEnabledState(byte state)
{
    DBGTrace("--> CTestbench::SetEnabledState");
    
    CEngTools::enabled = (state == ENABLED) ? true : false;
    
    String msg;
    String enabledStr = (CEngTools::enabled == ENABLED)? "enabled" : "disabled";
    sprintf(msg, "Testbench now %s.\n", (char *) enabledStr);
    DBGPrint(msg);
}


/******************************************************************************
    Name:   LVImageView
    Desc:   Displays a front panel with the input image decoded into the 
            validation register map display.
            Single input string version calls version that takes two.
            Page input specifies which dll function to call.
******************************************************************************/
int CTestbench::LVImageView(char* Image1, byte page)
{
    return LVImageView(Image1, "", page);
}

/******************************************************************************
    Name:   LVImageView
    Desc:   Displays a front panel with the input images decoded into the 
            validation register map display.
            Takes two images to allow visual comparison.
            Page input specifies which dll function to call.
******************************************************************************/
int CTestbench::LVImageView(char* Image1, char* Image2, byte page)
{
    if (CTestbench::hDLL == NULL)
    {
        ERRChk(ERROR_UNIMPLEMENTED, "ImageToPage00 and ImageToPage01 not implemented for this ASIC");
        return ERROR_UNIMPLEMENTED;
    }
    
    if (page > NUM_PAGES)
    {
        ERRChk(ERROR_UNDEFINED, "Requested page not available in this ASIC.", "CTestBench::LVImageView");
        return ERROR_UNDEFINED;
    }

    switch (page)
    {
        case 0:
            CTestbench::ImageToPage00(Image1, Image2);
            break;
        case 1:
            CTestbench::ImageToPage01(Image1, Image2);
            break;
        default:
            ERRChk(ERROR_UNIMPLEMENTED, "Requested page unimplimented.", "CTestBench::LVImageView");
            return ERROR_UNIMPLEMENTED;
    }

    return SUCCESS;
}