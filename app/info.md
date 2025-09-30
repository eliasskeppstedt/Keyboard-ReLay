# Keyboard ReLay - info
## Quartz Event Service

[**Quartz Event Service Reference (PDF)**](https://leopard-adc.pepas.com/documentation/Carbon/Reference/QuartzEventServicesRef/QuartzEventServicesRef.pdf) //
[**CFRunLoop Reference**](https://leopard-adc.pepas.com/documentation/CoreFoundation/Reference/CFRunLoopRef/CFRunLoopRef.pdf)

For enabling listening for certain events, every one of those events needs to be masked and put inside the
insteresting events spot at creation of the event tap.

## Modifier keys

Modifier keys produce **`kCGEventFlagsChanged`** events, not **`kCGEventKeyDown`** events, and do so both on press and release. \
The volume, brightness, and CD eject keys on some keyboards (both desktop and laptop) do not generate key up or key down events.  
*(page 27)*
### Used constants
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
### Used functions
#### In quartz event service reference
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
#### Run loop reference
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
#### Somewhere else...
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
# Mac