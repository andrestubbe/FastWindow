# Changelog: FastWindow

All notable changes to this project will be documented in this file.

## [0.1.0] - 2026-05-02
### Added
- **Native Resize Engine**: Implemented `WM_ERASEBKGND` and `ValidateRect` hooks for zero-flicker resizing.
- **Kernel-Level Constraints**: Added `WM_GETMINMAXINFO` enforcement for jitter-free window boundaries.
- **Fluid Scaling**: Optimized `WM_WINDOWPOSCHANGING` and `RDW_UPDATENOW` for liquid-smooth UI updates.
- **Color Sync**: Native background color matching to eliminate "black traces."
- **Maximize Control**: Native `WS_MAXIMIZEBOX` toggling.
- **Zero-Flicker Startup**: Automated `addNotify()` and pre-visibility handle capture.

### Changed
- **Architectural Shift**: Cleaned up the module to focus exclusively on **Window Management**. Visual styling (Mica, etc.) has been moved to **FastTheme**.
- **JNI API**: Standardized method names and handle management.

---
**Part of the FastJava Ecosystem**
