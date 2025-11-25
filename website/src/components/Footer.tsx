
import React from 'react';

export const Footer: React.FC = () => {
    return (
        <footer className="site-footer">
            <div className="footer-content">
                <p>Keyboard Relay &copy; {new Date().getFullYear()}</p>
            </div>
        </footer>
    );
};
