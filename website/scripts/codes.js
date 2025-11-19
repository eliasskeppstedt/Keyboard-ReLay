// ============================================================================
// Virtual Keycodes (hardware-independent, OS-level)
// ============================================================================
// == ANSI/ISO/JIS scan codes, US vk codes ==
export const VK_BASE_MAP = {
    // 0x00–0x0F
    KeyA: {
        LEGEND: "a",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x00,
    },
    KeyS: {
        LEGEND: "s",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x01,
    },
    KeyD: {
        LEGEND: "d",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x02,
    },
    KeyF: {
        LEGEND: "f",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x03,
    },
    KeyH: {
        LEGEND: "h",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x04,
    },
    KeyG: {
        LEGEND: "g",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x05,
    },
    KeyZ: {
        LEGEND: "z",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x06,
    },
    KeyX: {
        LEGEND: "x",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x07,
    },
    KeyC: {
        LEGEND: "c",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x08,
    },
    KeyV: {
        LEGEND: "v",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x09,
    },
    KeyB: {
        LEGEND: "b",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x0B,
    },
    KeyQ: {
        LEGEND: "q",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x0C,
    },
    KeyW: {
        LEGEND: "w",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x0D,
    },
    KeyE: {
        LEGEND: "e",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x0E,
    },
    KeyR: {
        LEGEND: "r",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x0F,
    },

    // 0x10–0x1F
    KeyY: {
        LEGEND: "y",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x10,
    },
    KeyT: {
        LEGEND: "t",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x11,
    },
    Digit1: {
        LEGEND: "1",
        DESCRIPTION: "one",
        SHOWDESCRIPTION: false,
        MAC: 0x12,
    },
    Digit2: {
        LEGEND: "2",
        DESCRIPTION: "two",
        SHOWDESCRIPTION: false,
        MAC: 0x13,
    },
    Digit3: {
        LEGEND: "3",
        DESCRIPTION: "three",
        SHOWDESCRIPTION: false,
        MAC: 0x14,
    },
    Digit4: {
        LEGEND: "4",
        DESCRIPTION: "four",
        SHOWDESCRIPTION: false,
        MAC: 0x15,
    },
    Digit6: {
        LEGEND: "6",
        DESCRIPTION: "six",
        SHOWDESCRIPTION: false,
        MAC: 0x16,
    },
    Digit5: {
        LEGEND: "5",
        DESCRIPTION: "five",
        SHOWDESCRIPTION: false,
        MAC: 0x17,
    },
    Equal: {
        LEGEND: "=",
        DESCRIPTION: "equal",
        SHOWDESCRIPTION: false,
        MAC: 0x18,
    },
    Digit9: {
        LEGEND: "9",
        DESCRIPTION: "nine",
        SHOWDESCRIPTION: false,
        MAC: 0x19,
    },
    Digit7: {
        LEGEND: "7",
        DESCRIPTION: "seven",
        SHOWDESCRIPTION: false,
        MAC: 0x1A,
    },
    Minus: {
        LEGEND: "-",
        DESCRIPTION: "minus",
        SHOWDESCRIPTION: false,
        MAC: 0x1B,
    },
    Digit8: {
        LEGEND: "8",
        DESCRIPTION: "eight",
        SHOWDESCRIPTION: false,
        MAC: 0x1C,
    },
    Digit0: {
        LEGEND: "0",
        DESCRIPTION: "zero",
        SHOWDESCRIPTION: false,
        MAC: 0x1D,
    },
    BracketRight: {
        LEGEND: "]",
        DESCRIPTION: "R bracket;right bracket",
        SHOWDESCRIPTION: false,
        MAC: 0x1E,
    },
    KeyO: {
        LEGEND: "o",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x1F,
    },

    // 0x20–0x2F
    KeyU: {
        LEGEND: "u",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x20,
    },
    BracketLeft: {
        LEGEND: "[",
        DESCRIPTION: "L bracket;left bracket",
        SHOWDESCRIPTION: false,
        MAC: 0x21,
    },
    KeyI: {
        LEGEND: "i",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x22,
    },
    KeyP: {
        LEGEND: "p",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x23,
    },
    Enter: {
        LEGEND: "⏎",
        DESCRIPTION: "enter",
        SHOWDESCRIPTION: false,
        MAC: 0x24,
    }, // Return
    KeyL: {
        LEGEND: "l",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x25,
    },
    KeyJ: {
        LEGEND: "j",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x26,
    },
    Quote: {
        LEGEND: "'",
        DESCRIPTION: "apostrophe",
        SHOWDESCRIPTION: false,
        MAC: 0x27,
    },
    KeyK: {
        LEGEND: "k",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x28,
    },
    Semicolon: {
        LEGEND: ";",
        DESCRIPTION: "semicolon",
        SHOWDESCRIPTION: false,
        MAC: 0x29,
    },
    Backslash: {
        LEGEND: "\\",
        DESCRIPTION: "backslash",
        SHOWDESCRIPTION: false,
        MAC: 0x2A,
    },
    Comma: {
        LEGEND: ",",
        DESCRIPTION: "comma",
        SHOWDESCRIPTION: false,
        MAC: 0x2B,
    },
    Slash: {
        LEGEND: "/",
        DESCRIPTION: "slash",
        SHOWDESCRIPTION: false,
        MAC: 0x2C,
    },
    KeyN: {
        LEGEND: "n",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x2D,
    },
    KeyM: {
        LEGEND: "m",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x2E,
    },
    Period: {
        LEGEND: ".",
        DESCRIPTION: "period",
        SHOWDESCRIPTION: false,
        MAC: 0x2F,
    },
        // 0x30–0x3F
    Tab: {
        LEGEND: "⇥",
        DESCRIPTION: "tab",
        SHOWDESCRIPTION: true,
        MAC: 0x30,
    },
    Space: {
        LEGEND: "␣",
        DESCRIPTION: "space",
        SHOWDESCRIPTION: false,
        MAC: 0x31,
    },
    Backquote: {
        LEGEND: "`",
        DESCRIPTION: "backquote",
        SHOWDESCRIPTION: false,
        MAC: 0x32,
    },
    Backspace: {
        LEGEND: "⌫",
        DESCRIPTION: "backspace",
        SHOWDESCRIPTION: false,
        MAC: 0x33,
    }, // Delete (backspace)
    // 0x34 numeric keypad enter — NO Web code:
    // NumpadEnter     : { LEGEND: "⏎",    MAC: 0x34 },
    Escape: {
        LEGEND: "⎋",
        DESCRIPTION: "esc;escape",
        SHOWDESCRIPTION: true,
        MAC: 0x35,
    },
    MetaRight: {
        LEGEND: "R ⌘",
        DESCRIPTION: "R cmd;right command",
        SHOWDESCRIPTION: false,
        MAC: 0x36,
    }, // Right Command
    MetaLeft: {
        LEGEND: "L ⌘",
        DESCRIPTION: "L cmd;left command",
        SHOWDESCRIPTION: false,
        MAC: 0x37,
    }, // Command
    ShiftLeft: {
        LEGEND: "L ⇧",
        DESCRIPTION: "L shift;left shift",
        SHOWDESCRIPTION: false,
        MAC: 0x38,
    },
    CapsLock: {
        LEGEND: "⇪",
        DESCRIPTION: "caps lock",
        SHOWDESCRIPTION: true,
        MAC: 0x39,
    },
    AltLeft: {
        LEGEND: "L ⌥",
        DESCRIPTION: "L opt;left option",
        SHOWDESCRIPTION: false,
        MAC: 0x3A,
    }, // Option
    ControlLeft: {
        LEGEND: "L ⌃",
        DESCRIPTION: "L ctrl;left control",
        SHOWDESCRIPTION: false,
        MAC: 0x3B,
    },
    ShiftRight: {
        LEGEND: "R ⇧",
        DESCRIPTION: "R shift;right shift",
        SHOWDESCRIPTION: false,
        MAC: 0x3C,
    },
    AltRight: {
        LEGEND: "R ⌥",
        DESCRIPTION: "R opt;right option",
        SHOWDESCRIPTION: false,
        MAC: 0x3D,
    },
    ControlRight: {
        LEGEND: "R ⌃",
        DESCRIPTION: "R ctrl;right control",
        SHOWDESCRIPTION: false,
        MAC: 0x3E,
    },
    // 0x3F Fn key does not generate events
    // Fn              : { LEGEND: "Fn",   MAC: 0x3F }, // no Web code fired

    // 0x40–0x4F
    F17: {
        LEGEND: "F17",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x40,
    },
    NumpadDecimal: {
        LEGEND: ".",
        DESCRIPTION: "period",
        SHOWDESCRIPTION: false,
        MAC: 0x41,
    },
    // 0x42 unused
    NumpadMultiply: {
        LEGEND: "*",
        DESCRIPTION: "multiply",
        SHOWDESCRIPTION: false,
        MAC: 0x43,
    },
    // 0x44 unused
    NumpadAdd: {
        LEGEND: "+",
        DESCRIPTION: "add",
        SHOWDESCRIPTION: false,
        MAC: 0x45,
    },
    // 0x46 unused
    NumLock: {
        LEGEND: "⇭",
        DESCRIPTION: "num lock",
        SHOWDESCRIPTION: true,
        MAC: 0x47,
    }, // Clear key
    AudioVolumeUp: {
        LEGEND: "🔊",
        DESCRIPTION: "vol up;volume up",
        SHOWDESCRIPTION: false,
        MAC: 0x48,
    },
    AudioVolumeDown: {
        LEGEND: "🔉",
        DESCRIPTION: "vol down;volume down",
        SHOWDESCRIPTION: false,
        MAC: 0x49,
    },
    AudioVolumeMute: {
        LEGEND: "🔇",
        DESCRIPTION: "mute",
        SHOWDESCRIPTION: false,
        MAC: 0x4A,
    },
    NumpadDivide: {
        LEGEND: "/",
        DESCRIPTION: "slash",
        SHOWDESCRIPTION: false,
        MAC: 0x4B,
    },
    NumpadEnter: {
        LEGEND: "⏎",
        DESCRIPTION: "enter",
        SHOWDESCRIPTION: false,
        MAC: 0x4C,
    },
    // 0x4D unused
    NumpadSubtract: {
        LEGEND: "-",
        DESCRIPTION: "minus",
        SHOWDESCRIPTION: false,
        MAC: 0x4E,
    },
    F18: {
        LEGEND: "F18",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x4F,
    },

    // 0x50–0x5F
    F19: {
        LEGEND: "F19",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x50,
    },
    NumpadEqual: {
        LEGEND: "=",
        DESCRIPTION: "equal",
        SHOWDESCRIPTION: false,
        MAC: 0x51,
    },
    Numpad0: {
        LEGEND: "0",
        DESCRIPTION: "zero",
        SHOWDESCRIPTION: false,
        MAC: 0x52,
    },
    Numpad1: {
        LEGEND: "1",
        DESCRIPTION: "one",
        SHOWDESCRIPTION: false,
        MAC: 0x53,
    },
    Numpad2: {
        LEGEND: "2",
        DESCRIPTION: "two",
        SHOWDESCRIPTION: false,
        MAC: 0x54,
    },
    Numpad3: {
        LEGEND: "3",
        DESCRIPTION: "three",
        SHOWDESCRIPTION: false,
        MAC: 0x55,
    },
    Numpad4: {
        LEGEND: "4",
        DESCRIPTION: "four",
        SHOWDESCRIPTION: false,
        MAC: 0x56,
    },
    Numpad5: {
        LEGEND: "5",
        DESCRIPTION: "five",
        SHOWDESCRIPTION: false,
        MAC: 0x57,
    },
    Numpad6: {
        LEGEND: "6",
        DESCRIPTION: "six",
        SHOWDESCRIPTION: false,
        MAC: 0x58,
    },
    Numpad7: {
        LEGEND: "7",
        DESCRIPTION: "seven",
        SHOWDESCRIPTION: false,
        MAC: 0x59,
    },
    F20: {
        LEGEND: "F20",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x5A,
    },
    Numpad8: {
        LEGEND: "8",
        DESCRIPTION: "eight",
        SHOWDESCRIPTION: false,
        MAC: 0x5B,
    },
    Numpad9: {
        LEGEND: "9",
        DESCRIPTION: "nine",
        SHOWDESCRIPTION: false,
        MAC: 0x5C,
    },
        // 0x60–0x6F
    F5: {
        LEGEND: "F5",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x60,
    },
    F6: {
        LEGEND: "F6",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x61,
    },
    F7: {
        LEGEND: "F7",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x62,
    },
    F3: {
        LEGEND: "F3",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x63,
    },
    F8: {
        LEGEND: "F8",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x64,
    },
    F9: {
        LEGEND: "F9",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x65,
    },
    F11: {
        LEGEND: "F11",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x67,
    },
    F13: {
        LEGEND: "F13",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x69,
    },
    F16: {
        LEGEND: "F16",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x6A,
    },
    F14: {
        LEGEND: "F14",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x6B,
    },
    // 0x6C unused
    F10: {
        LEGEND: "F10",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x6D,
    },
    ContextMenu: {
        LEGEND: "≣",
        DESCRIPTION: "context menu",
        SHOWDESCRIPTION: true,
        MAC: 0x6E,
    },
    F12: {
        LEGEND: "F12",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x6F,
    },

    // 0x70–0x7F
    // 0x70 unused
    F15: {
        LEGEND: "F15",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x71,
    },
    // Chromium: Insert, Firefox: Help → use Insert
    Insert: {
        LEGEND: "Ins",
        DESCRIPTION: "insert",
        SHOWDESCRIPTION: false,
        MAC: 0x72,
    },
    Home: {
        LEGEND: "Home",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x73,
    },
    PageUp: {
        LEGEND: "PgUp",
        DESCRIPTION: "page up",
        SHOWDESCRIPTION: false,
        MAC: 0x74,
    },
    Delete: {
        LEGEND: "⌦",
        DESCRIPTION: "del;delete",
        SHOWDESCRIPTION: false,
        MAC: 0x75,
    }, // ForwardDelete
    F4: {
        LEGEND: "F4",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x76,
    },
    End: {
        LEGEND: "End",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x77,
    },
    F2: {
        LEGEND: "F2",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x78,
    },
    PageDown: {
        LEGEND: "PgDn",
        DESCRIPTION: "page down",
        SHOWDESCRIPTION: false,
        MAC: 0x79,
    },
    F1: {
        LEGEND: "F1",
        DESCRIPTION: "",
        SHOWDESCRIPTION: false,
        MAC: 0x7A,
    },
    ArrowLeft: {
        LEGEND: "←",
        DESCRIPTION: "arrow left",
        SHOWDESCRIPTION: false,
        MAC: 0x7B,
    },
    ArrowRight: {
        LEGEND: "→",
        DESCRIPTION: "arrow right",
        SHOWDESCRIPTION: false,
        MAC: 0x7C,
    },
    ArrowDown: {
        LEGEND: "↓",
        DESCRIPTION: "arrow down",
        SHOWDESCRIPTION: false,
        MAC: 0x7D,
    },
    ArrowUp: {
        LEGEND: "↑",
        DESCRIPTION: "arrow up",
        SHOWDESCRIPTION: false,
        MAC: 0x7E,
    },
    // 0x7F Power key → no Web code
    // Power           : { LEGEND: "Power",MAC: 0x7F }, // no Web code

    // 0xA0
    // Mission Control key — NO standard Web code:
    // MissionControl  : { LEGEND: "Mission",      MAC: 0xA0 }, // no Web code
};

// ============================================================================
// Scan-code symbol maps (layout-dependent, language-neutral)
// ============================================================================
// == ANSI ==
export const SC_ANSI = {
    // Number row
    Backquote: "",
    Digit1: "",
    Digit2: "",
    Digit3: "",
    Digit4: "",
    Digit5: "",
    Digit6: "",
    Digit7: "",
    Digit8: "",
    Digit9: "",
    Digit0: "",
    Minus: "",
    Equal: "",

    // Letter row 1
    KeyQ: "",
    KeyW: "",
    KeyE: "",
    KeyR: "",
    KeyT: "",
    KeyY: "",
    KeyU: "",
    KeyI: "",
    KeyO: "",
    KeyP: "",
    BracketLeft: "",
    BracketRight: "",

    // Letter row 2
    KeyA: "",
    KeyS: "",
    KeyD: "",
    KeyF: "",
    KeyG: "",
    KeyH: "",
    KeyJ: "",
    KeyK: "",
    KeyL: "",
    Semicolon: "",
    Quote: "",
    Backslash: "",

    // Letter row 3
    KeyZ: "",
    KeyX: "",
    KeyC: "",
    KeyV: "",
    KeyB: "",
    KeyN: "",
    KeyM: "",
    Comma: "",
    Period: "",
    Slash: "",
}

// == ISO ==
export const SC_ISO = {
    IntlBackslash:  {
        LEGEND: "§",
        DESCRIPTION: "IntlBackslash;section",
        SHOWDESCRIPTION: false,
        MAC: 0x0A,
    },
};

// == JIS == 
export const SC_JIS = {
    IntlBackslash:  {
        LEGEND: "\\",
        DESCRIPTION: "backslash",
        SHOWDESCRIPTION: false,
        MAC: 0x0A,
    },
    IntlYen: {
        LEGEND: "¥",
        DESCRIPTION: "yen",
        SHOWDESCRIPTION: false,
        MAC: 0x5D,
    },
    IntlRo: {
        LEGEND: "_",
        DESCRIPTION: "underscore",
        SHOWDESCRIPTION: false,
        MAC: 0x5E,
    },
    NumpadComma: {
        LEGEND: ",",
        DESCRIPTION: "comma",
        SHOWDESCRIPTION: false,
        MAC: 0x5F,
    },
    Lang2: {
        LEGEND: "英", // JIS (Eisu)
        DESCRIPTION: "eisu;lang2",
        SHOWDESCRIPTION: false,
        MAC: 0x66,
    },
    Lang1: {
        LEGEND: "あ", // JIS (Kana)
        DESCRIPTION: "kana;lang1",
        SHOWDESCRIPTION: false,
        MAC: 0x68,
    },
};

// ============================================================================
// Language overlays (symbol remaps)
// ============================================================================
// == SWEDISH ==
export const VK_SE = {
    Backquote: {
        LEGEND: "<", // ANSI: ` 
        DESCRIPTION: "less than",
        SHOWDESCRIPTION: false,
    },
    Minus: {
        LEGEND: "+", // ANSI: -
        DESCRIPTION: "plus",
        SHOWDESCRIPTION: false,
    },
    Equal: {
        LEGEND: "´", // ANSI: =
        DESCRIPTION: "acute accent",
        SHOWDESCRIPTION: false,
    },
    BracketLeft: {
        LEGEND: "å", // ANSI: [
        DESCRIPTION: "a ring",
        SHOWDESCRIPTION: false,
    },     
    BracketRight: {
        LEGEND: "¨", // ANSI: ]
        DESCRIPTION: "diaeresis",
        SHOWDESCRIPTION: false,
    },
    Semicolon: {
        LEGEND: "ö", // ANSI: ;
        DESCRIPTION: "o umlaut",
        SHOWDESCRIPTION: false,
    },
    Quote: {
        LEGEND: "ä", // ANSI: '
        DESCRIPTION: "a umlaut",
        SHOWDESCRIPTION: false,
    },
    Backslash: {
        LEGEND: "'", // ANSI: \
        DESCRIPTION: "apostrophe",
        SHOWDESCRIPTION: false,
    },
    IntlBackslash: {
        LEGEND: "§", // (ISO only, no ANSI equivalent)
        DESCRIPTION: "IntlBackslash;section",
        SHOWDESCRIPTION: false,
    },    
    Slash: {
        LEGEND: "-", // ANSI: /
        DESCRIPTION: "minus",
        SHOWDESCRIPTION: false,
    },
};

// == KOREAN ==
export const VK_KR = {
    KeyQ: {
        LEGEND: "ㅂ",
        DESCRIPTION: "bieup",
        SHOWDESCRIPTION: false,
    },
    KeyW: {
        LEGEND: "ㅈ",
        DESCRIPTION: "jieut",
        SHOWDESCRIPTION: false,
    },
    KeyE: {
        LEGEND: "ㄷ",
        DESCRIPTION: "digeut",
        SHOWDESCRIPTION: false,
    },
    KeyR: {
        LEGEND: "ㄱ",
        DESCRIPTION: "giyeok",
        SHOWDESCRIPTION: false,
    },
    KeyT: {
        LEGEND: "ㅅ",
        DESCRIPTION: "siot",
        SHOWDESCRIPTION: false,
    },
    KeyY: {
        LEGEND: "ㅛ",
        DESCRIPTION: "yo",
        SHOWDESCRIPTION: false,
    },
    KeyU: {
        LEGEND: "ㅕ",
        DESCRIPTION: "yeo",
        SHOWDESCRIPTION: false,
    },
    KeyI: {
        LEGEND: "ㅑ",
        DESCRIPTION: "ya",
        SHOWDESCRIPTION: false,
    },
    KeyO: {
        LEGEND: "ㅐ",
        DESCRIPTION: "ae",
        SHOWDESCRIPTION: false,
    },
    KeyP: {
        LEGEND: "ㅔ",
        DESCRIPTION: "e",
        SHOWDESCRIPTION: false,
    },
    KeyA: {
        LEGEND: "ㅁ",
        DESCRIPTION: "mieum",
        SHOWDESCRIPTION: false,
    },
    KeyS: {
        LEGEND: "ㄴ",
        DESCRIPTION: "nieun",
        SHOWDESCRIPTION: false,
    },
    KeyD: {
        LEGEND: "ㅇ",
        DESCRIPTION: "ieung",
        SHOWDESCRIPTION: false,
    },
    KeyF: {
        LEGEND: "ㄹ",
        DESCRIPTION: "rieul",
        SHOWDESCRIPTION: false,
    },
    KeyG: {
        LEGEND: "ㅎ",
        DESCRIPTION: "hieut",
        SHOWDESCRIPTION: false,
    },
    KeyH: {
        LEGEND: "ㅗ",
        DESCRIPTION: "o",
        SHOWDESCRIPTION: false,
    },
    KeyJ: {
        LEGEND: "ㅓ",
        DESCRIPTION: "eo",
        SHOWDESCRIPTION: false,
    },
    KeyK: {
        LEGEND: "ㅏ",
        DESCRIPTION: "a",
        SHOWDESCRIPTION: false,
    },
    KeyL: {
        LEGEND: "ㅣ",
        DESCRIPTION: "i",
        SHOWDESCRIPTION: false,
    },

    KeyZ: {
        LEGEND: "ㅋ",
        DESCRIPTION: "kieuk",
        SHOWDESCRIPTION: false,
    },
    KeyX: {
        LEGEND: "ㅌ",
        DESCRIPTION: "tieut",
        SHOWDESCRIPTION: false,
    },
    KeyC: {
        LEGEND: "ㅊ",
        DESCRIPTION: "chieut",
        SHOWDESCRIPTION: false,
    },
    KeyV: {
        LEGEND: "ㅍ",
        DESCRIPTION: "pieup",
        SHOWDESCRIPTION: false,
    },
    KeyB: {
        LEGEND: "ㅠ",
        DESCRIPTION: "yu",
        SHOWDESCRIPTION: false,
    },
    KeyN: {
        LEGEND: "ㅜ",
        DESCRIPTION: "u",
        SHOWDESCRIPTION: false,
    },
    KeyM: {
        LEGEND: "ㅡ",
        DESCRIPTION: "eu",
        SHOWDESCRIPTION: false,
    },
};

// ============================================================================
// Layout resolver (final map selection logic)
// ============================================================================
export function getActiveLayout(lang, layout, os) {
    let ctrlKeyLegend = "Ctrl";
    let altKeyLegend = "Alt"; // must incorporate AltGr also later
    let metaKeyLegend = "";

    if (os === "MACOS") {
        ctrlKeyLegend = "⌃";
        altKeyLegend = "⌥";
        metaKeyLegend = "⌘";
    }
    else if (os === "LINUX") {
        metaKeyLegend = "❖"
    }
    else if (os === "WINDOWS") {
        metaKeyLegend = "⊞"
    }
    //
    // os is unused for now → always use MAC codes
    //
    const result = {};

    // 1. Start with ALL physical keys from VK_BASE_MAP
    for (const webCode in VK_BASE_MAP) {
        const base = VK_BASE_MAP[webCode];
        result[webCode] = {
            LEGEND: base.LEGEND || "",
            DESCRIPTION: base.DESCRIPTION || "",
            SHOWDESCRIPTION: base.SHOWDESCRIPTION === true,
            codeOS: base.MAC,
        };
    }

    // 2. Apply ISO / JIS physical overrides (optional keys)
    let physicalExtras = {};
    if (layout === "ISO") physicalExtras = SC_ISO;
    if (layout === "JIS") physicalExtras = SC_JIS;

    for (const webCode in physicalExtras) {
        const extra = physicalExtras[webCode];
        result[webCode] = {
            LEGEND: extra.LEGEND ?? result[webCode]?.LEGEND ?? "",
            DESCRIPTION: extra.DESCRIPTION ?? result[webCode]?.DESCRIPTION ?? "",
            SHOWDESCRIPTION: 
                (extra.SHOWDESCRIPTION !== undefined)
                ? extra.SHOWDESCRIPTION
                : result[webCode]?.SHOWDESCRIPTION,
            codeOS: extra.MAC ?? result[webCode]?.codeOS ?? null
        };
    }

    // 3. Apply language legend overlays (SE, KR)
    let langMap = {};
    if (lang === "SE") langMap = VK_SE;
    if (lang === "KR") langMap = VK_KR;

    for (const webCode in langMap) {
        const overlay = langMap[webCode];
        const newLegend = typeof overlay === "string"
            ? overlay
            : overlay?.LEGEND;

        if (!newLegend) continue;

        // Overwrite legends and descriptions if lang specifics colide with base
        if (result[webCode]) {
            if (overlay.LEGEND) result[webCode].LEGEND = overlay.LEGEND;
            if (overlay.DESCRIPTION) result[webCode].DESCRIPTION = overlay.DESCRIPTION;
            if (overlay.SHOWDESCRIPTION) result[webCode].SHOWDESCRIPTION = overlay.SHOWDESCRIPTION;
        }
    }
    return result;
}