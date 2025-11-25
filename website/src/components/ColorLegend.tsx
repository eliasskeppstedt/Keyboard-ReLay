
import React from 'react';

export const ColorLegend: React.FC = () => {
    return (
        <div className="color-legend-strip">
            <div className="legend-item">
                <span className="legend-dot mod-none"></span>
                <span className="legend-label">Default</span>
            </div>
            <div className="legend-item">
                <span className="legend-dot mod-press"></span>
                <span className="legend-label">Press</span>
            </div>
            <div className="legend-item">
                <span className="legend-dot mod-hold"></span>
                <span className="legend-label">Hold</span>
            </div>
            <div className="legend-item">
                <span className="legend-dot mod-both"></span>
                <span className="legend-label">Both</span>
            </div>
        </div>
    );
};
