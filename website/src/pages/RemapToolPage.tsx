import React, { useState, useMemo, useEffect, useRef } from 'react';
import { Keyboard } from '../components/Keyboard';
import { KeyInfoPanel } from '../components/KeyInfoPanel';
import { ColorLegend } from '../components/ColorLegend';
import { JsonEditorPopup } from '../components/JsonEditorPopup';
import { SettingsPanel } from '../components/SettingsPanel';
import { AlertModal } from '../components/AlertModal';
import { getActiveLayout } from '../layout/codes';
import { buildExportBundles } from '../utils/mappingUtils';
import { KeymapData, Language, LayoutKind, OSKind } from '../types';

const initialKeymapData: KeymapData = {
    layerEntries: 1,
    uniqueKeyEntries: 0,
    layers: [{ layerName: "Main", layerNr: 0, keyCodeEntries: 0, keyCode: [] }]
};

export const RemapToolPage: React.FC = () => {
    // The "committed" settings
    const [savedSettings, setSavedSettings] = useState<{ language: Language, layout: LayoutKind, os: OSKind }>({
        language: 'SE',
        layout: 'ISO',
        os: 'MACOS'
    });

    // The "live" settings (draft)
    const [settings, setSettings] = useState(savedSettings);

    const [activeBaseCode, setActiveBaseCode] = useState<string | null>(null);
    const [isBaseKeyActive, setIsBaseKeyActive] = useState(false);
    const [keymapData, setKeymapData] = useState<KeymapData>(initialKeymapData);
    
    const [showJson, setShowJson] = useState(false);
    const [alertConfig, setAlertConfig] = useState<{isOpen: boolean, title: string, message: string}>({
        isOpen: false,
        title: '',
        message: ''
    });

    const fileInputRef = useRef<HTMLInputElement>(null);

    // Derived from current (draft) settings so user sees changes live
    const activeLayout = useMemo(() => 
        getActiveLayout(settings.language, settings.layout, settings.os), 
    [settings]);

    // Handle settings logic
    const handleSettingUpdate = (key: keyof typeof settings, value: any) => {
        setSettings(prev => ({ ...prev, [key]: value }));
    };

    const handleSaveSettings = () => {
        setSavedSettings(settings);
    };

    const showAlert = (title: string, message: string) => {
        setAlertConfig({ isOpen: true, title, message });
    };

    const closeAlert = () => {
        setAlertConfig(prev => ({ ...prev, isOpen: false }));
    };

    // Check dirty state derived
    const hasUnsavedSettings = useMemo(() => {
        return settings.language !== savedSettings.language ||
               settings.layout !== savedSettings.layout ||
               settings.os !== savedSettings.os;
    }, [settings, savedSettings]);

    const handleKeySelect = (code: string) => {
        if (hasUnsavedSettings) {
            showAlert("Unsaved Changes", "Please save or revert your settings before selecting keys.");
            return;
        }
        setActiveBaseCode(code);
        setIsBaseKeyActive(true);
    };

    // Global click to deselect (only if not clicking specific UI areas)
    useEffect(() => {
        const handleClick = (e: MouseEvent) => {
            const target = e.target as HTMLElement;
            if (!target.closest('.kb') && 
                !target.closest('.key-info') && 
                !target.closest('.key-picker') && 
                !target.closest('.bottom-controls') &&
                !target.closest('.remap-popup')) {
                setActiveBaseCode(null);
                setIsBaseKeyActive(false);
            }
        };
        document.addEventListener('click', handleClick);
        return () => document.removeEventListener('click', handleClick);
    }, []);

    // Keydown handlers
    useEffect(() => {
        const handleKey = (e: KeyboardEvent) => {
            if (e.key === 'Escape') {
                if (alertConfig.isOpen) {
                    closeAlert();
                    return;
                }
                if (showJson) {
                    setShowJson(false);
                    return;
                }
                setActiveBaseCode(null);
                setIsBaseKeyActive(false);
                return;
            }
            
            // Check if user is typing in an input (like the search box)
            const target = e.target as HTMLElement;
            if (target.tagName === 'INPUT' || target.tagName === 'TEXTAREA') {
                return;
            }
            
            // Key mapping shortcut
            if (!alertConfig.isOpen && isBaseKeyActive && activeBaseCode && activeLayout[e.code]) {
                setKeymapData(prev => {
                    // Deep copy layer to ensure immutable update
                    const newLayers = [...prev.layers];
                    const activeLayerIdx = 0; 
                    const layer = { ...newLayers[activeLayerIdx], keyCode: [...newLayers[activeLayerIdx].keyCode] };
                    newLayers[activeLayerIdx] = layer;

                    const entryIndex = layer.keyCode.findIndex(k => k.fromS === activeBaseCode);
                    let entry = entryIndex >= 0 ? { ...layer.keyCode[entryIndex] } : undefined;

                    if (e.code === activeBaseCode) {
                        // Case 1: User pressed the base key itself.
                        // We want to remove the press mapping (reset to default).
                        if (entry) {
                            entry.toOnPressS = "";
                            entry.toOnPress = 999999;
                            
                            // If hold is also empty, remove the entry entirely
                            if (!entry.toOnHoldS) {
                                layer.keyCode.splice(entryIndex, 1);
                            } else {
                                layer.keyCode[entryIndex] = entry;
                            }
                        }
                        // If no entry exists, do nothing (default is already base key)
                    } else {
                        // Case 2: User pressed a different key (normal remapping)
                        if (!entry) {
                             entry = {
                                from: activeLayout[activeBaseCode!]?.codeOS ?? 999999,
                                fromS: activeBaseCode!,
                                toOnPress: 999999, toOnHold: 999999, toOnPressS: "", toOnHoldS: ""
                            };
                            layer.keyCode.push(entry);
                        } else {
                            layer.keyCode[entryIndex] = entry;
                        }

                        entry.toOnPressS = e.code;
                        entry.toOnPress = activeLayout[e.code]?.codeOS ?? 999999;
                    }

                    return { ...prev, layers: newLayers };
                });
                e.preventDefault();
                e.stopPropagation();
            }
        };

        window.addEventListener('keydown', handleKey);
        return () => window.removeEventListener('keydown', handleKey);
    }, [isBaseKeyActive, activeBaseCode, activeLayout, alertConfig.isOpen, showJson]);

    const handleUpload = (file: File) => {
        const reader = new FileReader();
        reader.onload = () => {
            try {
                const json = JSON.parse(reader.result as string);
                // Try to restore settings if they exist in the file
                if (json.settings) {
                    setSettings(prev => ({ ...prev, ...json.settings }));
                    setSavedSettings(prev => ({ ...prev, ...json.settings }));
                }

                if (json && json.layers) {
                     setKeymapData(prev => ({
                         ...prev,
                         layers: [json.layers[0], ...prev.layers.slice(1)]
                     }));
                }
            } catch (e) {
                showAlert("Error", "Invalid JSON file.");
            }
        };
        reader.readAsText(file);
    };

    const exportData = useMemo(() => buildExportBundles(activeLayout, keymapData, settings), [activeLayout, keymapData, settings]);

    const handleDownload = () => {
        const blob = new Blob([JSON.stringify(exportData.merged, null, 4)], { type: "application/json" });
        const url = URL.createObjectURL(blob);
        const a = document.createElement("a");
        a.href = url;
        a.download = "my_remaps.json";
        a.click();
        URL.revokeObjectURL(url);
    };

    return (
        <div className="panel-bg-box">
            {/* Top Section: Keyboard & KeyInfo */}
            <div className="layout-top">
                <div className="layout-left">
                    <div id="kbWrap" className="kb-wrap">
                        <ColorLegend />
                        <Keyboard 
                            activeLayout={activeLayout}
                            keymapData={keymapData}
                            activeBaseCode={activeBaseCode}
                            onKeySelect={handleKeySelect}
                            setKeymapData={setKeymapData}
                            layoutKind={settings.layout}
                        />
                    </div>
                </div>
                
                <KeyInfoPanel 
                    activeLayout={activeLayout}
                    activeBaseCode={activeBaseCode}
                    keymapData={keymapData}
                    setKeymapData={setKeymapData}
                />
            </div>

            {/* Bottom Section: Settings & Remap Actions */}
            <div className="layout-bottom bottom-controls">
                <div className="bottom-group" style={{ flex: 1 }}>
                    <h3 className="bottom-heading">Settings</h3>
                    <SettingsPanel 
                        settings={settings}
                        savedSettings={savedSettings}
                        onUpdate={handleSettingUpdate}
                        onSave={handleSaveSettings}
                    />
                </div>
                
                <div className="bottom-separator"></div>

                <div className="bottom-group">
                    <h3 className="bottom-heading">Remap File</h3>
                    <div className="remap-actions">
                        <button className="action-btn" onClick={() => fileInputRef.current?.click()}>
                            Upload Remap
                        </button>
                        <button className="action-btn" onClick={handleDownload}>
                            Download Remap
                        </button>
                        <span className="link-text" onClick={() => setShowJson(true)}>(view json)</span>
                    </div>
                </div>

                <input 
                    type="file" 
                    ref={fileInputRef} 
                    style={{display:'none'}} 
                    accept="application/json"
                    onChange={(e) => {
                        if (e.target.files?.[0]) handleUpload(e.target.files[0]);
                    }}
                />
            </div>

            {showJson && <JsonEditorPopup onClose={() => setShowJson(false)} data={exportData} />}
            
            <AlertModal 
                isOpen={alertConfig.isOpen}
                title={alertConfig.title}
                message={alertConfig.message}
                onClose={closeAlert}
            />
        </div>
    );
};