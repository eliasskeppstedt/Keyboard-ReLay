// ======================================================
// JSON EDITOR MODULE (CLEAN & SELF-CONTAINED)
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
    // Render JSON
    // -------------------------
    function render(data) {
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
    // Panel open/close
    // -------------------------
    function open(data) {
        render(data);
        panel.classList.remove("hidden");
    }

    function close() {
        panel.classList.add("hidden");
    }
  
    // -------------------------
    // Download JSON
    // -------------------------
    function download(data) {
        const blob = new Blob([JSON.stringify(data, null, 4)], { type: "application/json" });
        const url = URL.createObjectURL(blob);
        const a = document.createElement("a");
        a.href = url;
        a.download = "layout.json";
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
                open(json);
            } catch (err) {
                alert("Invalid JSON file");
            }
        };
        reader.readAsText(file);
    }
  
    // -------------------------
    // Event wiring

    if (btnUploadTrigger) {
        btnUploadTrigger.addEventListener("click", () => {
            btnUpload.click();
        });
    }

    // Original event wiring
    // -------------------------
    if (btnOpen) {
        btnOpen.addEventListener("click", () => {
            if (JsonEditor.onExport) {
                open(JsonEditor.onExport());
            }
        });
    }

    if (btnClose) {
        btnClose.addEventListener("click", close);
    }

    if (btnDownload) {
        btnDownload.addEventListener("click", () => {
            if (JsonEditor.onExport) download(JsonEditor.onExport());
        });
    }

    if (btnUpload) {
        btnUpload.addEventListener("change", e => {
            const file = e.target.files[0];
            if (file) uploadFile(file);
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
