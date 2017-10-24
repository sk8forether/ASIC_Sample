/******************************************************************************

    File:   DefineForEveryASIC.h
    Desc:   ASIC-specific attributes that the PTC, ASIC, and other parts of the
            software need to know about go here.  Simply comment or uncomment
            features as they apply.  For part-specific values, they should be
            set for each ASIC.

******************************************************************************/
#ifndef _DEFINE_FOR_EVERY_ASIC_
#define _DEFINE_FOR_EVERY_ASIC_

//-----------------------------------------------------------------------------
// sense element types
#define _ACCEL_ENABLED_                     // accelerometer
//#define _MAG_ENABLED_                     // magnetometer
//#define _GYRO_ENABLED_                    // gyroscope
#define _PED_ENABLED_                       // pedometer

//-----------------------------------------------------------------------------
// communication types
#define _I2C_SUPPORTED_                     // speaks I2C
//#define _I3C_SUPPORTED_                   // speaks I3C
#define _SPI3_SUPPORTED_                    // speaks SPI3
#define _SPI4_SUPPORTED_                    // speaks SPI4

//-----------------------------------------------------------------------------
// ASIC features
#define _OTP_                               // One Time Programmable
//#define _EEPROM_                          // Re-programmable
#define _HAS_PAGES_                         // has pages
#define _HAS_SPARE_                         // has spare registers in RAM
#define _HAS_BUFFER_                        // has a buffer
//#define _SPI_PAGE_TRANSLATION_            // SPI pages don't match I2C pages
//#define _BURN_WITH_VPP_                   // has a VPP pin
//#define _NO_FLIP_                         // has no-flip option
//#define _ONLY_1_POL_BIT_                  // X/Y/Z polarity are the same
//#define _NO_SAD1_BIT_                     // no REG_SAD1 present
//#define _I2C_SLAVE_CONFIG_                // fully-configurable slave address
//#define _TEST_MODE_                       // enable test mode to test
//#define _UNSIGNED_OUTPUTS_                // subtract mid to make signed
#define _DIGITAL_OFFSET_                    // correct offset with digital code

//-----------------------------------------------------------------------------
// ASIC-specific values
#define VOL_PG1             PAGE_00         // Volatile memory Page
#define RAM_PG              PAGE_01         // RAM Page
#define ROM_PG              PAGE_02         // ROM Page
#define NUM_PAGES           3               // total memory pages
#define NUM_VOL_PAGES       1               // some ASICs have more than 1
#if NUM_VOL_PAGES > 1
    #define NUM_VOL_REG_2   0               // size of Volatile memory Page1
    #define VOL_PG_2        NULL            // Volatile memory
#endif

#define NUM_VOL_REG         MAX_PAGE_SIZE   // size of Volatile memory
#define NUM_RAM_REG         39              // size of RAM
#define NUM_ROM_REG         NUM_RAM_REG     // size of ROM

#define NUM_RAM_VALUES      38              // int values after conversion
#define NUM_ACCEL_OUT       2               // accel output registers per axis
//#define NUM_GYRO_OUT        0             // gyro output registers
//#define NUM_MAG_OUT         0             // mag output registers
//#define NUM_TEMP_OUT        0             // temp comp registers

#define BURN_VOLTAGE        3.3             // volts
#ifdef _NO_FLIP_
    #define NO_FLIP_VOLTAGE 0.0             // volts (if it has no-flip option)
#endif

// delay times (ms)
#define DISABLE_DELAY       5               // after disabling
#define CHANGE_STATE_DELAY  10              // after changing state
#define ALTERNATE_DELAY     100             // after setting part to alternate    // TODO maybe later set this to 40, 100 might be too long
#define RESET_DELAY         40              // after resetting the part
//#define BIAS_DELAY          0             // after enabling bias current
#ifdef _OTP_
    #define MEMORY_DELAY    40              // after changing OTP memory access
#elif defined(_EEPROM_)
    #define BURN_DELAY      0               // during NVM burn of EEPROM
#endif

// LabView dll relative paths for development
#define LV_REGVIEW_PATH     L"..\\..\\..\\SoftwareLibrary\\ASIC\\BF\\LVViewRegistersBuild\\LVViewRegisters.dll"
#define LV_INTERPRETER_PATH L"..\\..\\..\\SoftwareLibrary\\ASIC\\BF\\LVInterpreterBuild\\LVInterpreter.dll"

#endif
