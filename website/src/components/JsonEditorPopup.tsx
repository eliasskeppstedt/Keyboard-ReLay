
import React, { useState } from 'react';

interface JsonEditorPopupProps {
    onClose: () => void;
    data: { base: any, remap: any, merged: any, settings: any };
}

export const JsonEditorPopup: React.FC<JsonEditorPopupProps> = ({ onClose, data }) => {
    const [tab, setTab] = useState<'base'|'remap'|'merged'|'settings'>('base');

    const currentJson = JSON.stringify(data[tab], null, 4);
    const lines = currentJson.split('\n');

    return (
        <div className="remap-popup">
            <div className="remap-popup-inner editor-popup">
                <button className="popup-close" onClick={onClose}>×</button>
                
                <div className="code-editor">
                    <div className="code-editor-header">
                        <div className="code-tabs">
                            <button className={`code-tab ${tab==='base'?'code-tab-active':''}`} onClick={()=>setTab('base')}>Base layout</button>
                            <button className={`code-tab ${tab==='remap'?'code-tab-active':''}`} onClick={()=>setTab('remap')}>My remaps</button>
                            <button className={`code-tab ${tab==='settings'?'code-tab-active':''}`} onClick={()=>setTab('settings')}>Settings</button>
                            <button className={`code-tab ${tab==='merged'?'code-tab-active':''}`} onClick={()=>setTab('merged')}>Merged output</button>
                        </div>
                    </div>
                    <div className="code-editor-body">
                        <div className="code-lines">
                            {lines.map((line, i) => (
                                <div key={i} className="code-line">
                                    <div className="line-number">{i+1}</div>
                                    <div className="code-text">
                                        {/* Simple rendering for now */}
                                        <pre style={{margin:0}}>{line}</pre>
                                    </div>
                                </div>
                            ))}
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
};
