/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_fifos_def.h     data and software definitions: devices that behave as a FIFO buffer
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_FIFOS_DEF_H
#define __SSIO_FIFOS_DEF_H

#ifdef  SSIO_OPTIN_FIFOS

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Noise about the software style and organization of ssIO with respect to the rest of SimpleSal:
//    The intention of ssIO is to be independent of the Time, Event, and Agency features of ssTEA.
//    Independence propagates out to the documentation extrusion style of ssTEA; all documents in
//      the form of explanatory notes for ssIO are contained in the ssIO files themselves.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
pAsciiA_t pcfifoByteCtIn         = S("Bytes in           ");
pAsciiA_t pcfifoByteCtOut        = S("Bytes out          ");
pAsciiA_t pcfifoByteCtNoData     = S("Read empty fifo    ");
pAsciiA_t pcfifoByteCtLost       = S("Bytes lost         ");
pAsciiA_t pcTransferCtErrors     = S("Transfer Errors    ");
pAsciiA_t pcTransferCtResets     = S("Transfer Reset     ");
pAsciiA_t pcTransferCtReqs       = S("Transfer Requests  ");
pAsciiA_t pcTransfersCt          = S("Transfer Completed ");

// -------------------------------------------------------------------------------------------------
// the two supported types of fifo buffers are: linear and circular; a reference to indexing rules.
// Separate from the indexing, each fifo is defined as to the overflow and underflow actions taken.
// Overflow and underfow actions are the result of the user setting ss_configuration values.
// -------------------------------------------------------------------------------------------------
// Underflow means asking for the next value from the fifo when the fifo is empty, maybe not error.
// Overflow means writing data into a full fifo: should old data be overwritten or new data dropped?
// Decision to not pull data from the source when a fifo is full moves buffering to the source.
// -------------------------------------------------------------------------------------------------
// linear or circular fifo indexing values: read_i and write_i
//      read_i is where the next value that "get" returns from the fifo will come from.
//      write_i is where the next value "put" to the fifo will go to.
//      an index is never used to access outside the buffer; write_i value may reach (last_i+1).
// -------------------------------------------------------------------------------------------------
//  linear fifo indexing rules
//  Indexing:
//      when the fifo is empty, write_i is equal to 0
//      when the fifo is full, write_i is equal to (last_i+1)
//      The minimum fifo size is 1 byte due to the indexing rules.
//  Underflow: return "empty"
//  fifo error Action rules chosen by ssIO OPTIN
//      ifdef SSIO_ONEOF_FIFO_OVERFLOW_DROP
//      Overflow:  pull and drop new data, set flag indicating error
//      ifdef SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
//      Overflow:  not applicable
//      ifdef SSIO_ONEOF_FLOW_OVERFLOW_FORCE
//      Overflow:
// -------------------------------------------------------------------------------------------------
// circular fifo rules
//  Indexing:
//      Indexing into the buffer is made circular by wrapping indices incremented past last_i to 0.
//      The minimum fifo size is 2 bytes due to the indexing rules.
//  Underflow: returns "empty"
//  fifo error Action rules chosen by ssIO OPTIN
//      ifdef SSIO_ONEOF_FIFO_OVERFLOW_DROP
//      Overflow:  drop new data, set flag indicating error
//      ifdef SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
//      Overflow:  keep new data by overwriting newest data
//      ifdef SSIO_ONEOF_FIFO_OVERFLOW_FORCE
//      Overflow:  lose new data by overwriting newest data with marker
//      ifdef SSIO_ONEOF_FLOW_BLOCK
//      Overflow:  only pull data from the source if the fifo is not full
//
//  The circular fifo rule for "fifo is empty" is: "when write_i equals read_i".
//  The corresponding fifo rule for "fifo is full" is: "when (write+1) equals read_i"
//  This costs a byte to maintain the gap between write_i and read_i; the alternative is to count
//  bytes as they are added and removed, which costs in execution time and CPU energy to compute.
//
//  Specific numbers makes it possible to create a basic case that prove all the corner cases.
//  Imagine the fifo size is 2, and the signal sent through the fifo consists of 2 bytes of data.
//    initialization        read_i = 0; write_i = 0;        the fifo is empty
//    one byte is added     read_i = 0; write_i = 1;        one byte fills the fifo Incr(1)==0
//    one byte is removed   read_i = 1; write_i = 1;        the fifo is empty
//    one byte is added     read_i = 1; write_i = 0;        one byte fills the fifo Incr(0)==1
//    one byte is removed   read_i = 0; write_i = 0;        the fifo is empty
// -------------------------------------------------------------------------------------------------
#define Incr_circular(curr,last_i)   (((curr) == last_i) ? 0      : (curr+1))
#define decr_circular(curr,last_i)   (((curr) == 0)      ? last_i : (curr-1))

// #define SSIO_ONEOF_FIFO_OVERFLOW_DROP
// #define SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
// #define SSIO_ONEOF_FIFO_OVERFLOW_FORCE

#ifdef SSIO_ONEOF_FIFO_OVERFLOW_DROP
#endif // SSIO_ONEOF_FIFO_OVERFLOW_DROP
#ifdef SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
#endif // SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
#ifdef SSIO_ONEOF_FIFO_OVERFLOW_FORCE
#define SSIO_ONEOF_FIFO_OVERFLOW_FORCE_VALUE    ('*')
#endif // SSIO_ONEOF_FIFO_OVERFLOW_FORCE

// =================================================================================================
#ifdef SSIO_OPTIN_FIFOS_STATS
// -------------------------------------------------------------------------------------------------
void    ssIO_fifoStatOp_ClearAll (ssIO_pFifo_t pFifo)
{
    int stat_i;

    for (stat_i=0; stat_i <= FIFO_STAT_LAST_I; stat_i++)
    {
        pFifo->Stats[stat_i] = 0;
    }   // for
}   // ssIO_fifoStatOp_ClearAll

// usage Incr_fifoStat (pFifo, Byte_Ct_In);
// usage Incr_fifoStat (pFifo, Byte_Ct_Out);
// usage Incr_fifoStat (pFifo, Byte_Ct_Lost);
// usage Incr_fifoStat (pFifo, Transfer_Ct_Errors);
// usage Incr_fifoStat (pFifo, Transfer_Ct_Resets);
// usage Incr_fifoStat (pFifo, Transfer_Ct_Reqs);
// usage Incr_fifoStat (pFifo, Transfer_Ct_OK);
// usage
// -------------------------------------------------------------------------------------------------
#define this_newLine() mesa_uiOp_emit_newline ()

void ssIO_fifostatOp_ShowAll (pAsciiA_t pStatName, ssIO_pFifo_t pFifo)
{
    this_newLine ();
    mesa_uiOp_emit_qAsciiA ("  ==  statistics for <");
    mesa_uiOp_emit_pAsciiA (pStatName);
    mesa_uiOp_emit_qAsciiA ("> fifo (current)");
    this_newLine ();
    if (pFifo->Stats[Byte_Ct_In] != 0)
    {   ss_uiOp_emit_lbld_int(pcfifoByteCtIn, pFifo->Stats[Byte_Ct_In]);              this_newLine (); }
    if (pFifo->Stats[Byte_Ct_Out] != 0)
    {   ss_uiOp_emit_lbld_int(pcfifoByteCtOut, pFifo->Stats[Byte_Ct_Out]);            this_newLine (); }
    if (pFifo->Stats[Byte_Ct_ReadNoData] != 0)
    {   ss_uiOp_emit_lbld_int(pcfifoByteCtNoData, pFifo->Stats[Byte_Ct_ReadNoData]);  this_newLine (); }
    if (pFifo->Stats[Byte_Ct_Lost] != 0)
    {   ss_uiOp_emit_lbld_int(pcfifoByteCtLost, pFifo->Stats[Byte_Ct_Lost]);          this_newLine (); }
    if (pFifo->Stats[Transfer_Ct_Errors] != 0)
    {   ss_uiOp_emit_lbld_int(pcTransferCtErrors, pFifo->Stats[Transfer_Ct_Errors]);  this_newLine (); }
    if (pFifo->Stats[Transfer_Ct_Resets] != 0)
    {   ss_uiOp_emit_lbld_int(pcTransferCtResets, pFifo->Stats[Transfer_Ct_Resets]);  this_newLine (); }
    if (pFifo->Stats[Transfer_Ct_Reqs] != 0)
    {   ss_uiOp_emit_lbld_int(pcTransferCtReqs, pFifo->Stats[Transfer_Ct_Reqs]);      this_newLine (); }
    if (pFifo->Stats[Transfer_Ct_OK] != 0)
    {   ss_uiOp_emit_lbld_int(pcTransfersCt, pFifo->Stats[Transfer_Ct_OK]);           this_newLine (); }
}   // ssIO_fifostatOp_ShowAll
#endif  // SSIO_OPTIN_FIFOS_STATS
// -------------------------------------------------------------------------------------------------
void ssIO_fifoOp_Init (ssIO_pFifo_t pFifo, boolean isCircular, pBits8_t pBuffer, int last_i)
{
    pFifo->isCircular = isCircular;
    pFifo->pBuffer = pBuffer;
    pFifo->last_i = last_i;

    ssIO_fifoOp_ClearState (pFifo);
    ssIO_fifoStatOp_ClearAll (pFifo);
}   // ssIO_fifoOp_Init
// -------------------------------------------------------------------------------------------------
void ssIO_fifoOp_ClearState (ssIO_pFifo_t pFifo)
{
    pFifo->read_i    = 0;
    pFifo->write_i   = 0;
    ssIO_fifoOp_ClearDataLost (pFifo);
}   // ssIO_fifoOp_ClearState
// -------------------------------------------------------------------------------------------------
// a fifo has three states when it is operating properly, and one state that indicates a problem.
//  HasNone     (empty, not full)
//  HasSome     (not empty, not full)
//  HasFull     (not empty, full)
//  HasError    (empty, full)
// -------------------------------------------------------------------------------------------------
// The state of the fifo is not the same thing as the choice to go ahead and overwrite a full fifo,
// writing a value to a full fifo does not change the full state, only the data in the fifo, and
// possibly only the metadata about the fifo that says "some data in this buffer was corrupted".
// -------------------------------------------------------------------------------------------------
ssIO_fifo_State_t ssIO_fifoOp_State (ssIO_pFifo_t   pFifo)
{
    // easiest to eliminate but not most likely to be true is "empty?"
    if (pFifo->isCircular)
    {   // the basic choice made or rule that all other rules flow from: empty is when write==read
        if (pFifo->write_i == pFifo->read_i)
            return (ssIO_fifo_State_NoData);
    }   // isCircular
    else    // !isCircular == isLinear
    {   // the base requirement or rule that all other rules flow from: empty is when write==0
        if (pFifo->write_i == 0)
            return (ssIO_fifo_State_NoData);
    }   // isLinear

    // ---------------------------------------------------------------------------------------------
    // some data for sure, maybe full, refine the decision about what to return: is the fifo full?
    // ---------------------------------------------------------------------------------------------
    if (pFifo->isCircular)
    {
        // -----------------------------------------------------------------------------------------
        // if incremented next-write (last in) equals next-read (first in) gap rule says "full".
        // way to say "when a fifo is full, next-write is just before next-read, can't write there".
        // This is a design choice; another choice is to keep track of (count) read/write actions.
        // -----------------------------------------------------------------------------------------
        if (pFifo->read_i == Incr_circular (pFifo->write_i, pFifo->last_i))
        {
            return (ssIO_fifo_State_FullData);
        }
    }   // isCircular
    else    // !isCircular == isLinear
    {
        // -----------------------------------------------------------------------------------------
        // last_i is the element of the array as far as we know.   Can't write past that element.
        // -----------------------------------------------------------------------------------------
        if (pFifo->write_i == (pFifo->last_i+1))
            return (ssIO_fifo_State_FullData);
    }   // isLinear

    return (ssIO_fifo_State_SomeData);
}   // ssIO_fifoOp_State
// -------------------------------------------------------------------------------------------------
// The LostData semaphore indicates that some data has been lost at some point since the semaphore
// state was last cleared.  Any data lost at any time sets the flag, disregarding current value.
// Any successful result (no data was lost) does not clear the semaphore; every error signals it.
// -------------------------------------------------------------------------------------------------
void ssIO_fifoOp_SignalDataLost (ssIO_pFifo_t   pFifo)
{
    pFifo->LostDataAtSomePointSinceYouLastChecked = true;
}   // ssIO_fifoOp_SignalDataLost
// -------------------------------------------------------------------------------------------------
void ssIO_fifoOp_ClearDataLost (ssIO_pFifo_t   pFifo)
{
    pFifo->LostDataAtSomePointSinceYouLastChecked = false;
}   // ssIO_fifoOp_ClearDataLost
// -------------------------------------------------------------------------------------------------
// The idea "is the fifo is full or not?" is separate from the idea "is the fifo circular or linear?".
// *** circular fifo or linear fifo?
// A Linear fifo has a physical constant first and last index for allocated bytes.  Values added
// to the fifo are stored at the logical last location, values removed are taken from the logical
// first location.  The algorithm is simpler but a synchronizing signal is required to reset first.
// A Circular fifo has a physical constant first and last index for allocated bytes. The value of the
// logical first and last indices "circulate" as bytes are added and removed.  The algorithm is more
// complex but the synchronization between writes to and reads from the fifo is no longer required.
// -------------------------------------------------------------------------------------------------
void    ssIO_fifoOp_Put_LastIn (ssIO_pFifo_t   pTofifo, Bits8_t  Bits8)
{
    switch (ssIO_fifoOp_State (pTofifo))
    {
        case ssIO_fifo_State_FullData :
#ifdef SSIO_ONEOF_FIFO_OVERFLOW_DROP
            ssIO_fifoOp_SignalDataLost (pTofifo);
            Incr_fifoStat (pTofifo, Byte_Ct_Lost);
            break;
#endif  // SSIO_ONEOF_FIFO_OVERFLOW_DROP
            // -------------------------------------------------------------------------------------
            // *** The fifo is full or not?
            // The byte BEFORE the write_i value will either be overwritten or the data dropped.
            // Writing into the gap makes no sense because the write_i values is not changed, so the
            // the next value incoming will write over the value if room becomes available later.
            // -------------------------------------------------------------------------------------
            Serial.write (ssIO_fifoOp_Get_FirstIn (&AsciiEmit_fifo));
            if (pTofifo->isCircular)
            {
#ifdef SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
                pTofifo->pBuffer[decr_circular(pTofifo->write_i, pTofifo->last_i)] = Bits8;
                // leave write_i where it is, having crunched the previous last in
#endif  // SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
#ifdef SSIO_ONEOF_FIFO_OVERFLOW_FORCE
                pTofifo->pBuffer[decr_circular(pTofifo->write_i, pTofifo->last_i)] = SSIO_ONEOF_FIFO_OVERFLOW_FORCE_VALUE;
                // leave write_i where it is, having crunched the previous last in
#endif // SSIO_ONEOF_FIFO_OVERFLOW_FORCE
            }
            else    // is linear
            {
#ifdef SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
                pTofifo->pBuffer[pTofifo->write_i-1] = Bits8;
                // leave write_i where it is, having crunched the previous last in
#endif  // SSIO_ONEOF_FIFO_OVERFLOW_CRUNCH
#ifdef SSIO_ONEOF_FIFO_OVERFLOW_FORCE
                pTofifo->pBuffer[pTofifo->write_i-1] = SSIO_ONEOF_FIFO_OVERFLOW_FORCE_VALUE;
                // leave write_i where it is, having crunched the previous last in
#endif // SSIO_ONEOF_FIFO_OVERFLOW_FORCE
            }
            ssIO_fifoOp_SignalDataLost (pTofifo);
            Incr_fifoStat (pTofifo, Byte_Ct_Lost);
            // fall through to store the last in byte after having sent first in to make room
        case ssIO_fifo_State_NoData :
        case ssIO_fifo_State_SomeData :
            // ---------------------------------------------------------------------------------------------
            // With the knowledge that there is at least one byte available to store the Bits8, just do it.
            // ---------------------------------------------------------------------------------------------
            if (pTofifo->isCircular)
            {
                pTofifo->pBuffer[pTofifo->write_i] = Bits8;
                pTofifo->write_i = Incr_circular (pTofifo->write_i, pTofifo->last_i);
            }   // isCircular
            else    // !isCircular == isLinear
            {
                pTofifo->pBuffer[pTofifo->write_i] = Bits8;
                pTofifo->write_i++;
            }   // isLinear
            Incr_fifoStat (pTofifo, Byte_Ct_In);
            break;
        case ssIO_fifo_State_Error :
            break;
    }   // switch
}   // ssIO_fifoOp_Put_LastIn
// -------------------------------------------------------------------------------------------------
Bits8_t ssIO_fifoOp_Get_FirstIn (ssIO_pFifo_t   pFromfifo)
{
    Bits8_t ret_Bits8  = 0;

    switch (ssIO_fifoOp_State (pFromfifo))
    {
        case ssIO_fifo_State_SomeData :
        case ssIO_fifo_State_FullData :
            // ---------------------------------------------------------------------------------------------
            // With the knowledge that there is at least one byte available to return to the caller, do it.
            // ---------------------------------------------------------------------------------------------
            ret_Bits8 = pFromfifo->pBuffer[pFromfifo->read_i];
            if (pFromfifo->isCircular)
            {
                pFromfifo->read_i = Incr_circular (pFromfifo->read_i, pFromfifo->last_i);
            }   // isCircular
            else    // !isCircular == isLinear
            {
                pFromfifo->read_i++;
            }   // isLinear
            Incr_fifoStat (pFromfifo, Byte_Ct_Out);
            break;
        case ssIO_fifo_State_NoData :
#ifdef ssIO_fifo_ERROR_WHEN_READ_EMPTY
            Incr_fifoStat (pFromfifo, Byte_Ct_ReadNoData);
#endif  //  ssIO_fifo_ERROR_WHEN_READ_EMPTY
        case ssIO_fifo_State_Error :
            break;
    }   // switch

    return (ret_Bits8);
}   // ssIO_fifoOp_Get_FirstIn

#endif  // SSIO_OPTIN_FIFOS

#endif  // __SSIO_FIFOS_DEF_H

