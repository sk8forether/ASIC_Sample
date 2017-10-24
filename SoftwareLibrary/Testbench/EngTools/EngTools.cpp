/******************************************************************************

    File:   EngTools.cpp
    Desc:   EngToolss is a component in Level 1 of the design that contains a 
            set of useful debugging tools.

******************************************************************************/
#include "EngTools.h"

bool CEngTools::enabled = NO;                   // Enables Testbench
bool CEngTools::outputWindowEnabled = NO;       // Enables Writing to VS Output window
bool CEngTools::DebugTraceEnabled = NO;         // Enables DBGTrace function
bool CEngTools::DebugPrintEnabled = YES;        // Enables DBGPrint function
bool CEngTools::DebugVerboseEnabled = NO;       // Enables DBGVerbose function
bool CEngTools::logFileEnabled = YES;           // Enables Writing to Output.txt file
int CEngTools::DebugVerify = ENG_LEVEL_0;       // Enables Verification of read/write from part
bool CEngTools::simulate = NO;                  // This is superceded by a RealHardwareAvailable file check in CTestbench::Init
int CEngTools::DebugTestModule = NO;            // Used for tes/dev of Test Module

// Debug/dev listDut, used by Ack verification
word CEngTools::DebugListDut[TOOL_MAX_DUT + 1] = {0, 0};

/******************************************************************************
    Name:   CEngTools
    Desc:   Default constructor
******************************************************************************/
CEngTools::CEngTools(void)
{
    // 
}

/******************************************************************************
    Name:   ~CEngTools
    Desc:   Default destructor
******************************************************************************/
CEngTools::~CEngTools(void)
{
}

/******************************************************************************
    Name:   DebugListDut
    Desc:   Set the DebugListDut array equal to the values in the specified listDut array
******************************************************************************/
void CEngTools::Debug_SetDebugListDut(word* listDut)
{
    memcpy(CEngTools::DebugListDut, listDut, TOOL_MAX_DUT + 1);
}

/******************************************************************************
    Name:   DebugPrint
    Desc:   Display debug output
******************************************************************************/
void CEngTools::DebugPrint(char* msg)
{
    if (CEngTools::enabled && CEngTools::DebugPrintEnabled)
    {
        cout << "" << msg << endl;
        DisplayToOutputWindow("", msg);
        CEngTools::WriteToLogFile("",msg);
    }    
}

/******************************************************************************
    Name:   DebugVerbose
    Desc:   Display verbose debug output if Testbench is enabled
******************************************************************************/
void CEngTools::DebugVerbose(char* msg)
{
    if (CEngTools::enabled && DBGVerboseEnabled)
    {
        cout << "" << msg << endl;
        DisplayToOutputWindow("", msg);
        CEngTools::WriteToLogFile("",msg);
    }
}

/******************************************************************************
    Name:   DebugTrace
    Desc:   Display debug output for tracing function calls if Testbench is
            enabled
******************************************************************************/
void CEngTools::DebugTrace(char* msg)
{
    if (CEngTools::enabled && CEngTools::DebugTraceEnabled)
    {
        cout << "" << msg << endl;
        DisplayToOutputWindow("", msg);
        CEngTools::WriteToLogFile("",msg);
    }
}

/******************************************************************************
    Name:   WriteToLogFile
    Desc:   Save output to log file
******************************************************************************/
void CEngTools::WriteToLogFile(char* optionalLabel, char* msg)
{
    if (CEngTools::enabled && CEngTools::logFileEnabled)
    {
        bool origEnabled = DBGTraceEnabled;
        DBGTraceEnabled = NO;
        
        CFileIO* WriteFileTo = CFileIO::GetInstance();
        WriteFileTo->Write(optionalLabel);
        WriteFileTo->WriteLine(msg);
        
        DBGTraceEnabled = origEnabled;
    }
}

/******************************************************************************
    Name:   DisplayToOutputWindow
    Desc:   Display debug output for tracing function calls if Testbench is 
            enabled
******************************************************************************/
void CEngTools::DisplayToOutputWindow(char* optionalLabel, char* msg)
{
    if (CEngTools::enabled && CEngTools::outputWindowEnabled)
    {
        String str;
        sprintf(str, "%s%s\n", optionalLabel, msg);
        OutputDebugStringA(str);
    }
}

/******************************************************************************
    Name:   TestFlowToString
    Desc:   Returns a String representation of the current testflow
******************************************************************************/
String CEngTools::TestFlowToString(int testFlow)
{
    DBGTrace("---> CEngTools::TestFlowToString");
    
    String msg;
    
    switch(testFlow)
    {
        case PROGRAMMING:
            msg = "Programming";
            break;
        case READONLY:
            msg = "ReadOnly";
            break;
        case AUDIT:
            msg = "Audit";
            break;
        default:
            msg = "Unknown testflow";
            break;
    }
    
    return msg;
}

/******************************************************************************
    Name:   DisplayTestFlow
    Desc:   Display currently running testflow
******************************************************************************/
void CEngTools::DisplayTestFlow(const char* testName, int testFlow)
{
    if (DBGTraceEnabled)
        DBGPrint("---> CEngTools::DisplayTestFlow");
    
    if (DBGTraceEnabled)
    {
        String msg;
        sprintf(msg,"        Running %s test using %s testflow.", testName, (char *) CTestbench::TestFlowToString(testFlow));
        DBGTrace(msg);
    }
}

/******************************************************************************
    Name:   PrintListDut
    Desc:   Display the list of duts specified in the argument listDut
******************************************************************************/
void CEngTools::PrintListDut(char *label, word* listDut, bool printAll)
{
    DBGTrace("---> CEngTools::PrintListDut");
    
    if (listDut == NULL)
        return;
    else
        if (listDut[0] == 0)
            DBGPrint("WARNING: There are not duts enabled in the listDut array.");
        
    String msg;
    String tmp;
    
    sprintf(msg,"%20s: \t\t\t  ", label);
    for (int d = 0; 
        (listDut[d] != 0) || (printAll && d < TOOL_MAX_DUT); 
        d++)
    {
        sprintf(tmp, "%7i", listDut[d]);
        strcat(msg, (char *)tmp);
        if (listDut[d + 1] != 0)
            strcat(msg, ", ");
    }
    DBGPrint(msg);
}
