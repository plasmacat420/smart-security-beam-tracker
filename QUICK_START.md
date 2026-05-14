# 🚀 Quick Start Guide

Get up and running with the Security Laser Beam Visualizer in minutes!

## 📦 Option 1: Web Demo (Fastest - No Build Required!)

1. **Open the web interface:**
   ```bash
   cd web
   # Option A: Direct file open
   # Just double-click index.html in your file browser

   # Option B: Local server (recommended)
   python -m http.server 8000
   # Visit: http://localhost:8000
   ```

2. **Start visualizing:**
   - Click cells to add/remove security devices
   - Click "Calculate Beams" to see the laser connections
   - Try the preset examples
   - Click "Animate" for step-by-step visualization

**Perfect for:** Quick demos, sharing with non-technical people, LinkedIn posts

---

## 💻 Option 2: CLI Application (Full Features)

### Windows (MSVC)

```bash
# Open Developer Command Prompt for VS

# Build
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release

# Run
.\Release\SecurityLaserVisualizer.exe

# Or run demo
.\Release\SecurityLaserVisualizer.exe --demo
```

### Linux / macOS

```bash
# Install dependencies (if needed)
# Ubuntu/Debian:
sudo apt-get install build-essential cmake

# macOS:
brew install cmake

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Run
./SecurityLaserVisualizer

# Or run demo
./SecurityLaserVisualizer --demo
```

### MinGW on Windows (Git Bash)

```bash
# Build
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Run
./SecurityLaserVisualizer.exe --demo
```

---

## 🎮 Using the CLI

### Quick Demo
```bash
./SecurityLaserVisualizer --demo
```

### Interactive Mode
```bash
./SecurityLaserVisualizer
```

**Main Menu Options:**
1. **Interactive Grid Editor** - Edit grids with arrow keys
2. **Animate Beam Calculation** - See step-by-step visualization
3. **View Current Grid** - Display with statistics
4. **Load Preset Example** - Try pre-configured examples
5. **Random Grid Generator** - Create test cases
6. **Performance Benchmark** - Test scalability
7. **Save/Load** - Persist your grids

**Editor Controls:**
- `↑↓←→` - Navigate
- `SPACE` - Toggle device
- `ENTER` - Calculate & view
- `A` - Animate
- `R` - Randomize
- `H` - Help
- `Q` - Quit

---

## 📊 Testing Your Build

### Run Unit Tests
```bash
# Configure with tests
cmake .. -DBUILD_TESTS=ON

# Build
cmake --build .

# Run tests
./tests/SecurityLaserTests  # Linux/Mac
# or
.\tests\Release\SecurityLaserTests.exe  # Windows
```

### Performance Benchmark
```bash
./SecurityLaserVisualizer
# Select option 6: Performance Benchmark
```

---

## 🎯 Quick Examples to Try

### Example 1: Basic Configuration (8 beams)
```
Grid: 011001
      000000
      010100
      001000

Expected: 8 beams
```

### Example 2: Maximum Beams (9 beams)
```
Grid: 111
      000
      111

Expected: 9 beams (3×3)
```

### Example 3: No Beams
```
Grid: 000
      111
      000

Expected: 0 beams (all same row)
```

---

## 🐛 Troubleshooting

### "CMake not found"
- **Windows:** Install from https://cmake.org/download/
- **Linux:** `sudo apt-get install cmake`
- **macOS:** `brew install cmake`

### "Compiler not found"
- **Windows:** Install Visual Studio or MinGW
- **Linux:** `sudo apt-get install build-essential`
- **macOS:** `xcode-select --install`

### Colors not showing (Windows)
- Use Windows 10+ terminal or Windows Terminal
- Git Bash also supports ANSI colors

### Web demo not interactive
- Use a local server instead of opening file directly
- Modern browsers may block features on `file://` protocol

---

## 📸 Creating a Demo GIF for LinkedIn

### Using the CLI
1. Use screen recording software (OBS, ScreenToGif, etc.)
2. Run: `./SecurityLaserVisualizer --demo`
3. Or use interactive mode and show animation feature
4. Convert to GIF and upload to LinkedIn

### Using the Web Interface
1. Open web demo in browser
2. Load a preset (e.g., "Example 1")
3. Click "Animate"
4. Record screen
5. Post with caption:

```
Just built an interactive Security Laser Beam Visualizer! 🔐

Transformed a LeetCode algorithm into a full-featured project:
✅ Interactive CLI with animations
✅ Web interface for demos
✅ O(m×n) optimized algorithm
✅ Cross-platform support
✅ Performance benchmarking

Tech: C++17, HTML/CSS/JS, CMake
GitHub: https://github.com/plasmacat420/smart-security-beam-tracker

#SoftwareEngineering #C++ #WebDev #Algorithms
```

---

## 🌟 Next Steps

1. ✅ Build the project
2. ✅ Try the web demo
3. ✅ Run interactive CLI
4. ✅ Test with random grids
5. ✅ Check performance benchmarks
6. 📸 Create demo video/GIF
7. 🌐 Deploy web demo to GitHub Pages
8. 📝 Customize README with your info
9. 🚀 Share on LinkedIn/GitHub

---

## 💡 Pro Tips

- The web demo is perfect for LinkedIn video posts
- Use `--demo` flag for quick visual demonstrations
- Interactive mode shows off the full feature set
- Benchmark results make great talking points in interviews
- The algorithm explanation in ALGORITHM.md is interview prep gold

---

**Need help?** Open an issue on GitHub or check the full README.md

**Ready to showcase?** You've built something impressive - share it! 🎉
