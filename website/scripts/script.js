/* ========================================================================
   Keyboard Relay – ISO SE Layout
   ------------------------------------------------------------------------
   This file powers the interactive keyboard view + key remapping UI.

   Responsibilities:
   - Manage active layout (SE / US / KR) and OS-specific symbols
   - Render labels onto the visual keyboard
   - Let the user select a "base key" and map:
       - press: what key is sent on tap
       - hold:  what key is sent while held
   - Store mappings in `keymapData` (imported from mappingData.js)
   - Upload / download mappings as JSON
   - Handle dropdown settings and info tooltips
   ======================================================================== */

import * as codes from "./codes.js";
import { keymapData } from "./mappingData.js";

/**
 * scLayout:
 * - maps KeyboardEvent.code → visual label (string)
 * - labels are later written into `data-l` on each key element
 * - also patched with OS-specific modifier symbols (⌘, ⌃, ⌥...)
 */
/**
 * The currently selected language layout.
 * Allowed values: "SE", "US" (implicit fallback), "KR"
 */
let currentLanguage = "SE";
let currentLayout = "ISO";
let currentOS = "MACOS";

let scLayout = applyModifierSymbols(codes.getActiveLayout(currentLanguage, currentLayout, currentOS));

/**
 * keyCodes:
 * - Reserved for any mapping from KeyboardEvent.code → mac keyCode numbers
 * - Currently imported but not used in this UI layer, kept for future use.
 */
const keyCodes = { ...codes.macKeyCode };

/* ========================================================================
   DOM references
   ======================================================================== */

// Visual keyboard wrapper
const kbWrap = document.getElementById("kbWrap");
// All visual key elements
const keys = [...document.querySelectorAll(".key")];

// Key info panel fields
const pressField = document.getElementById("pressField");
const holdField = document.getElementById("holdField");

// Key picker
const keyPicker = document.getElementById("keyPicker");
const keyPickerList = document.getElementById("keyPickerList");
const pickerSearch = document.getElementById("keyPickerSearch");

// Settings dropdowns (language / layout / OS)
const dropdowns = [...document.querySelectorAll(".setting-dropdown")];

// Upload / download elements for remap JSON
const uploadBtn = document.getElementById("uploadRemap");
const downloadBtn = document.getElementById("downloadRemap");
const uploadInput = document.getElementById("uploadRemapInput");


/* ========================================================================
   Global state
   ======================================================================== */

/**
 * pickerTargetField:
 * - Which field (pressField / holdField) the key picker is currently editing.
 * - null when the picker is closed.
 */
let pickerTargetField = null;

/**
 * activeKeyLabel:
 * - The label (value of data-l) of the currently selected base key on the
 *   visual keyboard.
 * - null when no base key is active.
 */
let activeKeyLabel = null;

/**
 * currentMapping:
 * - Represents the mapping that is currently saved in keymapData
 *   for the active base key.
 */
let currentMapping = { base: null, press: null, hold: null };

/**
 * workingMapping:
 * - Represents the mapping the user is currently editing in the UI
 *   (possibly unsaved changes compared to currentMapping).
 */
let workingMapping = { base: null, press: null, hold: null };

/**
 * labelToKeyEl:
 * - Map from visual label (data-l) → corresponding DOM element (.key).
 * - This is refreshed whenever labels are recalculated.
 */
const labelToKeyEl = {};

/**
 * allKeys:
 * - Flat list of all logical keys for the key picker
 *   (code + label + category).
 */
let allKeys = [];

/* ========================================================================
   Utility functions
   ======================================================================== */

/**
 * Updates the “base key” label in the Key Info panel.
 * Uses an em dash when nothing is selected.
 */
function setBaseKeyLabel(label) {
    document.getElementById("baseKeyValue").textContent = label || "—";
}

/**
 * Updates a "pill" field (press / hold).
 *
 * @param {HTMLElement} fieldEl - The button that represents the field.
 * @param {string|null} code    - KeyboardEvent.code to store.
 * @param {string|null} label   - Display label to show in the pill.
 * @param {Object} options
 * @param {boolean} options.updateWorking - Whether to update workingMapping.
 */
function setFieldValue(fieldEl, code, label, { updateWorking = true } = {}) {
    fieldEl.dataset.code = code || "";
    fieldEl.textContent = label || "-- choose a key --";

    if (!updateWorking) return;

    if (fieldEl === pressField) {
        workingMapping.press = code;
    } else if (fieldEl === holdField) {
        workingMapping.hold = code;
    }

    syncSaveButton();
}

/**
 * Enables/disables the “Save changes” button based on whether
 * workingMapping differs from currentMapping.
 */
function syncSaveButton() {
    const btn = document.getElementById("saveChanges");

    const changed =
        workingMapping.base !== currentMapping.base ||
        workingMapping.press !== currentMapping.press ||
        workingMapping.hold !== currentMapping.hold;

    if (changed) {
        btn.classList.add("enabled");
        btn.classList.remove("disabled");
    } else {
        btn.classList.add("disabled");
        btn.classList.remove("enabled");
    }
}

/* ========================================================================
   Key picker: categorisation & list building
   ======================================================================== */

/**
 * Human-readable key categories used in the picker.
 */
const CATEGORY = {
    numbers: "Numbers",
    letters: "Letters",
    modifiers: "Modifiers",
    arrows: "Arrows",
    function: "Function",
    other: "Other",
};

/**
 * Display order for categories in the picker.
 */
const CATEGORY_ORDER = [
    CATEGORY.letters,
    CATEGORY.numbers,
    CATEGORY.modifiers,
    CATEGORY.arrows,
    CATEGORY.function,
    CATEGORY.other,
];

/**
 * Classifies a KeyboardEvent.code into a picker category.
 */
function classify(code) {
    if (code.startsWith("Digit")) return CATEGORY.numbers;
    if (code.startsWith("Key")) return CATEGORY.letters;
    if (/^F\d+$/.test(code)) return CATEGORY.function;

    if ([
        "ShiftLeft", "ShiftRight", "ControlLeft", "ControlRight",
        "AltLeft", "AltRight", "MetaLeft", "MetaRight",
        "Space", "CapsLock", "Tab"
    ].includes(code)) {
        return CATEGORY.modifiers;
    }

    if (["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight"].includes(code)) {
        return CATEGORY.arrows;
    }

    return CATEGORY.other;
}

/**
 * Rebuilds `allKeys` from the current scLayout.
 * This is called whenever the language / layout changes.
 */
function buildAllKeys() {
    allKeys = Object.keys(scLayout).map(code => ({
        code,
        label: scLayout[code],
        display: scLayout[code] || code,
        category: classify(code),
    }));

    // Stable sort: category, then alphabetical within category.
    allKeys.sort((a, b) => {
        if (a.category === b.category) {
            return a.display.localeCompare(b.display);
        }
        return a.category.localeCompare(b.category);
    });
}

/**
 * Opens the key picker for a given field (press / hold),
 * resets the search and populates the list.
 */
function openKeyPicker(fieldEl) {
    pickerTargetField = fieldEl;

    // Open picker
    keyPicker.classList.add("open");

    // Clear search + FOCUS immediately
    pickerSearch.value = "";
    pickerSearch.focus();

    // Build list
    buildPickerList("");
}

/**
 * Closes the key picker and clears the active field target.
 */
function closeKeyPicker() {
    keyPicker.classList.remove("open");
    pickerTargetField = null;
}

/**
 * Populates the key picker list based on a search query.
 *
 * @param {string} rawQuery - User input from the search field.
 */
function buildPickerList(rawQuery) {
    keyPickerList.innerHTML = "";
    let query = rawQuery.toLowerCase();

    const filtered = allKeys
        .filter(k => {
            if (!k.label?.trim()) return false;
            return (
                k.display.toLowerCase().includes(query) ||
                k.code.toLowerCase().includes(query)
            );
        })
        .sort((a, b) =>
            CATEGORY_ORDER.indexOf(a.category) - CATEGORY_ORDER.indexOf(b.category)
        );

    let currentCat = null;
    let grid = null;

    filtered.forEach(item => {
        // Create new category block if needed
        if (item.category !== currentCat) {
            currentCat = item.category;

            const container = document.createElement("div");
            container.style.display = "flex";
            container.style.flexDirection = "column";
            keyPickerList.appendChild(container);

            const cat = document.createElement("div");
            cat.className = "key-picker-category";
            cat.textContent = currentCat.toUpperCase();
            container.appendChild(cat);

            grid = document.createElement("div");
            grid.className = "key-picker-grid";
            container.appendChild(grid);
        }

        const tile = document.createElement("div");
        tile.className = "key-picker-tile";
        tile.textContent = item.display;
        tile.onclick = () => {
            if (!pickerTargetField) return;
            setFieldValue(pickerTargetField, item.code, item.display);
            closeKeyPicker();
        };

        grid.appendChild(tile);
    });
}

/* ========================================================================
   Physical keyboard interaction
   ======================================================================== */

/**
 * Handles physical keyboard events while the page is focused.
 *
 * Rules:
 * - If the key picker is open *and* the active element is a text field,
 *   we let the keypress go through (for typing in search).
 * - If the picker is open but focus is elsewhere → ignore.
 * - If a base key is active and no picker is open, pressing a key
 *   assigns that code as the “press” mapping for the base key.
 */
document.addEventListener("keydown", (e) => {
    const code = e.code;
    const label = scLayout[code] ?? code;

    // 1. Picker open + input focused → allow typing
    if (pickerTargetField && document.activeElement === pickerTargetField) {
        return;
    }

    // 2. Picker open but field not focused → ignore
    if (pickerTargetField) {
        return;
    }

    // 3. No picker, base key active → record press mapping
    if (activeKeyLabel) {
        workingMapping.press = code;
        setFieldValue(pressField, code, label);
        syncSaveButton();

        e.preventDefault();
        e.stopPropagation();
        return;
    }

    // 4. Otherwise ignore keyboard input
});

/* ========================================================================
   Dynamic labeling & initial DOMContentLoaded init
   ======================================================================== */

/**
 * Applies OS-specific modifier symbols to a given layout object.
 * For macOS, replaces Control / Alt / Meta with ⌃ / ⌥ / ⌘ labels.
 *
 * @param {Object} layout - base layout mapping (code → label)
 * @returns {Object} patched layout
 */
function applyModifierSymbols(layout) {
    const dd = document.querySelector(
        '.setting-dropdown[data-setting="os"] .dropdown-pill-label'
    );
    const currentOS = dd ? dd.textContent : "macOS";

    let patched = { ...layout };

    if (currentOS === "macOS") {
        patched = {
            ...patched,
            ControlLeft: "L ⌃",
            ControlRight: "R ⌃",
            AltLeft: "L ⌥",
            AltRight: "R ⌥",
            MetaLeft: "L ⌘",
            MetaRight: "R ⌘",
        };
    }

    return patched;   // <-- the missing return
}

/**
 * Updates all .key elements with the correct visual labels
 * based on scLayout.
 *
 * - Sets el.dataset.l for each key (for CSS and mapping).
 * - Rebuilds labelToKeyEl so label → element lookups stay valid.
 */
function applyDynamicLabels() {
    // Reset mapping and rebuild from scratch
    Object.keys(labelToKeyEl).forEach(k => delete labelToKeyEl[k]);

    document.querySelectorAll(".key").forEach(el => {
        const code = el.dataset.code;
        const label = scLayout[code] ?? "";

        el.dataset.l = label;

        // Key lookup table now uses the KeyboardEvent.code, not the glyph
        if (code) {
            labelToKeyEl[code] = el;
        }
    });
}

/**
 * Full init routine that runs once the DOM is available:
 * - Apply labels
 * - Build key picker list
 * - Refresh keyboard key colors
 */
document.addEventListener("DOMContentLoaded", () => {
    applyDynamicLabels();
    buildAllKeys();
    refreshKeyboardColors();
});

/* ========================================================================
   Key picker events
   ======================================================================== */

// Filter picker list as the user types
pickerSearch.addEventListener("input", e => {
    buildPickerList(e.target.value.trim().toLowerCase());
});

// Close picker when clicking outside of it and outside of the fields
document.addEventListener("click", (e) => {
    if (!keyPicker.contains(e.target) &&
        !pressField.contains(e.target) &&
        !holdField.contains(e.target)) {
        closeKeyPicker();
    }
});

// Stop propagation when clicking inside picker
keyPicker.addEventListener("click", (e) => e.stopPropagation());

// Open picker for "press" / "hold" fields
pressField.addEventListener("click", e => {
    e.stopPropagation();
    openKeyPicker(pressField);
});

holdField.addEventListener("click", e => {
    e.stopPropagation();
    openKeyPicker(holdField);
});

/* ========================================================================
   Remove buttons (clear mapping)
   ======================================================================== */

/**
 * Global click handler for any "delete-btn" (🚫) next to a pill input.
 * Clears that field’s mapping and updates save button state.
 */
document.addEventListener("click", (e) => {
    if (!e.target.classList.contains("delete-btn")) return;

    e.stopPropagation();

    const field = e.target.parentElement.querySelector(".pill-input");
    if (!field) return;

    setFieldValue(field, "", "");
    syncSaveButton();
});

/* ========================================================================
   Base key selection on the visual keyboard
   ======================================================================== */

/**
 * When clicking on any visual .key, select it as the base key.
 * - Highlights the key
 * - Loads its existing mapping from keymapData
 * - Updates the base key label in the side panel
 */
keys.forEach(keyEl => {
    keyEl.addEventListener("click", () => {
        const code = keyEl.dataset.code;
        if (!code) return;

        // Check whether this key has a visible label in the current layout
        const label = scLayout[code];
        if (!label) return;

        // Base key identity = Web API code
        activeKeyLabel = code;

        // Visual highlight
        keys.forEach(k => k.classList.remove("active"));
        keyEl.classList.add("active");

        // Load mapping for this base key (by Web code)
        loadKeyMapping(code);

        // Update UI (display the glyph, but store code internally)
        setBaseKeyLabel(label);

        closeKeyPicker();
    });
});

/**
 * Clicking anywhere outside a .key or the key-info panel
 * will clear the current selection and reset the fields.
 */
document.addEventListener("click", (e) => {
    const isKey = e.target.closest(".key");
    const isKeyInfo = e.target.closest(".key-info");

    if (!isKey && !isKeyInfo) {
        keys.forEach(k => k.classList.remove("active"));
        activeKeyLabel = null;
        setBaseKeyLabel(null);

        // Clear press/hold pills (but keep workingMapping in sync)
        setFieldValue(pressField, null, "");
        setFieldValue(holdField, null, "");
        syncSaveButton();
    }
});

/* ========================================================================
   Load / save mapping for a given base key
   ======================================================================== */

/**
 * Loads mapping information from keymapData for the given
 * base label and updates currentMapping, workingMapping, and
 * the press/hold UI fields.
 *
 * @param {string} label - base key label (matches entry.from)
 */
function loadKeyMapping(code) {
    const layer = keymapData.layers[0];
    const entry = layer.keyCode.find(k => k.from === code);

    currentMapping.base = code;
    currentMapping.press = entry?.toOnPress ?? null;
    currentMapping.hold = entry?.toOnHold ?? null;

    workingMapping.base = currentMapping.base;
    workingMapping.press = currentMapping.press;
    workingMapping.hold = currentMapping.hold;

    // Update UI fields from currentMapping (without re-writing workingMapping)
    setFieldValue(
        pressField,
        currentMapping.press,
        currentMapping.press
            ? (scLayout[currentMapping.press] || currentMapping.press)
            : "",
        { updateWorking: false }
    );

    setFieldValue(
        holdField,
        currentMapping.hold,
        currentMapping.hold
            ? (scLayout[currentMapping.hold] || currentMapping.hold)
            : "",
        { updateWorking: false }
    );

    syncSaveButton();
}

/**
 * Writes the current workingMapping for the active base key
 * back into keymapData and refreshes colors.
 */
document.getElementById("saveChanges").addEventListener("click", () => {
    if (!activeKeyLabel) return;

    const layer = keymapData.layers[0];
    let entry = layer.keyCode.find(k => k.from === activeKeyLabel);

    // Create new entry if this base key has never been mapped
    if (!entry) {
        entry = {
            // Web API code, e.g. "KeyH"
            from: activeKeyLabel,
            toOnPress: null,
            toOnHold: null,

            // String fields = exact Web API code
            fromS: activeKeyLabel,
            toOnPressS: "",
            toOnHoldS: "",
        };
        layer.keyCode.push(entry);
        layer.keyCodeEntries++;
    }

    // Save raw Web codes
    entry.toOnPress = workingMapping.press;
    entry.toOnHold = workingMapping.hold;

    // Update string versions (always Web code)
    entry.fromS = entry.from;
    entry.toOnPressS = entry.toOnPress || "";
    entry.toOnHoldS = entry.toOnHold || "";

    // Keep currentMapping in sync
    currentMapping.press = workingMapping.press;
    currentMapping.hold = workingMapping.hold;

    syncSaveButton();
    refreshKeyboardColors();
});

/* ========================================================================
   Keyboard color refresh based on mappings
   ======================================================================== */

/**
 * Colors keys on the visual keyboard depending on how they are mapped
 * in the active layer:
 *
 * - no mapping      → default background
 * - press only      → green
 * - hold only       → brown/orange
 * - press + hold    → purple
 */
function refreshKeyboardColors() {
    // Reset all keys to default background
    keys.forEach(k => k.style.background = "");

    const layer = keymapData.layers[0];

    layer.keyCode.forEach(entry => {
        const el = labelToKeyEl[entry.from];
        if (!el) return;

        const press = !!entry.toOnPress;
        const hold = !!entry.toOnHold;

        if (press && hold) {
            el.style.background = "#69408f";
        } else if (press) {
            el.style.background = "#2d7d2d";
        } else if (hold) {
            el.style.background = "#8a6e33";
        } else {
            el.style.background = "#2a2a2a";
        }
    });
}

/* ========================================================================
   Settings dropdowns (Language / Layout / OS)
   ======================================================================== */

/**
 * Closes all dropdown menus (used whenever clicking outside, etc.).
 */
function closeAllDropdowns() {
    dropdowns.forEach(d => d.classList.remove("open"));
}

// Global click → close all dropdowns
document.addEventListener("click", () => {
    closeAllDropdowns();
});

// Per-dropdown wiring
dropdowns.forEach(dd => {
    const pill = dd.querySelector(".dropdown-pill");
    const menu = dd.querySelector(".dropdown-menu");
    const labelEl = dd.querySelector(".dropdown-pill-label");

    // Toggle dropdown open/closed when clicking pill
    pill.addEventListener("click", e => {
        e.stopPropagation();
        const wasOpen = dd.classList.contains("open");
        closeAllDropdowns();
        if (!wasOpen) dd.classList.add("open");
    });

    // Handle item selection
    menu.querySelectorAll(".dropdown-item").forEach(item => {
        item.addEventListener("click", () => {
            labelEl.textContent = item.textContent;
            dd.classList.remove("open");

            if (dd.dataset.setting === "language") {
                // Switch layout language
                currentLanguage = item.dataset.value;
                scLayout = applyModifierSymbols(codes.getActiveLayout(currentLanguage, currentLayout, currentOS));
                applyDynamicLabels();
                buildAllKeys();
                refreshKeyboardColors();
            }

            // Layout + OS changes are already respected by using
            // applyModifierSymbols + applyDynamicLabels.
        });
    });
});

/* ========================================================================
   Tooltip logic for "press" / "hold" info
   ======================================================================== */

/**
 * Simple toggle logic:
 * - Clicking an info icon shows its tooltip and hides all others.
 * - Clicking anywhere else hides all tooltips.
 */
document.querySelectorAll(".info-icon").forEach(icon => {
    const tooltip = icon.parentElement.querySelector(".tooltip-bubble");

    icon.addEventListener("click", (e) => {
        e.stopPropagation();

        // Hide all other tooltips first
        document.querySelectorAll(".tooltip-bubble").forEach(t => {
            if (t !== tooltip) t.classList.remove("visible");
        });

        // Toggle this one
        tooltip.classList.toggle("visible");
    });
});

document.addEventListener("click", () => {
    document.querySelectorAll(".tooltip-bubble").forEach(t =>
        t.classList.remove("visible")
    );
});

/* ========================================================================
   Download remap JSON
   ======================================================================== */
function createExportData() {
    const layer = keymapData.layers[0];

    // count unique base keys that have ANY mapping
    const uniqueKeys = new Set(
        layer.keyCode.map(e => e.from)
    ).size;

    const exportLayer = {
        layerName: layer.layerName,
        layerNr: layer.layerNr,
        keyCodeEntries: layer.keyCodeEntries,
        keyCode: layer.keyCode.map(entry => {
            const fromMac = codes.VK_MAC_INDEPENDENT[entry.from]?.MAC
                ?? codes.VK_MAC_JIS[entry.from]?.MAC
                ?? -1;

            const pressMac = entry.toOnPress
                ? (codes.VK_MAC_INDEPENDENT[entry.toOnPress]?.MAC
                    ?? codes.VK_MAC_JIS[entry.toOnPress]?.MAC)
                : -1;

            const holdMac = entry.toOnHold
                ? (codes.VK_MAC_INDEPENDENT[entry.toOnHold]?.MAC
                    ?? codes.VK_MAC_JIS[entry.toOnHold]?.MAC)
                : -1;

            return {
                from: fromMac,
                toOnPress: pressMac ?? -1,
                toOnHold: holdMac ?? -1,

                // MUST be the exact Web API code
                fromS: entry.from,
                toOnPressS: entry.toOnPress ?? "",
                toOnHoldS: entry.toOnHold ?? ""
            };
        })
    };

    return {
        layerEntries: 1,
        uniqueKeyCodeEntries: uniqueKeys,
        layers: [exportLayer]
    };
}

/**
 * Serialises the current keymapData to JSON and triggers a download.
 */
downloadBtn.addEventListener("click", () => {
    const data = createExportData();

    const blob = new Blob(
        [JSON.stringify(data, null, 4)],
        { type: "application/json" }
    );

    const a = document.createElement("a");
    a.href = URL.createObjectURL(blob);
    a.download = "my_remap.json";
    a.click();
    URL.revokeObjectURL(a.href);
});

/* ========================================================================
   Upload remap JSON
   ======================================================================== */

/**
 * Opens the hidden <input type="file"> when clicking the Upload button.
 */
uploadBtn.addEventListener("click", () => {
    uploadInput.click();
});

/**
 * Reads a selected JSON file, parses it and replaces the current layer
 * in keymapData, then refreshes colors and the UI for the active key.
 */
uploadInput.addEventListener("change", () => {
    const file = uploadInput.files[0];
    if (!file) return;

    const reader = new FileReader();

    reader.onload = () => {
        try {
            const json = JSON.parse(reader.result);

            // Expected minimal structure:
            // {
            //   layers: [
            //     { keyCode: [ { from, toOnPress, toOnHold, ... }, ... ] }
            //   ]
            // }
            if (!json.layers || !json.layers[0]) {
                alert("Invalid remap file.");
                return;
            }

            keymapData.layers[0].keyCode = json.layers[0].keyCode;
            keymapData.layers[0].keyCodeEntries = json.layers[0].keyCode.length;

            refreshKeyboardColors();

            // If a key is active, reload its mapping into the panel
            if (activeKeyLabel) {
                loadKeyMapping(activeKeyLabel);
            }
        } catch (e) {
            alert("Failed to load remap: " + e);
        }
    };

    reader.readAsText(file);
});