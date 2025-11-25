import type {
    WebKeyCode,
    BaseKeyEntry,
    ScanCodeANSI,
    ScanCodeISO,
    ScanCodeJIS,
    OverlayMap,
    ActiveLayout,
    Language,
    LayoutKind,
    OSKind,
} from "../types";

export const VK_BASE_MAP: Record<WebKeyCode, BaseKeyEntry> = {
    None: { LEGEND: "🚫", DESCRIPTION: "none;null;nothing;skip", SHOWDESCRIPTION: true, MAC: 100000 },
    KeyA: { LEGEND: "a", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x00 },
    KeyS: { LEGEND: "s", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x01 },
    KeyD: { LEGEND: "d", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x02 },
    KeyF: { LEGEND: "f", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x03 },
    KeyH: { LEGEND: "h", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x04 },
    KeyG: { LEGEND: "g", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x05 },
    KeyZ: { LEGEND: "z", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x06 },
    KeyX: { LEGEND: "x", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x07 },
    KeyC: { LEGEND: "c", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x08 },
    KeyV: { LEGEND: "v", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x09 },
    KeyB: { LEGEND: "b", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x0B },
    KeyQ: { LEGEND: "q", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x0C },
    KeyW: { LEGEND: "w", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x0D },
    KeyE: { LEGEND: "e", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x0E },
    KeyR: { LEGEND: "r", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x0F },
    KeyY: { LEGEND: "y", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x10 },
    KeyT: { LEGEND: "t", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x11 },
    Digit1: { LEGEND: "1", DESCRIPTION: "one", SHOWDESCRIPTION: false, MAC: 0x12 },
    Digit2: { LEGEND: "2", DESCRIPTION: "two", SHOWDESCRIPTION: false, MAC: 0x13 },
    Digit3: { LEGEND: "3", DESCRIPTION: "three", SHOWDESCRIPTION: false, MAC: 0x14 },
    Digit4: { LEGEND: "4", DESCRIPTION: "four", SHOWDESCRIPTION: false, MAC: 0x15 },
    Digit6: { LEGEND: "6", DESCRIPTION: "six", SHOWDESCRIPTION: false, MAC: 0x16 },
    Digit5: { LEGEND: "5", DESCRIPTION: "five", SHOWDESCRIPTION: false, MAC: 0x17 },
    Equal: { LEGEND: "=", DESCRIPTION: "equal", SHOWDESCRIPTION: false, MAC: 0x18 },
    Digit9: { LEGEND: "9", DESCRIPTION: "nine", SHOWDESCRIPTION: false, MAC: 0x19 },
    Digit7: { LEGEND: "7", DESCRIPTION: "seven", SHOWDESCRIPTION: false, MAC: 0x1A },
    Minus: { LEGEND: "-", DESCRIPTION: "minus", SHOWDESCRIPTION: false, MAC: 0x1B },
    Digit8: { LEGEND: "8", DESCRIPTION: "eight", SHOWDESCRIPTION: false, MAC: 0x1C },
    Digit0: { LEGEND: "0", DESCRIPTION: "zero", SHOWDESCRIPTION: false, MAC: 0x1D },
    BracketRight: { LEGEND: "]", DESCRIPTION: "R bracket;right bracket", SHOWDESCRIPTION: false, MAC: 0x1E },
    KeyO: { LEGEND: "o", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x1F },
    KeyU: { LEGEND: "u", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x20 },
    BracketLeft: { LEGEND: "[", DESCRIPTION: "L bracket;left bracket", SHOWDESCRIPTION: false, MAC: 0x21 },
    KeyI: { LEGEND: "i", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x22 },
    KeyP: { LEGEND: "p", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x23 },
    Enter: { LEGEND: "⏎", DESCRIPTION: "enter", SHOWDESCRIPTION: false, MAC: 0x24 },
    KeyL: { LEGEND: "l", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x25 },
    KeyJ: { LEGEND: "j", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x26 },
    Quote: { LEGEND: "'", DESCRIPTION: "apostrophe", SHOWDESCRIPTION: false, MAC: 0x27 },
    KeyK: { LEGEND: "k", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x28 },
    Semicolon: { LEGEND: ";", DESCRIPTION: "semicolon", SHOWDESCRIPTION: false, MAC: 0x29 },
    Backslash: { LEGEND: "\\", DESCRIPTION: "backslash", SHOWDESCRIPTION: false, MAC: 0x2A },
    Comma: { LEGEND: ",", DESCRIPTION: "comma", SHOWDESCRIPTION: false, MAC: 0x2B },
    Slash: { LEGEND: "/", DESCRIPTION: "slash", SHOWDESCRIPTION: false, MAC: 0x2C },
    KeyN: { LEGEND: "n", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x2D },
    KeyM: { LEGEND: "m", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x2E },
    Period: { LEGEND: ".", DESCRIPTION: "period", SHOWDESCRIPTION: false, MAC: 0x2F },
    Tab: { LEGEND: "⇥", DESCRIPTION: "tab", SHOWDESCRIPTION: true, MAC: 0x30 },
    Space: { LEGEND: "␣", DESCRIPTION: "space", SHOWDESCRIPTION: false, MAC: 0x31 },
    Backquote: { LEGEND: "`", DESCRIPTION: "backquote", SHOWDESCRIPTION: false, MAC: 0x32 },
    Backspace: { LEGEND: "⌫", DESCRIPTION: "backspace", SHOWDESCRIPTION: false, MAC: 0x33 },
    Escape: { LEGEND: "⎋", DESCRIPTION: "esc;escape", SHOWDESCRIPTION: true, MAC: 0x35 },
    MetaRight: { LEGEND: "R ⌘", DESCRIPTION: "R cmd;right command", SHOWDESCRIPTION: false, MAC: 0x36 },
    MetaLeft: { LEGEND: "L ⌘", DESCRIPTION: "L cmd;left command", SHOWDESCRIPTION: false, MAC: 0x37 },
    ShiftLeft: { LEGEND: "L ⇧", DESCRIPTION: "L shift;left shift", SHOWDESCRIPTION: false, MAC: 0x38 },
    CapsLock: { LEGEND: "⇪", DESCRIPTION: "caps lock", SHOWDESCRIPTION: true, MAC: 0x39 },
    AltLeft: { LEGEND: "L ⌥", DESCRIPTION: "L opt;left option", SHOWDESCRIPTION: false, MAC: 0x3A },
    ControlLeft: { LEGEND: "L ⌃", DESCRIPTION: "L ctrl;left control", SHOWDESCRIPTION: false, MAC: 0x3B },
    ShiftRight: { LEGEND: "R ⇧", DESCRIPTION: "R shift;right shift", SHOWDESCRIPTION: false, MAC: 0x3C },
    AltRight: { LEGEND: "R ⌥", DESCRIPTION: "R opt;right option", SHOWDESCRIPTION: false, MAC: 0x3D },
    ControlRight: { LEGEND: "R ⌃", DESCRIPTION: "R ctrl;right control", SHOWDESCRIPTION: false, MAC: 0x3E },
    F17: { LEGEND: "F17", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x40 },
    NumpadDecimal: { LEGEND: ".", DESCRIPTION: "period", SHOWDESCRIPTION: false, MAC: 0x41 },
    NumpadMultiply: { LEGEND: "*", DESCRIPTION: "multiply", SHOWDESCRIPTION: false, MAC: 0x43 },
    NumpadAdd: { LEGEND: "+", DESCRIPTION: "add", SHOWDESCRIPTION: false, MAC: 0x45 },
    NumLock: { LEGEND: "⇭", DESCRIPTION: "num lock", SHOWDESCRIPTION: true, MAC: 0x47 },
    AudioVolumeUp: { LEGEND: "🔊", DESCRIPTION: "vol up;volume up", SHOWDESCRIPTION: false, MAC: 0x48 },
    AudioVolumeDown: { LEGEND: "🔉", DESCRIPTION: "vol down;volume down", SHOWDESCRIPTION: false, MAC: 0x49 },
    AudioVolumeMute: { LEGEND: "🔇", DESCRIPTION: "mute", SHOWDESCRIPTION: false, MAC: 0x4A },
    NumpadDivide: { LEGEND: "/", DESCRIPTION: "slash", SHOWDESCRIPTION: false, MAC: 0x4B },
    NumpadEnter: { LEGEND: "⏎", DESCRIPTION: "enter", SHOWDESCRIPTION: false, MAC: 0x4C },
    NumpadSubtract: { LEGEND: "-", DESCRIPTION: "minus", SHOWDESCRIPTION: false, MAC: 0x4E },
    F18: { LEGEND: "F18", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x4F },
    F19: { LEGEND: "F19", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x50 },
    NumpadEqual: { LEGEND: "=", DESCRIPTION: "equal", SHOWDESCRIPTION: false, MAC: 0x51 },
    Numpad0: { LEGEND: "0", DESCRIPTION: "zero", SHOWDESCRIPTION: false, MAC: 0x52 },
    Numpad1: { LEGEND: "1", DESCRIPTION: "one", SHOWDESCRIPTION: false, MAC: 0x53 },
    Numpad2: { LEGEND: "2", DESCRIPTION: "two", SHOWDESCRIPTION: false, MAC: 0x54 },
    Numpad3: { LEGEND: "3", DESCRIPTION: "three", SHOWDESCRIPTION: false, MAC: 0x55 },
    Numpad4: { LEGEND: "4", DESCRIPTION: "four", SHOWDESCRIPTION: false, MAC: 0x56 },
    Numpad5: { LEGEND: "5", DESCRIPTION: "five", SHOWDESCRIPTION: false, MAC: 0x57 },
    Numpad6: { LEGEND: "6", DESCRIPTION: "six", SHOWDESCRIPTION: false, MAC: 0x58 },
    Numpad7: { LEGEND: "7", DESCRIPTION: "seven", SHOWDESCRIPTION: false, MAC: 0x59 },
    F20: { LEGEND: "F20", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x5A },
    Numpad8: { LEGEND: "8", DESCRIPTION: "eight", SHOWDESCRIPTION: false, MAC: 0x5B },
    Numpad9: { LEGEND: "9", DESCRIPTION: "nine", SHOWDESCRIPTION: false, MAC: 0x5C },
    F5: { LEGEND: "F5", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x60 },
    F6: { LEGEND: "F6", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x61 },
    F7: { LEGEND: "F7", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x62 },
    F3: { LEGEND: "F3", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x63 },
    F8: { LEGEND: "F8", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x64 },
    F9: { LEGEND: "F9", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x65 },
    F11: { LEGEND: "F11", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x67 },
    F13: { LEGEND: "F13", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x69 },
    F16: { LEGEND: "F16", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x6A },
    F14: { LEGEND: "F14", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x6B },
    F10: { LEGEND: "F10", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x6D },
    ContextMenu: { LEGEND: "≣", DESCRIPTION: "context menu", SHOWDESCRIPTION: true, MAC: 0x6E },
    F12: { LEGEND: "F12", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x6F },
    F15: { LEGEND: "F15", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x71 },
    Insert: { LEGEND: "Ins", DESCRIPTION: "insert", SHOWDESCRIPTION: false, MAC: 0x72 },
    Home: { LEGEND: "Home", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x73 },
    PageUp: { LEGEND: "PgUp", DESCRIPTION: "page up", SHOWDESCRIPTION: false, MAC: 0x74 },
    Delete: { LEGEND: "⌦", DESCRIPTION: "del;delete", SHOWDESCRIPTION: false, MAC: 0x75 },
    F4: { LEGEND: "F4", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x76 },
    End: { LEGEND: "End", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x77 },
    F2: { LEGEND: "F2", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x78 },
    PageDown: { LEGEND: "PgDn", DESCRIPTION: "page down", SHOWDESCRIPTION: false, MAC: 0x79 },
    F1: { LEGEND: "F1", DESCRIPTION: "", SHOWDESCRIPTION: false, MAC: 0x7A },
    ArrowLeft: { LEGEND: "←", DESCRIPTION: "arrow left", SHOWDESCRIPTION: false, MAC: 0x7B },
    ArrowRight: { LEGEND: "→", DESCRIPTION: "arrow right", SHOWDESCRIPTION: false, MAC: 0x7C },
    ArrowDown: { LEGEND: "↓", DESCRIPTION: "arrow down", SHOWDESCRIPTION: false, MAC: 0x7D },
    ArrowUp: { LEGEND: "↑", DESCRIPTION: "arrow up", SHOWDESCRIPTION: false, MAC: 0x7E },
};

export const SC_ANSI: ScanCodeANSI = {
    Backquote: "", Digit1: "", Digit2: "", Digit3: "", Digit4: "", Digit5: "", Digit6: "", Digit7: "", Digit8: "", Digit9: "", Digit0: "", Minus: "", Equal: "",
    KeyQ: "", KeyW: "", KeyE: "", KeyR: "", KeyT: "", KeyY: "", KeyU: "", KeyI: "", KeyO: "", KeyP: "", BracketLeft: "", BracketRight: "",
    KeyA: "", KeyS: "", KeyD: "", KeyF: "", KeyG: "", KeyH: "", KeyJ: "", KeyK: "", KeyL: "", Semicolon: "", Quote: "", Backslash: "",
    KeyZ: "", KeyX: "", KeyC: "", KeyV: "", KeyB: "", KeyN: "", KeyM: "", Comma: "", Period: "", Slash: "",
}

export const SC_ISO: ScanCodeISO = {
    IntlBackslash: { LEGEND: "§", DESCRIPTION: "IntlBackslash;section", SHOWDESCRIPTION: false, MAC: 0x0A },
};

export const SC_JIS: ScanCodeJIS = {
    IntlBackslash: { LEGEND: "\\", DESCRIPTION: "backslash", SHOWDESCRIPTION: false, MAC: 0x0A },
    IntlYen: { LEGEND: "¥", DESCRIPTION: "yen", SHOWDESCRIPTION: false, MAC: 0x5D },
    IntlRo: { LEGEND: "_", DESCRIPTION: "underscore", SHOWDESCRIPTION: false, MAC: 0x5E },
    NumpadComma: { LEGEND: ",", DESCRIPTION: "comma", SHOWDESCRIPTION: false, MAC: 0x5F },
    Lang2: { LEGEND: "英", DESCRIPTION: "eisu;lang2", SHOWDESCRIPTION: false, MAC: 0x66 },
    Lang1: { LEGEND: "あ", DESCRIPTION: "kana;lang1", SHOWDESCRIPTION: false, MAC: 0x68 },
};

export const VK_SE: OverlayMap = {
    Backquote: { LEGEND: "<", DESCRIPTION: "less than", SHOWDESCRIPTION: false },
    Minus: { LEGEND: "+", DESCRIPTION: "plus", SHOWDESCRIPTION: false },
    Equal: { LEGEND: "´", DESCRIPTION: "acute accent", SHOWDESCRIPTION: false },
    BracketLeft: { LEGEND: "å", DESCRIPTION: "a ring", SHOWDESCRIPTION: false },
    BracketRight: { LEGEND: "¨", DESCRIPTION: "diaeresis", SHOWDESCRIPTION: false },
    Semicolon: { LEGEND: "ö", DESCRIPTION: "o umlaut", SHOWDESCRIPTION: false },
    Quote: { LEGEND: "ä", DESCRIPTION: "a umlaut", SHOWDESCRIPTION: false },
    Backslash: { LEGEND: "'", DESCRIPTION: "apostrophe", SHOWDESCRIPTION: false },
    IntlBackslash: { LEGEND: "§", DESCRIPTION: "IntlBackslash;section", SHOWDESCRIPTION: false },
    Slash: { LEGEND: "-", DESCRIPTION: "minus", SHOWDESCRIPTION: false },
};

export const VK_KR: OverlayMap = {
    KeyQ: { LEGEND: "ㅂ", DESCRIPTION: "bieup", SHOWDESCRIPTION: false },
    KeyW: { LEGEND: "ㅈ", DESCRIPTION: "jieut", SHOWDESCRIPTION: false },
    KeyE: { LEGEND: "ㄷ", DESCRIPTION: "digeut", SHOWDESCRIPTION: false },
    KeyR: { LEGEND: "ㄱ", DESCRIPTION: "giyeok", SHOWDESCRIPTION: false },
    KeyT: { LEGEND: "ㅅ", DESCRIPTION: "siot", SHOWDESCRIPTION: false },
    KeyY: { LEGEND: "ㅛ", DESCRIPTION: "yo", SHOWDESCRIPTION: false },
    KeyU: { LEGEND: "ㅕ", DESCRIPTION: "yeo", SHOWDESCRIPTION: false },
    KeyI: { LEGEND: "ㅑ", DESCRIPTION: "ya", SHOWDESCRIPTION: false },
    KeyO: { LEGEND: "ㅐ", DESCRIPTION: "ae", SHOWDESCRIPTION: false },
    KeyP: { LEGEND: "ㅔ", DESCRIPTION: "e", SHOWDESCRIPTION: false },
    KeyA: { LEGEND: "ㅁ", DESCRIPTION: "mieum", SHOWDESCRIPTION: false },
    KeyS: { LEGEND: "ㄴ", DESCRIPTION: "nieun", SHOWDESCRIPTION: false },
    KeyD: { LEGEND: "ㅇ", DESCRIPTION: "ieung", SHOWDESCRIPTION: false },
    KeyF: { LEGEND: "ㄹ", DESCRIPTION: "rieul", SHOWDESCRIPTION: false },
    KeyG: { LEGEND: "ㅎ", DESCRIPTION: "hieut", SHOWDESCRIPTION: false },
    KeyH: { LEGEND: "ㅗ", DESCRIPTION: "o", SHOWDESCRIPTION: false },
    KeyJ: { LEGEND: "ㅓ", DESCRIPTION: "eo", SHOWDESCRIPTION: false },
    KeyK: { LEGEND: "ㅏ", DESCRIPTION: "a", SHOWDESCRIPTION: false },
    KeyL: { LEGEND: "ㅣ", DESCRIPTION: "i", SHOWDESCRIPTION: false },
    KeyZ: { LEGEND: "ㅋ", DESCRIPTION: "kieuk", SHOWDESCRIPTION: false },
    KeyX: { LEGEND: "ㅌ", DESCRIPTION: "tieut", SHOWDESCRIPTION: false },
    KeyC: { LEGEND: "ㅊ", DESCRIPTION: "chieut", SHOWDESCRIPTION: false },
    KeyV: { LEGEND: "ㅍ", DESCRIPTION: "pieup", SHOWDESCRIPTION: false },
    KeyB: { LEGEND: "ㅠ", DESCRIPTION: "yu", SHOWDESCRIPTION: false },
    KeyN: { LEGEND: "ㅜ", DESCRIPTION: "u", SHOWDESCRIPTION: false },
    KeyM: { LEGEND: "ㅡ", DESCRIPTION: "eu", SHOWDESCRIPTION: false },
};

export function getActiveLayout(
    lang: Language,
    layout: LayoutKind,
    os: OSKind
): ActiveLayout {
    const result: ActiveLayout = {};

    for (const webCode in VK_BASE_MAP) {
        const base = VK_BASE_MAP[webCode as WebKeyCode];
        result[webCode] = {
            LEGEND: base.LEGEND,
            DESCRIPTION: base.DESCRIPTION,
            SHOWDESCRIPTION: base.SHOWDESCRIPTION,
            codeOS: base.MAC,
        };
    }

    let physicalExtras: ScanCodeISO | ScanCodeJIS | ScanCodeANSI = {};
    if (layout === "ISO") physicalExtras = SC_ISO;
    if (layout === "JIS") physicalExtras = SC_JIS;
    if (layout === "ANSI") physicalExtras = SC_ANSI;

    for (const webCode in physicalExtras) {
        const extra = physicalExtras[webCode as WebKeyCode];
        if (typeof extra === "string") continue;

        result[webCode] = {
            LEGEND: extra.LEGEND ?? result[webCode]?.LEGEND ?? "",
            DESCRIPTION: extra.DESCRIPTION ?? result[webCode]?.DESCRIPTION ?? "",
            SHOWDESCRIPTION: extra.SHOWDESCRIPTION !== undefined ? extra.SHOWDESCRIPTION : result[webCode]?.SHOWDESCRIPTION ?? false,
            codeOS: extra.MAC ?? result[webCode]?.codeOS ?? 999999,
        };
    }

    let langMap: OverlayMap = {};
    if (lang === "SE") langMap = VK_SE;
    if (lang === "KR") langMap = VK_KR;

    for (const webCode in langMap) {
        const overlay = langMap[webCode as WebKeyCode];
        if (!overlay) continue;
        const target = result[webCode];
        if (!target) continue;

        if (typeof overlay === "object") {
            if (overlay.LEGEND) target.LEGEND = overlay.LEGEND;
            if (overlay.DESCRIPTION) target.DESCRIPTION = overlay.DESCRIPTION;
            if (overlay.SHOWDESCRIPTION !== undefined) target.SHOWDESCRIPTION = overlay.SHOWDESCRIPTION;
        }
    }

    return result;
}