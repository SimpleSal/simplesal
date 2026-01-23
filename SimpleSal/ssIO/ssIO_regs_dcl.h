/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_regs_dcl.h      data type, prototype declarations: interact with true/virtual registers.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_REGS_DCL_H
#define __SSIO_REGS_DCL_H

#ifdef  SSIO_OPTIN_REGS

typedef     Bits8_t     RegNumber_t;

// =================================================================================================
typedef union RegValue_union_u
{                                   // Value when physical or virtual (one of)
    Bits8_t         Bits8;          // physical: the RegValue may contain a copy of the physical value
    Bits16_t        Bits16;         // Virt: the RegValue is the physical value
    Bits32_t        Bits32;
}   ssIO_uRegValue_Desc_t, *ssIO_puRegValue_Desc_t;

typedef ssIO_uRegValue_Desc_t   ssIO_uRegValue_t;
typedef ssIO_puRegValue_Desc_t  ssIO_puRegValue_t;
#define ssIO_puRegValueNull     ((ssIO_puRegValue_t) NULL)

// number of 8-bit values each access: 1, 2, 4; not 8/16/24.
#define RegSize_8bits       (sizeof (Bits8_t))
#define RegSize_16bits      (sizeof (Bits16_t))
#define RegSize_32bits      (sizeof (Bits32_t))

// -------------------------------------------------------------------------------------------------
typedef struct ssIO_RegBounds_Desc_s
{
    ssIO_uRegValue_t         BitMaskValidBits;
    ssIO_uRegValue_t         MinValue;
    ssIO_uRegValue_t         MaxValue;
}   ssIO_RegBounds_Desc_t, *ssIO_pRegBounds_Desc_t;

typedef ssIO_RegBounds_Desc_t   ssIO_RegBounds_t;
typedef ssIO_pRegBounds_Desc_t  ssIO_pRegBounds_t;
#define ssIO_pRegBoundsNull     ((ssIO_pRegBounds_t) NULL)

// -------------------------------------------------------------------------------------------------
// A single-value register, e.g., any of the registers provided by the Serial Port physical device,
// may be accessed as a memory location.  The address of the device register is defined by the mesa.
// This model can be used to define individual registers of a device; specific addresses and types.
// The alternative, shown below, is to create a map with the ssHL compiler matching the registers.
// -------------------------------------------------------------------------------------------------
// The rule that must be followed: for any register, all references must use the SAME BitsXX field.
// -------------------------------------------------------------------------------------------------
typedef struct ssIO_Reg_Desc_s
{
    ssIO_puRegValue_t   pRegPhys;       // the memory space register defined by the mesa
    ssIO_puRegValue_t   pRegVirt;       // an allocated space in memory pretending to be a register

    ssIO_RegBounds_t    Bounds;         // values that limit or bound the value read or written

    int                 RegSize;        // number of 8-bit values each access: 1, 2, 4; not 8/16/24.
    ssIO_uRegValue_t    RegValue;       // when referenced based on size, the register is accessed
}   ssIO_Reg_Desc_t, *ssIO_pReg_Desc_t;

typedef ssIO_Reg_Desc_t     ssIO_Reg_t;
typedef ssIO_pReg_Desc_t    ssIO_pReg_t;
#define ssIO_pRegNull       ((ssIO_pReg_t) NULL)

void    ssio_Reg_Default        (ssIO_pReg_t  pReg);
void    ssIO_Reg_MAC_ID         (ssIO_pReg_t  pReg, ssIO_puRegValue_t pRegPhys, int RegSize);
void    ssio_Reg_Undefine       (ssIO_pReg_t  pReg);

Bits8_t  ssIO_Reg_ReadBits8     (ssIO_pReg_t  pReg, pBits8_t    pConsumer);
Bits16_t ssIO_Reg_ReadBits16    (ssIO_pReg_t  pReg, pBits16_t   pConsumer);
Bits32_t ssIO_Reg_ReadBits32    (ssIO_pReg_t  pReg, pBits32_t   pConsumer);

Bits8_t  ssIO_Reg_WriteBits8    (ssIO_pReg_t  pReg, pBits8_t    pConsumer);
Bits16_t ssIO_Reg_WriteBits16   (ssIO_pReg_t  pReg, pBits16_t   pConsumer);
Bits32_t ssIO_Reg_WriteBits32   (ssIO_pReg_t  pReg, pBits32_t   pConsumer);

#endif  // SSIO_OPTIN_REGS

#endif  // __SSIO_REGS_DCL_H

