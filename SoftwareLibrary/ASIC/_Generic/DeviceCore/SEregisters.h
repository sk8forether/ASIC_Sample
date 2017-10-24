/******************************************************************************

    File:   SEregisters.h
    Desc:   Contains declarations for registers of any Kionix ASIC that might
            be used by one of the Sense Element classes (Accel, Gyro, etc.).
            These registers are then defined inside the unique ASIC class for
            each specific ASIC.  New or special registers may also be declared
            and/or defined in the class for the unique ASIC.

******************************************************************************/
#ifndef _SE_REGISTERS_H_
#define _SE_REGISTERS_H_

// NOTE: This file must be kept up-to-date with all registers from RegisterMap.h
//       that will be used by any Sense Elements

#ifdef _HAS_PAGES_
ASICregister CDeviceBase::REG_MEMPAGE = ASICregister();
#endif

#ifdef _ACCEL_ENABLED_
ASICregister CDeviceBase::REG_ACCEL_OUT_X = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_OUT_Y = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_OUT_Z = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_ODR = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_CGX = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_CGY = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_CGZ = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_FGX = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_FGY = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_FGZ = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_FOX = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_FOY = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_FOZ = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_DOX = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_DOY = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_DOZ = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_TCX = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_TCY = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_TCZ = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STX = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STY = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STZ = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STX_POS = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STY_POS = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STZ_POS = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STX_NEG = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STY_NEG = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_STZ_NEG = ASICregister();
#ifdef _ONLY_1_POL_BIT_
    ASICregister CDeviceBase::REG_ACCEL_POL = ASICregister();
#else
    ASICregister CDeviceBase::REG_ACCEL_POLX = ASICregister();
    ASICregister CDeviceBase::REG_ACCEL_POLY = ASICregister();
    ASICregister CDeviceBase::REG_ACCEL_POLZ = ASICregister();
#endif

ASICregister CDeviceBase::REG_ACCEL_CNTL = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_SWAP = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_DIS = ASICregister();
ASICregister CDeviceBase::REG_ACCEL_TC_EN = ASICregister();
ASICregister CDeviceBase::REG_SSB = ASICregister();
ASICregister CDeviceBase::REG_STNF = ASICregister();
ASICregister CDeviceBase::REG_I2C_MODE = ASICregister();
ASICregister CDeviceBase::REG_TCEN = ASICregister();
ASICregister CDeviceBase::REG_S_MODE = ASICregister();
ASICregister CDeviceBase::REG_ATH = ASICregister();
#endif

ASICregister CDeviceBase::REG_TEST = ASICregister();

#endif