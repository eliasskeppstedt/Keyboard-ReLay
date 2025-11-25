
import React from "react";
import type { ActiveLayout, KeymapData, WebKeyCode, LayoutKind } from "../types";
import { KB_ISO, KB_ANSI, KB_JIS } from "../layout/kbGeometry";
import { getActiveLayer } from "../utils/mappingUtils";

interface KeyboardProps {
  activeLayout: ActiveLayout;
  keymapData: KeymapData;
  activeBaseCode: WebKeyCode | null;
  onKeySelect: (code: WebKeyCode) => void;
  setKeymapData: React.Dispatch<React.SetStateAction<KeymapData>>;
  layoutKind: LayoutKind;
}

export const Keyboard: React.FC<KeyboardProps> = ({
  activeLayout,
  activeBaseCode,
  onKeySelect,
  keymapData,
  layoutKind
}) => {
  const handleClick = (code: WebKeyCode) => {
    if (!code) return; 
    onKeySelect(code);
  };

  const layer = getActiveLayer(keymapData);
  
  // Dynamic geometry selection based on settings
  let geometry = KB_ISO;
  if (layoutKind === 'ANSI') geometry = KB_ANSI;
  if (layoutKind === 'JIS') geometry = KB_JIS;

  return (
    <div className="kb" key={layoutKind}>
      {geometry.map((row, rowIdx) => (
        <div className="kb-row" key={`row-${rowIdx}`}>
          {row.map((key, keyIdx) => {
            if (key.type === 'spacer') {
                return (
                    <div 
                        key={`spacer-${rowIdx}-${keyIdx}`} 
                        className="key-spacer"
                        style={{ "--w": key.w } as React.CSSProperties}
                    />
                );
            }

            const entry = activeLayout[key.code];
            const isActive = activeBaseCode === key.code;
            
            // Check mapping state
            const mapping = layer.keyCode.find(k => k.fromS === key.code);
            let modClass = "";
            let labelTop = "";
            let labelBottom = "";
            let labelSingle = entry?.LEGEND ?? "";
            let isDouble = false;

            if (mapping) {
                // Ignore self-mappings for visual "modified" state
                const hasPress = !!mapping.toOnPressS && mapping.toOnPressS !== key.code;
                const hasHold = !!mapping.toOnHoldS;

                if (hasPress && hasHold) {
                    modClass = "mod-both";
                    isDouble = true;
                    labelTop = activeLayout[mapping.toOnPressS]?.LEGEND || "";
                    labelBottom = activeLayout[mapping.toOnHoldS]?.LEGEND || "";
                } else if (hasPress) {
                    modClass = "mod-press";
                    labelSingle = activeLayout[mapping.toOnPressS]?.LEGEND || "";
                } else if (hasHold) {
                    modClass = "mod-hold";
                    isDouble = true;
                    labelTop = entry?.LEGEND || "";
                    labelBottom = activeLayout[mapping.toOnHoldS]?.LEGEND || "";
                }
            }

            return (
              <button
                key={`${key.code}-${keyIdx}`}
                className={`key ${isActive ? "active" : ""} ${modClass}`}
                style={{ "--w": key.w } as React.CSSProperties}
                onClick={(e) => { e.stopPropagation(); handleClick(key.code); }}
              >
                <div className={`key-content ${isDouble ? "double" : "single"}`}>
                    {isDouble ? (
                        <>
                            <div className="top-line">{labelTop}</div>
                            <div className="bottom-line">{labelBottom}</div>
                        </>
                    ) : (
                        <div className="single-line">{labelSingle}</div>
                    )}
                </div>
              </button>
            );
          })}
        </div>
      ))}
    </div>
  );
};
