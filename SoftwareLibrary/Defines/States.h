/******************************************************************************

    File:   States.h
    Desc:   Contains defines for all possible states that can be set for any
            device.  For a specific device, check which states are enabled in
            the ASIC class before trying to set it.

******************************************************************************/
#ifndef _STATES_H_
#define _STATES_H_

//-----------------------------------------------------------------------------
// bit states
#define LOW     0
#define HIGH    1

//-----------------------------------------------------------------------------
// ASIC States
#define STATE_DEVICE_UNKNOWN                    0
#define STATE_DEVICE_ENABLE                     1
#define STATE_DEVICE_DISABLE                    2
#define STATE_DEVICE_ALTERNATE                  3
#define STATE_RESPONSE_ENABLE                   4
#define STATE_RESPONSE_ENABLE_SPI3              5
#define STATE_RESPONSE_ENABLE_SPI4              6
#define STATE_DEVICE_TRIM                       7
#define STATE_DEVICE_TRIM_2G                    8
#define STATE_DEVICE_SWEEP                      9
#define STATE_REGULATOR_ENABLE                  10
#define STATE_REGULATOR_DISABLE                 11
#define STATE_OSCILLATOR_ENABLE                 12
#define STATE_OSCILLATOR_DISABLE                13
#define STATE_WATCHDOG_ENABLE                   14
#define STATE_WATCHDOG_DISABLE                  15
#define STATE_SELFTEST_POS                      16
#define STATE_SELFTEST_NEG                      17
#define STATE_SCAN_VECTOR                       18
#define STATE_DEVICE_INTERRUPT_ON               19
#define STATE_DEVICE_INTERRUPT_OFF              20
#define STATE_BUFFER_ENABLE                     21
#define STATE_TEMP_ENABLE                       22
#define STATE_BIAS_CURRENT_ENABLE               23
#define STATE_BIAS_CURRENT_DISABLE              24
#define STATE_LPREF_ENABLE                      25
#define STATE_LPREF_DISABLE                     26
#define STATE_SLOW_OSCILLATOR_ENABLE            27
#define STATE_SLOW_OSCILLATOR_DISABLE           28
#define STATE_TEMPERATURE_TRIM_ENABLE           29
#define STATE_TEMPERATURE_TRIM_DISABLE          30
#define STATE_REDUNDANT_READ_MODE               31
#define STATE_SINGLE_ENDED_READ_MODE            32
#define STATE_LOCK_MEMORY_ACCESS                33

#ifdef _NO_FLIP_
    #define STATE_SENS_TRIM_XY_1                41
    #define STATE_SENS_TRIM_XY_2                42
    #define STATE_SENS_TRIM_XY_3                43
    #define STATE_SENS_TRIM_Z_1                 44
    #define STATE_SENS_TRIM_Z_2                 45
    #define STATE_SENS_TRIM_Z_3                 46
#endif


#endif