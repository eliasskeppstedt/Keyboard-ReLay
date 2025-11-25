
import React, { useState } from 'react';
import { Header, Page } from './components/Header';
import { Footer } from './components/Footer';
import { RemapToolPage } from './pages/RemapToolPage';
import { HomePage } from './pages/HomePage';
import { DownloadAppPage } from './pages/DownloadAppPage';
import { AboutPage } from './pages/AboutPage';

export default function App() {
    const [page, setPage] = useState<Page>('remap');

    return (
        <div className="app-layout">
            <Header activePage={page} onNavigate={setPage} />
            <main className="main-content">
                {page === 'home' && <HomePage />}
                {page === 'remap' && <RemapToolPage />}
                {page === 'download' && <DownloadAppPage onNavigate={setPage} />}
                {page === 'about' && <AboutPage />}
            </main>
            <Footer />
        </div>
    );
}
