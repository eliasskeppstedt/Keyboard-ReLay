
import React, { useState, useMemo, useEffect, useRef } from 'react';
import { ActiveLayout } from '../types';

interface KeyPickerProps {
    activeLayout: ActiveLayout;
    isOpen: boolean;
    onClose: () => void;
    onSelect: (code: string) => void;
    mode?: "press" | "hold" | null;
}

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

export const KeyPicker: React.FC<KeyPickerProps> = ({ activeLayout, isOpen, onClose, onSelect, mode }) => {
    const [search, setSearch] = useState("");
    const inputRef = useRef<HTMLInputElement>(null);

    useEffect(() => {
        if (isOpen && inputRef.current) {
            inputRef.current.focus();
        }
        if (!isOpen) setSearch("");
    }, [isOpen]);

    const classifiedKeys = useMemo(() => {
        const keys = Object.keys(activeLayout).map(code => {
            let category = CATEGORY.other;
            if (/^Key[A-Z]$/.test(code)) category = CATEGORY.letters;
            else if (/^Digit[0-9]$/.test(code)) category = CATEGORY.numbers;
            else if (/^F[0-9]{1,2}$/.test(code)) category = CATEGORY.function;
            else if (["ShiftLeft", "ShiftRight", "ControlLeft", "ControlRight", "AltLeft", "AltRight", "MetaLeft", "MetaRight", "CapsLock", "Tab", "Space"].includes(code)) category = CATEGORY.modifiers;
            else if (["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight"].includes(code)) category = CATEGORY.arrows;

            return {
                code,
                label: activeLayout[code].LEGEND,
                description: activeLayout[code].DESCRIPTION,
                category
            };
        });
        
        return keys.sort((a, b) => {
             if (a.category === b.category) return (a.label || "").localeCompare(b.label || "");
             return CATEGORY_ORDER.indexOf(a.category) - CATEGORY_ORDER.indexOf(b.category);
        });
    }, [activeLayout]);

    const filteredKeys = useMemo(() => {
        const q = search.toLowerCase();
        return classifiedKeys.filter(k => {
            // Filter out None if mode is hold
            if (mode === 'hold' && k.code === 'None') return false;

            return k.label.toLowerCase().includes(q) || 
            k.code.toLowerCase().includes(q) || 
            k.description.toLowerCase().includes(q)
        });
    }, [classifiedKeys, search, mode]);

    const groupedKeys = useMemo(() => {
        const groups: Record<string, typeof filteredKeys> = {};
        filteredKeys.forEach(k => {
            if (!groups[k.category]) groups[k.category] = [];
            groups[k.category].push(k);
        });
        return groups;
    }, [filteredKeys]);

    if (!isOpen) return null;

    return (
        <div className="key-picker open" onClick={(e) => e.stopPropagation()}>
            <input 
                ref={inputRef}
                className="key-picker-search" 
                placeholder="search..." 
                value={search}
                onChange={e => setSearch(e.target.value)}
            />
            <div className="key-picker-list">
                {CATEGORY_ORDER.map(cat => {
                    if (!groupedKeys[cat]) return null;
                    return (
                        <div key={cat} style={{display:'flex', flexDirection:'column'}}>
                            <div className="key-picker-category">{cat}</div>
                            <div className="key-picker-grid">
                                {groupedKeys[cat].map(k => (
                                    <div key={k.code} className="key-picker-tile" onClick={() => { onSelect(k.code); onClose(); }}>
                                        <div className="kp-legend">{k.label}</div>
                                        <div className="kp-desc">{k.description.split(';')[0]}</div>
                                    </div>
                                ))}
                            </div>
                        </div>
                    );
                })}
            </div>
        </div>
    );
};
