import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './src/App';
import './src/styles.css';

// Initialize the HTML document structure via JavaScript
const initializeDocument = () => {
    // Set document title
    document.title = "Keyboard Relay";

    // Add viewport meta tag if missing
    if (!document.querySelector('meta[name="viewport"]')) {
        const meta = document.createElement('meta');
        meta.name = "viewport";
        meta.content = "width=device-width, initial-scale=1.0";
        document.head.appendChild(meta);
    }

    // Ensure root element exists
    let root = document.getElementById('root');
    if (!root) {
        root = document.createElement('div');
        root.id = 'root';
        document.body.appendChild(root);
    }
    return root;
};

const rootElement = initializeDocument();
const root = ReactDOM.createRoot(rootElement as HTMLElement);
root.render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
);