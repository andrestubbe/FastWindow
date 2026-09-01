# FastWindow 0.2.0 — Native Windows Window Engine for Java

[![Status](https://img.shields.io/badge/status-0.2.0-brightgreen.svg)](https://github.com/andrestubbe/FastWindow/releases/tag/0.2.0)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-ready-green.svg)](https://jitpack.io/#andrestubbe/FastWindow)

**⚡ High-performance Win32 native window engine and AWT/Swing turbocharger for Java applications.**

FastWindow provides ultra-high performance native window creation and management for the FastJava ecosystem. It offers two powerful modes:
1. **Pure Native Win32 Windows (`FastWindow.create`)**: Ultra-lightweight, zero-overhead standalone native windows directly rendering via Vulkan/DirectX/DWM.
2. **Native Shell for AWT/Swing (`FastWindow.attach`)**: Subclasses existing Java frames for kernel-level geometry constraints and flicker-free resizing.

[![FastWindow Showcase](docs/screenshot.png)](https://www.youtube.com/watch?v=BZsqQl7WqWk)

---

## Quick Start

### 1. Pure Standalone Native Window (Recommended for Vulkan/DirectX)

```java
import fastwindow.FastNativeWindow;
import fastwindow.FastWindow;
import fasttheme.FastTheme;

public class NativeExample {
    public static void main(String[] args) {
        try (FastNativeWindow window = FastWindow.create("FastWindow — Native Engine", 1024, 600)) {
            long hwnd = window.getHWND();
            
            // Dark Mode & Black Titlebar via FastTheme
            FastTheme.setTitleBarDarkMode(hwnd, true);
            FastTheme.setTitleBarColor(hwnd, 20, 20, 20);
            FastTheme.setCornerStyle(hwnd, 2); // Windows 11 Rounded Corners
            
            window.setVisible(true);

            while (window.pollEvents()) {
                // Render frame...
            }
        }
    }
}
```

### 2. Supercharging Existing Swing / AWT Windows

```java
import fastwindow.FastWindow;
import javax.swing.JFrame;

public class SwingExample {
    public static void main(String[] args) {
        JFrame frame = new JFrame("FastWindow Demo");
        frame.addNotify(); // Create native peer WITHOUT showing yet
        FastWindow win = FastWindow.attach(frame);
        win.setConstraints(400, 300, 1500, 960);
        win.setBackgroundColor(30, 30, 30); // Eliminate resize flicker
        frame.setVisible(true); // Appears already constrained and stable!
    }
}
```

---

## Key Features

- 🪟 **Pure Standalone Win32 Windows** — Zero-overhead native windows with Unicode titlebars, fullscreen, centering, and icon support.
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
        <version>0.1.0</version>
    </dependency>

    <!-- FastCore (Required Native Loader) -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastCore</artifactId>
        <version>0.1.0</version>
    </dependency>
</dependencies>
```

### Option 2: Gradle (via JitPack)

```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:FastWindow:0.1.0'
    implementation 'com.github.andrestubbe:FastCore:0.1.0'
}
```

### Option 3: Direct Download (No Build Tool)

Download the latest JARs directly to add them to your classpath:

1. 📦 **[fastwindow-0.1.0.jar](https://github.com/andrestubbe/FastWindow/releases/download/0.1.0/fastwindow-0.1.0.jar)** (The Core Library)
2. ⚙️ **[fastcore-0.1.0.jar](https://github.com/andrestubbe/FastCore/releases/download/0.1.0/fastcore-0.1.0.jar)** (The Mandatory Native Loader)

> [!IMPORTANT]
> All JARs must be in your classpath for the native JNI calls to function correctly.

---

## API Reference

| Method | Description |
|-----------------------------------------------|-----------------------------------------------------|
| `static FastWindow attach(Component c)` | Attaches the native engine to a Java window/canvas. |
| `void setConstraints(minW, minH, maxW, maxH)` | Enforces kernel-level size limits. |
| `void setMaximizable(boolean)` | Enables/Disables the native maximize button. |
| `void setBackgroundColor(r, g, b)` | Syncs native background erase to your UI color. |
| `long getHWND()` | Returns the native window handle (HWND). |

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

---

## Related Projects

- [FastCore](https://github.com/andrestubbe/FastCore) — Native Library Loader for Java
- [FastTheme](https://github.com/andrestubbe/FastTheme) — Advanced UI Styling Engine (DWM/Mica/Acrylic)
- [FastUI](https://github.com/andrestubbe/FastUI) — High-Performance Retained-Mode UI Framework

---
**Part of the FastJava Ecosystem** — *Making the JVM faster. ⚡*
