/* ========================================================================
   INDEPENDENT OF KEYBOARD LAYOUT  (same key positions on all layouts)
   Based on:
   - kVK_Return, kVK_Tab, kVK_Delete, kVK_Escape, kVK_Command, ...
   - All arrow keys
   - Function keys F1–F12
   - Modifiers (Shift, Control, Option, Command)
   ======================================================================== */

export const SC_INDEPENDENT_US = {
    Escape: "esc",
    Backspace: "⌫",
    Tab: "tab",
    CapsLock: "⇪",

    // Modifiers (left/right physically exist regardless of layout)
    ShiftLeft: "L ⇧",
    ShiftRight: "R ⇧",
    ControlLeft: "controlL",
    ControlRight: "controlR",
    AltLeft: "altL",
    AltRight: "altR",
    MetaLeft: "metaL",
    MetaRight: "metaR",

    Enter: "↵",
    Space: "␣",

    ArrowLeft: "←",
    ArrowDown: "↓",
    ArrowUp: "↑",
    ArrowRight: "→",

    // Function keys (standard Mac layout)
    F1:  "f1",
    F2:  "f2",
    F3:  "f3",
    F4:  "f4",
    F5:  "f5",
    F6:  "f6",
    F7:  "f7",
    F8:  "f8",
    F9:  "f9",
    F10: "f10",
    F11: "f11",
    F12: "f12",
};

/* ========================================================================
   ANSI US CHARACTER KEYS (layout-dependent)
   Based on:
   - kVK_ANSI_A … kVK_ANSI_Grave
   - All letters A–Z
   - Number row 1–0, Minus, Equal
   - Brackets, Semicolon, Quote, Backslash, Slash
   - Comma, Period
   - Grave/backquote `
   These keys change depending on the layout (US vs SE vs KR).
   ======================================================================== */

export const SC_ANSI_US = {
    // Number row
    Backquote: "`",  // kVK_ANSI_Grave
    Digit1: "1",
    Digit2: "2",
    Digit3: "3",
    Digit4: "4",
    Digit5: "5",
    Digit6: "6",
    Digit7: "7",
    Digit8: "8",
    Digit9: "9",
    Digit0: "0",
    Minus: "-",
    Equal: "=",

    // Letter row 1
    KeyQ: "q",
    KeyW: "w",
    KeyE: "e",
    KeyR: "r",
    KeyT: "t",
    KeyY: "y",
    KeyU: "u",
    KeyI: "i",
    KeyO: "o",
    KeyP: "p",
    BracketLeft: "[",
    BracketRight: "]",

    // Letter row 2
    KeyA: "a",
    KeyS: "s",
    KeyD: "d",
    KeyF: "f",
    KeyG: "g",
    KeyH: "h",
    KeyJ: "j",
    KeyK: "k",
    KeyL: "l",
    Semicolon: ";",
    Quote: "'",

    // ANSI backslash (ISO has IntlBackslash)
    Backslash: "\\",

    // Letter row 3
    KeyZ: "z",
    KeyX: "x",
    KeyC: "c",
    KeyV: "v",
    KeyB: "b",
    KeyN: "n",
    KeyM: "m",
    Comma: ",",
    Period: ".",
    Slash: "/",

    // ANSI has no ISO extra key
    IntlBackslash: null,
};

export const SC_ISO_US = {
    IntlBackslash: "§",
}

export const SC_ISO_SE = {
    Backquote: "<",        // ANSI: ` 
    Minus: "+",            // ANSI: -
    Equal: "´",            // ANSI: =

    BracketLeft: "å",      // ANSI: [
    BracketRight: "¨",     // ANSI: ]

    Semicolon: "ö",        // ANSI: ;
    Quote: "ä",            // ANSI: '
    Backslash: "'",        // ANSI: \

    IntlBackslash: "§",    // (ISO only, no ANSI equivalent)

    Slash: "-",            // ANSI: /
};

export const SC_ISO_KR = {
  KeyQ: "ㅂ",
  KeyW: "ㅈ",
  KeyE: "ㄷ",
  KeyR: "ㄱ",
  KeyT: "ㅅ",
  KeyY: "ㅛ",
  KeyU: "ㅕ",
  KeyI: "ㅑ",
  KeyO: "ㅐ",
  KeyP: "ㅔ",

  KeyA: "ㅁ",
  KeyS: "ㄴ",
  KeyD: "ㅇ",
  KeyF: "ㄹ",
  KeyG: "ㅎ",
  KeyH: "ㅗ",
  KeyJ: "ㅓ",
  KeyK: "ㅏ",
  KeyL: "ㅣ",

  KeyZ: "ㅋ",
  KeyX: "ㅌ",
  KeyC: "ㅊ",
  KeyV: "ㅍ",
  KeyB: "ㅠ",
  KeyN: "ㅜ",
  KeyM: "ㅡ",
};

/* ============================================================
   MAC VIRTUAL KEY → WEB KEY CODE (KeyboardEvent.code)
   Independent keys = all keys EXCEPT JIS-exclusive ones
   JIS keys go into VK_MAC_JIS.
   ============================================================ */

export const VK_MAC_INDEPENDENT = {
    // 0x00–0x0F
    KeyA: { MAC: 0x00 },
    KeyS: { MAC: 0x01 },
    KeyD: { MAC: 0x02 },
    KeyF: { MAC: 0x03 },
    KeyH: { MAC: 0x04 },
    KeyG: { MAC: 0x05 },
    KeyZ: { MAC: 0x06 },
    KeyX: { MAC: 0x07 },
    KeyC: { MAC: 0x08 },
    KeyV: { MAC: 0x09 },
    // 0x0A is JIS Section → VK_MAC_JIS
    KeyB: { MAC: 0x0B },
    KeyQ: { MAC: 0x0C },
    KeyW: { MAC: 0x0D },
    KeyE: { MAC: 0x0E },
    KeyR: { MAC: 0x0F },

    // 0x10–0x1F
    KeyY: { MAC: 0x10 },
    KeyT: { MAC: 0x11 },
    Digit1: { MAC: 0x12 },
    Digit2: { MAC: 0x13 },
    Digit3: { MAC: 0x14 },
    Digit4: { MAC: 0x15 },
    Digit6: { MAC: 0x16 },
    Digit5: { MAC: 0x17 },
    Equal: { MAC: 0x18 },
    Digit9: { MAC: 0x19 },
    Digit7: { MAC: 0x1A },
    Minus: { MAC: 0x1B },
    Digit8: { MAC: 0x1C },
    Digit0: { MAC: 0x1D },
    BracketRight: { MAC: 0x1E },
    KeyO: { MAC: 0x1F },

    // 0x20–0x2F
    KeyU: { MAC: 0x20 },
    BracketLeft: { MAC: 0x21 },
    KeyI: { MAC: 0x22 },
    KeyP: { MAC: 0x23 },
    Enter: { MAC: 0x24 },             // Return
    KeyL: { MAC: 0x25 },
    KeyJ: { MAC: 0x26 },
    Quote: { MAC: 0x27 },
    KeyK: { MAC: 0x28 },
    Semicolon: { MAC: 0x29 },
    Backslash: { MAC: 0x2A },
    Comma: { MAC: 0x2B },
    Slash: { MAC: 0x2C },
    KeyN: { MAC: 0x2D },
    KeyM: { MAC: 0x2E },
    Period: { MAC: 0x2F },

    // 0x30–0x3F
    Tab: { MAC: 0x30 },
    Space: { MAC: 0x31 },
    Backquote: { MAC: 0x32 },
    Backspace: { MAC: 0x33 },         // Delete (backspace)
    // 0x34 numeric keypad enter — NO Web code:
    // NumpadEnter exists but differs on powerbook
    // NumpadEnter: { MAC: 0x34 },
    Escape: { MAC: 0x35 },
    MetaRight: { MAC: 0x36 },         // Right Command
    MetaLeft: { MAC: 0x37 },          // Command
    ShiftLeft: { MAC: 0x38 },
    CapsLock: { MAC: 0x39 },
    AltLeft: { MAC: 0x3A },           // Option
    ControlLeft: { MAC: 0x3B },
    ShiftRight: { MAC: 0x3C },
    AltRight: { MAC: 0x3D },
    ControlRight: { MAC: 0x3E },
    // 0x3F Fn key does not generate events
    // Fn: { MAC: 0x3F }, // no Web code fired

    // 0x40–0x4F
    F17: { MAC: 0x40 },
    NumpadDecimal: { MAC: 0x41 },
    // 0x42 unused
    NumpadMultiply: { MAC: 0x43 },
    // 0x44 unused
    NumpadAdd: { MAC: 0x45 },
    // 0x46 unused
    NumLock: { MAC: 0x47 },           // Clear key
    AudioVolumeUp: { MAC: 0x48 },
    AudioVolumeDown: { MAC: 0x49 },
    AudioVolumeMute: { MAC: 0x4A },
    NumpadDivide: { MAC: 0x4B },
    NumpadEnter: { MAC: 0x4C },
    // 0x4D unused
    NumpadSubtract: { MAC: 0x4E },
    F18: { MAC: 0x4F },

    // 0x50–0x5F
    F19: { MAC: 0x50 },
    NumpadEqual: { MAC: 0x51 },
    Numpad0: { MAC: 0x52 },
    Numpad1: { MAC: 0x53 },
    Numpad2: { MAC: 0x54 },
    Numpad3: { MAC: 0x55 },
    Numpad4: { MAC: 0x56 },
    Numpad5: { MAC: 0x57 },
    Numpad6: { MAC: 0x58 },
    Numpad7: { MAC: 0x59 },
    F20: { MAC: 0x5A },
    Numpad8: { MAC: 0x5B },
    Numpad9: { MAC: 0x5C },
    // 0x5D–0x5F are JIS → VK_MAC_JIS

    // 0x60–0x6F
    F5: { MAC: 0x60 },
    F6: { MAC: 0x61 },
    F7: { MAC: 0x62 },
    F3: { MAC: 0x63 },
    F8: { MAC: 0x64 },
    F9: { MAC: 0x65 },
    // 0x66 is JIS (Eisu)
    F11: { MAC: 0x67 },
    // 0x68 is JIS (Kana)
    F13: { MAC: 0x69 },
    F16: { MAC: 0x6A },
    F14: { MAC: 0x6B },
    // 0x6C unused
    F10: { MAC: 0x6D },
    ContextMenu: { MAC: 0x6E },
    F12: { MAC: 0x6F },

    // 0x70–0x7F
    // 0x70 unused
    F15: { MAC: 0x71 },
    // Chromium: Insert, Firefox: Help → use Insert
    Insert: { MAC: 0x72 },
    Home: { MAC: 0x73 },
    PageUp: { MAC: 0x74 },
    Delete: { MAC: 0x75 },           // ForwardDelete
    F4: { MAC: 0x76 },
    End: { MAC: 0x77 },
    F2: { MAC: 0x78 },
    PageDown: { MAC: 0x79 },
    F1: { MAC: 0x7A },
    ArrowLeft: { MAC: 0x7B },
    ArrowRight: { MAC: 0x7C },
    ArrowDown: { MAC: 0x7D },
    ArrowUp: { MAC: 0x7E },
    // 0x7F Power key → no Web code
    // Power: { MAC: 0x7F }, // no Web code

    // 0xA0
    // Mission Control key — NO standard Web code:
    // MissionControl: { MAC: 0xA0 }, // no Web code
};


/* ============================================================
   JIS-ONLY KEYS
   ============================================================ */

export const VK_MAC_JIS = {
    IntlBackslash: { MAC: 0x0A },  // Section key
    IntlYen: { MAC: 0x5D },        // Yen
    IntlRo: { MAC: 0x5E },         // Underscore / ロ key
    NumpadComma: { MAC: 0x5F },    // Keypad comma
    Lang2: { MAC: 0x66 },          // Eisu
    Lang1: { MAC: 0x68 },          // Kana
};


/* ============================================================
   REVERSE MAP:
   Given a VK → return the Web code string
   ============================================================ */

export const VK_MAC_TO_CODE_VALUE = (() => {
    const reverse = {};

    const add = (obj) => {
        for (const code in obj) {
            const vk = obj[code].MAC;
            reverse[vk] = code;
        }
    };

    add(VK_MAC_INDEPENDENT);
    add(VK_MAC_JIS);

    return reverse;
})();

/* ========================================================================
   Layout & scan-code mappings
   ======================================================================== */

/**
 * Returns a flat map from KeyboardEvent.code → visual label
 * based on the current language.
 */
export function getActiveLayout(currentLanguage, currentLayout, currentOS) {
    // 1. Choose base layout family (ANSI or ISO)
    let baseLayout = { ...SC_INDEPENDENT_US, ...SC_ANSI_US };

    if (currentLayout === "ISO") {
        // ISO default = ANSI + optional ISO_US override
        baseLayout = { ...baseLayout, ...SC_ISO_US, };
    }

    // 2. Apply language-specific overrides
    if (currentLanguage === "SE") {
        // Swedish ISO only—Swedish makes sense only on ISO physical layout
        return { ...baseLayout, ...SC_ISO_SE, };
    }
    else if (currentLanguage === "KR") {
        // Korean replaces only A–Z keys
        return { ...baseLayout, ...SC_ISO_KR, };
    }

    // 3. Default: US layout (ANSI or ISO depending on selection)
    return baseLayout;
}