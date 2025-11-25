
import React from 'react';

export const HomePage: React.FC = () => {
    return (
        <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', color: '#EDEAE3', marginTop: '60px' }}>
            <h1 style={{ fontSize: '3rem', marginBottom: '20px' }}>Keyboard Relay</h1>
            <p style={{ fontSize: '1.2rem', opacity: 0.8, maxWidth: '600px', textAlign: 'center', lineHeight: '1.6' }}>
                Costumization for keyboards! 
            </p>
            <p style={{ fontSize: '1.2rem', opacity: 0.8, maxWidth: '600px', textAlign: 'center', lineHeight: '1.6' }}>
                Umm, about for more info :D
            </p>
        </div>
    );
};
