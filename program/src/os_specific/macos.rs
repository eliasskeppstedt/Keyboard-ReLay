/*
use rdev::{Key};
use core_foundation::runloop::{kCFRunLoopCommonModes, CFRunLoop};
use core_graphics::event::{CGEventTap, CGEventTapLocation, CGEventTapPlacement, CGEventTapOptions, CGEventType, CallbackResult};
let current = CFRunLoop::get_current();

CGEventTap::with_enabled(
    CGEventTapLocation::HID,
    CGEventTapPlacement::HeadInsertEventTap,
    CGEventTapOptions::Default,
    vec![CGEventType::MouseMoved],
    |_proxy, _type, event| {
        println!("{:?}", event.location());
        CallbackResult::Keep
    },
    ||  CFRunLoop::run_current(),
).expect("Failed to install event tap");

*/