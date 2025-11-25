
import { KeyGeom } from "../types";

export const KB_ISO: KeyGeom[][] = [
  // Row 1 (Total 59u)
  [
    { code: "Escape", w: 5 },
    { code: "F1", w: 4 },
    { code: "F2", w: 4 },
    { code: "F3", w: 4 },
    { code: "F4", w: 4 },
    { code: "F5", w: 4 },
    { code: "F6", w: 4 },
    { code: "F7", w: 4 },
    { code: "F8", w: 4 },
    { code: "F9", w: 4 },
    { code: "F10", w: 4 },
    { code: "F11", w: 4 },
    { code: "F12", w: 4 },
    { code: "", w: 6 }, // Power/Spacer
  ],
  // Row 2 (Total 59u)
  [
    { code: "IntlBackslash", w: 4 },
    { code: "Digit1", w: 4 },
    { code: "Digit2", w: 4 },
    { code: "Digit3", w: 4 },
    { code: "Digit4", w: 4 },
    { code: "Digit5", w: 4 },
    { code: "Digit6", w: 4 },
    { code: "Digit7", w: 4 },
    { code: "Digit8", w: 4 },
    { code: "Digit9", w: 4 },
    { code: "Digit0", w: 4 },
    { code: "Minus", w: 4 },
    { code: "Equal", w: 4 },
    { code: "Backspace", w: 7 },
  ],
  // Row 3 (Total 59u)
  [
    { code: "Tab", w: 6 },
    { code: "KeyQ", w: 4 },
    { code: "KeyW", w: 4 },
    { code: "KeyE", w: 4 },
    { code: "KeyR", w: 4 },
    { code: "KeyT", w: 4 },
    { code: "KeyY", w: 4 },
    { code: "KeyU", w: 4 },
    { code: "KeyI", w: 4 },
    { code: "KeyO", w: 4 },
    { code: "KeyP", w: 4 },
    { code: "BracketLeft", w: 4 },
    { code: "BracketRight", w: 4 },
    { code: "Enter", w: 5 }, // ISO Enter Top
  ],
  // Row 4 (Total 59u)
  [
    { code: "CapsLock", w: 7 },
    { code: "KeyA", w: 4 },
    { code: "KeyS", w: 4 },
    { code: "KeyD", w: 4 },
    { code: "KeyF", w: 4 },
    { code: "KeyG", w: 4 },
    { code: "KeyH", w: 4 },
    { code: "KeyJ", w: 4 },
    { code: "KeyK", w: 4 },
    { code: "KeyL", w: 4 },
    { code: "Semicolon", w: 4 },
    { code: "Quote", w: 4 },
    { code: "Backslash", w: 4 }, // ISO Enter Bottom / Extra key
  ],
  // Row 5 (Total 59u)
  [
    { code: "ShiftLeft", w: 5 },
    { code: "Backquote", w: 4 }, // ISO key next to shift
    { code: "KeyZ", w: 4 },
    { code: "KeyX", w: 4 },
    { code: "KeyC", w: 4 },
    { code: "KeyV", w: 4 },
    { code: "KeyB", w: 4 },
    { code: "KeyN", w: 4 },
    { code: "KeyM", w: 4 },
    { code: "Comma", w: 4 },
    { code: "Period", w: 4 },
    { code: "Slash", w: 4 },
    { code: "ShiftRight", w: 10 },
  ],
  // Row 6 (Total 59u)
  [
    { code: "Fn", w: 4 },
    { code: "ControlLeft", w: 4 },
    { code: "AltLeft", w: 4 },
    { code: "MetaLeft", w: 5 },
    { code: "Space", w: 20 },
    { code: "MetaRight", w: 5 },
    { code: "AltRight", w: 4 },
    { code: "_spacer1", w: 5, type: "spacer" },
    { code: "ArrowUp", w: 4 },
  ],
  // Row 7 (Total 59u)
  [
    { code: "_spacer2", w: 47, type: "spacer" },
    { code: "ArrowLeft", w: 4 },
    { code: "ArrowDown", w: 4 },
    { code: "ArrowRight", w: 4 },
  ],
];

export const KB_ANSI: KeyGeom[][] = [
  // Row 1 (Total 59u)
  [
    { code: "Escape", w: 5 },
    { code: "F1", w: 4 },
    { code: "F2", w: 4 },
    { code: "F3", w: 4 },
    { code: "F4", w: 4 },
    { code: "F5", w: 4 },
    { code: "F6", w: 4 },
    { code: "F7", w: 4 },
    { code: "F8", w: 4 },
    { code: "F9", w: 4 },
    { code: "F10", w: 4 },
    { code: "F11", w: 4 },
    { code: "F12", w: 4 },
    { code: "", w: 6 },
  ],
  // Row 2 (Total 59u)
  [
    { code: "Backquote", w: 4 },
    { code: "Digit1", w: 4 },
    { code: "Digit2", w: 4 },
    { code: "Digit3", w: 4 },
    { code: "Digit4", w: 4 },
    { code: "Digit5", w: 4 },
    { code: "Digit6", w: 4 },
    { code: "Digit7", w: 4 },
    { code: "Digit8", w: 4 },
    { code: "Digit9", w: 4 },
    { code: "Digit0", w: 4 },
    { code: "Minus", w: 4 },
    { code: "Equal", w: 4 },
    { code: "Backspace", w: 7 },
  ],
  // Row 3 (Total 59u)
  [
    { code: "Tab", w: 6 },
    { code: "KeyQ", w: 4 },
    { code: "KeyW", w: 4 },
    { code: "KeyE", w: 4 },
    { code: "KeyR", w: 4 },
    { code: "KeyT", w: 4 },
    { code: "KeyY", w: 4 },
    { code: "KeyU", w: 4 },
    { code: "KeyI", w: 4 },
    { code: "KeyO", w: 4 },
    { code: "KeyP", w: 4 },
    { code: "BracketLeft", w: 4 },
    { code: "BracketRight", w: 4 },
    { code: "Backslash", w: 5 },
  ],
  // Row 4 (Total 59u)
  [
    { code: "CapsLock", w: 7 },
    { code: "KeyA", w: 4 },
    { code: "KeyS", w: 4 },
    { code: "KeyD", w: 4 },
    { code: "KeyF", w: 4 },
    { code: "KeyG", w: 4 },
    { code: "KeyH", w: 4 },
    { code: "KeyJ", w: 4 },
    { code: "KeyK", w: 4 },
    { code: "KeyL", w: 4 },
    { code: "Semicolon", w: 4 },
    { code: "Quote", w: 4 },
    { code: "Enter", w: 8 },
  ],
  // Row 5 (Total 59u)
  [
    { code: "ShiftLeft", w: 9 },
    { code: "KeyZ", w: 4 },
    { code: "KeyX", w: 4 },
    { code: "KeyC", w: 4 },
    { code: "KeyV", w: 4 },
    { code: "KeyB", w: 4 },
    { code: "KeyN", w: 4 },
    { code: "KeyM", w: 4 },
    { code: "Comma", w: 4 },
    { code: "Period", w: 4 },
    { code: "Slash", w: 4 },
    { code: "ShiftRight", w: 10 },
  ],
  // Row 6 (Total 59u)
  [
    { code: "Fn", w: 4 },
    { code: "ControlLeft", w: 4 },
    { code: "AltLeft", w: 4 },
    { code: "MetaLeft", w: 5 },
    { code: "Space", w: 20 },
    { code: "MetaRight", w: 5 },
    { code: "AltRight", w: 4 },
    { code: "_spacer1", w: 5, type: "spacer" },
    { code: "ArrowUp", w: 4 },
  ],
  // Row 7 (Total 59u)
  [
    { code: "_spacer2", w: 47, type: "spacer" },
    { code: "ArrowLeft", w: 4 },
    { code: "ArrowDown", w: 4 },
    { code: "ArrowRight", w: 4 },
  ],
];

export const KB_JIS: KeyGeom[][] = [
  // Row 1 (Total 59u)
  [
    { code: "Escape", w: 5 },
    { code: "F1", w: 4 },
    { code: "F2", w: 4 },
    { code: "F3", w: 4 },
    { code: "F4", w: 4 },
    { code: "F5", w: 4 },
    { code: "F6", w: 4 },
    { code: "F7", w: 4 },
    { code: "F8", w: 4 },
    { code: "F9", w: 4 },
    { code: "F10", w: 4 },
    { code: "F11", w: 4 },
    { code: "F12", w: 4 },
    { code: "", w: 6 },
  ],
  // Row 2 (Total 59u)
  [
    { code: "Digit1", w: 4 },
    { code: "Digit2", w: 4 },
    { code: "Digit3", w: 4 },
    { code: "Digit4", w: 4 },
    { code: "Digit5", w: 4 },
    { code: "Digit6", w: 4 },
    { code: "Digit7", w: 4 },
    { code: "Digit8", w: 4 },
    { code: "Digit9", w: 4 },
    { code: "Digit0", w: 4 },
    { code: "Minus", w: 4 },
    { code: "Equal", w: 4 },
    { code: "IntlYen", w: 4 },
    { code: "Backspace", w: 7 },
  ],
  // Row 3 (Total 59u)
  [
    { code: "Tab", w: 6 },
    { code: "KeyQ", w: 4 },
    { code: "KeyW", w: 4 },
    { code: "KeyE", w: 4 },
    { code: "KeyR", w: 4 },
    { code: "KeyT", w: 4 },
    { code: "KeyY", w: 4 },
    { code: "KeyU", w: 4 },
    { code: "KeyI", w: 4 },
    { code: "KeyO", w: 4 },
    { code: "KeyP", w: 4 },
    { code: "BracketLeft", w: 4 },
    { code: "BracketRight", w: 4 },
    { code: "Enter", w: 5 }, // JIS Vertical Enter Top
  ],
  // Row 4 (Total 59u)
  [
    { code: "ControlLeft", w: 7 }, // Replaces CapsLock position
    { code: "KeyA", w: 4 },
    { code: "KeyS", w: 4 },
    { code: "KeyD", w: 4 },
    { code: "KeyF", w: 4 },
    { code: "KeyG", w: 4 },
    { code: "KeyH", w: 4 },
    { code: "KeyJ", w: 4 },
    { code: "KeyK", w: 4 },
    { code: "KeyL", w: 4 },
    { code: "Semicolon", w: 4 },
    { code: "Quote", w: 4 },
    { code: "Backslash", w: 4 }, // JIS ] key
    // Remaining 4u is gap for Enter Bottom
  ],
  // Row 5 (Total 59u)
  [
    { code: "ShiftLeft", w: 9 }, // JIS Shift is wide
    { code: "KeyZ", w: 4 },
    { code: "KeyX", w: 4 },
    { code: "KeyC", w: 4 },
    { code: "KeyV", w: 4 },
    { code: "KeyB", w: 4 },
    { code: "KeyN", w: 4 },
    { code: "KeyM", w: 4 },
    { code: "Comma", w: 4 },
    { code: "Period", w: 4 },
    { code: "Slash", w: 4 },
    { code: "IntlRo", w: 4 }, // JIS _ key
    { code: "ShiftRight", w: 6 },
  ],
  // Row 6 (Total 59u)
  [
    { code: "CapsLock", w: 4 },
    { code: "AltLeft", w: 4 },
    { code: "MetaLeft", w: 5 },
    { code: "Lang2", w: 4 }, // Eisu
    { code: "Space", w: 16 },
    { code: "Lang1", w: 4 }, // Kana
    { code: "MetaRight", w: 5 },
    { code: "Fn", w: 4 },
    { code: "_spacer1", w: 9, type: "spacer" },
    { code: "ArrowUp", w: 4 },
  ],
  // Row 7 (Total 59u)
  [
    { code: "_spacer2", w: 47, type: "spacer" },
    { code: "ArrowLeft", w: 4 },
    { code: "ArrowDown", w: 4 },
    { code: "ArrowRight", w: 4 },
  ],
];
