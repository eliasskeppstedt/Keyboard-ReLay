import React from 'react';

interface LegendPanelProps {
    onOpenSettings: () => void;
    onOpenJson: () => void;
    onUpload: (file: File) => void;
    onDownload: () => void;
}

export const LegendPanel: React.FC<LegendPanelProps> = ({
    onOpenSettings,
    onOpenJson, onUpload, onDownload
}) => {
    const fileInputRef = React.useRef<HTMLInputElement>(null);

    return (
        <div className="legend-box full-width-legend">
            <div className="legend-columns">
                
                <div className="legend-column legend-column-legend">
                    <h2 className="legend-title">Key color code</h2>
                    <div className="legend-row">
                        <span className="legend-color mod-none"></span><span className="legend-text">Key not modified</span>
                    </div>
                    <div className="legend-row">
                        <span className="legend-color mod-press"></span><span className="legend-text">Modified on press</span>
                    </div>
                    <div className="legend-row">
                        <span className="legend-color mod-hold"></span><span className="legend-text">Modified on hold</span>
                    </div>
                    <div className="legend-row">
                        <span className="legend-color mod-both"></span><span className="legend-text">Modified on press and hold</span>
                    </div>
                </div>

                <div className="legend-divider"></div>

                <div className="legend-column legend-column-settings">
                    <h2 className="legend-title">Layout settings</h2>
                    <p className="legend-subtext" style={{ opacity: 0.6, fontSize: '14px', marginBottom: '16px' }}>
                        Configure language, physical layout, and OS compatibility.
                    </p>
                    <button className="pill-btn" onClick={onOpenSettings}>Open Settings</button>
                </div>

                <div className="legend-divider"></div>

                <div className="legend-column legend-column-remap">
                    <h2 className="legend-title">Remap file</h2>
                    <p className="legend-subtext" style={{ opacity: 0.6, fontSize: '14px', marginBottom: '16px' }}>
                        Upload or download your key remap as JSON. <span id="view-remap-link" className="link-text" onClick={onOpenJson}>(view)</span>
                    </p>
                    <div className="remap-buttons">
                        <button className="pill-btn" onClick={() => fileInputRef.current?.click()}>Upload remap</button>
                        <button className="pill-btn" onClick={onDownload}>Download remap</button>
                    </div>
                    <input 
                        type="file" 
                        ref={fileInputRef} 
                        style={{display:'none'}} 
                        accept="application/json"
                        onChange={(e) => {
                            if (e.target.files?.[0]) onUpload(e.target.files[0]);
                        }}
                    />
                </div>
            </div>
        </div>
    );
};