use rdev::{simulate, Button, Event, EventType, Key, SimulateError};
use std::{collections::{HashMap, HashSet}, fs, process, sync::{Mutex, MutexGuard}, thread, time};
use once_cell::sync::Lazy;
use crate::remap;

// lock mechanism for each key, since i cant modify events with tools from rdev crate (YET!)
pub static LOCKED_KEYS : Lazy<Mutex<HashSet<Key>>> = Lazy::new(|| {
    Mutex::new(HashSet::new())
});

// Yes
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

// does the logic for which keys should be sent
pub fn send_mapped_key(remap_table: &HashMap<Key, Key>, event: Event) -> Option<Event> {
    match event.event_type {
        EventType::KeyPress(Key::Escape) => { 
            process::exit(0) 
        },
        EventType::KeyPress(pressed_key) => {
            let mut lock = LOCKED_KEYS.lock().unwrap();
            
            if lock.contains(&pressed_key) {
                // if lock have pressed key, this event was simulated and we should therefor
                // not simulate a new press
                lock.remove(&pressed_key);
            } else if let Some(&mapped_key) = remap_table.get(&pressed_key) {
                // no lock means it was a physical press, and we should then simulate the mapped key.
                lock.insert(mapped_key);
                send(&EventType::KeyPress(mapped_key));
                return None
            }
            Some(event)
        }
        EventType::KeyRelease(released_key) => {
            // If key is locked, it is an event triggered my a simulated keypress. To enable holding on
            // a key KeyRelease should only be triggered by the physical release of the key.
            let lock = LOCKED_KEYS.lock().unwrap();
            if lock.contains(&released_key) {
                return None;
            } else {
                return Some(event);
            }
        }
        _ => Some(event),
    }
}

fn get_mapped_key(remap_table: &HashMap<Key, Key>, pressed_key: Key) -> Key {
    if let Some(&remapped_key) = remap_table.get(&pressed_key) {
        return remapped_key;
    } else {
        return pressed_key;
    }
}