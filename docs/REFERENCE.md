# FastWindow Reference Manual

`FastWindow` is the ultra-fast Win32 native window engine and direct HWND management substrate of the FastJava ecosystem.

---

## 1. Core Vocabulary

* **Win32 Native Window**: Bypasses Java AWT/Swing to create pure Win32 windows (`CreateWindowExW`) directly on the OS thread.
* **Direct HWND Handle**: Exposes the 64-bit native window handle (`HWND`) for hardware-accelerated presentation surfaces (`FastVulkan`, `FastGraphics`, DirectX).
* **Hardware-Synced Message Pump**: Direct non-blocking message dispatching (`PeekMessageW`, `TranslateMessage`, `DispatchMessageW`) via `pollEvents()`.
* **DWM & FastTheme Harmony**: Zero-jitter live resizing, flicker-free background presentation, and native Windows 11 title bar styling.

---

## 2. FastWindow API Reference

### Class: `fastwindow.FastWindow`
Factory entry point for creating native window contexts.

- `public static FastNativeWindow create(String title, int width, int height)`  
  Creates and initializes a standalone native Win32 window with the specified title and client area dimensions.

---

### Class: `fastwindow.FastNativeWindow`
Primary window lifecycle and state abstraction. Implements `AutoCloseable`.

#### Event Loop & Lifecycle
- `public boolean pollEvents()`  
  Pumps native Win32 messages. Returns `true` while the window is active and running; returns `false` when `WM_CLOSE` / `WM_QUIT` occurs.
- `public boolean isOpen()`  
  Returns `true` if the underlying native window handle is valid.
- `public long getHWND()`  
  Returns the raw 64-bit native window handle (`HWND`) for Vulkan/DirectX surface attachment.
- `public void close()`  
  Destroys the native window (`DestroyWindow`) and releases associated unmanaged resources.

#### Window Styling & Text
- `public void setTitle(String title)`  
  Updates the window title dynamically with UTF-16 Unicode text.
- `public void setIconImage(BufferedImage image)`  
  Applies a 32-bit ARGB image as the window taskbar and title bar icon (`WM_SETICON`).
- `public void setVisible(boolean visible)`  
  Controls window visibility (`ShowWindow(SW_SHOW / SW_HIDE)`).
- `public void setResizable(boolean resizable)`  
  Toggles resize borders and maximize buttons (`WS_THICKFRAME`, `WS_MAXIMIZEBOX`).
- `public void setAlwaysOnTop(boolean alwaysOnTop)`  
  Toggles topmost Z-order placement (`HWND_TOPMOST` / `HWND_NOTOPMOST`).

#### Geometry & Constraints
- `public int getWidth()`  
  Returns the client area width in pixels.
- `public int getHeight()`  
  Returns the client area height in pixels.
- `public int getX()` / `public int getY()`  
  Returns the screen coordinates of the top-left window position.
- `public void setLocation(int x, int y)`  
  Moves the window to `(x, y)`.
- `public void setSize(int width, int height)`  
  Resizes client bounds.
- `public void setBounds(int x, int y, int width, int height)`  
  Simultaneously sets position and size in a single Win32 call (`SetWindowPos`).
- `public void centerOnScreen()`  
  Centers the window on the active display monitor.
- `public void setMinimumSize(int minWidth, int minHeight)`  
  Enforces hardware-level minimum tracking dimensions via kernel `WM_GETMINMAXINFO`.
- `public void setMaximumSize(int maxWidth, int maxHeight)`  
  Enforces hardware-level maximum tracking dimensions via kernel `WM_GETMINMAXINFO`.

#### State Management
- `public void minimize()`  
  Minimizes window to taskbar (`SW_MINIMIZE`).
- `public void maximize()`  
  Maximizes window to cover desktop workspace (`SW_MAXIMIZE`).
- `public void restore()`  
  Restores window from minimized or maximized state (`SW_RESTORE`).
- `public void setFullscreen(boolean fullscreen)`  
  Toggles exclusive borderless fullscreen mode.
- `public boolean isFullscreen()`  
  Returns `true` if borderless fullscreen is currently active.

---

## 3. Guarantees & Hardware Model

* **Zero AWT Dependencies**: Completely free of `java.awt.*` peer synchronization locks.
* **Flicker-Free Live Resize**: Handles `WM_ERASEBKGND` directly in native window procedures to eliminate blank flashes during GPU frame presentations.
* **High-Throughput Message Pump**: Delivers over 80 million `pollEvents()` cycles per second with sub-microsecond latency.

---

**Part of the FastJava Ecosystem** — *Making the JVM faster.* 🚀
