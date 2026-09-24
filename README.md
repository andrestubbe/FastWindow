# FastWindow 0.1.2 — Ultra-Fast Win32 Native Window Engine for Java

[![Status](https://img.shields.io/badge/status-0.1.2-brightgreen.svg)](https://github.com/andrestubbe/FastWindow/releases/tag/0.1.2)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-ready-green.svg)](https://jitpack.io/#andrestubbe/FastWindow)

---

**⚡ Ultra-high performance Win32 native window engine specifically designed as the core windowing foundation for FastVulkan, FastGraphics, DirectX and hardware-accelerated composition pipelines.**

FastWindow provides zero-overhead, kernel-direct Win32 native window creation (`FastWindow.create(...)`), bypassing Java AWT/Swing entirely. It guarantees flicker-free resizing, seamless Dark Mode and DWM styling, dynamic Unicode titling, and clean HWND lifecycle management.

[**Watch the Demo**](https://www.youtube.com/watch?v=BZsqQl7WqWk) | Watch JMH Benchmark (YouTube)

[![FastWindow Showcase](docs/screenshot.png)](https://www.youtube.com/watch?v=BZsqQl7WqWk)

---

## Quick Start

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

## Table of Contents

- [Quick Start](#quick-start)
- [Why FastWindow?](#why-fastwindow)
- [Key Features](#key-features)
- [Real-World Use Cases](#real-world-use-cases)
- [Performance Benchmarks](#performance-benchmarks)
- [API Quick Reference](#api-quick-reference)
- [Technical Demos & Benchmarks](#technical-demos--benchmarks)
- [Installation](#installation)
- [Documentation](#documentation)
- [Platform Support](#platform-support)
- [Related Projects](#related-projects)
- [License](#license)

---

## Why FastWindow?

Standard Java windows (`JFrame`, `Frame`) on Windows suffer from significant native limitations when used with modern GPU rendering pipelines:

- ❌ **Swing/AWT Event Loop Overhead** — AWT uses asynchronous, multi-threaded event dispatching that induces boundary jitter and frame drops during live resize.
- ❌ **Flicker & Erase Issues** — Default Win32 background erases cause white/black flashing before GPU frames present.
- ❌ **Lack of Modern DWM Harmony** — Java frames cannot natively toggle Windows 11 rounded corners, immersive Dark Mode, or title bar colors without custom JNI hooks.

**FastWindow** eliminates the entire Java AWT layer and creates pure, kernel-direct Win32 windows with direct HWND access for **FastVulkan** and **FastGraphics**:

| Feature | Java Swing (`JFrame`) | JavaFX (`Stage`) | FastWindow |
|:---|:---|:---|:---|
| **Window Subsystem** | Heavy AWT Peer wrapper | Prism / Glass toolkit | **Direct Win32 `CreateWindowExW`** |
| **GPU Surface Integration** | AWT Canvas JAWT bridge | JNI offscreen texture | **Raw 64-bit HWND for FastVulkan** |
| **Message Pump Latency** | AWT EventQueue dispatch | JavaFX Pulse loop | **~12 ns (`pollEvents` Win32 pump)** |
| **Resize Flicker** | Severe (`WM_ERASEBKGND`) | Occasional frame stutter | **Zero-Flicker Hardware Sync** |
| **Windows 11 DWM Styling** | OS default light titlebar | Custom undecorated hacks | **Native DWM Dark Mode & Corners** |
| **Dependencies** | JDK Desktop module | Modular JavaFX runtime | **Pure Java 17+ backed by FastCore** |

---

## Key Features

- 🪟 **Pure Standalone Win32 Windows** — Zero-overhead native windows with direct UTF-16 Unicode titlebars, fullscreen, centering, and icon support.
- 🌋 **First-Class Vulkan & DirectX Support** — Clean HWND handle lifecycle designed for surface creation and zero-jitter live resizing.
- ⚡ **Zero-Jitter Live Resize** — Hardware-synced message pump with immediate bounds dispatching.
- 📏 **Kernel-Level Constraints** — Enforces hard Min/Max window sizes directly in the Windows kernel via `WM_GETMINMAXINFO`.
- 🎨 **DWM & FastTheme Harmony** — Native Dark Mode, title bar coloring, and rounded corners for Windows 11.
- 🔑 **Universal HWND Access** — Provides a clean 64-bit native HWND handle for Vulkan, DirectX, and DWM composition engines.

---

## Real-World Use Cases

- 🌋 **High-FPS Vulkan Rendering** — Dedicated native window surface lifecycle for `vkCreateWin32SurfaceKHR` without AWT peer synchronization lags.
- 🎮 **Game Engines & Real-Time Simulators** — Microsecond message pump with `pollEvents()` delivering zero-allocation game loops exceeding 1,000+ FPS.
- 🖥️ **Borderless Fullscreen Presenters** — Instant transition to exclusive borderless fullscreen for presentations and hardware-accelerated viewport overlays.
- 🎨 **Immersive Windows 11 Desktop Apps** — Native dark titlebars, custom background tinting, and rounded corners harmonized with `FastTheme`.

---

## Performance Benchmarks

FastWindow is rigorously profiled using **JMH** (Java Microbenchmark Harness) to guarantee zero-overhead event polling and native Win32 dispatching.

| Operation / Benchmark | Throughput (ops/ms) | Ops per Second | Latency / Overhead |
|---|---|---|---|
| **Message Pump (`pollEvents`)** | **~80,159 ops/ms** | **> 80.1 Million** | **~12 ns** |
| **Kernel Constraints (`setConstraints`)** | **~57,144 ops/ms** | **> 57.1 Million** | **~17 ns** |
| **Dynamic Unicode Titling (`setTitle`)** | **~211 ops/ms** | **> 211,000** | **~4.7 µs (DWM Sync)** |
| **Bounds Dispatch (`setBounds`)** | **~118 ops/ms** | **> 118,000** | **~8.4 µs (Win32 API)** |

*Measured on Windows 11, Intel Core i5-1135G7 (Surface Pro 8), JDK 21.0.12.*

---

## API Quick Reference

| Method | Return Type | Description | Docs |
|---|---|---|---|
| `FastWindow.create(title, w, h)` | `FastNativeWindow` | Creates a standalone native Win32 window context. | [Reference](docs/REFERENCE.md#class-fastwindowfastwindow) |
| `getHWND()` | `long` | Returns the raw 64-bit native window handle (HWND). | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `pollEvents()` | `boolean` | Pumps native Win32 message loop (`PeekMessageW`). | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `setTitle(String title)` | `void` | Updates native window title with UTF-16 Unicode text. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `setVisible(boolean visible)` | `void` | Shows (`SW_SHOW`) or hides the native window. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `setBounds(x, y, w, h)` | `void` | Updates window position and size simultaneously. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `setMinimumSize(minW, minH)` | `void` | Sets kernel-level min tracking size via `WM_GETMINMAXINFO`. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `setMaximumSize(maxW, maxH)` | `void` | Sets kernel-level max tracking size via `WM_GETMINMAXINFO`. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `setFullscreen(boolean fs)` | `void` | Toggles borderless exclusive fullscreen mode. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `setIconImage(BufferedImage img)` | `void` | Sets the native 32-bit ARGB title bar and taskbar icon. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |
| `close()` | `void` | Destroys the window handle and frees unmanaged resources. | [Reference](docs/REFERENCE.md#class-fastwindowfastnativewindow) |

---

## Technical Demos & Benchmarks

| Case | Java Example | Launcher | Description |
|---|---|---|---|
| **Native Black Window & Theme Demo** | [Demo.java](examples/Demo/src/main/java/fastwindow/Demo.java) | `run-demo.bat` | Standalone native Win32 window with Dark Mode, black titlebar, dynamic FPS title, and round icon. |
| **JMH Microbenchmark Suite** | [Benchmark.java](examples/Benchmark/src/main/java/fastwindow/benchmark/Benchmark.java) | `run-benchmark.bat` | High-frequency message pump and native call throughput benchmarking. |

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
        <version>0.1.2</version>
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
    implementation 'com.github.andrestubbe:FastWindow:0.1.2'
    implementation 'com.github.andrestubbe:FastCore:0.1.0'
    implementation 'com.github.andrestubbe:FastTheme:0.1.4'
}
```

### Option 3: Direct Download (No Build Tool)

Download the latest JARs directly to add them to your classpath:

1. 📦 **[fastwindow-0.1.2.jar](https://github.com/andrestubbe/FastWindow/releases/download/0.1.2/fastwindow-0.1.2.jar)** (The Core Library)
2. 📦 **[fastcore-0.1.0.jar](https://github.com/andrestubbe/FastCore/releases/download/0.1.0/fastcore-0.1.0.jar)** (Required Native JNI loader)
3. 📦 **[fasttheme-0.1.4.jar](https://github.com/andrestubbe/FastTheme/releases/download/0.1.4/fasttheme-0.1.4.jar)** (Recommended for Dark Mode & Styling)

---

## Documentation

- **[COMPILE.md](docs/COMPILE.md)**: Full compilation guide (MSVC C++17 build chain + JNI Setup).
- **[REFERENCE.md](docs/REFERENCE.md)**: Full API descriptions and method reference.
- **[PHILOSOPHY.md](docs/PHILOSOPHY.md)**: The engineering rationale for zero-allocation performance.
- **[ROADMAP.md](docs/ROADMAP.md)**: Future milestones and planned features.

---

## Platform Support

| Platform | Architecture | Status | Driver / Subsystem |
|:---|:---:|:---:|:---|
| **Windows 10 / 11** | x64 | ✅ Fully Supported | Native Win32 Subclassing & DirectComposition |
| **Linux** | x64 / AArch64 | 🚧 Planned | X11 (`Xlib`/`xcb`) & Wayland `xdg-shell` |
| **macOS** | Apple Silicon / x64 | 🚧 Planned | Cocoa `NSWindow` & Metal Layer Hosting |

---

## Related Projects

- **[`FastCore`](https://github.com/andrestubbe/FastCore)** — Native Library Loader for Java
- **[`FastTheme`](https://github.com/andrestubbe/FastTheme)** — Advanced UI Styling Engine (DWM/Mica/Acrylic)
- **[`FastUI`](https://github.com/andrestubbe/FastUI)** — High-Performance Retained-Mode UI Framework

---

## License

MIT License — See [LICENSE](LICENSE) file for details.

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.* 🚀
