# SideQuester Website

This directory contains the source code for the SideQuester project website, built with Rust, WebAssembly, and modern web technologies.

## 🌐 Live Website

**Visit:** [https://amreslim.github.io/SideQuester/](https://amreslim.github.io/SideQuester/)

## 🚀 Features

- **Interactive Compass Demo** - Experience the same algorithms running on hardware
- **Rust + WebAssembly** - High-performance calculations in the browser
- **Responsive Design** - Works on desktop, tablet, and mobile
- **GitHub Pages Hosting** - Free, fast, and reliable
- **Auto-Deploy** - Automatic deployment via GitHub Actions

## 📦 Project Structure

```
website/
├── src/
│   └── lib.rs              # Rust WASM module (compass algorithms)
├── www/
│   ├── index.html          # Main landing page
│   ├── demo.html           # Interactive demo page
│   ├── styles.css          # All styling
│   ├── app.js              # WASM loader & demo logic
│   └── assets/
│       └── animations.js   # Landing page animations
├── Cargo.toml              # Rust dependencies
├── build.sh                # Build script (Linux/Mac)
├── build.ps1               # Build script (Windows)
└── README.md               # This file
```

## 🛠️ Development Setup

### Prerequisites

1. **Rust** (latest stable):
   ```bash
   curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
   ```

2. **wasm-pack**:
   ```bash
   curl https://rustwasm.github.io/wasm-pack/installer/init.sh -sSf | sh
   ```
   
   Or with cargo:
   ```bash
   cargo install wasm-pack
   ```

3. **Python 3** (for local testing)

### Building the Website

**Linux/Mac:**
```bash
cd website
chmod +x build.sh
./build.sh
```

**Windows (PowerShell):**
```powershell
cd website
.\build.ps1
```

**Manual build:**
```bash
cd website
wasm-pack build --target web --out-dir www/pkg
```

### Testing Locally

After building, serve the website locally:

```bash
cd www
python3 -m http.server 8080
```

Then open: http://localhost:8080

## 📤 Deployment

### Automatic Deployment (GitHub Actions)

The website automatically deploys to GitHub Pages when you push to the `main` branch.

**Setup:**
1. Go to your repository → **Settings** → **Pages**
2. Under "Source", select: **GitHub Actions**
3. Push your code to `main` branch
4. GitHub Actions will automatically build and deploy

### Manual Deployment

1. Build the website:
   ```bash
   cd website
   ./build.sh  # or .\build.ps1 on Windows
   ```

2. The `www/` directory contains all static files ready for deployment

3. Deploy the contents of `www/` to any static hosting service:
   - GitHub Pages
   - Netlify
   - Vercel
   - CloudFlare Pages

## 🧪 Testing the WASM Module

Run Rust tests:
```bash
cd website
cargo test
```

## 🎨 Customization

### Colors

Edit CSS variables in `www/styles.css`:
```css
:root {
    --primary-color: #4A90E2;
    --secondary-color: #50C878;
    --accent-color: #FF6B6B;
    /* ... */
}
```

### Algorithms

Edit the Rust code in `src/lib.rs` to modify:
- Heading calculation
- Distance calculation (Haversine)
- Bearing calculation
- Smoothing parameters

After changes, rebuild with `./build.sh`

## 📊 What's Using WebAssembly?

The following functions run in WebAssembly for maximum performance:

- `calculate_heading()` - Compass heading from magnetometer data
- `calculate_distance()` - Haversine distance between GPS coordinates
- `calculate_bearing()` - Initial bearing to target location
- `calculate_relative_bearing()` - Relative angle for compass display
- `get_direction()` - Cardinal direction (N, NE, E, etc.)
- `format_distance()` - Human-readable distance formatting

## 🐛 Troubleshooting

**WASM module fails to load:**
- Check browser console for errors
- Ensure you're serving via HTTP (not `file://`)
- Clear browser cache
- Rebuild with `./build.sh`

**Build fails:**
- Ensure Rust and wasm-pack are installed
- Run `rustup update`
- Check `Cargo.toml` for correct dependencies

**GitHub Pages not updating:**
- Check Actions tab for build errors
- Ensure GitHub Pages is enabled in settings
- Wait 2-3 minutes for CDN cache to clear

## 📚 Resources

- [wasm-pack Documentation](https://rustwasm.github.io/wasm-pack/)
- [WebAssembly.org](https://webassembly.org/)
- [Rust WASM Book](https://rustwasm.github.io/docs/book/)
- [GitHub Pages Docs](https://docs.github.com/en/pages)

## 📝 License

This website and all SideQuester code is licensed under [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).

## 🤝 Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](../CONTRIBUTING.md) in the root directory.

## 💬 Questions?

Open an issue on [GitHub](https://github.com/AmrEslim/SideQuester/issues) or check the [documentation](../docs/).

---

Built with ❤️ for the maker community
