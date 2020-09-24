/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfinterrupt.h

Abstract:

    This is the C header for driver framework Interrupt object

Revision History:


--*/

//
// NOTE: This header is generated by stubwork.  Please make any
//       modifications to the corresponding template files
//       (.x or .y) and use stubwork to regenerate the header
//

#ifndef _WDFINTERRUPT_H_
#define _WDFINTERRUPT_H_

#ifndef WDF_EXTERN_C
  #ifdef __cplusplus
    #define WDF_EXTERN_C       extern "C"
    #define WDF_EXTERN_C_START extern "C" {
    #define WDF_EXTERN_C_END   }
  #else
    #define WDF_EXTERN_C
    #define WDF_EXTERN_C_START
    #define WDF_EXTERN_C_END
  #endif
#endif

WDF_EXTERN_C_START



#if (NTDDI_VERSION >= NTDDI_WIN2K)

//
// Message Signaled Interrupts (MSI) information structure
//
typedef enum _WDF_INTERRUPT_POLARITY {
    WdfInterruptPolarityUnknown = 0,
    WdfInterruptActiveHigh,
    WdfInterruptActiveLow,
} WDF_INTERRUPT_POLARITY, *PWDF_INTERRUPT_POLARITY;

typedef enum _WDF_INTERRUPT_POLICY {
    WdfIrqPolicyMachineDefault = 0,
    WdfIrqPolicyAllCloseProcessors,
    WdfIrqPolicyOneCloseProcessor,
    WdfIrqPolicyAllProcessorsInMachine,
    WdfIrqPolicySpecifiedProcessors,
    WdfIrqPolicySpreadMessagesAcrossAllProcessors,
} WDF_INTERRUPT_POLICY, *PWDF_INTERRUPT_POLICY;

typedef enum _WDF_INTERRUPT_PRIORITY {
    WdfIrqPriorityUndefined = 0,
    WdfIrqPriorityLow,
    WdfIrqPriorityNormal,
    WdfIrqPriorityHigh,
} WDF_INTERRUPT_PRIORITY, *PWDF_INTERRUPT_PRIORITY;




//
// This is the function that gets invoked when the hardware ISR occurs.
// This function is called at the IRQL at which the interrupt is serviced:
//  - DIRQL for DIRQL interrupt handling.
//  - PASSIVE_LEVEL for passive-level interrupt handling.
//
typedef
_Function_class_(EVT_WDF_INTERRUPT_ISR)
_IRQL_requires_same_
_IRQL_requires_min_(PASSIVE_LEVEL)
BOOLEAN
EVT_WDF_INTERRUPT_ISR(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    ULONG MessageID
    );

typedef EVT_WDF_INTERRUPT_ISR *PFN_WDF_INTERRUPT_ISR;

//
// This is the function that gets invoked when a Synchronize execution occurs.
// This function is called at the IRQL at which the interrupt is serviced:
//  - DIRQL for DIRQL interrupt handling.
//  - PASSIVE_LEVEL for passive-level interrupt handling.
//
typedef
_Function_class_(EVT_WDF_INTERRUPT_SYNCHRONIZE)
_IRQL_requires_same_
_IRQL_requires_min_(PASSIVE_LEVEL)
BOOLEAN
EVT_WDF_INTERRUPT_SYNCHRONIZE(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    WDFCONTEXT Context
    );

typedef EVT_WDF_INTERRUPT_SYNCHRONIZE *PFN_WDF_INTERRUPT_SYNCHRONIZE;

//
// This is the function that gets called back into the driver
// when the DpcForIsr fires.  It will be called at DISPATCH_LEVEL.
//
typedef
_Function_class_(EVT_WDF_INTERRUPT_DPC)
_IRQL_requires_same_
_IRQL_requires_(DISPATCH_LEVEL)
VOID
EVT_WDF_INTERRUPT_DPC(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    WDFOBJECT AssociatedObject
    );

typedef EVT_WDF_INTERRUPT_DPC *PFN_WDF_INTERRUPT_DPC;

//
// This is the function that gets called back into the driver
// when the WorkItemForIsr fires.  It will be called at PASSIVE_LEVEL.
//
typedef
_Function_class_(EVT_WDF_INTERRUPT_WORKITEM)
_IRQL_requires_same_
_IRQL_requires_(PASSIVE_LEVEL)
VOID
EVT_WDF_INTERRUPT_WORKITEM(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    WDFOBJECT AssociatedObject
    );

typedef EVT_WDF_INTERRUPT_WORKITEM *PFN_WDF_INTERRUPT_WORKITEM;

//
// This is the function that gets called back into the driver
// to enable the interrupt in the hardware.  It will be called
// at the same IRQL at which the interrupt will be serviced:
//  - DIRQL for DIRQL interrupt handling.
//  - PASSIVE_LEVEL for passive-level interrupt handling.
//
typedef
_Function_class_(EVT_WDF_INTERRUPT_ENABLE)
_IRQL_requires_same_
_IRQL_requires_min_(PASSIVE_LEVEL)
NTSTATUS
EVT_WDF_INTERRUPT_ENABLE(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    WDFDEVICE AssociatedDevice
    );

typedef EVT_WDF_INTERRUPT_ENABLE *PFN_WDF_INTERRUPT_ENABLE;

//
// This is the function that gets called back into the driver
// to disable the interrupt in the hardware.  It will be called
// at the same IRQL at which the interrupt is serviced:
//  - DIRQL for DIRQL interrupt handling.
//  - PASSIVE_LEVEL for passive-level interrupt handling.
//
typedef
_Function_class_(EVT_WDF_INTERRUPT_DISABLE)
_IRQL_requires_same_
_IRQL_requires_min_(PASSIVE_LEVEL)
NTSTATUS
EVT_WDF_INTERRUPT_DISABLE(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    WDFDEVICE AssociatedDevice
    );

typedef EVT_WDF_INTERRUPT_DISABLE *PFN_WDF_INTERRUPT_DISABLE;

//
// Interrupt Configuration Structure
//
typedef struct _WDF_INTERRUPT_CONFIG {
    ULONG              Size;

    //
    // If this interrupt is to be synchronized with other interrupt(s) assigned
    // to the same WDFDEVICE, create a WDFSPINLOCK and assign it to each of the
    // WDFINTERRUPTs config.
    //
    WDFSPINLOCK                     SpinLock;

    WDF_TRI_STATE                   ShareVector;

    BOOLEAN                         FloatingSave;

    //
    // DIRQL handling: automatic serialization of the DpcForIsr/WaitItemForIsr.
    // Passive-level handling: automatic serialization of all callbacks.
    //
    BOOLEAN                         AutomaticSerialization;

    //
    // Event Callbacks
    //
    PFN_WDF_INTERRUPT_ISR           EvtInterruptIsr;
    PFN_WDF_INTERRUPT_DPC           EvtInterruptDpc;
    PFN_WDF_INTERRUPT_ENABLE        EvtInterruptEnable;
    PFN_WDF_INTERRUPT_DISABLE       EvtInterruptDisable;
    PFN_WDF_INTERRUPT_WORKITEM      EvtInterruptWorkItem;

    //
    // These fields are only used when interrupt is created in
    // EvtDevicePrepareHardware callback.
    //
    PCM_PARTIAL_RESOURCE_DESCRIPTOR InterruptRaw;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR InterruptTranslated;

    //
    // Optional passive lock for handling interrupts at passive-level.
    //
    WDFWAITLOCK                     WaitLock;

    //
    // TRUE: handle interrupt at passive-level.
    // FALSE: handle interrupt at DIRQL level. This is the default.
    //
    BOOLEAN                         PassiveHandling;

    //
    // TRUE: Interrupt is reported inactive on explicit power down
    //       instead of disconnecting it.
    // FALSE: Interrupt is disconnected instead of reporting inactive
    //        on explicit power down.
    // DEFAULT: Framework decides the right value.
    //
    WDF_TRI_STATE                   ReportInactiveOnPowerDown;

    //
    // TRUE: Interrupt is used to wake the device from low-power states
    //       and when the device is armed for wake this interrupt should
    //       remain connected.
    // FALSE: Interrupt is not used to wake the device from low-power states.
    //        This is the default.
    //
    BOOLEAN                         CanWakeDevice;
} WDF_INTERRUPT_CONFIG, *PWDF_INTERRUPT_CONFIG;


VOID
FORCEINLINE
WDF_INTERRUPT_CONFIG_INIT(
    _Out_ PWDF_INTERRUPT_CONFIG Configuration,
    _In_ PFN_WDF_INTERRUPT_ISR EvtInterruptIsr,
    _In_opt_ PFN_WDF_INTERRUPT_DPC EvtInterruptDpc
    )
{
    RtlZeroMemory(Configuration, sizeof(WDF_INTERRUPT_CONFIG));

    Configuration->Size = sizeof(WDF_INTERRUPT_CONFIG);

    Configuration->ShareVector = WdfUseDefault;

    Configuration->EvtInterruptIsr     = EvtInterruptIsr;
    Configuration->EvtInterruptDpc     = EvtInterruptDpc;

    Configuration->ReportInactiveOnPowerDown = WdfUseDefault;
}




//
// Disable warning C4324: structure was padded due to DECLSPEC_ALIGN
// This padding is intentional and necessary.
#pragma warning(push)
#pragma warning(disable: 4324)

typedef struct _WDF_INTERRUPT_INFO {
    //
    // Size of this structure in bytes
    //
    ULONG                  Size;
    ULONG64                Reserved1;
    KAFFINITY              TargetProcessorSet;
    ULONG                  Reserved2;
    ULONG                  MessageNumber;
    ULONG                  Vector;
    KIRQL                  Irql;
    KINTERRUPT_MODE        Mode;
    WDF_INTERRUPT_POLARITY Polarity;
    BOOLEAN                MessageSignaled;
    UCHAR                  ShareDisposition; //CM_SHARE_DISPOSITION
    DECLSPEC_ALIGN(8) USHORT Group;

} WDF_INTERRUPT_INFO, *PWDF_INTERRUPT_INFO;

#pragma warning(pop)

VOID
FORCEINLINE
WDF_INTERRUPT_INFO_INIT(
    _Out_ PWDF_INTERRUPT_INFO Info
    )
{
    RtlZeroMemory(Info, sizeof(WDF_INTERRUPT_INFO));
    Info->Size = sizeof(WDF_INTERRUPT_INFO);
}

//
// Interrupt Extended Policy Configuration Structure
//
typedef struct _WDF_INTERRUPT_EXTENDED_POLICY {
    //
    // Size of this structure in bytes
    //
    ULONG                   Size;
    WDF_INTERRUPT_POLICY    Policy;
    WDF_INTERRUPT_PRIORITY  Priority;
    GROUP_AFFINITY          TargetProcessorSetAndGroup;

} WDF_INTERRUPT_EXTENDED_POLICY, *PWDF_INTERRUPT_EXTENDED_POLICY;

VOID
FORCEINLINE
WDF_INTERRUPT_EXTENDED_POLICY_INIT(
    _Out_ PWDF_INTERRUPT_EXTENDED_POLICY ExtendedPolicy
    )
{
    RtlZeroMemory(ExtendedPolicy, sizeof(WDF_INTERRUPT_EXTENDED_POLICY));
    ExtendedPolicy->Size     = sizeof(WDF_INTERRUPT_EXTENDED_POLICY);
    ExtendedPolicy->Policy   = WdfIrqPolicyMachineDefault;
    ExtendedPolicy->Priority = WdfIrqPriorityUndefined;
}


//
// WDF Function: WdfInterruptCreate
//
typedef
_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
WDFAPI
NTSTATUS
(*PFN_WDFINTERRUPTCREATE)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE Device,
    _In_
    PWDF_INTERRUPT_CONFIG Configuration,
    _In_opt_
    PWDF_OBJECT_ATTRIBUTES Attributes,
    _Out_
    WDFINTERRUPT* Interrupt
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS
FORCEINLINE
WdfInterruptCreate(
    _In_
    WDFDEVICE Device,
    _In_
    PWDF_INTERRUPT_CONFIG Configuration,
    _In_opt_
    PWDF_OBJECT_ATTRIBUTES Attributes,
    _Out_
    WDFINTERRUPT* Interrupt
    )
{
    return ((PFN_WDFINTERRUPTCREATE) WdfFunctions[WdfInterruptCreateTableIndex])(WdfDriverGlobals, Device, Configuration, Attributes, Interrupt);
}

//
// WDF Function: WdfInterruptQueueDpcForIsr
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFINTERRUPTQUEUEDPCFORISR)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

BOOLEAN
FORCEINLINE
WdfInterruptQueueDpcForIsr(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTQUEUEDPCFORISR) WdfFunctions[WdfInterruptQueueDpcForIsrTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptQueueWorkItemForIsr
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFINTERRUPTQUEUEWORKITEMFORISR)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

BOOLEAN
FORCEINLINE
WdfInterruptQueueWorkItemForIsr(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTQUEUEWORKITEMFORISR) WdfFunctions[WdfInterruptQueueWorkItemForIsrTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptSynchronize
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
WDFAPI
BOOLEAN
(*PFN_WDFINTERRUPTSYNCHRONIZE)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    PFN_WDF_INTERRUPT_SYNCHRONIZE Callback,
    _In_
    WDFCONTEXT Context
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
BOOLEAN
FORCEINLINE
WdfInterruptSynchronize(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    PFN_WDF_INTERRUPT_SYNCHRONIZE Callback,
    _In_
    WDFCONTEXT Context
    )
{
    return ((PFN_WDFINTERRUPTSYNCHRONIZE) WdfFunctions[WdfInterruptSynchronizeTableIndex])(WdfDriverGlobals, Interrupt, Callback, Context);
}

//
// WDF Function: WdfInterruptAcquireLock
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL + 1)
WDFAPI
VOID
(*PFN_WDFINTERRUPTACQUIRELOCK)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    _Requires_lock_not_held_(_Curr_)
    _Acquires_lock_(_Curr_)
    WDFINTERRUPT Interrupt
    );

_IRQL_requires_max_(DISPATCH_LEVEL + 1)
VOID
FORCEINLINE
WdfInterruptAcquireLock(
    _In_
    _Requires_lock_not_held_(_Curr_)
    _Acquires_lock_(_Curr_)
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTACQUIRELOCK) WdfFunctions[WdfInterruptAcquireLockTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptReleaseLock
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL + 1)
WDFAPI
VOID
(*PFN_WDFINTERRUPTRELEASELOCK)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    _Requires_lock_held_(_Curr_)
    _Releases_lock_(_Curr_)
    WDFINTERRUPT Interrupt
    );

_IRQL_requires_max_(DISPATCH_LEVEL + 1)
VOID
FORCEINLINE
WdfInterruptReleaseLock(
    _In_
    _Requires_lock_held_(_Curr_)
    _Releases_lock_(_Curr_)
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTRELEASELOCK) WdfFunctions[WdfInterruptReleaseLockTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptEnable
//
typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
WDFAPI
VOID
(*PFN_WDFINTERRUPTENABLE)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID
FORCEINLINE
WdfInterruptEnable(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTENABLE) WdfFunctions[WdfInterruptEnableTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptDisable
//
typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
WDFAPI
VOID
(*PFN_WDFINTERRUPTDISABLE)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID
FORCEINLINE
WdfInterruptDisable(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTDISABLE) WdfFunctions[WdfInterruptDisableTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptWdmGetInterrupt
//
typedef
_Must_inspect_result_
WDFAPI
PKINTERRUPT
(*PFN_WDFINTERRUPTWDMGETINTERRUPT)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

_Must_inspect_result_
PKINTERRUPT
FORCEINLINE
WdfInterruptWdmGetInterrupt(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTWDMGETINTERRUPT) WdfFunctions[WdfInterruptWdmGetInterruptTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptGetInfo
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
WDFAPI
VOID
(*PFN_WDFINTERRUPTGETINFO)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt,
    _Out_
    PWDF_INTERRUPT_INFO Info
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
FORCEINLINE
WdfInterruptGetInfo(
    _In_
    WDFINTERRUPT Interrupt,
    _Out_
    PWDF_INTERRUPT_INFO Info
    )
{
    ((PFN_WDFINTERRUPTGETINFO) WdfFunctions[WdfInterruptGetInfoTableIndex])(WdfDriverGlobals, Interrupt, Info);
}

//
// WDF Function: WdfInterruptSetPolicy
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
WDFAPI
VOID
(*PFN_WDFINTERRUPTSETPOLICY)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    WDF_INTERRUPT_POLICY Policy,
    _In_
    WDF_INTERRUPT_PRIORITY Priority,
    _In_
    KAFFINITY TargetProcessorSet
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
FORCEINLINE
WdfInterruptSetPolicy(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    WDF_INTERRUPT_POLICY Policy,
    _In_
    WDF_INTERRUPT_PRIORITY Priority,
    _In_
    KAFFINITY TargetProcessorSet
    )
{
    ((PFN_WDFINTERRUPTSETPOLICY) WdfFunctions[WdfInterruptSetPolicyTableIndex])(WdfDriverGlobals, Interrupt, Policy, Priority, TargetProcessorSet);
}

//
// WDF Function: WdfInterruptSetExtendedPolicy
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
WDFAPI
VOID
(*PFN_WDFINTERRUPTSETEXTENDEDPOLICY)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    PWDF_INTERRUPT_EXTENDED_POLICY PolicyAndGroup
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
FORCEINLINE
WdfInterruptSetExtendedPolicy(
    _In_
    WDFINTERRUPT Interrupt,
    _In_
    PWDF_INTERRUPT_EXTENDED_POLICY PolicyAndGroup
    )
{
    ((PFN_WDFINTERRUPTSETEXTENDEDPOLICY) WdfFunctions[WdfInterruptSetExtendedPolicyTableIndex])(WdfDriverGlobals, Interrupt, PolicyAndGroup);
}

//
// WDF Function: WdfInterruptGetDevice
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFINTERRUPTGETDEVICE)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

WDFDEVICE
FORCEINLINE
WdfInterruptGetDevice(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTGETDEVICE) WdfFunctions[WdfInterruptGetDeviceTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptTryToAcquireLock
//
typedef
_Must_inspect_result_
_Post_satisfies_(return == 1 || return == 0)
_IRQL_requires_max_(PASSIVE_LEVEL)
WDFAPI
BOOLEAN
(*PFN_WDFINTERRUPTTRYTOACQUIRELOCK)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    _Requires_lock_not_held_(_Curr_)
    _When_(return!=0, _Acquires_lock_(_Curr_))
    WDFINTERRUPT Interrupt
    );

_Must_inspect_result_
_Post_satisfies_(return == 1 || return == 0)
_IRQL_requires_max_(PASSIVE_LEVEL)
BOOLEAN
FORCEINLINE
WdfInterruptTryToAcquireLock(
    _In_
    _Requires_lock_not_held_(_Curr_)
    _When_(return!=0, _Acquires_lock_(_Curr_))
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTTRYTOACQUIRELOCK) WdfFunctions[WdfInterruptTryToAcquireLockTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptReportActive
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
WDFAPI
VOID
(*PFN_WDFINTERRUPTREPORTACTIVE)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
FORCEINLINE
WdfInterruptReportActive(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTREPORTACTIVE) WdfFunctions[WdfInterruptReportActiveTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptReportInactive
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
WDFAPI
VOID
(*PFN_WDFINTERRUPTREPORTINACTIVE)(
    _In_
    PWDF_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFINTERRUPT Interrupt
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
FORCEINLINE
WdfInterruptReportInactive(
    _In_
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTREPORTINACTIVE) WdfFunctions[WdfInterruptReportInactiveTableIndex])(WdfDriverGlobals, Interrupt);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


WDF_EXTERN_C_END

#endif // _WDFINTERRUPT_H_
