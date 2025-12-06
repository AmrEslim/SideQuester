# 🚀 Quick Start Guide - SideQuester Website

Get your SideQuester website up and running in minutes!

## ⚡ Quick Setup (3 steps)

### 1️⃣ Install Prerequisites

**Install Rust:**
```powershell
# Download and run from: https://rustup.rs/
# Or use winget:
winget install Rustlang.Rust.MSVC
```

**Install wasm-pack:**
```powershell
cargo install wasm-pack
```

### 2️⃣ Build the Website

```powershell
cd website
.\build.ps1
```

### 3️⃣ Test Locally

```powershell
cd www
python -m http.server 8080
```

Open: http://localhost:8080 🎉

## 🌐 Deploy to GitHub Pages

### Enable GitHub Pages

1. Go to your repo: https://github.com/AmrEslim/SideQuester
2. Click **Settings** → **Pages**
3. Under "Source", select: **GitHub Actions**
4. Done! ✅

### Push Your Code

```powershell
git add .
git commit -m "Add website with Rust WASM"
git push origin main
```

**Your website will be live at:**
`https://amreslim.github.io/SideQuester/`

(Wait 2-3 minutes for first deployment)

## 📁 What You Created

```
website/
├── src/lib.rs           ✅ Rust compass algorithms
├── www/
│   ├── index.html      ✅ Landing page
│   ├── demo.html       ✅ Interactive demo
│   ├── styles.css      ✅ Styling
│   └── app.js          ✅ WASM loader
├── build.ps1           ✅ Build script
└── Cargo.toml          ✅ Rust config
```

## 🎯 Features

✅ **Rust + WebAssembly** - Fast compass calculations  
✅ **Interactive Demo** - Live compass simulation  
✅ **Responsive Design** - Mobile-friendly  
✅ **Auto-Deploy** - GitHub Actions CI/CD  
✅ **Free Hosting** - GitHub Pages  

## 🐛 Common Issues

**"wasm-pack not found"**
```powershell
cargo install wasm-pack
```

**"Python not found"**
- Install Python 3 from: https://www.python.org/downloads/
- Or use: `winget install Python.Python.3`

**GitHub Pages not working**
- Check: Settings → Pages → Source = "GitHub Actions"
- Check: Actions tab for build errors
- Wait 2-3 minutes after first push

## 📚 Next Steps

- **Customize colors** in `www/styles.css`
- **Edit content** in `www/index.html`
- **Modify algorithms** in `src/lib.rs`
- **Add more features** to the demo

## 🆘 Need Help?

- 📖 Full docs: `website/README.md`
- 🐛 Report issues: https://github.com/AmrEslim/SideQuester/issues
- 💬 Ask questions in GitHub Discussions

---

**Built with ❤️ - Now go show off your project! 🧭**
