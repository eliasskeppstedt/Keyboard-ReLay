import React from 'react';

interface AlertModalProps {
    isOpen: boolean;
    onClose: () => void;
    title?: string;
    message: string;
}

export const AlertModal: React.FC<AlertModalProps> = ({ isOpen, onClose, title, message }) => {
    if (!isOpen) return null;

    return (
        <div className="remap-popup" onClick={onClose}>
            <div className="remap-popup-inner alert-box" onClick={e => e.stopPropagation()}>
                <div className="alert-content">
                    {title && <h2 className="alert-title">{title}</h2>}
                    <p className="alert-message">{message}</p>
                    <button className="pill-btn alert-btn" onClick={onClose}>OK</button>
                </div>
            </div>
        </div>
    );
};