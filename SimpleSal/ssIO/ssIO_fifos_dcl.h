/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_fifos_dcl.h     data type, prototype declarations: devices that behave as a FIFO buffer
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_FIFOS_DCL_H
#define __SSIO_FIFOS_DCL_H

#ifdef  SSIO_OPTIN_FIFOS

typedef     Bits8_t     FifoNumber_t;

#ifdef SSIO_OPTIN_FIFOS_STATS
#define     FIFO_STAT_BYTE_CT_IN            (0)
#define     FIFO_STAT_BYTE_CT_OUT           (1)
#define     FIFO_STAT_BYTE_CT_READNODATA    (2)
#define     FIFO_STAT_BYTE_CT_LOSTDATA      (3)
#define     FIFO_STAT_TRAN_CT_ERROR         (4)
#define     FIFO_STAT_TRAN_CT_RESET         (5)
#define     FIFO_STAT_TRAN_CT_REQS          (6)
#define     FIFO_STAT_TRAN_CT_OK            (7)

// -------------------------------------------------------------------------------------------------
// The convention within SimpleSal is to define an allocation, then base last_i on Alloc'd amount.
// rather than reside in the formal name space imposed by SimpleSal, the user of statistics may
// make the software a little easier to read by substituting a natural sequence to be used instead.
// -------------------------------------------------------------------------------------------------
#define     Byte_Ct_In              FIFO_STAT_BYTE_CT_IN
#define     Byte_Ct_Out             FIFO_STAT_BYTE_CT_OUT
#define     Byte_Ct_ReadNoData      FIFO_STAT_BYTE_CT_READNODATA    // read when no data available
#define     Byte_Ct_Lost            FIFO_STAT_BYTE_CT_LOSTDATA
#define     Transfer_Ct_Errors      FIFO_STAT_TRAN_CT_ERROR
#define     Transfer_Ct_Resets      FIFO_STAT_TRAN_CT_RESET
#define     Transfer_Ct_Reqs        FIFO_STAT_TRAN_CT_REQS
#define     Transfer_Ct_OK          FIFO_STAT_TRAN_CT_OK

// -------------------------------------------------------------------------------------------------
// This method differs from ssTEA allocations in that the definition before the ssTEA reference is
// to a array definition with content while the ssIO reference is to indices defined at build time.
// In this case the last index is the known fact, the amount to allocate is a side-effect of that.
// One minor delta during access is: ssTEA must reference by field, while ssIO can use a for loop.
// -------------------------------------------------------------------------------------------------
#define     FIFO_STAT_LAST_I        (FIFO_STAT_TRAN_CT_OK)
#define     FIFO_STAT_ALLOC         (FIFO_STAT_LAST_I+1)
#endif  // SSIO_OPTIN_FIFOS_STATS

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
typedef struct ssIO_fifoDesc_s          // DOES NOT ALLOCATE A BUFFER for data pointed to by pBuffer
{
    boolean             isCircular;     // t: oldest Bits8 is @[read_i]; f: oldest Bits8 is @[0]
    int                 read_i;         // location of next read:   (pBuffer+read_i)
    int                 write_i;        // location of next write:  (pBuffer+write_i)
    Bits8_t             Carrier;        // data read from or written to the fifo is placed here

    boolean             LostDataAtSomePointSinceYouLastChecked;

    pBits8_t            pBuffer;        // POINTS TO allocated memory of at least (last_i+1) size
    int                 last_i;         // varies by type of Buffer and/or App's desire to limit use.

#ifdef SSIO_OPTIN_FIFOS_STATS
    unsigned long       Stats[FIFO_STAT_ALLOC];     // just allocate a bunch of integers for now
#endif  // SSIO_OPTIN_FIFOS_STATS
}   ssIO_fifoDesc_t,  *ssIO_pFifoDesc_t;

typedef ssIO_fifoDesc_t     ssIO_fifo_t;
typedef ssIO_pFifoDesc_t    ssIO_pFifo_t;
#define ssIO_pFifoNull      ((ssIO_pFifo_t) NULL)

// -------------------------------------------------------------------------------------------------
// The fifo's current state is not recorded in a flag per fifo; it is a result of realtime analysis.
// The information needed to make decisions about the fifo in an algorithm are about what's there.
// For this reason, the State is returned by a smart function and then decisions are made in context.
// -------------------------------------------------------------------------------------------------
typedef enum ssIO_fifo_State_s
{
    ssIO_fifo_State_NoData,
    ssIO_fifo_State_SomeData,
    ssIO_fifo_State_FullData,
    ssIO_fifo_State_Error
} ssIO_fifo_State_t;

// "clear" sets all control values of a FIFO to "there is no data"; "init" sets size, clears.
void    ssIO_fifoOp_Init   (ssIO_pFifo_t pFifo, boolean isCircular, pBits8_t pBuffer, int last_i);
#define IS_CIRCULAR_fifo            (true)
#define IS_LINEAR_fifo              (!IS_CIRCULAR_fifo)

void    ssIO_fifoOp_ClearState      (ssIO_pFifo_t pFifo);
void    ssIO_fifoOp_SignalDataLost  (ssIO_pFifo_t pFifo);
void    ssIO_fifoOp_ClearDataLost   (ssIO_pFifo_t pFifo);

ssIO_fifo_State_t ssIO_fifoOp_State (ssIO_pFifo_t   pFifo);

boolean ssIO_fifoOp_IsError         (ssIO_pFifo_t pFifo);

void    ssIO_fifoOp_Put_LastIn      (ssIO_pFifo_t pFifo, Bits8_t Bits8);
Bits8_t ssIO_fifoOp_Get_FirstIn     (ssIO_pFifo_t pFifo);

// ------------------------------------------------------------------------------------------------
// Creating a #define that exists, or does not exist, is a cleaner way to vary content than ifdefs.
// ------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_FIFOS_STATS
void    ssIO_fifoStatOp_ClearAll    (ssIO_pFifo_t pFifo);
void    ssIO_fifostatOp_ShowAll     (pAsciiA_t pStatsName, ssIO_pFifo_t pFifo);

#define Incr_fifoStat(xpFifo,xpFifostatfield)  (xpFifo->Stats[xpFifostatfield]++)

#else   // SSIO_OPTIN_FIFOS_STATS

#define ssIO_fifoStatOp_ClearAll(pFifo)
#define ssIO_fifostatOp_ShowAll(pStatsName,pFifo)

#endif  // SSIO_OPTIN_FIFOS_STATS

#endif  // SSIO_OPTIN_FIFOS

#endif  // __SSIO_FIFOS_DCL_H



