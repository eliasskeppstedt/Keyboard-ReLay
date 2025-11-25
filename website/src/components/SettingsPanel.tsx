
import React, { useState, useEffect } from 'react';
import { Language, LayoutKind, OSKind } from '../types';

interface SettingsState {
    language: Language;
    layout: LayoutKind;
    os: OSKind;
}

interface SettingsPanelProps {
    settings: SettingsState;
    savedSettings: SettingsState;
    onUpdate: (key: keyof SettingsState, value: any) => void;
    onSave: () => void;
}

export const SettingsPanel: React.FC<SettingsPanelProps> = ({
    settings, savedSettings, onUpdate, onSave
}) => {
    const [openDropdown, setOpenDropdown] = useState<string | null>(null);

    // Close dropdowns when clicking outside
    useEffect(() => {
        const close = () => setOpenDropdown(null);
        document.addEventListener('click', close);
        return () => document.removeEventListener('click', close);
    }, []);

    const toggle = (name: string, e: React.MouseEvent) => {
        e.stopPropagation();
        setOpenDropdown(openDropdown === name ? null : name);
    };

    const isDirty = (key: keyof SettingsState) => settings[key] !== savedSettings[key];
    const hasAnyDirty = isDirty('language') || isDirty('layout') || isDirty('os');

    const renderDropdown = (
        key: keyof SettingsState, 
        options: string[], 
        label: string
    ) => {
        const dirty = isDirty(key);
        return (
            <div className="bottom-setting-block">
                <div className="setting-label-row">
                    <span className="setting-label-small">{label}</span>
                    {dirty && (
                        <span 
                            className="revert-text-small" 
                            onClick={(e) => { e.stopPropagation(); onUpdate(key, savedSettings[key]); }}
                        >
                            revert
                        </span>
                    )}
                </div>
                <div className={`setting-dropdown bottom-up ${openDropdown === key ? 'open' : ''}`}>
                    <button type="button" className="dropdown-pill" onClick={(e) => toggle(key, e)}>
                        <span className="dropdown-pill-label">{settings[key]}</span>
                        <span className="dropdown-pill-caret">▴</span>
                    </button>
                    <div className="dropdown-menu">
                        {options.map(opt => (
                            <button 
                                key={opt} 
                                className="dropdown-item" 
                                onClick={() => onUpdate(key, opt)}
                            >
                                {opt}
                            </button>
                        ))}
                    </div>
                </div>
            </div>
        );
    };

    return (
        <div className="settings-bar">
            <div className="settings-group">
                {renderDropdown('language', ['SE', 'US', 'KR'], 'Language')}
                {renderDropdown('layout', ['ISO', 'ANSI', 'JIS'], 'Layout')}
                {renderDropdown('os', ['MACOS'], 'OS')}
            </div>

            <button 
                className={`pill-btn save-btn ${hasAnyDirty ? 'btn-unsaved' : 'btn-saved'}`} 
                onClick={hasAnyDirty ? onSave : undefined}
            >
                {hasAnyDirty ? 'Save Changes' : 'Settings Saved'}
            </button>
        </div>
    );
};
