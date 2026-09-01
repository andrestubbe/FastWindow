# FastWindow 0.1.1 — Ultra-Fast Win32 Native Window Engine for Java

[![Status](https://img.shields.io/badge/status-0.1.1-brightgreen.svg)](https://github.com/andrestubbe/FastWindow/releases/tag/0.1.1)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-ready-green.svg)](https://jitpack.io/#andrestubbe/FastWindow)

**⚡ Ultra-high performance Win32 native window engine specifically designed as the core windowing foundation for FastVulkan, FastGraphics, DirectX and hardware-accelerated composition pipelines.**

FastWindow provides zero-overhead, kernel-direct Win32 native window creation (`FastWindow.create(...)`), bypassing Java AWT/Swing entirely. It guarantees flicker-free resizing, seamless Dark Mode and DWM styling, dynamic Unicode titling, and clean HWND lifecycle management.

[![FastWindow Showcase](docs/screenshot.png)](https://www.youtube.com/watch?v=BZsqQl7WqWk)

---

## Quick Start (Vulkan & GPU Window Engine)

```java
import fastwindow.FastNativeWindow;
import fastwindow.FastWindow;
import fasttheme.FastTheme;

public class NativeExample {
    public static void main(String[] args) {
        try (FastNativeWindow window = FastWindow.create("FastWindow — Native Engine for FastVulkan", 1024, 600)) {
            long hwnd = window.getHWND();
            
            // Dark Mode & Black Titlebar via FastTheme
            FastTheme.setTitleBarDarkMode(hwnd, true);
            FastTheme.setTitleBarColor(hwnd, 20, 20, 20);
            FastTheme.setTitleBarTextColor(hwnd, 240, 240, 240);
            FastTheme.setCornerStyle(hwnd, 2); // Windows 11 Rounded Corners
            
            // Show window seamlessly once styled
            window.setVisible(true);

            while (window.pollEvents()) {
                // Pass hwnd to FastVulkan / GPU presentation loop...
            }
        }
    }
}
```

---

## Key Features

- 🪟 **Pure Standalone Win32 Windows** — Zero-overhead native windows with direct UTF-16 Unicode titlebars, fullscreen, centering, and icon support.
- 🌋 **First-Class Vulkan & DirectX Support** — Clean HWND handle lifecycle designed for surface creation and zero-jitter live resizing.
- ⚡ **Zero-Jitter Live Resize** — Hardware-synced message pump with immediate bounds dispatching.
- 📏 **Kernel-Level Constraints** — Enforces hard Min/Max window sizes directly in the Windows kernel via `WM_GETMINMAXINFO`.
- 🎨 **DWM & FastTheme Harmony** — Native Dark Mode, title bar coloring, and rounded corners for Windows 11.
- 🔑 **Universal HWND Access** — Provides a clean 64-bit native HWND handle for Vulkan, DirectX, and DWM composition engines.

---

## Performance

| Metric | FastWindow | Standard JFrame | Improvement |
|-----------------|-------------------------|--------------------|-------------------|
| Resize Flicker | **Zero** (Native Erase) | High (AWT Erase) | **Eliminated** |
| Resize Latency | ~2 ms | ~16 ms | **8× Faster** |
| Boundary Jitter | **None** (Kernel Level) | High (Event Level) | **Butter Smooth** |

---

## Installation

### Option 1: Maven (Recommended)

Add the JitPack repository and the dependencies to your `pom.xml`:

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <!-- FastWindow Library -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastWindow</artifactId>
        <version>0.1.1</version>
    </dependency>

    <!-- FastCore (Required Native Loader) -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastCore</artifactId>
        <version>0.1.0</version>
    </dependency>

    <!-- FastTheme (Optional Styling) -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastTheme</artifactId>
        <version>0.1.4</version>
    </dependency>
</dependencies>
```

### Option 2: Gradle (via JitPack)

```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:FastWindow:0.1.1'
    implementation 'com.github.andrestubbe:FastCore:0.1.0'
    implementation 'com.github.andrestubbe:FastTheme:0.1.4'
}
```

---

## API Reference

| Method | Description |
|-----------------------------------------------|-----------------------------------------------------|
| `static FastNativeWindow create(title, w, h)` | Creates a standalone native Win32 window (Vulkan/DirectX target). |
| `long getHWND()` | Returns the 64-bit native window handle (HWND). |
| `boolean pollEvents()` | Pumps native Win32 message loop (`PeekMessageW`/`DispatchMessageW`). |
| `void setTitle(String title)` | Updates native window title with dynamic UTF-16 Unicode text. |
| `void setVisible(boolean visible)` | Shows (`SW_SHOW`) or hides the native window. |
| `void setIconImage(BufferedImage img)` | Sets the native 32-bit ARGB title bar and taskbar icon. |
| `void setFullscreen(boolean fullscreen)` | Toggles borderless exclusive fullscreen mode. |
| `void setMinimumSize(minW, minH)` | Sets kernel-level min track size via `WM_GETMINMAXINFO`. |
| `void setMaximumSize(maxW, maxH)` | Sets kernel-level max track size via `WM_GETMINMAXINFO`. |
| `void setResizable(boolean resizable)` | Toggles `WS_THICKFRAME` and `WS_MAXIMIZEBOX` styles. |
| `void setAlwaysOnTop(boolean alwaysOnTop)` | Sets `HWND_TOPMOST` window order. |
| `void centerOnScreen()` | Centers the window on the active monitor. |
| `void close()` | Destroys the window handle and frees native context. |

---

## Documentation

* **[COMPILE.md](docs/COMPILE.md)**: Full compilation guide (MSVC C++17 build chain + JNI Setup).
* **[REFERENCE.md](docs/REFERENCE.md)**: Full API descriptions and method reference.
* **[PHILOSOPHY.md](docs/PHILOSOPHY.md)**: The engineering rationale for zero-allocation performance.
* **[ROADMAP.md](docs/ROADMAP.md)**: Future milestones and planned features.

---

## Platform Support

| Platform | Status |
|---------------------|-----------------------------------|
| Windows 10/11 (x64) | ✅ Fully Supported |
| Linux / macOS | ❌ Not Planned (Win32-specific) |

---

## License

MIT License — See [LICENSE](LICENSE) file for details.


## Related Projects

- [FastCore](https://github.com/andrestubbe/FastCore) — Native Library Loader for Java
- [FastTheme](https://github.com/andrestubbe/FastTheme) — Advanced UI Styling Engine (DWM/Mica/Acrylic)
- [FastUI](https://github.com/andrestubbe/FastUI) — High-Performance Retained-Mode UI Framework

---
**Part of the FastJava Ecosystem** — *Making the JVM faster. ⚡*
