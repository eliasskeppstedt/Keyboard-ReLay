# Quartz Event Service relevant info

[**Quartz Event Service Reference (PDF)**](https://leopard-adc.pepas.com/documentation/Carbon/Reference/QuartzEventServicesRef/QuartzEventServicesRef.pdf)

For enabling listening for certain events, every one of those events needs to be masked and put inside the
insteresting events spot at creation of the event tap.

## Functions:  
`CGEventGetIntegerValueField(event, value to extract from event)`

---

## Modifier keys

Modifier keys produce **`kCGEventFlagsChanged`** events, not **`kCGEventKeyDown`** events, and do so both on press and release. \
The volume, brightness, and CD eject keys on some keyboards (both desktop and laptop) do not generate key up or key down events.  
*(page 27)*
