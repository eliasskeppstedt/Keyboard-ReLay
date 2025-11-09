let FROM_KEY = null;
let TO_KEY = null;
let FROM_CODE = null;
let TO_CODE = null;
let FROM_DEC = null;
let TO_DEC = null;
let SELECTING = null;
let RELAY_KEYMAP = null;
let USER_OS = null;

const instruction = document.getElementById("instruction");
const fromBox = document.getElementById("from-key");
const toBox = document.getElementById("to-key");
const pressBtn = document.getElementById("press-btn");
const holdBtn = document.getElementById("hold-btn");
const downloadBtn = document.getElementById("download-btn");
const mappingList = document.getElementById("mappings");

const FROM_PLACEHOLDER = 'Click to set<br>“from” key';
const TO_PLACEHOLDER = 'Click to set<br>“to” key';

const mappings = [];

/* ------------------ Modal System ------------------ */
const modalEl = document.getElementById("relay-modal");
const modalText = document.getElementById("relay-modal-text");
const modalOk = document.getElementById("relay-modal-ok");
const modalCancel = document.getElementById("relay-modal-cancel");
const modalBackdrop = modalEl.querySelector(".modal-backdrop");

function openModal({ message, confirm = false }) {
    return new Promise((resolve) => {
        modalText.textContent = message;
        modalCancel.style.display = confirm ? "inline-block" : "none";
        modalEl.classList.remove("hidden");

        // Adjust layout for single-button modals
        const actions = modalEl.querySelector(".modal-actions");
        actions.style.justifyContent = confirm ? "space-between" : "center";
        modalText.style.textAlign = "center";

        const cleanup = () => {
            modalEl.classList.add("hidden");
            modalOk.removeEventListener("click", onOk);
            modalCancel.removeEventListener("click", onCancel);
            modalBackdrop.removeEventListener("click", onCancel);
            window.removeEventListener("keydown", onKey);
        };
        const onOk = () => { cleanup(); resolve(true); };
        const onCancel = () => { cleanup(); resolve(false); };
        const onKey = (e) => {
            if (e.key === "Escape" && confirm) onCancel();
            if (e.key === "Enter") onOk();
        };

        modalOk.addEventListener("click", onOk);
        modalCancel.addEventListener("click", onCancel);
        modalBackdrop.addEventListener("click", onCancel);
        window.addEventListener("keydown", onKey);

        setTimeout(() => modalOk.focus(), 0);
    });
}

function modalAlert(message) { return openModal({ message, confirm: false }); }
function modalConfirm(message) { return openModal({ message, confirm: true }); }

/* --------------------------------------------------- */
window.addEventListener("DOMContentLoaded", async () => {
    try {
        const response = await fetch("relay_keymap.json");
        if (!response.ok) throw new Error(`Failed to load JSON: ${response.status}`);

        RELAY_KEYMAP = await response.json();
        console.log("✅ relay_keymap.json loaded:", RELAY_KEYMAP);
    } catch (err) {
        console.error("❌ Could not load relay_keymap.json:", err);
    }

    if (navigator.userAgentData?.platform?.toUpperCase().includes("MAC") ||
        navigator.userAgent?.toUpperCase().includes("MAC")) {
        USER_OS = "mac";
    }
    else if (navigator.userAgentData?.platform?.toUpperCase().includes("WIN") ||
        navigator.userAgent?.toUpperCase().includes("WIN")) {
        USER_OS = "win";
    }
    else {
        USER_OS = "lin";
    }
});

function setBoxHTML(box, html, waiting = false) {
    box.innerHTML = `<span class="${waiting ? 'placeholder' : (html.includes('<br>') ? 'placeholder' : 'label')}">${html}</span>`;
    box.classList.toggle("waiting", waiting);
}

function resetUnchosenWaiting() {
    if (SELECTING === "from") {
        if (!TO_KEY) setBoxHTML(toBox, TO_PLACEHOLDER, false);
        toBox.classList.remove("waiting");
    } else if (SELECTING === "to") {
        if (!FROM_KEY) setBoxHTML(fromBox, FROM_PLACEHOLDER, false);
        fromBox.classList.remove("waiting");
    }
}

function enableRemapButtons() {
    const ready = !!(FROM_KEY && TO_KEY);
    [pressBtn, holdBtn].forEach(btn => {
        btn.disabled = !ready;
        btn.classList.toggle("active", ready);
    });
}

function lookupOsDec(webCodeValue) {
    if (!RELAY_KEYMAP) return null;
    const keymap = RELAY_KEYMAP.keymap;
    const entry =
        keymap.independent?.[webCodeValue] ||
        keymap.ANSI?.[webCodeValue] ||
        null;

    if (!entry) return null;
    const rec = entry[USER_OS];
    if (!rec || typeof rec.dec !== 'number') return null;
    return rec.dec; // <- the OS decimal code you want to store
}

function formatKey(e) {
    const code = e.code;
    //const isMac = navigator.platform.toUpperCase().includes("MAC"); // uhm is deprecated
    const isMac = USER_OS == "mac";

    // Helper to capitalize nicely
    const cap = (s) => s.charAt(0).toUpperCase() + s.slice(1).toLowerCase();

    // Modifier keys
    if (code.startsWith("Shift"))
        return code.includes("Right") ? "Right Shift" : "Left Shift";

    if (code.startsWith("Control"))
        return code.includes("Right") ? "Right Ctrl" : "Left Ctrl";

    if (code.startsWith("Alt"))
        return code.includes("Right")
            ? isMac ? "Right Opt" : "Right Alt"
            : isMac ? "Left Opt" : "Left Alt";

    if (code.startsWith("Meta"))
        return code.includes("Right")
            ? isMac ? "Right Cmd" : "Right Win"
            : isMac ? "Left Cmd" : "Left Win";

    // Spacebar
    if (e.code === "Space" || e.key === " " || e.key === "Spacebar")
        return "Space";

    // Arrow keys
    if (e.code.startsWith("Arrow")) {
        const dir = e.code.replace("Arrow", "");
        return `${cap(dir)} Arrow`;
    }

    // Dead keys (accent keys on intl layouts)
    if (e.key === "Dead") {
        return `Dead (Key Code ${e.code})`;
    }

    // Everything else
    const key = e.key.length === 1 ? e.key.toUpperCase() : e.key;
    return cap(key);
}

function startListening(square) {
    // Cancel any previous listener
    window.onkeydown = null;

    // If the user clicks the same box again, just toggle off focus
    if (SELECTING === square) {
        SELECTING = null;
        if (!FROM_KEY) setBoxHTML(fromBox, FROM_PLACEHOLDER, false);
        if (!TO_KEY) setBoxHTML(toBox, TO_PLACEHOLDER, false);
        fromBox.classList.remove("waiting");
        toBox.classList.remove("waiting");
        instruction.textContent = "Click a square to change keys or choose remap type.";
        return;
    }

    SELECTING = square;

    // Reset both boxes first
    if (!FROM_KEY) setBoxHTML(fromBox, FROM_PLACEHOLDER, false);
    if (!TO_KEY) setBoxHTML(toBox, TO_PLACEHOLDER, false);
    fromBox.classList.remove("waiting");
    toBox.classList.remove("waiting");

    // Apply focus to the correct one
    if (square === "from") {
        setBoxHTML(fromBox, "Press a key...", true);
    } else {
        setBoxHTML(toBox, "Press a key...", true);
    }

    instruction.textContent = `Press a key to set ${square === "from" ? "source" : "target"} key.`;

    window.onkeydown = async (e) => {
        e.preventDefault();

        // 1) Look up OS keycode from relay_keymap.json using e.code
        const dec = lookupOsDec(e.code);
        if (dec == null) {
            await modalAlert(`Could not assign key for "${e.code}" (not found in keymap).`);
            return; // keep listening so the user can press another key
        }

        // 2) Human-friendly label for UI
        const label = formatKey(e);

        // 3) Store both the *display label* and the *real code & OS-dec*
        if (square === "from") {
            FROM_KEY = label;
            FROM_CODE = e.code;
            FROM_DEC = dec;
            setBoxHTML(fromBox, label, false);
        } else {
            TO_KEY = label;
            TO_CODE = e.code;
            TO_DEC = dec;
            setBoxHTML(toBox, label, false);
        }

        // 4) Clean up & enable buttons
        SELECTING = null;
        instruction.textContent = "Click a square to change keys or choose remap type.";
        window.onkeydown = null;
        enableRemapButtons();
    };
}

// ---- Cancel focus when clicking outside ----
window.addEventListener("click", (e) => {
    // Ignore clicks inside boxes
    if (e.target.closest("#from-key") || e.target.closest("#to-key")) return;

    // Cancel focus when clicking elsewhere
    if (SELECTING !== null) {
        SELECTING = null;
        window.onkeydown = null;
        instruction.textContent = "Click a square to change keys or choose remap type.";

        if (!FROM_KEY) setBoxHTML(fromBox, FROM_PLACEHOLDER, false);
        if (!TO_KEY) setBoxHTML(toBox, TO_PLACEHOLDER, false);

        fromBox.classList.remove("waiting");
        toBox.classList.remove("waiting");
    }
});

fromBox.addEventListener("click", (e) => {
    e.stopPropagation(); // prevent outside click handler
    startListening("from");
});

toBox.addEventListener("click", (e) => {
    e.stopPropagation();
    startListening("to");
});

pressBtn.addEventListener("click", () => addMapping("on press"));
holdBtn.addEventListener("click", () => addMapping("on hold"));

async function addMapping(type) {
    if (!FROM_KEY || !TO_KEY) return;

    // Prevent mapping to itself (use OS codes to be precise)
    if (FROM_DEC === TO_DEC) {
        await modalAlert("You cannot map a key to itself.");
        return;
    }

    // If "from" already mapped, ask to replace (use FROM_DEC as the identity)
    const existingIndex = mappings.findIndex(m => m.fromDec === FROM_DEC);
    if (existingIndex !== -1) {
        const replace = await modalConfirm(`${FROM_KEY} is already mapped. Replace it?`);
        if (!replace) return;
        mappings.splice(existingIndex, 1);
    }

    // Store both human labels and actual OS codes
    mappings.push({
        fromDec: FROM_DEC,
        toDec: TO_DEC,
        fromS: FROM_KEY,
        toS: TO_KEY,
        type
    });

    renderMappings();

    // Reset selection
    FROM_KEY = TO_KEY = null;
    FROM_CODE = TO_CODE = null;
    FROM_DEC = TO_DEC = null;
    setBoxHTML(fromBox, FROM_PLACEHOLDER, false);
    setBoxHTML(toBox, TO_PLACEHOLDER, false);
    enableRemapButtons();
}

function renderMappings() {
    mappingList.innerHTML = "";
    mappings.forEach(map => {
        const li = document.createElement("li");

        const main = document.createElement("div");
        main.className = "mapping-main";
        main.textContent = `${map.fromS} → ${map.toS}`;

        const type = document.createElement("div");
        type.className = "mapping-type";
        type.textContent = `(${map.type})`;

        const del = document.createElement("button");
        del.className = "remove-btn";
        del.textContent = "✕";
        del.onclick = () => {
            const i = mappings.indexOf(map);
            mappings.splice(i, 1);
            renderMappings();
        };

        li.appendChild(main);
        li.appendChild(type);
        li.appendChild(del);
        mappingList.appendChild(li);
    });
}

downloadBtn.addEventListener("click", async () => {
    if (mappings.length === 0) {
        await modalAlert("No remaps to download.");
        return;
    }

    const json = {
        layerEntries: 1,
        layers: [
            {
                layerName: "Main",
                layerNr: "0",
                keyCode: mappings.map(m => ({
                    from: m.fromDec,
                    toOnPress: m.type === "on press" ? m.toDec : -1,
                    toOnHold: m.type === "on hold" ? m.toDec : -1,
                    fromS: m.fromS,
                    toOnPressS: m.type === "on press" ? m.toS : "",
                    toOnHoldS: m.type === "on hold" ? m.toS : ""
                }))
            }
        ]
    };

    const blob = new Blob([JSON.stringify(json, null, 4)], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = "my_remaps.json";
    document.body.appendChild(a);
    a.click();
    a.remove();
    URL.revokeObjectURL(url);
});

setBoxHTML(fromBox, FROM_PLACEHOLDER);
setBoxHTML(toBox, TO_PLACEHOLDER);