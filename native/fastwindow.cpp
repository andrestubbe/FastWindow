#include <jni.h>
#include <jawt_md.h>
#include <windows.h>
#include <dwmapi.h>
#include <map>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "jawt.lib")

// Global map to store window constraints and original WNDPROC
struct WindowState {
    WNDPROC originalWndProc;
    int minW, minH, maxW, maxH;
};

static std::map<HWND, WindowState> g_windowStates;

/**
 * Custom Window Procedure to intercept OS messages.
 */
LRESULT CALLBACK FastWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto it = g_windowStates.find(hwnd);
    if (it == g_windowStates.end()) return DefWindowProc(hwnd, msg, wParam, lParam);

    WindowState& state = it->second;

    switch (msg) {
        case WM_GETMINMAXINFO: {
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            if (state.minW > 0) mmi->ptMinTrackSize.x = state.minW;
            if (state.minH > 0) mmi->ptMinTrackSize.y = state.minH;
            if (state.maxW > 0) mmi->ptMaxTrackSize.x = state.maxW;
            if (state.maxH > 0) mmi->ptMaxTrackSize.y = state.maxH;
            return 0; // Handled
        }
        case WM_DESTROY: {
            // Restore original proc and cleanup
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)state.originalWndProc);
            g_windowStates.erase(hwnd);
            break;
        }
    }

    return CallWindowProc(state.originalWndProc, hwnd, msg, wParam, lParam);
}

extern "C" {

JNIEXPORT jlong JNICALL Java_fastwindow_FastWindowImpl_nGetHWND(JNIEnv* env, jobject obj, jobject component) {
    JAWT awt;
    awt.version = JAWT_VERSION_1_7;
    if (JAWT_GetAWT(env, &awt) == JNI_FALSE) return 0;

    JAWT_DrawingSurface* ds = awt.GetDrawingSurface(env, component);
    if (!ds) return 0;
    ds->Lock(ds);
    JAWT_DrawingSurfaceInfo* dsi = ds->GetDrawingSurfaceInfo(ds);
    HWND hwnd = ((JAWT_Win32DrawingSurfaceInfo*)dsi->platformInfo)->hwnd;
    ds->FreeDrawingSurfaceInfo(dsi);
    ds->Unlock(ds);
    awt.FreeDrawingSurface(ds);
    return (jlong)hwnd;
}

JNIEXPORT void JNICALL Java_fastwindow_FastWindowImpl_nSetConstraints(JNIEnv* env, jobject obj, jlong hwnd, jint minW, jint minH, jint maxW, jint maxH) {
    HWND h = (HWND)hwnd;
    
    // Subclass if not already done
    if (g_windowStates.find(h) == g_windowStates.end()) {
        WNDPROC oldProc = (WNDPROC)SetWindowLongPtr(h, GWLP_WNDPROC, (LONG_PTR)FastWindowProc);
        g_windowStates[h] = { oldProc, minW, minH, maxW, maxH };
    } else {
        g_windowStates[h].minW = minW;
        g_windowStates[h].minH = minH;
        g_windowStates[h].maxW = maxW;
        g_windowStates[h].maxH = maxH;
    }

    // Force immediate enforcement
    RECT rect;
    if (GetWindowRect(h, &rect)) {
        int w = rect.right - rect.left;
        int h_val = rect.bottom - rect.top;
        int newW = w;
        int newH = h_val;

        if (maxW > 0 && w > maxW) newW = maxW;
        if (minW > 0 && w < minW) newW = minW;
        if (maxH > 0 && h_val > maxH) newH = maxH;
        if (minH > 0 && h_val < minH) newH = minH;

        if (newW != w || newH != h_val) {
            SetWindowPos(h, NULL, 0, 0, newW, newH, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastWindowImpl_nSetDarkTheme(JNIEnv* env, jobject obj, jlong hwnd, jboolean enabled) {
    BOOL dark = enabled ? TRUE : FALSE;
    DwmSetWindowAttribute((HWND)hwnd, 20, &dark, sizeof(dark));
}

JNIEXPORT void JNICALL Java_fastwindow_FastWindowImpl_nEnableMica(JNIEnv* env, jobject obj, jlong hwnd, jboolean enabled) {
    if (enabled) {
        int backdrop = 2; // Mica
        DwmSetWindowAttribute((HWND)hwnd, 38, &backdrop, sizeof(backdrop));
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastWindowImpl_nSetCornerStyle(JNIEnv* env, jobject obj, jlong hwnd, jint style) {
    DwmSetWindowAttribute((HWND)hwnd, 33, &style, sizeof(style));
}

JNIEXPORT void JNICALL Java_fastwindow_FastWindowImpl_nSetMaximizable(JNIEnv* env, jobject obj, jlong hwnd, jboolean enabled) {
    HWND h = (HWND)hwnd;
    LONG_PTR style = GetWindowLongPtr(h, GWL_STYLE);
    if (enabled) {
        style |= WS_MAXIMIZEBOX;
    } else {
        style &= ~WS_MAXIMIZEBOX;
    }
    SetWindowLongPtr(h, GWL_STYLE, style);
    // Force a redraw of the frame
    SetWindowPos(h, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

} // extern "C"
