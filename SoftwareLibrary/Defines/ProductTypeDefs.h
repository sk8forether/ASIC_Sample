/******************************************************************************

    File:   ProductTypeDefs.h
    Desc:   Defines some basic product defines for C++ types and unions

******************************************************************************/
#ifndef _PRODUCT_TYPE_DEFS_H_
#define _PRODUCT_TYPE_DEFS_H_

//-----------------------------------------------------------------------------
// kionix part limits
#define MAX_NUM_PINS                    16              // pins on parts
#define MAX_PAGE_SIZE                   128             // max bytes for a page
#define MAX_RAM_SIZE                    MAX_PAGE_SIZE   // RAM is only 1 page
#define MAX_SN_SIZE                     5               // bytes in SN
#define MAX_NUM_BANKS                   4               // burnable PROM banks
#define MAX_NUM_AXES                    3               // X, Y, and Z

//-----------------------------------------------------------------------------
// Used to specify maximum number of slots in 2 PXI chassis
#define TOOL_MAX_NUM_SLOTS              24

//Used by PinOut to specify undefined lines, also used for a "zero terminated array", where zero is actually a valid value
#define HW_INVALID_PIN                  255

//Used by PTC to disable HW cards not available currently (for instance on ISMECA-4)
#define HW_SESSION_ENABLED              255
#define HW_SESSION_DISABLED             0

//Used to specify type of DCPowerCard
#define TOOL_INTERACE_PS                1
#define TOOL_DYNAMIC_PS                 2
#define TOOL_STATIC_PS                  3

// 2 channels for each power supply
#define CH0                             0
#define CH1                             1

//Used to initialize vdd
#define TOOL_INVALID_VDD                -999

//-----------------------------------------------------------------------------
// session typedefs
typedef struct DCPowerCard
{
    dword SessionID;
    double vdd[2];          //2 channels for each power supply
    String SessionName;
    int boardType;
    int sites[TOOL_MAX_NUM_SLOTS];
    
    DCPowerCard(void)
    {
        SessionID = HW_SESSION_ENABLED;
        memset(sites,0,sizeof(sites));
        vdd[CH0] = vdd[CH1] = TOOL_INVALID_VDD;
        SessionName = "";
        boardType = NULL;
        memset(sites,0,sizeof(sites));
    }
    
    DCPowerCard(MyString SessionName_in, int boardType_in) : SessionName(SessionName_in), boardType(boardType_in)
    {
        SessionID = HW_SESSION_ENABLED;
        vdd[CH0] = vdd[CH1] = TOOL_INVALID_VDD;
        memset(sites,0,sizeof(sites));
    }
    
    DCPowerCard(MyString SessionName_in, int boardType_in, int sites_in) : SessionName(SessionName_in), boardType(boardType_in)
    {
        SessionID = HW_SESSION_ENABLED;
        vdd[CH0] = vdd[CH1] = TOOL_INVALID_VDD;
        memset(sites,0,sizeof(sites));
        sites[0] = sites_in;
    }
    DCPowerCard(MyString SessionName_in, int boardType_in, int* sites_in) : SessionName(SessionName_in), boardType(boardType_in)
    {
        SessionID = HW_SESSION_ENABLED;
        vdd[CH0] = vdd[CH1] = TOOL_INVALID_VDD;
        memcpy(sites, sites_in, sizeof(sites));
    }
} DCPowerCard;

typedef struct HSDIOCard
{
    dword GenID;
    dword AcqID;
    String SessionName;
    String GenLines;
    String AcqLines;
    int AcqArray[8];
    int GroupFromDut[TOOL_MAX_DUT];
    
    HSDIOCard(void)
    {
        GenID = NULL;
        AcqID = NULL;
        SessionName = "";
        GenLines = "";
        AcqLines = "";
    }
} HSDIOCard;

typedef struct DAQmxCard
{
    void* SessionID;
    String GenLines;
    String AcqLines;
    
    DAQmxCard(void)
    {
        SessionID = NULL;
        GenLines = "";
        AcqLines = "";
    }
} DAQmxCard;

//-----------------------------------------------------------------------------
// pinout typedefs
#define KIONIX_MAX_PIN     16        // maximum number of pins for a Kionix part

enum PinOutChoice {
    KX022,
    KX023,
    KX127,
    KXG08,
    KXTJ2,
    KXCJK,
};

// used to select pins to measure or set
enum PinChoice {
    SDO_ADDR,
    SDI_SDA,
    TRIG,
    INT1,
    INT2,
    NCS,
    SCL,
    VPP,
    VDD,
    IO_VDD,
    GND,
    AUX_CL,
    AUX_DA,
    CPOut,
    SYNC
};

typedef struct PinOutStruct
{
    int num_pins;
    
    char name[APP_MAX_CHAR];
    int VDDs[KIONIX_MAX_PIN];
    int IO_VDDs[KIONIX_MAX_PIN];
    int GNDs[KIONIX_MAX_PIN];
    int NotConnected[KIONIX_MAX_PIN];
    
    int SDO_ADDR;
    int SDI_SDA;
    int TRIG;
    int INT1;
    int INT2;
    int NCS;
    int SCL;
    int VPP;
    
    // ISMECA Pins on socket board
    int INT1_AIN;
    int INT2_AIN;
    int GND_AIN;
    int IO_VDD_AIN;
    int VDD_AIN;
    bool hasVPP;
    int nCSConnectedInI2C;
    
    PinOutStruct(void) {}
    PinOutStruct(char* name_in, int* vdds_in, int* io_vdds_in, int* gnds_in, int* not_connected_in, int num_pins_in, 
        int sdo_addr_in, int sdi_sda_in, int trig_in, int int1_in, int int2_in, int ncs_in, int scl_in, int VPP_in)
        : num_pins(num_pins_in), SDO_ADDR(sdo_addr_in), SDI_SDA(sdi_sda_in), TRIG(trig_in), 
        INT1(int1_in), INT2(int2_in), NCS(ncs_in), SCL(scl_in), VPP(VPP_in)
    {
        sprintf(this->name,name_in);
        memcpy(VDDs, vdds_in, sizeof(VDDs));
        memcpy(IO_VDDs, io_vdds_in, sizeof(IO_VDDs));
        memcpy(GNDs, gnds_in, sizeof(GNDs));
        memcpy(NotConnected, not_connected_in, sizeof(NotConnected));
    }
    
    // Set Analog In lines for ISMECA static socket board (dynamic doesn't use this lines)
    void SetAIN(int int1_ain, int int2_ain, int gnd_ain, int io_vdd_ain, int vdd_ain, bool hasVPP_in, int nCSConnectedInI2C_in)
    {
        INT1_AIN = int1_ain;
        INT2_AIN = int2_ain;
        GND_AIN = gnd_ain;
        IO_VDD_AIN = io_vdd_ain;
        VDD_AIN = vdd_ain;
        hasVPP = hasVPP_in;
        nCSConnectedInI2C = nCSConnectedInI2C_in;
    }
} PinOutStruct;

typedef struct DevicePinsStruct
{
    int pins_digital[KIONIX_MAX_PIN];
    int pins_analog[KIONIX_MAX_PIN];
    int pins_vdd[KIONIX_MAX_PIN];
    int pins_gnd[KIONIX_MAX_PIN];
    int pins_power[KIONIX_MAX_PIN];
    
    DevicePinsStruct(void) {}
    DevicePinsStruct(int* pins_digital_in, int* pins_analog_in, int* pins_vdd_in, int* pins_gnd_in, int* pins_power_in)
    {
        memcpy(pins_digital, pins_digital_in, sizeof(pins_digital));
        memcpy(pins_analog, pins_analog_in, sizeof(pins_analog));
        memcpy(pins_vdd, pins_vdd_in, sizeof(pins_vdd));
        memcpy(pins_gnd, pins_gnd_in, sizeof(pins_gnd));
        memcpy(pins_power, pins_power_in, sizeof(pins_power));
    }
} DevicePinsStruct;

typedef struct CommPinsStruct
{
    int pins_com[KIONIX_MAX_PIN];
    int pins_vdd[KIONIX_MAX_PIN];
    int pins_gnd[KIONIX_MAX_PIN];
    int pin_ncs;
    int pin_sda;
    int pin_sdi;
    int pin_sdo;
    int pin_scl;
    
    CommPinsStruct(void) {}
    CommPinsStruct(int* pins_com_in, int* pins_vdd_in, int* pins_gnd_in, int pin_ncs_in, int pin_sda_in, int pin_sdi_in, int pin_sdo_in, int pin_scl_in)
        : pin_ncs(pin_ncs_in), pin_sda(pin_sda_in), pin_sdi(pin_sdi_in), pin_sdo(pin_sdo_in), pin_scl(pin_scl_in)
    {
        memcpy(pins_com, pins_com_in, sizeof(pins_com));
        memcpy(pins_vdd, pins_vdd_in, sizeof(pins_vdd));
        memcpy(pins_gnd, pins_gnd_in, sizeof(pins_gnd));
    }
} CommPinsStruct;

typedef struct PinUsageStruct
{
    PinOutStruct pin_out;
    int pins_all[KIONIX_MAX_PIN];
    DevicePinsStruct device_pins;
    CommPinsStruct i2c_pins;
    CommPinsStruct spi3_pins;
    CommPinsStruct spi4_pins;
    
    PinUsageStruct(void) {}
    PinUsageStruct(PinOutStruct pin_out_in, int* pins_all_in, DevicePinsStruct device_pins_in, CommPinsStruct i2c_pins_in, CommPinsStruct spi3_pins_in, CommPinsStruct spi4_pins_in)
        : pin_out(pin_out_in), device_pins(device_pins_in), i2c_pins(i2c_pins_in), spi3_pins(spi3_pins_in), spi4_pins(spi4_pins_in)
    {
        memcpy(pins_all, pins_all_in, sizeof(pins_all));
    }
} PinUsageStruct;

enum TestFlowPassChoice {
    PreScr,
    PostScr,
    AmbTrim,
    Hot,
    Cold,
    AmbVer,
    Programming,
    Retest,
    ReadOnly

};

enum RunModeChoice {
    Engineering,
    Qual,
    Audit,
    Production,
    Samples,
    FA,
    QA
};

// Struct to store database infomation until the Database module is built
typedef struct DBStruct
{
    TestFlowPassChoice TestFlowPass;
    RunModeChoice RunMode;
    bool Reprocess;
    
    int finalBin[TOOL_MAX_DUT];
    String finalBinDiagnostic[TOOL_MAX_DUT];
    
    DBStruct(void) { Reset(); }
    
    void Reset(void)
    {
        memset(finalBin,0,sizeof(finalBin));
        for (int d = 0; d < TOOL_MAX_DUT; d++)
        {
            finalBinDiagnostic[d] = "";
        }
    }
} DBStruct;

// Struct to store Spec infomation until the Database module is built to retrieve it
typedef struct SpecStruct
{
    String Name;
    String ProductName;
    
    string Default_Image;
    string Default_Mask;
    
    byte slave_addr;
    
    double op_vdd;
    
#ifdef _ACCEL_ENABLED_
    int    Accel_Max_Count;
    double Accel_Prog_Off[MAX_NUM_AXES];
    double Accel_Off[MAX_NUM_AXES];
    double Accel_OffTol[MAX_NUM_AXES];
    double Accel_FOMin[MAX_NUM_AXES];
    double Accel_FOMax[MAX_NUM_AXES];
    
    double Accel_Prog_Sens[MAX_NUM_AXES];
    double Accel_Sens[MAX_NUM_AXES];
    double Accel_SensTol[MAX_NUM_AXES];
#endif
} SpecStruct;

typedef struct PTCInfoStruct
{
    String productName;
    int testFlow;
    PinUsageStruct PinUsage;
    
    DBStruct DB;
    SpecStruct Spec;
    
    PTCInfoStruct(void) {}
    PTCInfoStruct(int testflow_in, PinUsageStruct PinUsage_in)
        : testFlow(testflow_in), PinUsage(PinUsage_in) {}
} PTCInfoStruct;

#endif