/******************************************************************************

    File:   ASIC.cpp
    Desc:   ASIC is a component in Level 2 of the design that controls all 
    interaction with any ASIC of any type, and thus is generic.  Unique ASIC
    classes inherit this class.

******************************************************************************/
#include "ASIC.h"

CASIC *CASIC::Instance = NULL;

/******************************************************************************
    Name:   CASIC
    Desc:   Default constructor
******************************************************************************/
CASIC::CASIC()
{
    this->SetDeviceTypes();
    this->SetCommTypes();
}

/******************************************************************************
    Name:   ~CASIC
    Desc:   Default destructor
******************************************************************************/
CASIC::~CASIC(void)
{
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance 
******************************************************************************/
CASIC *CASIC::GetInstance(void)
{
    if (CASIC::Instance == NULL)
        CASIC::Instance = new CASIC();
    
    return CASIC::Instance;
}

/******************************************************************************
    Name:   Init
    Desc:   Initializes the ASIC class and subclasses
******************************************************************************/
void CASIC::Init(char* DefaultImg, char* DefaultMask, word* listDut)
{
    DBGTrace("--> CASIC::Init");
    
    DBGPrint("\tInitializing the Generic ASIC module");
    CDeviceCore::Init(DefaultImg, DefaultMask);
    
#ifdef _LV_COMM_
    double DefaultSAD = 30;
    double ClockFrequency = 100000;
    CLVInterpreter::GetInstance()->Init(COM_I2C, DefaultSAD, ClockFrequency, Tool, listDut);
#endif
}

/******************************************************************************
    Name:   SetDeviceTypes
    Desc:   Initializes the device_types variable in DeviceCore based on the
            defines for the particular ASIC from DefineForEveryASIC
******************************************************************************/
void CASIC::SetDeviceTypes(void)
{
    this->device_types = 0;
    
#ifdef _ACCEL_ENABLED_
    this->device_types |= SENSE_ACCEL;
#endif
}

/******************************************************************************
    Name:   SetCommTypes
    Desc:   Initializes the comm_types variable in DeviceCore based on the
            defines for the particular ASIC from DefineForEveryASIC
******************************************************************************/
void CASIC::SetCommTypes(void)
{
    this->comm_types = 0;
    
#ifdef _I2C_SUPPORTED_
    this->comm_types |= COM_I2C;
#endif
#ifdef _SPI3_SUPPORTED_
    this->comm_types |= COM_SPI3;
#endif
#ifdef _SPI4_SUPPORTED_
    this->comm_types |= COM_SPI4;
#endif
#ifdef _I3C_SUPPORTED_
    this->comm_types |= COM_I3C;
#endif
}

/******************************************************************************
    Name:   SetState
    Desc:   Sets various registers in the part to change settings to match one
            of several common "states" we put the part in when testing.  All
            states are declared in States.h and are able to be set here.
******************************************************************************/
void CASIC::SetState(int state, word* listDut)
{
    DBGTrace("--> CASIC::SetState");
    
    int origDBGVerify = DBGVerify;
    //DBGVerify = YES;
    
    switch(state)
    {
        case STATE_DEVICE_UNKNOWN:
        {
            DBGTrace("STATE_DEVICE_UNKNOWN");
            this->CurrState = STATE_DEVICE_UNKNOWN;
            break;
        }
        case STATE_DEVICE_TRIM:
        {
            // TODO implement (used in OffsetTrim)
        }
        case STATE_DEVICE_ENABLE:
        {
            DBGTrace("STATE_DEVICE_ENABLE");
            
            // disable self-test
            this->SetRegister(this->REG_SELFTEST, (byte)0x00, listDut);
            
            // disable part (CNTL1)
            this->SetRegister(this->REG_CNTL1, (byte)0x00, listDut);
            
            //TIMEDelay(DISABLE_DELAY);
            
             // cfg accel odr
            this->SetRegister(this->REG_ACCEL_ODR, (byte)0x02, listDut);//TODO: generalize
            
            // cfg self-test (INC1)
            this->SetRegister(this->REG_INC1, (byte)0x10, listDut);//TODO: generalize
            
            // enable part (CNTL1)
            this->SetRegister(this->REG_RES, HIGH, listDut);
            this->SetRegister(this->REG_PC1, HIGH, listDut);
            
            //TIMEDelay(CHANGE_STATE_DELAY);
            this->CurrState = STATE_DEVICE_ENABLE;
            break;
        }
        case STATE_DEVICE_DISABLE:
        {
            DBGTrace("STATE_DEVICE_DISABLE");
            
            // disable self-test
            this->SetRegister(this->REG_SELFTEST, (byte)0x00, listDut);
            
            // disable part (CNTL1)
            this->SetRegister(this->REG_CNTL1, (byte)0x00, listDut);
            
            //TIMEDelay(DISABLE_DELAY);
            
            // cfg accel odr
            this->SetRegister(this->REG_ACCEL_ODR, (byte)0x02, listDut);//TODO: generalize
            
            // cfg self-test (INC1)
            this->SetRegister(this->REG_INC1, (byte)0x10, listDut);//TODO: generalize
            
            // cfg cmd-test (COTC)
            // the highest bit is already 0, since if it were one, the part would restart
            this->SetRegister(this->REG_COTC, LOW, listDut);
            
            //TIMEDelay(DISABLE_DELAY);
            this->CurrState = STATE_DEVICE_DISABLE;
            break;
        }
        case STATE_RESPONSE_ENABLE:
        {
            DBGTrace("STATE_RESPONSE_ENABLE");
            
            // disable self-test
            this->SetRegister(this->REG_SELFTEST, (byte)0x00, listDut);
            
            // disable part (CNTL1)
            this->SetRegister(this->REG_CNTL1, (byte)0x00, listDut);
            
            //TIMEDelay(DISABLE_DELAY);
            
            // cfg accel odr
            this->SetRegister(this->REG_ACCEL_ODR, (byte)0x02, listDut);//TODO: generalize
            
            // cfg self-test (INC1)
            this->SetRegister(this->REG_INC1, (byte)0x10, listDut);//TODO: generalize
            
            // set COTC to 1 (7th bit) (CNTL2)
            // the highest bit is already 0, since if it were one, the part would restart
            this->SetRegister(this->REG_COTC, HIGH, listDut);
            
            // enable part (CNTL1)
            this->SetRegister(this->REG_RES, HIGH, listDut);//TODO: generalize
            this->SetRegister(this->REG_PC1, HIGH, listDut);//TODO: generalize
            
            //TIMEDelay(CHANGE_STATE_DELAY);
            this->CurrState = STATE_RESPONSE_ENABLE;
            break;
        }
        case STATE_DEVICE_ALTERNATE:
        {
            DBGTrace("STATE_DEVICE_ALTERNATE");
            
            // disable self-test
            this->SetRegister(this->REG_SELFTEST, (byte)0x00, listDut);
            
            // disable part (CNTL1)
            this->SetRegister(REG_CNTL1, (byte)0x00, listDut);
            
            //TIMEDelay(DISABLE_DELAY);
            
            // cfg accel odr
            this->SetRegister(this->REG_ACCEL_ODR, (byte)0x02, listDut);//TODO: generalize
            
            // cfg self-test (INC1)
            this->SetRegister(this->REG_INC1, (byte)0x10, listDut);//TODO: generalize
            
            // cfg cmd-test (COTC)
            this->SetRegister(this->REG_COTC, LOW, listDut);
            // the highest bit is already 0, since if it were one, the part would restart
            
            // enable part (CNTL1)
            this->SetRegister(this->REG_PC1, HIGH, listDut);
            
            //TIMEDelay(ALTERNATE_DELAY);
            this->CurrState = STATE_DEVICE_ALTERNATE;
            break;
        }
        case STATE_SELFTEST_POS:
        {
            DBGTrace("STATE_SELFTEST_POS");
            
            // disable self-test
            this->SetRegister(this->REG_SELFTEST, (byte)0x00, listDut);
            
            // disable part (CNTL1)
            this->SetRegister(this->REG_CNTL1, (byte)0x00, listDut);
            
            //TIMEDelay(DISABLE_DELAY);
            
             // cfg accel odr
            this->SetRegister(this->REG_ACCEL_ODR, (byte)0x05, listDut);//TODO: generalize
            
            // cfg self-test (INC1)
            this->SetRegister(this->REG_INC1, (byte)0x10, listDut);//TODO: generalize
            
            // cfg cmd-test (COTC)
            this->SetRegister(this->REG_COTC, LOW, listDut);
            
            // enable part (CNTL1)
            this->SetRegister(this->REG_RES, HIGH, listDut);
            this->SetRegister(this->REG_PC1, HIGH, listDut);
            
            // enable self-test
            this->SetRegister(this->REG_SELFTEST, this->KEY_SELFTEST, listDut);
            
            //TIMEDelay(CHANGE_STATE_DELAY);
            this->CurrState = STATE_SELFTEST_POS;
            break;
        }
#ifdef _OTP_
        case STATE_SINGLE_ENDED_READ_MODE:
        {
            DBGTrace("STATE_SINGLE_ENDED_READ_MODE");
            
            // set access memory key 1
            this->SetRegister(this->REG_AMEM1, this->KEY_AMEM1, listDut);
            
            // set access memory key 2
            this->SetRegister(this->REG_AMEM2, this->KEY_AMEM2, listDut);
            
            // set mr register to single-ended
            this->SetRegister(this->REG_MR, this->SINGLE_ENDED, listDut);
            
            //TIMEDelay(MEMORY_DELAY);
            
            this->CurrState = STATE_SINGLE_ENDED_READ_MODE;
            break;
        }
        case STATE_REDUNDANT_READ_MODE:
        {
            DBGTrace("STATE_REDUNDANT_READ_MODE");
            
            // set access memory key 1
            this->SetRegister(this->REG_AMEM1, this->KEY_AMEM1, listDut);
            
            // set access memory key 2
            this->SetRegister(this->REG_AMEM2, this->KEY_AMEM2, listDut);
            
            // set mr register to differential-redundant
            this->SetRegister(this->REG_MR, this->REDUNDANT, listDut);
            
            //TIMEDelay(MEMORY_DELAY);
            
            this->CurrState = STATE_REDUNDANT_READ_MODE;
            break;
        }
        case STATE_LOCK_MEMORY_ACCESS:
        {
            DBGTrace("STATE_LOCK_MEMORY_ACCESS");
            
            // set access memory key 1
            this->SetRegister(this->REG_AMEM1, 0x00, listDut);
            
            // set access memory key 2
            this->SetRegister(this->REG_AMEM2, 0x00, listDut);
            
            //TIMEDelay(MEMORY_DELAY);
            
            this->CurrState = STATE_LOCK_MEMORY_ACCESS;
            break;
        }
#endif
        default:
        {
            String msg;
            sprintf(msg, "Unknown state: %i", state);
            ERRLog(msg);
            this->CurrState = STATE_DEVICE_UNKNOWN;
            break;
        }
    }
    
    DBGVerify = origDBGVerify;
}

/******************************************************************************
    Name:   GetResponse
    Desc:   TODO fill in
******************************************************************************/
void CASIC::GetResponse(byte* output, word* listDut)
{
    DBGTrace("--> CASIC::GetResponse");
    
    this->GetRegister(this->REG_RESP, output, listDut);
}