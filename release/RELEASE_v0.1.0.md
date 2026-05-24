# FastWindow v0.1.0 — Initial Release 🚀

## 🎉 Version 0.1.0: Professional Native Window Management for Java
**Release Date:** 2026-05-02  
**Tag:** `v0.1.0`

---

## ✨ Features

### 🛡️ Flicker-Free Resizing (The "Strobe Fix")
- **Native Erase Hook**: Intercepts `WM_ERASEBKGND` to eliminate the white/black flashes during window resizing.
- **Color Synchronization**: Syncs the native Win32 background color to your Java UI for seamless visual transitions.
- **Fluid UI Scaling**: Optimized `WM_WINDOWPOSCHANGING` strategy allows the OS to stretch content while Java repaints, creating a liquid-smooth experience.

### 📏 Kernel-Level Geometry Control
- **Hard Constraints**: Enforces minimum and maximum window sizes directly in the Windows kernel via `WM_GETMINMAXINFO`.
- **Zero Jitter**: Eliminates the "snap-back" effect seen with standard Java `setMinimumSize`.
- **Maximize Toggle**: Native control over the window maximize button and functionality.

### 🏗️ Architected for the FastJava Ecosystem
- **Handle Provider**: Acting as the core "identity" provider, FastWindow captures and exposes the stable `HWND` handle needed by **FastTheme** and **FastOverlay**.
- **AWT Native Bridge**: Uses the standard JAWT interface for reliable, low-overhead native window access.

---

## 📦 Installation (JitPack)

### Maven
```xml
<dependencies>
    <!-- FastWindow Library (v0.1.0) -->
    <dependency>
        <groupId>io.github.andrestubbe</groupId>
        <artifactId>fastwindow</artifactId>
        <version>v0.1.0</version>
    </dependency>

    <!-- FastCore (Required Native Loader) -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastcore</artifactId>
        <version>v1.0.0</version>
    </dependency>
</dependencies>
```

### Gradle
```groovy
dependencies {
    implementation 'io.github.andrestubbe:fastwindow:v0.1.0'
    implementation 'com.github.andrestubbe:fastcore:v1.0.0'
}
```

---

## 🔧 Technical Details
- **Native DLL:** `fastwindow.dll`
- **Hook Strategy:** Native Win32 Subclassing (`SetWindowLongPtr`).
- **Compatibility:** Windows 10/11 (x64), Java 17+.

---

## 🙏 Credits
- **FastCore**: Unified JNI loading engine.
- **FastTheme**: Aesthetic sibling module.

**Part of the FastJava Ecosystem** — *Making the JVM faster.*
