# FastWindow — Native Windows Window Engine for Java

**High-performance window management for Java/Swing using the Win32 API.**

[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Overview

FastWindow is a "Turbocharger" for Java AWT/Swing windows. It bypasses the limitations of the standard Java window management by interacting directly with the Windows **Desktop Window Manager (DWM)** and the Win32 message loop.

## Key Features

- **🚀 Smooth Resizing** — Eliminates flickering and lag during window resizing.
- **🛡️ Native Constraints** — Hard limits for min/max size enforced by the OS kernel.
- **💎 Modern Aesthetics** — Native support for Mica, Acrylic, and Immersive Dark Mode.
- **👻 Ghost Mode** — Create mouse-transparent (click-through) overlays.
- **🧲 Window Tracking** — Perfectly sync overlays to other native application windows.

## Quick Start

```java
JFrame frame = new JFrame();
frame.setVisible(true);

FastWindow win = FastWindow.attach(frame);
win.setDarkTheme(true);
win.setConstraints(400, 300, 1920, 1080);
```

---
**Made with ⚡ by Andre Stubbe**
