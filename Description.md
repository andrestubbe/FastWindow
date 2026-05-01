# FastWindow

**FastWindow** ist das Modul zur Kontrolle der nativen Fenster-Eigenschaften unter Windows. Es ist der "Hardware-Turbo" für AWT/Swing-Fenster.

## Kernfunktionen

- **DWM-Integration:** Mica, Acrylic, Dark Mode und abgerundete Ecken nativ steuern.
- **Resize-Engine:** Behebt das typische Java-Flackern beim Resizen durch native `WM_SIZE` und `WM_PAINT` Hooks.
- **Constraints:** Erzwingt Min/Max-Größen direkt im Windows-Kernel (`WM_GETMINMAXINFO`), sodass das Fenster beim Erreichen der Grenze nicht "springt".
- **Overlay-Support:** Click-Through-Fenster und präzises Z-Order Management.
- **Tracking:** Erlaubt es einem Fenster, sich an ein anderes Fenster "zu heften" (Sticky-Overlay).

## Status
- [x] JNI HWND Capture (JAWT)
- [x] Immersive Dark Mode
- [x] Mica Material Effect
- [ ] Native Size Constraints (Step 2)
- [ ] Smooth Resize Hook (Step 3)
- [ ] Click-Through Mode
- [ ] Window Tracking
