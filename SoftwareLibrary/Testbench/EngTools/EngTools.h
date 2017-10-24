/******************************************************************************

    File:   EngTools.h
    Desc:   EngTools is a component in Level 1 of the design that contains a 
            set of useful debugging tools.

******************************************************************************/
#include "Defines.h"

#ifndef _ENG_TOOLS_H_
#define _ENG_TOOLS_H_

// Debug Test Module levels
#define ENG_LEVEL_0              0
#define ENG_LEVEL_1              1
#define ENG_LEVEL_2              2
#define ENG_LEVEL_3              3
#define ENG_LEVEL_4              4

//-----------------------------------------------------------------------------
//  EngTools class definition
class CEngTools
{
public:
    CEngTools(void);
    ~CEngTools(void);
    
    static bool enabled;
    static bool simulate;
    
    static int DebugVerify;
    
    static bool DebugTraceEnabled;
    static bool DebugPrintEnabled;
    static bool outputWindowEnabled;
    static bool logFileEnabled;
    static bool DebugVerboseEnabled;
    
    static int DebugTestModule;
    static int DebugTraceMode;
    static int DebugDelayInMs;
    
    static word DebugListDut[TOOL_MAX_DUT + 1];
    
    // Debug ListDut function
    static void Debug_SetDebugListDut(word* listDut);
    
    // Debug statements
    static void DebugPrint(char* msg);
    static void DebugVerbose(char* msg);
    static void DebugTrace(char* msg);
    static void DisplayToOutputWindow(char *optionalLabel, char* str);
    static void WriteToLogFile(char* optionalLabel, char* msg);    
    static String TestFlowToString(int testFlow);
    static void DisplayTestFlow(const char* testName, int testFlow);
    static void PrintListDut(char *label, word* listDut, bool printAll = false);
};

#endif