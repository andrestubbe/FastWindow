# FastWindow

**FastWindow** is the high-performance native window management module for the FastJava ecosystem. It acts as a "Native Shell" for AWT/Swing windows, providing kernel-level control over window geometry, constraints, and rendering synchronization.

## Core Features

- **🚀 Flicker-Free Resizing** — Eliminates the classic Java "strobe" effect during resize operations via native background erase hooks.
- **🛡️ Kernel-Level Constraints** — Enforces hard Min/Max window sizes directly in the Windows kernel (`WM_GETMINMAXINFO`), providing jitter-free boundaries.
- **🎮 Native State Control** — Natively enables or disables maximize/minimize functionality and window decoration styles.
- **🎨 Render Synchronization** — Matches native background "void" colors to Java UI colors for seamless visual transitions.
- **⚡ HWND Identity** — Provides the stable native handle used by other modules (FastTheme, FastOverlay) to identify and style the window.

## Current Status (v0.1.0)
- [x] JNI HWND Capture (JAWT)
- [x] Native Size Constraints (Kernel Enforcement)
- [x] Flicker-Free Resize Hook (WM_ERASEBKGND)
- [x] Native Background Color Sync
- [x] Native Maximize Toggle
- [ ] Click-Through Mode (Ghost Mode)
- [ ] Window Tracking (Sticky Overlay)

## Strategic Role
FastWindow is the **Identity Provider** for the FastJava ecosystem. It finds and manages the window, while modules like **FastTheme** (Visuals) and **FastOverlay** (Graphics) use the handles provided by FastWindow to do their work.
