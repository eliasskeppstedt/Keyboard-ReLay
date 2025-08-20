use rdev::{grab, listen, Event, Button, Key};
use std::{collections::HashMap};
mod remap;
mod eventhandler;

use std::time::{Duration, Instant};
pub fn main() {
    let start_time = Instant::now();
    let timeout = Duration::from_secs(30); // timeout 5 sec if i screw up inputs in eventhandler
    // check which os and start listening mechanism corresponding to that os if implemented
    // not needed if using rdev since its cross os support
    #[cfg(feature = "unstable_grab")] // grab needs this feature thus this cfg flag,,, 
    let callback = {
        let remap_table: HashMap<Key, Key> = remap::build_map();
        println!("Map built!");
        move |event: Event| -> Option<Event> {
            timer(start_time, timeout);
            eventhandler::send_mapped_key(&remap_table, event)
        }
    };

    #[cfg(feature = "unstable_grab")]
    // This will block.
    if let Err(error) = grab(callback) {
        println!("Error: {:?}", error)
    }
}

fn timer(start_time : Instant, timeout : Duration) {
    if start_time.elapsed() > timeout {
        println!("timeout!!!");
        std::process::exit(0);
    }
}