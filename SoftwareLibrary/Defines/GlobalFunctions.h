/******************************************************************************

    File:   GlobalFunctions.h
    Desc:   Re-defines some global functions for easier use throughout the code

******************************************************************************/
#ifndef _GLOBAL_FUNCTIONS_H_
#define _GLOBAL_FUNCTIONS_H_

//-----------------------------------------------------------------------------
// special string
#define String                  MyString

// variable name to string
#define STR( t )                #t

//-----------------------------------------------------------------------------
// device under test and site
#define _dut                    listDut[d] - 1
#define _site                   listDut[d]

//-----------------------------------------------------------------------------
// debug
#define DBGTrace( ... )         CTestbench::DebugTrace( __VA_ARGS__ )
#define DBGPrint( ... )         CTestbench::DebugPrint( __VA_ARGS__ )
#define DBGVerbose( ... )       CTestbench::DebugVerbose( __VA_ARGS__ )
#define DBGTestFlow( ... )      CTestbench::DisplayTestFlow( __VA_ARGS__ )
#define DBGSetDebugListDut(...) CTestbench::Debug_SetDebugListDut( __VA_ARGS__ )
#define DBGTestModule           CEngTools::DebugTestModule

// debug tests
#define DBGTrim( ... )          CTestbench::DebugTrim( __VA_ARGS__ )
#define DBGTrimSummary( ... )   CTestbench::DebugTrimSummary( __VA_ARGS__ )
#define DBGTestSummary( ... )   CTestbench::DebugTestSummary( __VA_ARGS__ )

// debug arrays
#define DBGDuts( ... )          CTestbench::PrintListDut( __VA_ARGS__ )

// related variables
#define DBGVerboseEnabled       CTestbench::DebugVerboseEnabled
#define DBGVerify               CTestbench::DebugVerify
#define DBGSimulate             CTestbench::simulate
#define DBGTraceEnabled         CTestbench::DebugTraceEnabled
#define DBGDelay                CTestbench::DebugDelayInMs
#define DBGListDut              CTestbench::DebugListDut

//-----------------------------------------------------------------------------
// plots
#define PLTArray( ... )         CTestbench::PlotArray( __VA_ARGS__ )
#define PLTSweep( ... )         CTestbench::PlotSweep( __VA_ARGS__ )

//-----------------------------------------------------------------------------
// utilities
#define IsOdd( ... )            CUtilities::IsOdd( __VA_ARGS__ )
#define B2S( ... )              CUtilities::ByteToString( __VA_ARGS__ )
#define B2SArray( ... )         CUtilities::ByteToStringArray( __VA_ARGS__ )
#define S2B( ... )              CUtilities::StringToByte( __VA_ARGS__ )
#define ARR2D( ... )            CUtilities::CopyArrayToDouble( __VA_ARGS__ )
#define XtoY( ... )             CUtilities::XToTheY( __VA_ARGS__ )
#define Clip( ... )             CUtilities::ClipToRange( __VA_ARGS__ )

//-----------------------------------------------------------------------------
// errors
#define ERRWarn( ... )          CUtilities::Error.Warn( __VA_ARGS__ )
#define ERRLog( ... )           CUtilities::Error.Add( __VA_ARGS__ )
#define ERRMsg( ... )           CUtilities::Error.MsgBox( __VA_ARGS__ )
#define ERRCrit( ... )          CUtilities::Error.Critical( __VA_ARGS__ )
#define ERRChk( ... )           CUtilities::Error.Check( __VA_ARGS__ )

//-----------------------------------------------------------------------------
// delays
//#define TIMEDelay( ... )        CUtilities::Timer.Delay( __VA_ARGS__ )

//-----------------------------------------------------------------------------
// safe character array functions
#define sprintf( str, ... )     sprintf_s( str, APP_MAX_CHAR, __VA_ARGS__ )
#define strcat( str, ... )      strcat_s( str, APP_MAX_CHAR, __VA_ARGS__ )
#define strcpy( str, ... )      strcpy_s( str, APP_MAX_CHAR, __VA_ARGS__ )

//-----------------------------------------------------------------------------
// hardware assert function - return values > 0 are warnings and < 0 are errors
#define Assert(status, label, sessionID)                        \
    this->AssertInternal(status, label, sessionID);             \
    if (status < SUCCESS)                                       \
        return ERROR_HARDWARE;                                  \

#endif
