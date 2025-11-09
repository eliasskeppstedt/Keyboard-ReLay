# Keyboard ReLay - info

## How it works


## Core Foundation
A framework Apple provides that enables low level functionalities for programs on Mac OS.

### Creating objects - [**Ownership Policy**](https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html#//apple_ref/doc/uid/20001148-CJBEJBHH)
Creating a CF object makes caller the owner, thus it is responsible for releasing it with `CFRelease()` ⇒ reference count for the object is now 1.  
In order to let an object live longer than its caller, ownership must be transfered with `CFRetain()`. You could say that the object now is owned by both the original owner and the new owner, and the objects reference count is therefor now 2. An object is disposed only when its reference count is 0, i.e. when all owners has freed their respective reference to the object.

### Quartz Event Service - [**Quartz Event Service Reference (PDF)**](https://leopard-adc.pepas.com/documentation/Carbon/Reference/QuartzEventServicesRef/QuartzEventServicesRef.pdf)
An API provided by Apple for handling low-level input events under the `Core Graphics` framework, which provides functionalities for 2D rendering. `Core Graphics` is in turn part of the `Core Foundation`.

[**CFRunLoop Reference**](https://leopard-adc.pepas.com/documentation/CoreFoundation/Reference/CFRunLoopRef/CFRunLoopRef.pdf)
For enabling listening for certain events, every one of those events needs to be masked and put inside the
insteresting events spot at creation of the event tap.

### Modifier keys

Modifier keys produce **`kCGEventFlagsChanged`** events, not **`kCGEventKeyDown`** events, and do so both on press and release. \
The volume, brightness, and CD eject keys on some keyboards (both desktop and laptop) do not generate key up or key down events.  
*(page 27)*
#### Used constants
```c
K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
// CGEventType:
kCGEventKeyDown
kCGEventKeyUp
kCGEventFlagsChanged
//
kCGKeyboardEventKeycode
kCGKeyboardEventAutorepeat
kCGHIDEventTap
kCGHeadInsertEventTap
kCFAllocatorDefault
kCFRunLoopCommonModes
```
#### Used functions
##### In quartz event service reference
```c
CFMachPortRef CGEventTapCreate (
    CGEventTapLocation tap, // window server, login session, specific annotation
    CGEventTapPlacement place, // head, tail
    CGEventTapOptions options, // default, listen only
    CGEventMask eventsOfInterest, // mouse, keyboard, etc
    CGEventTapCallBack callback, // callback func called when a quartz event is triggered
    void *refcon // custom info
);

CGEventRef MyEventTapCallBack (
    CGEventTapProxy proxy,
    CGEventType type,
    CGEventRef event,
    void *refcon
);

int64_t CGEventGetIntegerValueField (
    int64_t CGEventRef event,
    CGEventField field
);

CGEventFlags CGEventGetFlags (
    CGEventRef event
);

void CGEventSetFlags (
    CGEventRef event,
    CGEventFlags flags
); 

void CGEventSetIntegerValueField (
    CGEventRef event,
    CGEventField field,
    int64_t value
);

void CGEventTapEnable (
    CFMachPortRef myTap,
    bool enable
);
```
##### Run loop reference
```c
CFRunLoopTimerRef CFRunLoopTimerCreate(
    CFAllocatorRef allocator, 
    CFAbsoluteTime fireDate, 
    CFTimeInterval interval, 
    CFOptionFlags flags, 
    CFIndex order, 
    CFRunLoopTimerCallBack callout, 
    CFRunLoopTimerContext * context
);
```
##### Somewhere else...
```c
CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
    kCFAllocatorDefault, 
    eventTap, 
    0
);

CFRunLoopAddSource(
    runLoop, 
    runLoopSource, 
    kCFRunLoopCommonModes
);
CFRunLoopRun();
CFMachPortInvalidate(eventTap);
CFRelease(runLoopSource);
CFRelease(eventTap);
CFRunLoopStop(pData->runLoop);
``` 