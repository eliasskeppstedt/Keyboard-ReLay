// ======================================================
// JSON EDITOR MODULE (with Base / Remaps tabs)
// ======================================================

const JsonEditor = (() => {

    // -------------------------
    // DOM references
    // -------------------------
    const panel = document.getElementById("remapPopup");
    const codeLinesEl = document.getElementById("codeEditorLines");

    const btnOpen = document.getElementById("view-remap-link");
    const btnClose = document.querySelector("#remapPopup .popup-close");
    const btnUpload = document.getElementById("json-upload-input");
    const btnUploadTrigger = document.getElementById("btn-upload-json");
    const btnDownload = document.getElementById("btn-download-json");

    // new tab buttons
    const tabBase = document.getElementById("json-tab-base");
    const tabRemap = document.getElementById("json-tab-remap");
    const tabMerged = document.getElementById("json-tab-merged");

    // current data for tabs
    let currentBaseJson = null;
    let currentRemapJson = null;
    let currentMergedJson = null;
    let activeTab = "base"; // "base" | "remap" | "single"

    // -------------------------
    // Syntax highlighting
    // -------------------------
    function tokenize(text) {
        text = text.replace(/"([^"\\]*(?:\\.[^"\\]*)*)"/g,
            (m, inner) => `<span class="tok-string">"${inner}"</span>`
        );

        text = text.replace(
            /<span class=\"tok-string\">"([^"]+)"<\/span>(?=\s*:)/g,
            (m, key) => `<span class="tok-key">"${key}"</span>`
        );

        text = text.replace(/\b-?\d+(\.\d+)?\b/g,
            m => `<span class="tok-number">${m}</span>`
        );

        text = text.replace(/\b(true|false|null)\b/g,
            m => `<span class="tok-primitive">${m}</span>`
        );

        return text;
    }

    // -------------------------
    // Indentation guides
    // -------------------------
    function addIndentGuides(textEl, level) {
        for (let i = 0; i < level; i++) {
            const guide = document.createElement("span");
            guide.className = "indent-col";
            guide.style.left = `${i * 32}px`;
            textEl.appendChild(guide);
        }
    }

    // -------------------------
    // Render JSON (single object)
    // -------------------------
    function render(data) {
        if (!data) {
            codeLinesEl.innerHTML = "";
            return;
        }

        const jsonStr = JSON.stringify(data, null, 4);
        const lines = jsonStr.split("\n");

        codeLinesEl.innerHTML = "";

        lines.forEach((line, i) => {
            const row = document.createElement("div");
            row.className = "code-line";

            const num = document.createElement("div");
            num.className = "line-number";
            num.textContent = i + 1;

            const textEl = document.createElement("div");
            textEl.className = "code-text";

            const expanded = line.replace(/\t/g, "    ");
            const indentMatch = /^(\s*)/.exec(expanded);
            const indentLevel = (indentMatch ? indentMatch[1].length : 0) / 4;

            addIndentGuides(textEl, indentLevel);

            const content = document.createElement("span");
            content.innerHTML = tokenize(expanded.trimStart());
            content.style.marginLeft = `${indentLevel * 32}px`;

            textEl.appendChild(content);
            row.appendChild(num);
            row.appendChild(textEl);
            codeLinesEl.appendChild(row);
        });
    }

    // -------------------------
    // Tab handling
    // -------------------------
    function updateTabUI() {
        if (!tabBase || !tabRemap) return;
        
        tabBase.classList.toggle("code-tab-active", activeTab === "base");
        tabRemap.classList.toggle("code-tab-active", activeTab === "remap");
        tabMerged.classList.toggle("code-tab-active", activeTab === "merged");

        // hide tab bar when we only have a single JSON
        const bar = tabBase.parentElement;
        if (!currentRemapJson) {
            bar.classList.add("code-tabs--hidden");
        } else {
            bar.classList.remove("code-tabs--hidden");
        }
    }

    function showTab(tab) {
        activeTab = tab;
        updateTabUI();

        if (tab === "base") {
            render(currentBaseJson);
        } else if (tab === "remap") {
            render(currentRemapJson);
        } else {
            render(currentBaseJson);
        }
    }

    if (tabMerged) {
        tabMerged.addEventListener("click", () => {
            if (!currentMergedJson) return;
            activeTab = "merged";
            updateTabUI();
            render(currentMergedJson);
        });
    }

    // -------------------------
    // Panel open/close
    // -------------------------
    function open(dataOrBundle) {
        // dataOrBundle can be:
        //   1) plain layout JSON  (old mode)
        //   2) { base, remap, merged, filename } (new mode)
        if (dataOrBundle && dataOrBundle.base && dataOrBundle.remap) {
            currentBaseJson = dataOrBundle.base;
            currentRemapJson = dataOrBundle.remap;
            currentMergedJson = dataOrBundle.merged || null;
            activeTab = "base";
        } else {
            currentBaseJson = dataOrBundle || null;
            currentRemapJson = null;
            currentMergedJson = null;
            activeTab = "single";
        }

        showTab(activeTab);
        panel.classList.remove("hidden");
    }

    function close() {
        panel.classList.add("hidden");
    }

    // -------------------------
    // Download JSON
    // -------------------------
    function download(data, filename = "my_remaps.json") {
        const blob = new Blob([JSON.stringify(data, null, 4)], { type: "application/json" });
        const url = URL.createObjectURL(blob);
        const a = document.createElement("a");
        a.href = url;
        a.download = filename;
        a.click();
        URL.revokeObjectURL(url);
    }

    // -------------------------
    // Upload JSON
    // -------------------------
    function uploadFile(file) {
        const reader = new FileReader();
        reader.onload = () => {
            try {
                const json = JSON.parse(reader.result);
                if (JsonEditor.onImport) JsonEditor.onImport(json);
                // IMPORTANT: do NOT auto-open the editor on upload
                // open(json);  <-- we removed this per your request
            } catch (err) {
                alert("Invalid JSON file");
            }
        };
        reader.readAsText(file);
    }

    // -------------------------
    // Event wiring
    // -------------------------

    // trigger hidden <input type="file">
    if (btnUploadTrigger) {
        btnUploadTrigger.addEventListener("click", () => {
            btnUpload.click();
        });
    }

    // View JSON
    if (btnOpen) {
        btnOpen.addEventListener("click", () => {
            if (JsonEditor.onExport) {
                open(JsonEditor.onExport());
            }
        });
    }

    // close popup
    if (btnClose) {
        btnClose.addEventListener("click", close);
    }

    // download JSON (merged layout)
    if (btnDownload) {
        btnDownload.addEventListener("click", () => {
            if (!JsonEditor.onExport) return;
            const bundle = JsonEditor.onExport();

            if (bundle && bundle.merged) {
                download(bundle.merged, bundle.filename || "my_remaps.json");
            } else {
                download(bundle, "my_remaps.json");
            }
        });
    }

    // upload file
    if (btnUpload) {
        btnUpload.addEventListener("change", e => {
            const file = e.target.files[0];
            if (file) uploadFile(file);
        });
    }

    // tab buttons
    if (tabBase) {
        tabBase.addEventListener("click", () => {
            if (!currentBaseJson) return;
            showTab("base");
        });
    }

    if (tabRemap) {
        tabRemap.addEventListener("click", () => {
            if (!currentRemapJson) return;
            showTab("remap");
        });
    }

    // -------------------------
    // Public API
    // -------------------------
    return {
        open,
        close,
        render,
        download,
        onExport: null,
        onImport: null,
    };
})();