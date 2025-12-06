#!/bin/bash
# Build script for SideQuester website

echo "🚀 Building SideQuester Website..."
echo "=================================="

# Check if wasm-pack is installed
if ! command -v wasm-pack &> /dev/null; then
    echo "❌ wasm-pack not found!"
    echo "Install it with: curl https://rustwasm.github.io/wasm-pack/installer/init.sh -sSf | sh"
    exit 1
fi

# Build Rust to WASM
echo "📦 Compiling Rust to WebAssembly..."
wasm-pack build --target web --out-dir www/pkg

if [ $? -eq 0 ]; then
    echo "✅ WASM build successful!"
else
    echo "❌ WASM build failed!"
    exit 1
fi

# Check if files exist
if [ -f "www/pkg/sidequester_wasm.js" ] && [ -f "www/pkg/sidequester_wasm_bg.wasm" ]; then
    echo "✅ WASM files generated successfully"
    echo ""
    echo "📁 Output files:"
    ls -lh www/pkg/
    echo ""
    echo "🎉 Build complete!"
    echo ""
    echo "To test locally:"
    echo "  cd www"
    echo "  python3 -m http.server 8080"
    echo "  Then open: http://localhost:8080"
else
    echo "❌ WASM files not found!"
    exit 1
fi
