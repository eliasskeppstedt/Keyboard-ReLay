
export type WebKeyCode = string;

export interface BaseKeyEntry {
    LEGEND: string;
    DESCRIPTION: string;
    SHOWDESCRIPTION: boolean;
    MAC?: number;
    codeOS?: number;
}

export type ActiveLayout = Record<string, BaseKeyEntry>;

export interface ScanCodeANSI { [key: string]: string | BaseKeyEntry; }
export interface ScanCodeISO { [key: string]: BaseKeyEntry; }
export interface ScanCodeJIS { [key: string]: BaseKeyEntry; }
export interface OverlayMap { [key: string]: Partial<BaseKeyEntry> | string; }

export type Language = "SE" | "US" | "KR";
export type LayoutKind = "ISO" | "ANSI" | "JIS";
export type OSKind = "MACOS" | "WINDOWS" | "LINUX";

export interface KeyMapEntry {
    from: number;
    fromS: string;
    toOnPress: number;
    toOnHold: number;
    toOnPressS: string;
    toOnHoldS: string;
}

export interface Layer {
    layerName: string;
    layerNr: number;
    keyCodeEntries: number;
    keyCode: KeyMapEntry[];
}

export interface KeymapData {
    layerEntries: number;
    uniqueKeyEntries?: number;
    layers: Layer[];
}

export interface KeyGeom {
    code: string;
    w: number;
    type?: 'key' | 'spacer';
}
