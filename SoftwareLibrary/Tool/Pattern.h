/******************************************************************************

    File:   Pattern.h
    Desc:   

******************************************************************************/
#ifndef _PATTERN_H_
#define _PATTERN_H_

#include "Defines.h"

#include "PatternLS.h"

#ifdef _ISMECA_
    #include "PatternHS.h"
#endif

//------------------------------------------------------------------------------
//  pattern class
class CPattern
{
private:
    word CurrSpeed;
    
    CPatternLS* PatternLS;
    
    #ifdef _ISMECA_
        CPatternHS* PatternHS;
    #endif
    
public:
    CPattern(void) {}
    ~CPattern(void) {}
    
    String PatternList[PATT_MAX_NUM];
    String LongPatternList[PATT_MAX_NUM];
    
    int Init(word speed) { return NULL; }
    int LoadAll(byte slave_addr, word* listDut) { return NULL; }
    
    int Connect(word com, word* listDut) { return NULL; }
    int Disconnect(word* listDut) { return NULL; }
    
    int ModifySad(char *name, byte slave_addr, word* listDut, int level = LOW) { return NULL; }
    
    int Send(PatternInfo pattInfo, byte slave_addr, byte* toSet, word* listDut, int level = LOW) { return NULL; }
    int Receive(PatternInfo pattInfo, byte slave_addr, byte* toGet, int itr, word* listDut, PatternInfo pattInfoSad) { return NULL; }
    int Measure(PatternInfo pattInfo, byte slave_addr, double* toGet, int itr, word* listDut) { return NULL; }
    int Scan(PatternInfo pattInfo, long* toGet, word* listDut) { return NULL; }
    
    int SendSAD(PatternInfo pattInfo, word* listDut, int level = LOW) { return NULL; }
    int ReceiveSAD(PatternInfo pattInfo, word* listDut, bool* acks = NULL) { return NULL; }
    
    int GetInfo(char *name, PatternInfo* info, bool slave_addr = false) { return NULL; }
    int GetInfo(word index, PatternInfo* info, bool slave_addr = false) { return NULL; }
    
    word GetSpeed(void) { return NULL; }
    
    word GetCom(void) { return NULL; }
};

#endif
