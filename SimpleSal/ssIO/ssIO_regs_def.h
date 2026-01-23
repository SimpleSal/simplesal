/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_regs_def.h      data and software definitions: interact with true/virtual registers.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_REGS_DEF_H
#define __SSIO_REGS_DEF_H

#ifdef  SSIO_OPTIN_REGS

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Noise about the software style and organization of ssIO with respect to the rest of SimpleSal:
//    The intention of ssIO is to be independent of the Time, Event, and Agency features of ssTEA.
//    Independence propagates out to the documentation extrusion style of ssTEA; all documents in
//      the form of explanatory notes for ssIO are contained in the ssIO files themselves.
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// There are combinations of definitions registers as physical and virtual registers. ssIO performs
// each access to a register using a pointer to EITHER the physical or virtual memory map location.
// The case where the register is both not physical and not virtual doesn't make sense but exists;
//    ssIO_Reg_Init will complain about this and then default to a true virtual register.
// The case where the register is both physical and virtual allows switching at build or run time,
//    by setting the value of TrueState to one of ssIO_Virtual and ssIO_NotVirtual.
// -------------------------------------------------------------------------------------------------
//    not virtual   :    not virtual    :   virtual      :   virtual
//    not physical  :      physical     :  not physical  :  physical
// -------------------------------------------------------------------------------------------------
// A true ssIO_Virtual register has no physical memory register and only a virtual register.
// A true ssIO_NotVirtual register has no virtual register and only a physical memory register.
// A physical register may have a virtual register associated.
// -------------------------------------------------------------------------------------------------
// All registers have a virtual component: a pointer to the local memory that acts as a register.
//      The memory acting as a register is always pointed to by the Virtual Reg pointer.
//      if no physical Reg pointer given, use the virtual component to create a physical component.
// if physical Reg pointer given, this must be a physical register (no way to verify).
//      use physical pointer to true register (in memory map), use virtual as with no physical
// -------------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------------
void    ssIO_Reg_MAC_ID (ssIO_pMAC_t pMAC, ssIO_puRegValue_t pRegPhys, int RegSize)
{
    pMAC->u.Reg.RegSize    = RegSize;

    switch (RegSize)
    {
        case RegSize_8bits    :
            // the pointer in pBits8 will cause ssHL to generate Bits8-sized accesses
            // 8 bits of memory, somewhere as pointed to by pBits8, is allocated accessible space
            if (pMAC->TrueState == ssIO_VirtNow)
            {
                pMAC->u.Reg.pRegPhys = (ssIO_puRegValue_t) &pMAC->u.Reg.RegValue.Bits8;
                pMAC->u.Reg.pRegVirt = pMAC->u.Reg.pRegPhys;
            }
            else    // TrueState == ssIO_TrueNow
            {
                pMAC->u.Reg.pRegPhys = pRegPhys;
                pMAC->u.Reg.pRegVirt = (ssIO_puRegValue_t) &pMAC->u.Reg.RegValue.Bits8;
            }
            // initialize the virtual value to zero: when physical==Virt and when physical!=Virt
            pMAC->u.Reg.RegValue.Bits8 = 0;
            break;
        case RegSize_16bits   :
            // the pointer in pBits16 will cause ssHL to generate Bits16-sized accesses
            // 16 bits of memory, somewhere as pointed to by pBits16, is allocated accessible space
            if (pMAC->TrueState == ssIO_VirtNow)
            {
                pMAC->u.Reg.pRegPhys = (ssIO_puRegValue_t) &pMAC->u.Reg.RegValue.Bits16;
                pMAC->u.Reg.pRegVirt = pMAC->u.Reg.pRegPhys;
            }
            else
            {
                pMAC->u.Reg.pRegPhys = pRegPhys;
                pMAC->u.Reg.pRegVirt = (ssIO_puRegValue_t) &pMAC->u.Reg.RegValue.Bits16;
            }
            pMAC->u.Reg.RegValue.Bits16 = 0;
            break;
        case RegSize_32bits   :
            // the pointer in pBits32 will cause ssHL to generate Bits32-sized accesses
            // 32 bits of memory, somewhere as pointed to by pBits32, is allocated accessible space
            if (pMAC->TrueState == ssIO_VirtNow)
            {
                pMAC->u.Reg.pRegPhys = (ssIO_puRegValue_t) &pMAC->u.Reg.RegValue.Bits32;
                pMAC->u.Reg.pRegVirt = pMAC->u.Reg.pRegPhys;
            }
            else
            {
                pMAC->u.Reg.pRegPhys = pRegPhys;
                pMAC->u.Reg.pRegVirt = (ssIO_puRegValue_t) &pMAC->u.Reg.RegValue.Bits32;
            }
            pMAC->u.Reg.RegValue.Bits32 = 0;
            break;
    }   // switch RegSize
}   // ssIO_Reg_MAC_ID
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void    ssio_Reg_Undefine     (ssIO_pReg_t pReg)
{
}   //  ssio_Reg_Undefine

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
Bits8_t ssIO_Reg_ReadByte  (ssIO_pReg_t pReg, pBits8_t pConsumer)
{
    *pConsumer = 0;
    return (0);
}   // ssIO_Reg_ReadByte

#endif  // SSIO_OPTIN_REGS

#endif  // __SSIO_REG_DEF_H

