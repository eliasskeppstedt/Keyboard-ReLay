use rdev::{Event, listen};

mod evhandler;
mod remap;

fn main() {
    let remap_table = remap::build_map();

    if let Err(error) = listen({
        let remap_table = remap_table;
        move | event: Event | evhandler::callback(&remap_table, event)
    }) {
        println!("Error: {:?}", error);
    };
}