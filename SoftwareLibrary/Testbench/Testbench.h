/******************************************************************************

    File:   Testbench.h
    Desc:   Testbench is a component in Level 2 of the design that contains a 
            set of useful debugging tools.

******************************************************************************/
#ifndef _TEST_BENCH_H_
#define _TEST_BENCH_H_

#include "EngTools.h"


//-----------------------------------------------------------------------------
// LV dll setup
typedef void (*dllRegViewType) (char Image1[], char Image2[]);

//-----------------------------------------------------------------------------
//  Testbench class definition
class CTestbench : public CEngTools
{
private:
    static HINSTANCE hDLL; // LVViewRegisters.dll handle
    static dllRegViewType ImageToPage00;
    static dllRegViewType ImageToPage01;

public:
    CTestbench(void);
    ~CTestbench(void);
    
    static void CTestbench::SetEnabledState(byte state);
    
    static int LVImageView(char* Image1, byte page);
    static int LVImageView(char* Image1, char* Image2, byte page);
    
    static void Init(int TestModuleDebug);
    static void End(void);
};

#endif