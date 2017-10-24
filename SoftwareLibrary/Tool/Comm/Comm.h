/******************************************************************************

    File:   Comm.h
    Desc:   Comm is a component in Level 2 of the design that controls all 
            of the communciation that occurs between the tool and DUTs, on a
            variety of protocols including: I2C, I3C, SPI3, and SPI4.  The PLU
            method is handled within the SPEA class.

******************************************************************************/
#ifndef _COMM_H_
#define _COMM_H_

#include "Defines.h"
#include "Pin.h"
#include "Pattern.h"
#ifdef _SPEA_
    #include "PLU.h"
#endif

//-----------------------------------------------------------------------------
//  Comm class definition
class Comm
{
protected:
    static Comm* Instance;
    
    word CurrCom;
    int CurrCommMethod;
    bool forISMECASetThirdLineHigh;
    byte I2CRelayState;
    PinUsageStruct pinUsage;
    
    #ifdef _SPI_PAGE_TRANSLATION_
        byte CurrPage_SPI;
        byte CurrPage_I2C;
        
        byte TranslatePage(void);
        byte TranslateAddr(byte addr_SPI);
    #endif
    
    CPattern* Pattern;
    
    void SetCom(word com) { this->CurrCom = com; }
    void ResolveAddr(byte reg_addr, int action = READ);

public:
    Comm(void);
    ~Comm(void);
    static Comm* GetInstance(void);
    
    CPin* Pin;
    
    void Init(PinUsageStruct pinUsage, word* listDut);
    int LoadAllPatterns(byte slave_addr, word* listDut) { return this->Pattern->LoadAll(slave_addr, listDut); }
    
    word GetCom(void) { return this->CurrCom; }
    
    int GetCommMethod(void) { return this->CurrCommMethod; }
    void SetCommMethod(int newCommMethod) { this->CurrCommMethod = newCommMethod; }
    
    #ifdef _SPEA_
        int ConnectComm(word com, word* listDut, int method = COM_METHOD_PLU);
    #else
        int ConnectComm(word com, word* listDut, int method = COM_METHOD_PATT);
    #endif
    int DisconnectComm(word* listDut);
    
    int WritePage(byte slave_addr, byte mempage, byte page, word* listDut);
    
    int Read(byte slave_addr, byte reg_addr, int count, byte* data, word* listDut);
    int Write(byte slave_addr, byte reg_addr, int count, byte* data, word* listDut);
    
    int TestMode(word* listDut);
};

#endif