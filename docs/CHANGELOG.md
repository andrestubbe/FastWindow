# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [0.1.2] - 2026-09-24

### Added
- `WindowPaintListener` interface and `FastNativeWindow.setPaintListener(...)` for synchronous, zero-latency painting during the modal Windows sizing loop (`WM_SIZE`).
- Native JNI paint callback bridge connecting Win32 window procedures directly to Java render loops.

### Fixed
- Eliminated black borders and resizing artifacts by removing competing asynchronous `WM_TIMER` and redundant `WM_SIZING` window-rect dispatch.
- Enforced exact client area dimensions (`w`, `h`) across live resizing.

## [0.1.1] - 2026-09-23

### Added
- Direct Win32 standalone windowing engine (`FastWindow.create(...)`).
- Complete `FastTheme` and DWM Dark Mode integration.
- Hardware-enforced minimum and maximum sizing via `WM_GETMINMAXINFO`.
- Standalone message pump (`pollEvents()`).

## [0.1.0] - 2026-09-17

### Added
- Initial release and basic native JNI bindings.
