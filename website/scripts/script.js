import { getActiveLayout } from "./codes.js";
import { keymapData } from "./mappingData.js";

/* ========================================================================
   JSON helpers: base snapshot + remaps + merged export/import
   ======================================================================== */

/**
 * Build a "base layout" keyCode array from the currently activeLayout.
 * - One entry per physical key
 * - Default behaviour: tap = base key, hold = none
 * - This is the layout your daemon should see if there are NO custom remaps.
 */
function buildBaseKeyCodeSnapshot() {
    const keyCode = [];

    for (const codeWeb in activeLayout) {
        if (!Object.prototype.hasOwnProperty.call(activeLayout, codeWeb)) continue;
        const layoutEntry = activeLayout[codeWeb];
        if (!layoutEntry) continue;

        const codeOS = layoutEntry.codeOS;
        if (codeOS === undefined || codeOS === null || codeOS === -1) continue;

        keyCode.push({
            // OS codes
            from: codeOS,
            toOnPress: codeOS, // default: press = base
            toOnHold: -1,      // default: no hold mapping

            // WebAPI codes
            fromS: codeWeb,
            toOnPressS: codeWeb,
            toOnHoldS: "",
        });
    }

    return keyCode;
}

/**
 * Build a clean copy of just your custom remaps from keymapData.
 * These are stored in WebAPI form + OS form already in keymapData.layers[0].keyCode.
 */
function buildRemapKeyCodeSnapshot() {
    const layer = getActiveLayer();
    return layer.keyCode.map(entry => ({
        from: entry.from ?? -1,
        toOnPress: entry.toOnPress ?? -1,
        toOnHold: entry.toOnHold ?? -1,
        fromS: entry.fromS || "",
        toOnPressS: entry.toOnPressS || "",
        toOnHoldS: entry.toOnHoldS || "",
    }));
}

/**
 * Overlay your remaps on top of the base layout.
 * Any remap overrides only the fields it actually uses (press / hold).
 */
function mergeRemapsOverBase(baseKeyCode, remapKeyCode) {
    const byFromS = new Map();

    // Start from base
    baseKeyCode.forEach(entry => {
        byFromS.set(entry.fromS, { ...entry });
    });

    // Apply remaps
    remapKeyCode.forEach(remap => {
        const key = remap.fromS;
        if (!key) return;

        const base = byFromS.get(key);
        if (!base) {
            // Key not part of base for some reason → just add as-is
            byFromS.set(key, { ...remap });
            return;
        }

        const merged = { ...base };

        const hasPressRemap =
            remap.toOnPress !== undefined &&
            remap.toOnPress !== null &&
            remap.toOnPress !== -1 &&
            remap.toOnPressS;

        const hasHoldRemap =
            remap.toOnHold !== undefined &&
            remap.toOnHold !== null &&
            remap.toOnHold !== -1 &&
            remap.toOnHoldS;

        if (hasPressRemap) {
            merged.toOnPress = remap.toOnPress;
            merged.toOnPressS = remap.toOnPressS;
        }

        if (hasHoldRemap) {
            merged.toOnHold = remap.toOnHold;
            merged.toOnHoldS = remap.toOnHoldS;
        }

        byFromS.set(key, merged);
    });

    return Array.from(byFromS.values());
}

/**
 * Wrap a keyCode array in the standard layout object shape.
 */
function wrapAsLayout(keyCodeArray) {
    return {
        layerEntries: 1,
        uniqueKeyCodeEntries: keyCodeArray.length,
        layers: [
            {
                layerName: "Main",
                layerNr: 0,
                keyCodeEntries: keyCodeArray.length,
                keyCode: keyCodeArray,
            },
        ],
    };
}

/**
 * What the JSON editor asks for when you click "View JSON" or "Download JSON".
 * We return:
 *   base   → full base layout (no custom remaps)
 *   remap  → only your remaps
 *   merged → base + remap overlay (this is what you download)
 *   filename → suggested download name
 */
function buildExportBundles() {
    const baseKeyCode = buildBaseKeyCodeSnapshot();
    const remapKeyCode = buildRemapKeyCodeSnapshot();
    const mergedKeyCode = mergeRemapsOverBase(baseKeyCode, remapKeyCode);

    return {
        base: wrapAsLayout(baseKeyCode),
        remap: wrapAsLayout(remapKeyCode),
        merged: wrapAsLayout(mergedKeyCode),
        filename: "my_remaps.json",
    };
}

// JSON editor hooks
JsonEditor.onExport = () => buildExportBundles();

JsonEditor.onImport = (json) => {
    importLayoutFromJson(json);
};

/**
 * Take a FULL layout JSON (base + remaps) and convert it back into:
 *   keymapData.layers[0].keyCode  = ONLY THE REMAPS (diff from base).
 * Base is always computed fresh from activeLayout, so language/layout/OS
 * switches still work.
 */
window.importLayoutFromJson = function (json) {
    if (!json.layers || !json.layers[0] || !Array.isArray(json.layers[0].keyCode)) {
        alert("Invalid remap file.");
        return;
    }

    const importedArray = json.layers[0].keyCode;

    const layer = getActiveLayer();
    layer.keyCode = []; // clear existing remaps

    // Build OS→Web lookup from current active layout
    const osToWeb = {};
    for (const codeWeb in activeLayout) {
        if (!Object.prototype.hasOwnProperty.call(activeLayout, codeWeb)) continue;
        const entry = activeLayout[codeWeb];
        if (!entry) continue;
        const osCode = entry.codeOS;
        if (osCode === undefined || osCode === null || osCode === -1) continue;
        osToWeb[osCode] = codeWeb;
    }

    // Snapshot of base layout to compare against
    const baseSnapshot = {};
    buildBaseKeyCodeSnapshot().forEach(e => {
        baseSnapshot[e.fromS] = e;
    });

    importedArray.forEach(rawEntry => {
        // -------- determine base key (WebAPI code) --------
        let fromCodeWeb = rawEntry.fromS || null;
        if (!fromCodeWeb) {
            const fromOS = typeof rawEntry.from === "number" ? rawEntry.from : -1;
            if (fromOS !== -1 && osToWeb[fromOS]) {
                fromCodeWeb = osToWeb[fromOS];
            }
        }
        if (!fromCodeWeb) return;
        if (!activeLayout[fromCodeWeb]) return;

        const base = baseSnapshot[fromCodeWeb];
        if (!base) return;

        // -------- resolve press / hold OS + Web codes from imported --------
        let pressOS = (typeof rawEntry.toOnPress === "number") ? rawEntry.toOnPress : -1;
        let holdOS = (typeof rawEntry.toOnHold === "number") ? rawEntry.toOnHold : -1;

        let pressWeb = rawEntry.toOnPressS || "";
        if (!pressWeb && pressOS !== -1 && osToWeb[pressOS]) {
            pressWeb = osToWeb[pressOS];
        }

        let holdWeb = rawEntry.toOnHoldS || "";
        if (!holdWeb && holdOS !== -1 && osToWeb[holdOS]) {
            holdWeb = osToWeb[holdOS];
        }

        // -------- check if this entry actually differs from base --------
        const modifiesPress =
            (pressOS !== -1 && pressOS !== base.toOnPress) ||
            (pressWeb && pressWeb !== base.toOnPressS);

        const modifiesHold =
            (holdOS !== -1 && holdOS !== base.toOnHold) ||
            (holdWeb && holdWeb !== base.toOnHoldS);

        if (!modifiesPress && !modifiesHold) {
            // behaves exactly like base → no custom remap
            return;
        }

        const fromEntry = activeLayout[fromCodeWeb];

        const remap = {
            // OS base key
            from: fromEntry?.codeOS ?? -1,
            fromS: fromCodeWeb,

            // only keep OS/Web for the sides that actually change
            toOnPress: modifiesPress ? (pressOS !== -1 ? pressOS : -1) : -1,
            toOnHold: modifiesHold ? (holdOS !== -1 ? holdOS : -1) : -1,

            toOnPressS: modifiesPress ? (pressWeb || "") : "",
            toOnHoldS: modifiesHold ? (holdWeb || "") : "",
        };

        layer.keyCode.push(remap);
    });

    layer.keyCodeEntries = layer.keyCode.length;

    // recolor + refresh UI
    refreshKeyboardColors();

    if (activeBaseCode) {
        loadKeyMapping(activeBaseCode);
        setBaseKeyLabel(getDisplayLabelForKey(activeBaseCode));
    }
};

/* ========================================================================
   1. Global layout state
   ======================================================================== */

let currentLanguage = "SE";
let currentLayout = "ISO";
let currentOS = "MACOS";

let activeLayout = {};          // codeWeb → { LEGEND, DESCRIPTION, SHOWDESCRIPTION, codeOS, ... }
let isBaseKeyActive = false;

/* ========================================================================
   2. Rebuild layout
   ======================================================================== */

function rebuildLayout() {
    activeLayout = getActiveLayout(currentLanguage, currentLayout, currentOS) || {};

    applyDynamicLabels();
    buildAllKeys();
    refreshKeyboardColors();

    if (activeBaseCode) {
        loadKeyMapping(activeBaseCode);
        setBaseKeyLabel(getDisplayLabelForKey(activeBaseCode));
    }
}

/* ========================================================================
   3. DOM refs
   ======================================================================== */

const keys = Array.from(document.querySelectorAll(".key"));

const pressField = document.getElementById("pressField");
const holdField = document.getElementById("holdField");
const deletePressBtn = document.getElementById("deletePress");
const deleteHoldBtn = document.getElementById("deleteHold");

const keyPicker = document.getElementById("keyPicker");
const keyPickerList = document.getElementById("keyPickerList");
const pickerSearch = document.getElementById("keyPickerSearch");

const dropdowns = Array.from(document.querySelectorAll(".setting-dropdown"));

/* ========================================================================
   4. Global state for selection / editing
   ======================================================================== */

let pickerTargetField = null;   // pressField or holdField
let activeBaseCode = null;      // WebAPI code of selected base key

let currentMapping = {          // saved mapping for active base key (WebAPI codes)
    press: null,
    hold: null,
};

let workingMapping = {          // mapping being edited in UI (WebAPI codes)
    press: null,
    hold: null,
};

const codeToKeyEl = {};         // codeWeb → .key element (for colors)
let allKeys = [];               // for picker: [{code,label,display,category}]

/* ========================================================================
   5. Helpers – labels, base key label, status
   ======================================================================== */

function getDisplayLabelForKey(codeWeb) {
    const entry = activeLayout[codeWeb];
    if (!entry) return "";
    return entry.LEGEND || "";
}

function setBaseKeyLabel(label) {
    document.getElementById("baseKeyValue").textContent = label || "—";
}

function updateBaseKeyInfoVisibility() {
    const icon = document.getElementById("baseKeyInfo");
    const bubble = document.querySelector(".base-key-tooltip"); // may be null

    if (!icon || !bubble) return;

    if (!activeBaseCode) {
        icon.style.display = "none";
        bubble.classList.remove("visible");
        return;
    }

    const entry = activeLayout[activeBaseCode];

    const shouldShow =
        entry &&
        entry.SHOWDESCRIPTION &&
        entry.DESCRIPTION &&
        entry.DESCRIPTION.trim() !== "";

    if (!shouldShow) {
        icon.style.display = "none";
        bubble.classList.remove("visible");
    } else {
        icon.style.display = "inline-flex";
        bubble.textContent = entry.DESCRIPTION;
    }
}

function updateKeyStatus(status) {
    const el = document.getElementById("keyStatus");
    if (!el) return;
    el.textContent = status;
}

function setFieldFromCode(fieldEl, codeWebOrNull, { updateWorking = true } = {}) {
    const label = codeWebOrNull ? getDisplayLabelForKey(codeWebOrNull) : "-- choose a key --";
    fieldEl.dataset.code = codeWebOrNull || "";
    fieldEl.textContent = label;

    if (!updateWorking) return;

    if (fieldEl === pressField) {
        workingMapping.press = codeWebOrNull || null;
    } else if (fieldEl === holdField) {
        workingMapping.hold = codeWebOrNull || null;
    }

    saveMappingsAfterChange();
}

/* ========================================================================
   6. Apply layout labels to visual keyboard
   ======================================================================== */

function applyDynamicLabels() {
    Object.keys(codeToKeyEl).forEach(k => delete codeToKeyEl[k]);

    keys.forEach(el => {
        const codeWeb = el.dataset.code;
        if (!codeWeb) return;

        const entry = activeLayout[codeWeb];

        if (!entry) {
            el.textContent = "";
            el.dataset.l = "";
            el.classList.add("disabled-key");
            return;
        }

        const baseLabel = getDisplayLabelForKey(codeWeb);

        while (el.firstChild) el.removeChild(el.firstChild);

        el.innerHTML = `
            <div class="key-content single">
                <div class="single-line">${baseLabel}</div>
                <div class="top-line"></div>
                <div class="bottom-line"></div>
            </div>
        `;

        el.dataset.l = baseLabel;
        el.classList.remove("disabled-key");

        codeToKeyEl[codeWeb] = el;
    });
}

/* ========================================================================
   7. Key picker – categories, list building, open/close
   ======================================================================== */

const CATEGORY = {
    letters: "LETTERS",
    numbers: "NUMBERS",
    modifiers: "MODIFIERS",
    arrows: "ARROWS",
    function: "FUNCTION",
    other: "OTHER",
};

const CATEGORY_ORDER = [
    CATEGORY.letters,
    CATEGORY.numbers,
    CATEGORY.modifiers,
    CATEGORY.arrows,
    CATEGORY.function,
    CATEGORY.other,
];

const OVERRIDES_SE = {
    LETTERS: [
        "BracketLeft", // å
        "Quote",       // ä
        "Semicolon",   // ö
    ],
    NUMBERS: [],
    SYMBOLS: [],
};

function overridesCategoryLang(codeWeb) {
    let overridesLang;

    switch (currentLanguage) {
        case "SE":
            overridesLang = OVERRIDES_SE;
            break;
        case "KR":
            overridesLang = { LETTERS: [], NUMBERS: [], SYMBOLS: [] };
            break;
        default:
            overridesLang = { LETTERS: [], NUMBERS: [], SYMBOLS: [] };
            break;
    }

    if (overridesLang.LETTERS?.includes(codeWeb)) return CATEGORY.letters;
    if (overridesLang.NUMBERS?.includes(codeWeb)) return CATEGORY.numbers;
    if (overridesLang.SYMBOLS?.includes(codeWeb)) return CATEGORY.other;

    return null;
}

function classify(codeWeb) {
    const overrideCat = overridesCategoryLang(codeWeb);
    if (overrideCat !== null) return overrideCat;

    if (/^Key[A-Z]$/.test(codeWeb)) return CATEGORY.letters;
    if (/^Digit[0-9]$/.test(codeWeb)) return CATEGORY.numbers;
    if (/^F[0-9]{1,2}$/.test(codeWeb)) return CATEGORY.function;

    if ([
        "ShiftLeft", "ShiftRight",
        "ControlLeft", "ControlRight",
        "AltLeft", "AltRight",
        "MetaLeft", "MetaRight",
        "CapsLock", "Tab", "Space",
    ].includes(codeWeb)) {
        return CATEGORY.modifiers;
    }

    if (["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight"].includes(codeWeb)) {
        return CATEGORY.arrows;
    }

    return CATEGORY.other;
}

function buildAllKeys() {
    allKeys = Object.keys(activeLayout).map(codeWeb => {
        const display = getDisplayLabelForKey(codeWeb);
        return {
            code: codeWeb,
            label: display,
            display,
            category: classify(codeWeb),
        };
    });

    allKeys.sort((a, b) => {
        if (a.category === b.category) {
            return (String(a.display) || "").localeCompare(String(b.display) || "");
        }
        return CATEGORY_ORDER.indexOf(a.category) - CATEGORY_ORDER.indexOf(b.category);
    });
}

function buildPickerList(rawQuery) {
    keyPickerList.innerHTML = "";
    const query = (rawQuery || "").trim().toLowerCase();

    const filtered = allKeys.filter(k => {
        if (!k.label) return false;
        const entry = activeLayout[k.code];
        const descTerms = (entry?.DESCRIPTION || "").toLowerCase();

        return (
            k.display.toLowerCase().includes(query) ||
            k.code.toLowerCase().includes(query) ||
            descTerms.includes(query)
        );
    });

    let currentCat = null;
    let catContainer = null;
    let grid = null;

    filtered.forEach(item => {
        if (item.category !== currentCat) {
            currentCat = item.category;

            catContainer = document.createElement("div");
            catContainer.style.display = "flex";
            catContainer.style.flexDirection = "column";

            const catHeader = document.createElement("div");
            catHeader.className = "key-picker-category";
            catHeader.textContent = currentCat;
            catContainer.appendChild(catHeader);

            grid = document.createElement("div");
            grid.className = "key-picker-grid";
            catContainer.appendChild(grid);

            keyPickerList.appendChild(catContainer);
        }

        const tile = document.createElement("div");
        tile.className = "key-picker-tile";

        const entry = activeLayout[item.code];
        const rawDesc = entry?.DESCRIPTION || "";
        const firstSentence = rawDesc.split(";")[0].trim();

        let descHTML = "";
        if (entry?.SHOWDESCRIPTION) {
            descHTML = `<div class="kp-desc">${firstSentence}</div>`;
        }

        tile.innerHTML = `
            <div class="kp-legend">${item.label}</div>
            ${descHTML}
        `;

        tile.addEventListener("click", () => {
            if (!pickerTargetField) return;
            setFieldFromCode(pickerTargetField, item.code);
            closeKeyPicker();
        });

        grid.appendChild(tile);
    });
}

function openKeyPicker(targetField) {
    pickerTargetField = targetField;
    keyPicker.classList.add("open");
    pickerSearch.value = "";
    buildPickerList("");
    pickerSearch.focus();
    pickerSearch.select();
}

function closeKeyPicker() {
    keyPicker.classList.remove("open");
    pickerTargetField = null;
}

/* ========================================================================
   8. Key picker events
   ======================================================================== */

pickerSearch.addEventListener("input", e => {
    buildPickerList(e.target.value);
});

document.addEventListener("click", e => {
    if (!keyPicker.contains(e.target) &&
        !pressField.contains(e.target) &&
        !holdField.contains(e.target)) {
        closeKeyPicker();
    }
});

keyPicker.addEventListener("click", e => e.stopPropagation());

pressField.addEventListener("click", e => {
    e.stopPropagation();
    if (!activeBaseCode) return;
    openKeyPicker(pressField);
});

holdField.addEventListener("click", e => {
    e.stopPropagation();
    if (!activeBaseCode) return;
    openKeyPicker(holdField);
});

/* ========================================================================
   9. Delete buttons for press / hold
   ======================================================================== */

deletePressBtn.addEventListener("click", e => {
    e.stopPropagation();
    setFieldFromCode(pressField, null);
});

deleteHoldBtn.addEventListener("click", e => {
    e.stopPropagation();
    setFieldFromCode(holdField, null);
});

/* ========================================================================
   10. Base key selection (clicking on visual keyboard)
   ======================================================================== */

keys.forEach(keyEl => {
    keyEl.addEventListener("click", e => {
        e.stopPropagation();

        const codeWeb = keyEl.dataset.code;
        if (!codeWeb) return;
        if (!activeLayout[codeWeb]) return;

        activeBaseCode = codeWeb;

        keys.forEach(k => k.classList.remove("active"));
        keyEl.classList.add("active");
        isBaseKeyActive = true;

        setBaseKeyLabel(getDisplayLabelForKey(codeWeb));
        loadKeyMapping(codeWeb);
        updateBaseKeyInfoVisibility();
        closeKeyPicker();
        updateKeyStatus("");
    });
});

// Global click: clear selection when clicking outside keyboard + panel + picker
document.addEventListener("click", e => {
    const clickedKey = e.target.closest(".key");
    const clickedPanel = e.target.closest(".key-info");
    const clickedPicker = e.target.closest("#keyPicker");

    if (!clickedKey && !clickedPanel && !clickedPicker) {
        activeBaseCode = null;
        isBaseKeyActive = false;

        keys.forEach(k => k.classList.remove("active"));

        setBaseKeyLabel(null);
        setFieldFromCode(pressField, null, { updateWorking: false });
        setFieldFromCode(holdField, null, { updateWorking: false });

        currentMapping = { press: null, hold: null };
        workingMapping = { press: null, hold: null };

        updateBaseKeyInfoVisibility();
        updateKeyStatus("no key selected");
    }
});

/* ========================================================================
   11. Load/save mapping for a base key (keymapData)
   ======================================================================== */

function getActiveLayer() {
    if (!keymapData.layers || keymapData.layers.length === 0) {
        keymapData.layers = [{
            layerName: "Main",
            layerNr: 0,
            keyCodeEntries: 0,
            keyCode: [],
        }];
        keymapData.layerEntries = 1;
    }
    return keymapData.layers[0];
}

function initialiseFullJsonLayout() {
    const layer = getActiveLayer();
    layer.keyCode = [];

    for (const codeWeb in activeLayout) {
        const entry = activeLayout[codeWeb];
        layer.keyCode.push({
            from: entry.codeOS ?? -1,   // OS code
            fromS: codeWeb,             // WebAPI code
            toOnPress: -1,
            toOnHold: -1,
            toOnPressS: "",
            toOnHoldS: ""
        });
    }

    layer.keyCodeEntries = layer.keyCode.length;
}

function loadKeyMapping(baseCodeWeb) {
    const layer = getActiveLayer();
    const entry = layer.keyCode.find(k => k.fromS === baseCodeWeb);

    currentMapping.press = entry?.toOnPressS || null;
    currentMapping.hold = entry?.toOnHoldS || null;

    workingMapping.press = currentMapping.press;
    workingMapping.hold = currentMapping.hold;

    setFieldFromCode(pressField, currentMapping.press, { updateWorking: false });
    setFieldFromCode(holdField, currentMapping.hold, { updateWorking: false });
}

/**
 * saveMappingsAfterChange()
 * --------------------------
 * This function is called whenever:
 *  - You change the "press" or "hold" key in the UI
 *  - You delete a mapping
 *  - You select a new base key and confirm changes
 *
 * Responsibility:
 *  - Store the mapping for the currently selected base key (`activeBaseCode`)
 *  - Update the JSON model (`keymapData`)
 *  - Update the keyboard display (legends + colors)
 *  - Ensure invalid mappings are removed
 *  - Keep OS codes (codeOS) and WebAPI codes (codeWeb) synchronized
 */

function saveMappingsAfterChange() {
    // -----------------------------
    // 1. No base key selected → do nothing
    // -----------------------------
    if (!activeBaseCode) return;

    // Active layer where all remaps are stored
    const layer = getActiveLayer();

    // Base key we are editing (WebAPI code)
    const fromCodeWeb = activeBaseCode;

    // Current chosen mappings (WebAPI codes)
    const pressCodeWeb = workingMapping.press;
    const holdCodeWeb = workingMapping.hold;

    // -----------------------------
    // 2. Prevent self-mapping:
    //    If press/hold maps to itself → remove entry completely
    // -----------------------------
    const selfPress = pressCodeWeb === fromCodeWeb;
    const selfHold = holdCodeWeb === fromCodeWeb;

    // Reset mapping when user clears both or maps to itself
    if (selfPress || selfHold || (!pressCodeWeb && !holdCodeWeb)) {

        // Always keep the key entry — just reset values
        if (!entry) {
            const fromEntry = activeLayout[fromCodeWeb];
            entry = {
                from: fromEntry?.codeOS ?? -1,
                toOnPress: -1,
                toOnHold: -1,
                fromS: fromCodeWeb,
                toOnPressS: "",
                toOnHoldS: ""
            };
            layer.keyCode.push(entry);
        }

        entry.toOnPress = -1;
        entry.toOnHold = -1;
        entry.toOnPressS = "";
        entry.toOnHoldS = "";

        // Reset visuals
        const keyEl = codeToKeyEl[fromCodeWeb];
        if (keyEl) {
            const baseLabel = getDisplayLabelForKey(fromCodeWeb);
            const kc = keyEl.querySelector(".key-content");
            kc.classList.remove("double");
            kc.classList.add("single");

            kc.querySelector(".single-line").textContent = baseLabel;
            kc.querySelector(".top-line").textContent = "";
            kc.querySelector(".bottom-line").textContent = "";
        }

        refreshKeyboardColors();
        return;
    }

    // -----------------------------
    // 3. Try to locate an existing JSON entry for this key
    // -----------------------------
    //let entry = layer.keyCode.find(k => k.fromS === fromCodeWeb);

    // -----------------------------
    // 4. If no entry exists → create a new blank mapping for this key
    // -----------------------------
    let entry = layer.keyCode.find(k => k.fromS === fromCodeWeb);
    if (!entry) {
        console.warn("Missing entry in full-layout model. This should NOT happen.");
        return; // full layout mode assumes entry is always there
    }

    // -----------------------------
    // 5. Update WebAPI mapping values
    // -----------------------------
    entry.toOnPressS = pressCodeWeb || "";
    entry.toOnHoldS = holdCodeWeb || "";

    // -----------------------------
    // 6. Update OS codes to match WebAPI codes
    // -----------------------------
    const fromEntry = activeLayout[fromCodeWeb];
    const pressEntry = pressCodeWeb ? activeLayout[pressCodeWeb] : null;
    const holdEntry = holdCodeWeb ? activeLayout[holdCodeWeb] : null;

    entry.from = fromEntry?.codeOS ?? -1;
    entry.toOnPress = pressEntry?.codeOS ?? -1;
    entry.toOnHold = holdEntry?.codeOS ?? -1;

    // -----------------------------
    // X. Automatically delete empty entries
    // -----------------------------
    const nothingMapped =
        (!entry.toOnPressS || entry.toOnPressS === "") &&
        (!entry.toOnHoldS || entry.toOnHoldS === "");

    if (nothingMapped) {
        // Remove from layer entirely
        layer.keyCodeEntries = layer.keyCode.length;

        // Reset flags
        currentMapping.press = null;
        currentMapping.hold = null;

        workingMapping.press = null;
        workingMapping.hold = null;

        // Reset UI fields
        pressField.dataset.code = "";
        holdField.dataset.code = "";
        pressField.textContent = "-- choose a key --";
        holdField.textContent = "-- choose a key --";

        // Reset the key visual
        const keyEl = codeToKeyEl[fromCodeWeb];
        if (keyEl) {
            const baseLabel = getDisplayLabelForKey(fromCodeWeb);
            const kc = keyEl.querySelector(".key-content");

            kc.classList.remove("double");
            kc.classList.add("single");

            kc.querySelector(".single-line").textContent = baseLabel;
            kc.querySelector(".top-line").textContent = "";
            kc.querySelector(".bottom-line").textContent = "";
        }

        refreshKeyboardColors();
        return; // <--- IMPORTANT: stop executing the rest of function
    }

    // -----------------------------
    // 7. Update the key legends shown on the keyboard
    // -----------------------------
    const keyEl = codeToKeyEl[fromCodeWeb];
    if (keyEl) {
        const baseLabel = getDisplayLabelForKey(fromCodeWeb);
        const pressLabel = pressCodeWeb ? getDisplayLabelForKey(pressCodeWeb) : "";
        const holdLabel = holdCodeWeb ? getDisplayLabelForKey(holdCodeWeb) : "";

        const kc = keyEl.querySelector(".key-content");
        const single = kc.querySelector(".single-line");
        const top = kc.querySelector(".top-line");
        const bottom = kc.querySelector(".bottom-line");

        // Case A: both press + hold → show double layout
        if (pressLabel && holdLabel) {
            kc.classList.remove("single");
            kc.classList.add("double");

            top.textContent = pressLabel;
            bottom.textContent = holdLabel;
            single.textContent = "";
        }

        // Case B: press only → show single (press)
        else if (pressLabel) {
            kc.classList.remove("double");
            kc.classList.add("single");

            single.textContent = pressLabel;
            top.textContent = "";
            bottom.textContent = "";
        }

        // Case C: hold only → show base + hold stacked
        else if (holdLabel) {
            kc.classList.remove("single");
            kc.classList.add("double");

            top.textContent = baseLabel;
            bottom.textContent = holdLabel;
            single.textContent = "";
        }

        // Case D: nothing mapped → return to normal
        else {
            kc.classList.remove("double");
            kc.classList.add("single");

            single.textContent = baseLabel;
            top.textContent = "";
            bottom.textContent = "";
        }
    }

    // -----------------------------
    // 8. Update global counters for JSON export
    // -----------------------------
    layer.keyCodeEntries = layer.keyCode.length;
    keymapData.layerEntries = keymapData.layers.length;

    // -----------------------------
    // 9. Update currently saved mapping
    // -----------------------------
    currentMapping.press = workingMapping.press;
    currentMapping.hold = workingMapping.hold;

    // -----------------------------
    // 10. Refresh keyboard color highlighting
    // -----------------------------
    refreshKeyboardColors();
}

/* ========================================================================
   12. Physical keyboard → set "press" mapping
   ======================================================================== */

document.addEventListener("keydown", e => {
    if (!activeBaseCode || !isBaseKeyActive) return;

    if (e.key === "Escape") {
        keys.forEach(k => k.classList.remove("active"));
        isBaseKeyActive = false;
        activeBaseCode = null;
        setBaseKeyLabel(null);
        setFieldFromCode(pressField, null, { updateWorking: false });
        setFieldFromCode(holdField, null, { updateWorking: false });
        currentMapping = { press: null, hold: null };
        workingMapping = { press: null, hold: null };
        updateBaseKeyInfoVisibility();
        updateKeyStatus("no key selected");
        return;
    }

    if (keyPicker.classList.contains("open") &&
        document.activeElement === pickerSearch) {
        return;
    }

    const codeWeb = e.code;
    if (!activeLayout[codeWeb]) return;

    workingMapping.press = codeWeb;
    setFieldFromCode(pressField, codeWeb, { updateWorking: false });
    saveMappingsAfterChange();

    e.preventDefault();
    e.stopPropagation();
});

/* ========================================================================
   13. Keyboard coloring based on mapping
   ======================================================================== */

function refreshKeyboardColors() {
    // reset all colors
    keys.forEach(k => {
        k.style.backgroundColor = "";
    });

    const layer = getActiveLayer();

    layer.keyCode.forEach(entry => {
        // Determine base key's WebAPI code
        let baseCodeWeb = entry.fromS;
        if (!baseCodeWeb && typeof entry.from === "number" && entry.from !== -1) {
            baseCodeWeb = findWebCodeFromOS(entry.from);
        }
        if (!baseCodeWeb) return;

        const keyEl = codeToKeyEl[baseCodeWeb];
        if (!keyEl) return;

        // Use symbolic mappings for color logic
        const hasPress = !!(entry.toOnPressS && entry.toOnPressS !== "");
        const hasHold = !!(entry.toOnHoldS && entry.toOnHoldS !== "");

        if (hasPress && hasHold) {
            keyEl.style.backgroundColor = "#69408f"; // both
        } else if (hasPress) {
            keyEl.style.backgroundColor = "#2d7d2d"; // press only
        } else if (hasHold) {
            keyEl.style.backgroundColor = "#8a6e33"; // hold only
        } else {
            keyEl.style.backgroundColor = "";
        }
    });
}

/* ========================================================================
   14. Settings dropdowns (language/layout/os)
   ======================================================================== */

function closeAllDropdowns() {
    dropdowns.forEach(d => d.classList.remove("open"));
}

document.addEventListener("click", () => {
    closeAllDropdowns();
});

dropdowns.forEach(dd => {
    const pill = dd.querySelector(".dropdown-pill");
    const menu = dd.querySelector(".dropdown-menu");
    const labelEl = dd.querySelector(".dropdown-pill-label");

    pill.addEventListener("click", e => {
        e.stopPropagation();
        const wasOpen = dd.classList.contains("open");
        closeAllDropdowns();
        if (!wasOpen) dd.classList.add("open");
    });

    menu.querySelectorAll(".dropdown-item").forEach(item => {
        item.addEventListener("click", () => {
            labelEl.textContent = item.textContent;
            dd.classList.remove("open");

            if (dd.dataset.setting === "language") {
                currentLanguage = item.dataset.value;
            } else if (dd.dataset.setting === "layout") {
                currentLayout = item.dataset.value;
            } else if (dd.dataset.setting === "os") {
                currentOS = item.dataset.value;
            }

            rebuildLayout();
        });
    });
});

/* ========================================================================
   15. Tooltips for info icons
   ======================================================================== */

document.querySelectorAll(".info-icon").forEach(icon => {
    const tooltip = icon.parentElement.querySelector(".tooltip-bubble");
    if (!tooltip) return;

    icon.addEventListener("click", e => {
        e.stopPropagation();
        document.querySelectorAll(".tooltip-bubble").forEach(t => {
            if (t !== tooltip) t.classList.remove("visible");
        });
        tooltip.classList.toggle("visible");
    });
});

document.addEventListener("click", () => {
    document.querySelectorAll(".tooltip-bubble").forEach(t =>
        t.classList.remove("visible")
    );
});

/* ========================================================================
   16. Initialisation
   ======================================================================== */

document.addEventListener("DOMContentLoaded", () => {
    rebuildLayout();
    initialiseFullJsonLayout();  // <-- NEW
    refreshKeyboardColors();     // <-- ensure color update
    updateKeyStatus("No key selected");
});