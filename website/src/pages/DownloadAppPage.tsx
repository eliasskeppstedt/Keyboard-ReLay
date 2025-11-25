
import React from 'react';
import { Page } from '../components/Header';

interface DownloadAppPageProps {
    onNavigate: (page: Page) => void;
}

export const DownloadAppPage: React.FC<DownloadAppPageProps> = ({ onNavigate }) => {
    return (
        <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', color: '#EDEAE3', marginTop: '60px' }}>
            <h1 style={{ fontSize: '2.5rem', marginBottom: '20px' }}>Download App</h1>
            <div className="panel-bg-box" style={{ padding: '40px', alignItems: 'center', textAlign: 'center' }}>
                <p style={{ fontSize: '1.1rem', marginBottom: '30px' }}>
                    Here you will eventually be able to download the desktop program. The program is also not ready yet, but you can check 
                    it out on my github page. (<span 
                        className="link-text" 
                        style={{ fontSize: 'inherit', marginLeft: 0 }} 
                        onClick={() => onNavigate('about')}
                    >about</span>)
                </p>
                <div style={{ display: 'flex', gap: '20px' }}>
                    <button className="pill-btn" style={{ padding: '12px 24px', fontSize: '1rem', opacity: 0.5, cursor: 'not-allowed' }}>macOS</button>
                    <button className="pill-btn" style={{ padding: '12px 24px', fontSize: '1rem', opacity: 0.5, cursor: 'not-allowed' }}>Windows</button>
                    <button className="pill-btn" style={{ padding: '12px 24px', fontSize: '1rem', opacity: 0.5, cursor: 'not-allowed' }}>Linux</button>
                </div>
            </div>
        </div>
    );
};
