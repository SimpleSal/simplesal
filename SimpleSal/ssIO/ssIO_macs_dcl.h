/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_macs_dcl.h      data type, prototype declarations: SimpleSal Digital/Analog/Input/Output.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_MACS_DCL_H
#define __SSIO_MACS_DCL_H

#ifdef  SSIO_OPTIN_ANY_MAC

// =================================================================================================
// Virtual access to devices is the fundamental promise of the SimpleSal Safe Algorithm Laboratory.
// A SimpleSal App properly bounded by virtual definitions can support a learning curve to reality.
// -------------------------------------------------------------------------------------------------
// Noise about the software style and organization of ssIO with respect to the rest of SimpleSal:
//    The intention of ssIO is to be independent of the Time, Event, and Agency features of ssTEA.
//    Independence propagates out to the documentation extrusion style of ssTEA; all documents in
//      the form of explanatory notes for ssIO are contained in the ssIO files themselves.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The database in ssIO provides a mechanism for switching a MAC between accessing a medium that is
// operational in a true physical "space" into pretending to operate in a pretend space.  If you are
// going to be switching MACs into and out of reality, you need to have a unique name for each MAC.
// -------------------------------------------------------------------------------------------------
// The ID consists of a type of MAC (for a medium) defined by ssIO, a "select" of a specific MAC
// from the set of all MACs of that type of medium, and then within that set of all MACs for that
// type of medium, the address of the individual MAC.  Databases say "select", like a bit bus does.
// -------------------------------------------------------------------------------------------------
// This is a unique number created by the user of ssIO, in real OSes these are called "a handle" and
// the value is assigned by the OS to support wide varieties of mediums and connections between them.
// ssIO lets the user perform this task as part of organizing the App's view of the mesa and devices.
// The identifier model applies to the MAC database, which is shared between all configured devices.
// -------------------------------------------------------------------------------------------------
// An example to show: 1) what matters are unique combinations; 2) some combinations don't matter.
//      teacher type: math, languages, chemistry
//      academic grade level: lower, middle, upper
//      location: auditorium, lab
// valid MAC IDs:  [math:lower:auditorium] [languages:upper:auditorim] [chemistry:upper:lab]
// questionable or unused combinations: [languages:middle:lab]      should they be in there?
// -------------------------------------------------------------------------------------------------
// An example to show: 1) what matters are unique combinations; 2) some combinations don't matter.
//      medium type:  ethernet, wifi, Pin
//      bytes per second: 1000, 1M, 10M, 100M, 1000M
//      location: auditorium, lab
// valid MAC IDs:  [ethernet:10M:auditorium] [wifi:1000M:lab] [Pin:1000:lab]
// questionable or unused combinations: [Pin:100M:auditorium]     what is the Pin connected to?
// -------------------------------------------------------------------------------------------------
typedef unsigned long int MAC_ID_t;

#define MAC_ID_0    ((MAC_ID_t) 0x00)
#define MAC_ID_1    ((MAC_ID_t) 0x01)


// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A MAC is the communication link between the App and the PRFDSS used to carry the signal/message.
// The shorthand "PRFDSS" is used for "Pins, Regs, Fifos, DMAs, Signals, Sensors". These are all
// abstractions about physical things that happen in reality: a GPIO Pin emits an electronic signal,
// a device updates a memory interface as a register, fifo or DMA, a signal was instantiated.
// -------------------------------------------------------------------------------------------------
// All of the Endpoints define their own context and the information used to manage virtualization.
// Which is the same as saying: this is only a database of managed objects, the MAC is independent.
// -------------------------------------------------------------------------------------------------
// Pins and Registers are defined by the Mesa platform and chip electronics designers; a "true"
// category uses electrical circuits and memory-like device registers with specific access methods.
// Pins are electronic mediums: a one bit value, high or low voltage, for digital pins; a value
// sensed and interpreted by circuits and then expressed as a multibit "register", for analog pins.
// References to Pin numbers invoke a Mesa support function specific to each specific Mesa design,
// this specialization per Mesa is why digital and analog read/write services are function calls.
// -------------------------------------------------------------------------------------------------
// Unlike the processor registers at the core of processing, a device register is visible in memory.
// A set of registers is common for sophisticated devices, with the device designed at a location.
// References to registers of devices visible in memory look like a memory access to the processor.
// -------------------------------------------------------------------------------------------------
// In order to virtualize a Register set, space of the right size and form is allocated as a MAC.
// In order to virtualize a Pin or Register, the Mesa access method and activities are emulated.
// All characteristics that a Pin has (mode, address, owner) available from the Mesa exist virtually.
// -------------------------------------------------------------------------------------------------
// DMA is "direct memory access", which happens when an processor-independent device uses the bus.
// A bus-driving smart electronic "DMA engine" (a Mesa) is given a chunk of memory and left alone.
// An example signal is a DMA-based wifi transmitter that reads and processes a transmit request,
// an example sensor would DMA a data result into memory and a signal to the Driver to take Action.
// Both are emulated or implemented by memory using memory bus transactions from a processor access.
// Independently active devices are virtualized by Time Events making changes to the register set.
// -------------------------------------------------------------------------------------------------
// If the SSIO_OPTIN_XXX has not been #defined, the corresponding XXX file appears to be empty.
// -------------------------------------------------------------------------------------------------
#include "ssIO_pins_dcl.h"
#include "ssIO_regs_dcl.h"
#include "ssIO_fifos_dcl.h"
#include "ssIO_DMAs_dcl.h"
#include "ssIO_signals_dcl.h"
#include "ssIO_sensors_dcl.h"

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A feature of MAC: given a MAC definition of a PRFDSS, operations may be checked as they occur.
// The implementation of the checking is open to the user: valid pointer?  permission to signal?
// -------------------------------------------------------------------------------------------------
#define SSIO_OPTIN_CHECK_ACCESS

// -------------------------------------------------------------------------------------------------
// The debug options help debug definition and data problems, maybe even why the LED won't light up.
// -------------------------------------------------------------------------------------------------
// #define SSIO_OPTIN_DEBUG
#ifdef SSIO_OPTIN_DEBUG
// #define SSIO_OPTIN_DEBUG_MAC
// #define SSIO_OPTIN_DEBUG_DB

// #define SSIO_OPTIN_DEBUG_PINS
// #define SSIO_OPTIN_DEBUG_REGS
// #define SSIO_OPTIN_DEBUG_FIFOS
// #define SSIO_OPTIN_DEBUG_DMAS
// #define SSIO_OPTIN_DEBUG_SIGNALS
// #define SSIO_OPTIN_DEBUG_SENSORS
#endif  // SSIO_OPTIN_DEBUG

// -------------------------------------------------------------------------------------------------
// This model of defining abstractions into and out of the knowledge of the ssHL compiler allows all
// references to the abstraction in the source code, without being forced to ifdef the reference.
// A simpler explanation: what appears to be a function call in the source disappears if not debug.
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_DEBUG
#define ssIO_AnnounceThis(xpThis)    { mesa_uiOp_emit_qAsciiA (xpThis); mesa_uiOp_emit_newline (); }
// -- defined
#else   // not SSIO_OPTIN_DEBUG
// -- not defined
#define ssIO_AnnounceThis(xthis)
#endif  // SSIO_OPTIN_DEBUG

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// For the App using SSIO services for any variety of PRFDSS: memory must "instantiated", which
// means allocated for the SSIO service providing emulation of digital/analog/read/write actions.
// The value created here for each affects allocation of data for ssIO and by extension the App.
// When SSIO_OPTIN_xxx_NOT is not defined, the count is zero; all may add zero to the total.
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_PINS
#ifndef PINS_MAC_CT
#define PINS_MAC_CT  (2)
#endif  // PINS_MAC_CT
#else   // not SSIO_OPTIN_PINS
#define PINS_MAC_CT  (0)
#endif  // SSIO_OPTIN_PINS
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_REGS
#ifndef REGS_MAC_CT
#define REGS_MAC_CT  (2)
#endif  // REGS_MAC_CT
#else   // not SSIO_OPTIN_REGS
#define REGS_MAC_CT  (0)
#endif  // SSIO_OPTIN_REGS
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_FIFOS
#ifndef FIFOS_MAC_CT
#define FIFOS_MAC_CT  (2)
#endif  // FIFOS_MAC_CT
#else   // not SSIO_OPTIN_FIFOS
#define FIFOS_MAC_CT  (0)
#endif  // SSIO_OPTIN_FIFOS
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_DMAS
#ifndef DMAS_MAC_CT
#define DMAS_MAC_CT  (2)
#endif  // DMAS_MAC_CT
#else   // not SSIO_OPTIN_DMAS
#define DMAS_MAC_CT  (0)
#endif  // SSIO_OPTIN_DMAS
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_SIGNALS
#ifndef SIGNALS_MAC_CT
#define SIGNALS_MAC_CT  (2)
#endif  // SIGNALS_MAC_CT
#else   // not SSIO_OPTIN_SIGNALS
#define SIGNALS_MAC_CT  (0)
#endif  // SSIO_OPTIN_SIGNALS
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_SENSORS
#ifndef SENSORS_MAC_CT
#define SENSORS_MAC_CT  (2)
#endif  // SENSORS_MAC_CT
#else   // not SSIO_OPTIN_SENSORS
#define SENSORS_MAC_CT  (0)
#endif  // SSIO_OPTIN_SENSORS

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#define MAC_DATA_ALLOC  (PINS_MAC_CT +                     \
                            REGS_MAC_CT +                   \
                                FIFOS_MAC_CT +              \
                                    DMAS_MAC_CT +           \
                                        SIGNALS_MAC_CT +    \
                                            SENSORS_MAC_CT)
// Note that MAX_I is (-1) when nothing is enabled
#define MAC_DATA_MAX_I  (MAC_DATA_ALLOC-1)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
typedef enum MACtype_e
{
    MACtype_Undefined,        // has no characteristics that are valid, value 0 as enum important.
    MACtype_Pin,              // only 1 bit and only digital and only input OR output, not both
    MACtype_Reg,              // an 8/16/32-bit device register  (this includes AnalogReg)
    MACtype_fifo,             // an independent data store or data source for a stream of data
    MACtype_DMA,              // an independent data transfer mechanism from memory to memory
    MACtype_Signal,           // abstract description of some type of signaling device
    MACtype_Sensor,           // abstract description of some type of sensor device
    MACtype_user1,            //
    MACtype_user2,            //
    MACtype_flaw
}   MACtype_t;

// -------------------------------------------------------------------------------------------------
// ssIO defines a bit map allocating fields for a type of MAC, a medium of that type, and an ID.
// The medium is dependent on the type of MAC: two MrB groups exist, each is a medium of 8 LEDs.
// The ID is probably going to be a Pin number, which is unique between all Pins; the ID could be
// mapped as an index into an array of Ethernet devices that are all unique networks.  Whatever.
// The MAC type field is always referenced before referencing other fields; there is a hierarchy.
// ----
// To support "a Pin number without any MAC_ID fields set" being treated as "not-ssIO", Undefined
// MACtype and the MACmedium field for a non-SSIO true physical PRFDSS must have the value 0.
// ----
// To define a new MAC type, use a unique value for type
// ----
// for all MACs:            0xF0000      type      value must be from enumerated list
// ----
// for Mesa Pin MACs        0x0FF00     medium     value must be zero
//                          0x000FF     number     ID: Mesa-specific in real and virtual contexts
// ----
// for MrB medium MACs      0x0FF00     medium     a value [0..1] selects a medium: which of 2 MrBs
//                          0x000FF     number     ID: a value [0..7] selects an LED within one MrB
// -------------------------------------------------------------------------------------------------
#define     MACtype_Mask        (0xF0000)
#define     MACtype_Shift       (16)
#define     MACmedium_Mask      (0x0FF00)
#define     MACmedium_Shift     (8)
#define     MACnumber_Mask      (0x000FF)
#define     MACnumber_Shift     (0)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// In the Real World, Digital and Analog Pins are signal carriers, supporting Input or Output flows
// of electrons between two Mesas.  The input message that was carried by the electrons is a
// voltage detected, perceived, measured, filtered, adjusted, and reported as a value by the mesa.
// App software writes a logical value to the pin LED_BUILTIN, the Mesa circuitry causes the power
// perceived by the device connected to the pin (LED_BUILTIN) to be on or off. The LED device says
// that given a certain non-zero power level, it will illuminate.  The power level can be driven to
// a zero power level, in which case the LED device says that it will not illuminate.  On or Off.
// -------------------------------------------------------------------------------------------------
// "True"      : the expectation is that the state of the Pin effects a real device within the mesa.
//      when the DueMrBlinky set the Pin to 1, the 110 AC power switched on and carried a signal.
//      when the UNOR4wifi sets the Pin to 1, the LED on the matrix corresponding changes state.
// not "True"  : the state of the Pin only exists in memory and any interaction is software based.
// -------------------------------------------------------------------------------------------------
// "Direct Pin": the state of the Pin changes the state of an FSM that immediately perceives/acts.
// "Mapped Pin": managed as a Pin by software, maybe behind a complex device with protocols/drivers.
// The 8 pins on the original Mr. Blinky are direct and true; LED Matrix pins are mapped and true.
// -------------------------------------------------------------------------------------------------
// An ssIO "not True" pin is maintained in RAM memory, and acts/reacts "as if" a Pin access occured.
//    The RAM memory supports input and output as the carrier of information; it's all the same.
// -------------------------------------------------------------------------------------------------
// The UNOR4wifi mesa has an 8x12 (or 12x8) LED Matrix.  8 pixels within the 96 of the LED matrix
// can be grouped and designated as a "Mr Blinky"; 2 groups of 8 pixels can each be a "Mr Blinky".
// This means the original Mr Blinky power switcher code is switching states of a pixel in a group.
// -------------------------------------------------------------------------------------------------
typedef struct ssIO_MACDesc_s
{
    MAC_ID_t        MAC_ID;             // the lookup mechanism the DB uses: given ID, return pMAC

    boolean         TrueState;          // not T/F: always compare with ssIO_VirtNow or ssIO_TrueNow
    boolean         MappedState;        // not T/F: always compare with ssIO_Direct or ssIO_Mapped

    boolean         isDigital;          // T/F: is this mesa access analog or digital I/O operation?
    boolean         InputAllowed;       // T/F: does this mesa access allow read from an input?
    boolean         OutputAllowed;      // T/F: does this mesa access allow emitting to an output?

    MACtype_t       MACtype;            // directs references to ONE of the data types in union '.u.'
    union
    {
#ifdef SSIO_OPTIN_PINS
        ssIO_Pin_t                 Pin;
#endif  // SSIO_OPTIN_PINS
#ifdef SSIO_OPTIN_REGS
        ssIO_Reg_t                 Reg;
#endif  // SSIO_OPTIN_REGS
#ifdef SSIO_OPTIN_FIFOS
          ssIO_fifo_t              Fifo;
#endif  // SSIO_OPTIN_FIFOS
#ifdef SSIO_OPTIN_DMAS
        ssIO_DMA_Engine_t          DMA;
#endif  // SSIO_OPTIN_DMAS
#ifdef SSIO_OPTIN_SIGNALS
        ssIO_Signal_t              Signal;
#endif  // SSIO_OPTIN_SIGNALS
#ifdef SSIO_OPTIN_SENSORS
        ssIO_Sensor_t              Sensor;
#endif  // SSIO_OPTIN_SENSORS
    }   u;  // u.Pin, u.Reg, u.fifo, u.Signal, u.Sensor refer to ONE view of the data in '.u.'
}   ssIO_MACDesc_t, *ssIO_pMACDesc_t;

typedef ssIO_MACDesc_t     ssIO_MAC_t;
typedef ssIO_pMACDesc_t    ssIO_pMAC_t;
#define ssIO_pMACNull      ((ssIO_pMAC_t) NULL)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// FSMs that are either in one state or another are "binary", which tends to encourage true/false
// boolean thinking in writing software, which creates a danger.  Creating a variable with the name
// "isStateTrueNow" allows for writing "if (isStateTrueNow)" when the variable is assigned either
// true or false boolean values.  When creating DEFINES to represent the two binary states based on
// inherent true and false values, the danger is the question "if (ssIO_VirtNow)" looks like a
// question, but what the ssHL compiler see is "true", so it properly compiles the body of the "if".
// "if (ssIO_TrueNow)" results in the ssHL compiler seeing "false", and it INVISIBLY SKIPS THE "IF".
// -------------------------------------------------------------------------------------------------
// The only solution is discipline applied to all semaphore state condition values and variables.
// convention: values may be assigned to a variable, comparison to variable's value must use a value.
// All of these defines for states of attributes of a Pin are opposites; there a binary model ends.
// The values for a pair could be 8 and 3.  Often a CPU efficiently compares for 0 and non-0.
// -------------------------------------------------------------------------------------------------
#define ssIO_VirtNow    (true)
#define ssIO_TrueNow    (!ssIO_VirtNow)

#define ssIO_Direct     (true)
#define ssIO_Mapped     (!ssIO_Direct)

#define ssIO_Digital    (true)
#define ssIO_Analog     (!ssIO_Digital)

#define ssIO_Input      (true)
#define ssIO_NotInput   (!ssIO_Input)

#define ssIO_Output     (true)
#define ssIO_NotOutput  (!ssIO_Output)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void                ssIO_MAC_Initialize (void);

void                ssIO_MAC_dbOp_Init (void);
ssIO_pMAC_t         ssIO_MAC_dbOp_Get  (void);
void                ssIO_MAC_dbOp_Put  (ssIO_pMAC_t pMAC);

ssIO_pMAC_t         ssIO_dbOp_pMAC_This_MAC_ID (MAC_ID_t MAC_ID);
void                ssIO_MAC_dbOp_dumpAll (void);

MAC_ID_t            ssIO_Build_MAC_ID (MACtype_t MACtype, int MrB_i, int mediumNumber_i);

// -------------------------------------------------------------------------------------------------
#define     SSIO_MAC_IO_Ability_Input        (1)
#define     SSIO_MAC_IO_Ability_Output       (2)
#define     SSIO_MAC_IO_Ability_InputOutput  (3)

#ifdef SSIO_OPTIN_PINS
void                ssIO_MAC_Pin_Default        (ssIO_pMAC_t pMAC, int MAC_IO_Ability);
ssIO_pMAC_t         ssIO_MAC_Pin_get_pMAC       (PinNumber_t  mesaPinNumber);
boolean             ssIO_MAC_Pin_UseCheckWrite  (ssIO_pMAC_t pMAC, LogicalPinValue_t PinValue);
boolean             ssIO_MAC_Pin_UseCheckRead   (ssIO_pMAC_t pMAC);
boolean             ssIO_MAC_Pin_UseCheckMode   (ssIO_pMAC_t pMAC, PinMode_t mesaPinMode);
#endif  // SSIO_OPTIN_PINS

#ifdef SSIO_OPTIN_REGS
void                ssIO_MAC_Reg_Default        (ssIO_pMAC_t pMAC);
ssIO_pMAC_t         ssIO_MAC_Reg_get_pMAC       (RegNumber_t  RegNumber);
boolean             ssIO_MAC_Reg_UseCheckWrite  (ssIO_pMAC_t pMAC, ssIO_pReg_t pReg);
boolean             ssIO_MAC_Reg_UseCheckRead   (ssIO_pMAC_t pMAC);
#endif  // SSIO_OPTIN_REGS

#ifdef SSIO_OPTIN_FIFOS
void                ssIO_MAC_fifo_Default       (ssIO_pMAC_t pMAC);
ssIO_pMAC_t         ssIO_MAC_fifo_get_pMAC      (FifoNumber_t FifoNumber);
boolean             ssIO_MAC_fifo_UseCheckWrite (ssIO_pMAC_t pMAC, ssIO_fifo_t pFifo);
boolean             ssIO_MAC_fifo_UseCheckRead  (ssIO_pMAC_t pMAC);
#endif  // SSIO_OPTIN_FIFOS

#ifdef SSIO_OPTIN_DMAS
void                ssIO_MAC_DMA_Default        (ssIO_pMAC_t pMAC);
ssIO_pMAC_t         ssIO_MAC_DMA_get_pMAC       (DMANumber_t  DMANumber);
boolean             ssIO_MAC_DMA_UseCheckWrite  (ssIO_pMAC_t pMAC, ssIO_DMA_pEngine_t pDMA);
boolean             ssIO_MAC_DMA_UseCheckRead   (ssIO_pMAC_t pMAC);
#endif  // SSIO_OPTIN_DMAS

#ifdef SSIO_OPTIN_SIGNALS
void                ssIO_MAC_Signal_Default         (ssIO_pSignaler_t pSignaler);
ssIO_pMAC_t         ssIO_MAC_Signal_get_pMAC        (SignalNumber_t   SignalNumber)
boolean             ssIO_MAC_Signal_UseCheckWrite   (ssIO_pMAC_t   pMAC, ssIO_pSignaler_t pSignaler);
boolean             ssIO_MAC_Signal_UseCheckRead    (ssIO_pMAC_t   pMAC);
#endif  // SSIO_OPTIN_SIGNALS

#ifdef SSIO_OPTIN_SENSORS
void                ssIO_MAC_Sensor_Default         (ssIO_pMAC_t pMAC);
ssIO_pMAC_t         ssIO_MAC_Sensor_get_pMAC        (SensorNumber_t SensorNumber)
boolean             ssIO_MAC_Sensor_UseCheckWrite   (ssIO_pMAC_t  pMAC, ssIO_pSensor_t pSensor);
boolean             ssIO_MAC_Sensor_UseCheckRead    (ssIO_pMAC_t  pMAC);
#endif  // SSIO_OPTIN_SENSORS

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This should really be in the fsmDemo App source files; included here as an example modification.
// A complex project will have a complex definition of all MAC types and addressing mechanisms.
// -------------------------------------------------------------------------------------------------
#define     MrB_Mask            MACmedium_Mask
#define     MrB_Shift           MACmedium_Shift

#define     MrB_GroupPin_Mask   MACnumber_Mask
#define     MrB_GroupPin_Shift  MACnumber_Shift

#endif  // SSIO_OPTIN_ANY_MAC

#endif  // __SSIO_MACS_DCL_H

