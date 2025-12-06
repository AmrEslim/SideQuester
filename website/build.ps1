# Build script for SideQuester website (Windows PowerShell)

Write-Host "Building SideQuester Website..." -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

# Check if wasm-pack is installed
if (-not (Get-Command wasm-pack -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: wasm-pack not found!" -ForegroundColor Red
    Write-Host "Install it from: https://rustwasm.github.io/wasm-pack/installer/" -ForegroundColor Yellow
    Write-Host "Or use: cargo install wasm-pack" -ForegroundColor Yellow
    exit 1
}

# Build Rust to WASM
Write-Host "Compiling Rust to WebAssembly..." -ForegroundColor Yellow
wasm-pack build --target web --out-dir www/pkg

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: WASM build failed!" -ForegroundColor Red
    exit 1
}

Write-Host "SUCCESS: WASM build complete!" -ForegroundColor Green

# Check if files exist
$wasmJs = "www/pkg/sidequester_wasm.js"
$wasmBg = "www/pkg/sidequester_wasm_bg.wasm"

if ((Test-Path $wasmJs) -and (Test-Path $wasmBg)) {
    Write-Host "WASM files generated successfully" -ForegroundColor Green
    Write-Host ""
    Write-Host "Output files:" -ForegroundColor Cyan
    Get-ChildItem www/pkg/ | Format-Table Name, Length
    Write-Host ""
    Write-Host "Build complete!" -ForegroundColor Green
    Write-Host ""
    Write-Host "To test locally:" -ForegroundColor Yellow
    Write-Host "  cd www" -ForegroundColor White
    Write-Host "  python -m http.server 8080" -ForegroundColor White
    Write-Host "  Then open: http://localhost:8080" -ForegroundColor White
}
else {
    Write-Host "ERROR: WASM files not found!" -ForegroundColor Red
    exit 1
}
