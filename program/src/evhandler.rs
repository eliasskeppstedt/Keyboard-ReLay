use rdev::{Event, EventType, Key, simulate, SimulateError};
use std::{collections::{HashMap, HashSet}, sync::Mutex};
use serde_json::{Value};
use once_cell::sync::{Lazy};
use std::{thread, time};

use crate::remap::{get_remaped_key};

// For a lock mechanism for the programs listening event, and is to prevent race conditionns which could
// lead to wrong key outputs or crashes (maybe? :P ). Each simulated keypress will trigger a KeyPress event which
// makes each physicall key press trigger 2 events, thus creating a forever "triggering" loop. 
// With this locking technique each physical keypress will lock that specific key from triggering a new
// KeyPress event, and when the simulated keypress event comes, we just unlock that key. 
static KEYS_DOWN: Lazy<Mutex<HashSet<Key>>> = Lazy::new(|| Mutex::new(HashSet::new()));

// is called on each time an event is triggered
pub fn callback(remap_table: &HashMap<Key, Key>, event: Event) {
    // if en event type matches the event for a certain key we should output it. This should only be done
    // after the physical.
    match event.event_type {
        EventType::KeyPress(pressed_key) => {
            if !physical_key_event(pressed_key) { return } // was the simulated keypressf
            
            let new_key: Key = get_remaped_key(pressed_key, remap_table);
            if new_key == pressed_key { return }

            send(&EventType::KeyPress(new_key));
        },
        EventType::KeyRelease(pressed_key) => {
            simulated_key_event(pressed_key);

            let new_key: Key = get_remaped_key(pressed_key, remap_table);
            if new_key != pressed_key { 
                send(&EventType::KeyRelease(new_key));
            }
        },
        _ => ()
    }
}

// yes
fn send(event_type: &EventType) {
    let delay = time::Duration::from_millis(20);
    match simulate(event_type) {
        Ok(()) => (),
        Err(SimulateError) => {
            println!("We could not send {:?}", event_type);
        }
    }
    // Let ths OS catchup (at least MacOS)
    thread::sleep(delay);
}

pub fn physical_key_event(pressed_key: Key) -> bool {
    let mut keys_down = KEYS_DOWN.lock().expect("key_pressed: KEYS_DOWN mutex crashed");
    // a key only has a mutex lock over it on the evaluation stage, line 57, since it is only local 
    // and will drop after the function. It is not important which thread is
    // evalutaing its key, the crucial part is that during the evaluation process it needs to have exclusive
    // access to that specific key inside the hashmap, to validate if its there or not. So this is done 
    // to assure a correct amount of physical and simulated keypresses.
    keys_down.insert(pressed_key)
}

pub fn simulated_key_event(pressed_key: Key) {
    let mut keys_down = KEYS_DOWN.lock().unwrap();
    keys_down.remove(&pressed_key);
}