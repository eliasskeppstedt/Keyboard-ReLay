use serde_json::{Map, Value};
use rdev::{Key};
use std::{collections::HashMap, fs};

pub fn get_remaped_key(pressed_key: Key, remap_table: &HashMap<Key, Key>) -> Key {
    if let Some(&new_key) = remap_table.get(&pressed_key) {
        return new_key;
    } else { pressed_key }
    //map.get(&src).copied().unwrap_or(src)
}

// build map for all new bindings, thus keys who hasnt been remapped
// will not enter this map
pub fn build_map() -> HashMap<Key, Key> {
    let mut map: HashMap<Key, Key> = HashMap::new();
    if let Some(obj) = remap_layout() {
        for (src_name, dst_val) in obj {
            if let Some(dst_name) = dst_val.as_str() {
                let src_key = get_key(&src_name);
                let dst_key = get_key(dst_name);
                map.insert(src_key, dst_key);
            }
        }
    }
    return map;
}

// build map for all bindings, even for keys who is not remapped
fn remap_layout() -> Option<Map<String, Value>> {
    let json_string: String  = fs::read_to_string("./src/remaps.json")
        .expect("Bad json file, begin thread destruction\n{error}");

    let json_value: Value = serde_json::from_str(&json_string)
        .expect("JSON is WRONG");
    //serde_json::from_str(&remap_us).expect("Invalid JSON in remaps.json")//unwrap()

    return json_value.get("remaps_rdev").and_then(|v| v.as_object().cloned());
}

// from str to the key enum type in rdev
pub fn get_key(name: &str) -> Key {
    match name {
        "Alt" => Key::Alt,
        "AltGr" => Key::AltGr,
        "Backspace" => Key::Backspace,
        "CapsLock" => Key::CapsLock,
        "ControlLeft" => Key::ControlLeft,
        "ControlRight" => Key::ControlRight,
        "Delete" => Key::Delete,
        "DownArrow" => Key::DownArrow,
        "End" => Key::End,
        "Escape" => Key::Escape,
        "F1" => Key::F1,
        "F10" => Key::F10,
        "F11" => Key::F11,
        "F12" => Key::F12,
        "F2" => Key::F2,
        "F3" => Key::F3,
        "F4" => Key::F4,
        "F5" => Key::F5,
        "F6" => Key::F6,
        "F7" => Key::F7,
        "F8" => Key::F8,
        "F9" => Key::F9,
        "Home" => Key::Home,
        "LeftArrow" => Key::LeftArrow,
        "MetaLeft" => Key::MetaLeft,
        "MetaRight" => Key::MetaRight,
        "PageDown" => Key::PageDown,
        "PageUp" => Key::PageUp,
        "Return" => Key::Return,
        "RightArrow" => Key::RightArrow,
        "ShiftLeft" => Key::ShiftLeft,
        "ShiftRight" => Key::ShiftRight,
        "Space" => Key::Space,
        "Tab" => Key::Tab,
        "UpArrow" => Key::UpArrow,
        "PrintScreen" => Key::PrintScreen,
        "ScrollLock" => Key::ScrollLock,
        "Pause" => Key::Pause,
        "NumLock" => Key::NumLock,
        "BackQuote" => Key::BackQuote,
        "Num1" => Key::Num1,
        "Num2" => Key::Num2,
        "Num3" => Key::Num3,
        "Num4" => Key::Num4,
        "Num5" => Key::Num5,
        "Num6" => Key::Num6,
        "Num7" => Key::Num7,
        "Num8" => Key::Num8,
        "Num9" => Key::Num9,
        "Num0" => Key::Num0,
        "Minus" => Key::Minus,
        "Equal" => Key::Equal,
        "KeyQ" => Key::KeyQ,
        "KeyW" => Key::KeyW,
        "KeyE" => Key::KeyE,
        "KeyR" => Key::KeyR,
        "KeyT" => Key::KeyT,
        "KeyY" => Key::KeyY,
        "KeyU" => Key::KeyU,
        "KeyI" => Key::KeyI,
        "KeyO" => Key::KeyO,
        "KeyP" => Key::KeyP,
        "LeftBracket" => Key::LeftBracket,
        "RightBracket" => Key::RightBracket,
        "KeyA" => Key::KeyA,
        "KeyS" => Key::KeyS,
        "KeyD" => Key::KeyD,
        "KeyF" => Key::KeyF,
        "KeyG" => Key::KeyG,
        "KeyH" => Key::KeyH,
        "KeyJ" => Key::KeyJ,
        "KeyK" => Key::KeyK,
        "KeyL" => Key::KeyL,
        "SemiColon" => Key::SemiColon,
        "Quote" => Key::Quote,
        "BackSlash" => Key::BackSlash,
        "IntlBackslash" => Key::IntlBackslash,
        "KeyZ" => Key::KeyZ,
        "KeyX" => Key::KeyX,
        "KeyC" => Key::KeyC,
        "KeyV" => Key::KeyV,
        "KeyB" => Key::KeyB,
        "KeyN" => Key::KeyN,
        "KeyM" => Key::KeyM,
        "Comma" => Key::Comma,
        "Dot" => Key::Dot,
        "Slash" => Key::Slash,
        "Insert" => Key::Insert,
        "KpReturn" => Key::KpReturn,
        "KpMinus" => Key::KpMinus,
        "KpPlus" => Key::KpPlus,
        "KpMultiply" => Key::KpMultiply,
        "KpDivide" => Key::KpDivide,
        "Kp0" => Key::Kp0,
        "Kp1" => Key::Kp1,
        "Kp2" => Key::Kp2,
        "Kp3" => Key::Kp3,
        "Kp4" => Key::Kp4,
        "Kp5" => Key::Kp5,
        "Kp6" => Key::Kp6,
        "Kp7" => Key::Kp7,
        "Kp8" => Key::Kp8,
        "Kp9" => Key::Kp9,
        "KpDelete" => Key::KpDelete,
        "Function" => Key::Function,
        _ => Key::Unknown(0),
    }
}