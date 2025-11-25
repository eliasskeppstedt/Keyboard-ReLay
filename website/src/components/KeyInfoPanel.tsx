import React, { useState, useEffect } from 'react';
import { ActiveLayout, KeymapData, WebKeyCode } from '../types';
import { getActiveLayer } from '../utils/mappingUtils';
import { KeyPicker } from './KeyPicker';

interface KeyInfoPanelProps {
    activeLayout: ActiveLayout;
    activeBaseCode: WebKeyCode | null;
    keymapData: KeymapData;
    setKeymapData: React.Dispatch<React.SetStateAction<KeymapData>>;
}

export const KeyInfoPanel: React.FC<KeyInfoPanelProps> = ({ 
    activeLayout, activeBaseCode, keymapData, setKeymapData 
}) => {
    const [pickerField, setPickerField] = useState<"press" | "hold" | null>(null);
    
    const layer = getActiveLayer(keymapData);
    const mapping = activeBaseCode ? layer.keyCode.find(k => k.fromS === activeBaseCode) : null;

    // If press code is same as base key, treat it as null (default) for UI purposes
    let pressCode = mapping?.toOnPressS || null;
    if (pressCode === activeBaseCode) {
        pressCode = null;
    }
    
    const holdCode = mapping?.toOnHoldS || null;

    const pressLabel = pressCode ? activeLayout[pressCode]?.LEGEND : "-- choose a key --";
    const holdLabel = holdCode ? activeLayout[holdCode]?.LEGEND : "-- choose a key --";

    const isPressNone = pressCode === 'None';

    const updateMapping = (type: "press" | "hold", newCode: string | null) => {
        if (!activeBaseCode) return;
        
        setKeymapData(prev => {
            const newLayer = getActiveLayer(prev);
            let entry = newLayer.keyCode.find(k => k.fromS === activeBaseCode);
            
            if (!entry) {
                // Create if not exists
                entry = {
                    from: activeLayout[activeBaseCode]?.codeOS ?? 999999,
                    fromS: activeBaseCode,
                    toOnPress: 999999, toOnHold: 999999, toOnPressS: "", toOnHoldS: ""
                };
                newLayer.keyCode.push(entry);
            }

            if (type === "press") {
                entry.toOnPressS = newCode || "";
                entry.toOnPress = newCode ? (activeLayout[newCode]?.codeOS ?? 999999) : 999999;

                // If explicitly setting None, clear Hold
                if (newCode === 'None') {
                    entry.toOnHoldS = "";
                    entry.toOnHold = 999999;
                }
            } else {
                entry.toOnHoldS = newCode || "";
                entry.toOnHold = newCode ? (activeLayout[newCode]?.codeOS ?? 999999) : 999999;
            }

            // Cleanup empty
            if (!entry.toOnPressS && !entry.toOnHoldS) {
                newLayer.keyCode = newLayer.keyCode.filter(k => k.fromS !== activeBaseCode);
            }

            return { ...prev };
        });
    };

    return (
        <div className="key-info" onClick={() => setPickerField(null)}>
             <div className="key-info-header">
                <h1>Key Info</h1>
                <div className="key-status">{activeBaseCode ? "" : "No key selected"}</div>
            </div>

            <div>
                <div className="label-with-tooltip">
                    <label>base key</label>
                    {activeBaseCode && activeLayout[activeBaseCode]?.SHOWDESCRIPTION && (
                        <div className="info-icon" title={activeLayout[activeBaseCode].DESCRIPTION}>i</div>
                    )}
                </div>
                <span id="baseKeyValue">{activeBaseCode ? activeLayout[activeBaseCode]?.LEGEND : "—"}</span>
            </div>

            <div>
                <div className="label-with-tooltip"><label>press</label></div>
                <div className="pill-row">
                    <button 
                        className="pill-input" 
                        onClick={(e) => { e.stopPropagation(); if(activeBaseCode) setPickerField("press"); }}
                    >
                        {pressLabel}
                    </button>
                    <button className="delete-btn" onClick={() => updateMapping("press", null)}>×</button>
                </div>
            </div>

            <div>
                <div className="label-with-tooltip"><label>hold</label></div>
                <div className="pill-row">
                    <button 
                        className="pill-input" 
                        disabled={isPressNone}
                        style={isPressNone ? { opacity: 0.5, cursor: 'not-allowed' } : {}}
                        onClick={(e) => { e.stopPropagation(); if(activeBaseCode && !isPressNone) setPickerField("hold"); }}
                    >
                        {holdLabel}
                    </button>
                    <button 
                        className="delete-btn" 
                        disabled={isPressNone}
                        style={isPressNone ? { opacity: 0.5, cursor: 'not-allowed' } : {}}
                        onClick={() => updateMapping("hold", null)}
                    >×</button>
                </div>
            </div>

            <KeyPicker 
                activeLayout={activeLayout}
                isOpen={!!pickerField}
                onClose={() => setPickerField(null)}
                onSelect={(code) => updateMapping(pickerField!, code)}
                mode={pickerField}
            />
        </div>
    );
};