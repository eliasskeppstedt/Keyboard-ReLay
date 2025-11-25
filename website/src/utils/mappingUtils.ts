import { KeymapData, ActiveLayout, KeyMapEntry, Language, LayoutKind, OSKind } from "../types";

export function getActiveLayer(keymapData: KeymapData) {
    if (!keymapData.layers || keymapData.layers.length === 0) {
        return {
            layerName: "Main",
            layerNr: 0,
            keyCodeEntries: 0,
            keyCode: [] as KeyMapEntry[],
        };
    }
    return keymapData.layers[0];
}

export function buildBaseKeyCodeSnapshot(activeLayout: ActiveLayout) {
    const keyCode: any[] = [];
    for (const codeWeb in activeLayout) {
        if (codeWeb === 'None') continue; // Skip virtual None key from base layout

        const layoutEntry = activeLayout[codeWeb];
        if (!layoutEntry) continue;
        const codeOS = layoutEntry.codeOS;
        if (codeOS === undefined || codeOS === null || codeOS === 999999) continue;

        keyCode.push({
            from: codeOS,
            toOnPress: codeOS,
            toOnHold: 999999,
            fromS: codeWeb,
            toOnPressS: codeWeb,
            toOnHoldS: "",
        });
    }
    return keyCode;
}

export function buildRemapKeyCodeSnapshot(keymapData: KeymapData, activeLayout: ActiveLayout) {
    const layer = getActiveLayer(keymapData);
    return layer.keyCode.map(entry => {
        // Re-resolve numeric codes from current layout to ensure they match settings (e.g. None -> 1000)
        // and sanitize invalid states (like None on hold)
        
        const fromS = entry.fromS || "";
        const pressS = entry.toOnPressS || "";
        let holdS = entry.toOnHoldS || "";

        // Sanity check: If press is None, hold must be empty
        if (pressS === 'None') {
            holdS = "";
        }

        const from = activeLayout[fromS]?.codeOS ?? entry.from ?? 999999;
        const onPress = activeLayout[pressS]?.codeOS ?? 999999;
        const onHold = activeLayout[holdS]?.codeOS ?? 999999;

        return {
            from: from,
            toOnPress: onPress,
            toOnHold: onHold,
            fromS: fromS,
            toOnPressS: pressS,
            toOnHoldS: holdS,
        };
    });
}

export function mergeRemapsOverBase(baseKeyCode: any[], remapKeyCode: any[]) {
    const byFromS = new Map();
    baseKeyCode.forEach(entry => byFromS.set(entry.fromS, { ...entry }));

    remapKeyCode.forEach(remap => {
        const key = remap.fromS;
        if (!key) return;
        const base = byFromS.get(key);
        if (!base) {
            byFromS.set(key, { ...remap });
            return;
        }
        const merged = { ...base };
        
        // Check for presence of mapping string to allow mapping to -1 (None/Invalid)
        const hasPressRemap = remap.toOnPress !== undefined && !!remap.toOnPressS;
        const hasHoldRemap = remap.toOnHold !== undefined; // Allow empty string for hold reset

        if (hasPressRemap) {
            merged.toOnPress = remap.toOnPress;
            merged.toOnPressS = remap.toOnPressS;
        }
        
        // Always apply hold remap if it exists in the remap snapshot (even if empty to clear it)
        if (hasHoldRemap) {
            merged.toOnHold = remap.toOnHold;
            merged.toOnHoldS = remap.toOnHoldS;
        }
        byFromS.set(key, merged);
    });
    return Array.from(byFromS.values());
}

export function wrapAsLayout(keyCodeArray: any[]) {
    return {
        layerEntries: 1,
        uniqueKeyCodeEntries: keyCodeArray.length,
        layers: [{
            layerName: "Main",
            layerNr: 0,
            keyCodeEntries: keyCodeArray.length,
            keyCode: keyCodeArray,
        }],
    };
}

export function buildExportBundles(
    activeLayout: ActiveLayout, 
    keymapData: KeymapData,
    settings: { language: Language, layout: LayoutKind, os: OSKind }
) {
    const baseKeyCode = buildBaseKeyCodeSnapshot(activeLayout);
    // Pass activeLayout to correctly resolve codes (fixes None -> 1000)
    const remapKeyCode = buildRemapKeyCodeSnapshot(keymapData, activeLayout);

    // Full merge: Base layout overridden by user remaps
    const mergedList = mergeRemapsOverBase(baseKeyCode, remapKeyCode);

    const mergedLayout = wrapAsLayout(mergedList);
    const mergedWithSettings = {
        ...mergedLayout,
        settings
    };

    return {
        base: wrapAsLayout(baseKeyCode),
        remap: wrapAsLayout(remapKeyCode),
        merged: mergedWithSettings,
        settings: settings,
        filename: "my_remaps.json",
    };
}