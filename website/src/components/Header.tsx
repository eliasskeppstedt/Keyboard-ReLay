
import React from 'react';

export type Page = 'home' | 'remap' | 'download' | 'about';

interface HeaderProps {
    activePage: Page;
    onNavigate: (page: Page) => void;
}

export const Header: React.FC<HeaderProps> = ({ activePage, onNavigate }) => {
    const linkProps = (page: Page) => ({
        href: "#",
        className: `nav-link ${activePage === page ? 'active' : ''}`,
        onClick: (e: React.MouseEvent) => {
            e.preventDefault();
            onNavigate(page);
        }
    });

    return (
        <header className="site-header">
            <div className="header-inner">
                <nav className="site-nav">
                    <a {...linkProps('home')}>Home</a>
                    <a {...linkProps('remap')}>Remap Tool</a>
                    <a {...linkProps('download')}>Download App</a>
                    <a {...linkProps('about')}>About</a>
                </nav>
            </div>
        </header>
    );
};
