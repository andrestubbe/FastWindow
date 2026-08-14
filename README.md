# FastWindow 0.1.0 [ALPHA-2026-06-14] — Native Windows Window Engine for Java

[![Status](https://img.shields.io/badge/status-0.1.0-brightgreen.svg)](https://github.com/andrestubbe/FastWindow/releases/tag/0.1.0)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-ready-green.svg)](https://jitpack.io/#andrestubbe/FastWindow)

**⚡ High-performance window management and Win32 turbocharging for Java applications.**

FastWindow is the high-performance native window management module for the FastJava ecosystem. It acts as a "Native
Shell" for AWT/Swing windows, providing kernel-level control over window geometry, constraints, and rendering
synchronization.

[![FastWindow Showcase](docs/screenshot.png)](https://www.youtube.com/watch?v=BZsqQl7WqWk)

---

## Quick Start

```java
import fastwindow.FastWindow;

public class Example {
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

---

## Table of Contents

- [Why FastWindow?](#why-fastwindow)
- [Key Features](#key-features)
- [Performance](#performance)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [API Reference](#api-reference)
- [Documentation](#documentation)
- [Platform Support](#platform-support)
- [License](#license)
- [Related Projects](#related-projects)

---

## Why FastWindow?

FastWindow was built to solve the long-standing native limitations of the standard Java `JFrame` and `Frame` on Windows:

- 🌑 **Title Bar Clutter** — Standard Java frames cannot natively toggle Dark Mode, resulting in a white title bar clashing with dark themes.
- 🔆 **Resizing Flicker** — Java's `RepaintManager` clears the background with a white brush before drawing, causing strobing. FastWindow uses native `WM_ERASEBKGND` hooks to eliminate this.
- 📏 **Soft Constraints** — Java's `setMinimumSize` is enforced via async events, causing jittery snapping. FastWindow enforces limits at kernel level via `WM_GETMINMAXINFO`.
- 🎨 **Lack of Modern Materials** — AWT has no built-in support for Windows 11 Mica or Acrylic. FastWindow provides direct DWM integration via `FastTheme`.

---

## Key Features

- 🪟 **Fluid UI Scaling** — Eliminates black traces and flickering during resize via a "Safe & Smooth" native scaling strategy.
- 📏 **Kernel-Level Constraints** — Enforces hard Min/Max window sizes directly in the Windows kernel for jitter-free boundaries.
- 🎛️ **Native State Control** — Natively enables or disables maximize/minimize functionality and window decoration styles.
- 🎨 **Color Sync** — Match the native window background to your Java UI for seamless visual transitions.
- 🔑 **HWND Identity** — Provides the stable native handle used by other modules (FastTheme, FastOverlay).

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
