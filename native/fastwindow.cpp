#include <jni.h>
#include <jawt_md.h>
#include <windows.h>
#include <map>

#pragma comment(lib, "jawt.lib")

struct WindowState {
    WNDPROC originalWndProc;
    int minW, minH, maxW, maxH;
    int bgR, bgG, bgB;
};

static std::map<HWND, WindowState> g_windowStates;

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
            return 0;
        }
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH brush = CreateSolidBrush(RGB(state.bgR, state.bgG, state.bgB));
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);
            return 1;
        }
        case WM_WINDOWPOSCHANGING: {
            // REMOVED SWP_NOCOPYBITS: 
            // We now let Windows stretch the old content. 
            // This prevents black traces and keeps the UI visible.
            break;
        }
        case WM_SIZE: {
            // Still validate to prevent extra OS erases
            ValidateRect(hwnd, NULL);
            break;
        }
        case WM_DESTROY: {
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
    if (g_windowStates.find(h) == g_windowStates.end()) {
        WNDPROC oldProc = (WNDPROC)SetWindowLongPtr(h, GWLP_WNDPROC, (LONG_PTR)FastWindowProc);
        
        // Keep WS_CLIPCHILDREN to protect the client area
        LONG_PTR style = GetWindowLongPtr(h, GWL_STYLE);
        SetWindowLongPtr(h, GWL_STYLE, style | WS_CLIPCHILDREN);
        
        g_windowStates[h] = { oldProc, minW, minH, maxW, maxH, 30, 30, 30 };
    } else {
        g_windowStates[h].minW = minW;
        g_windowStates[h].minH = minH;
        g_windowStates[h].maxW = maxW;
        g_windowStates[h].maxH = maxH;
    }

    RECT rect;
    if (GetWindowRect(h, &rect)) {
        int w = rect.right - rect.left;
        int h_val = rect.bottom - rect.top;
        int newW = w; int newH = h_val;
        if (maxW > 0 && w > maxW) newW = maxW;
        if (minW > 0 && w < minW) newW = minW;
        if (maxH > 0 && h_val > maxH) newH = maxH;
        if (minH > 0 && h_val < minH) newH = minH;
        if (newW != w || newH != h_val) {
            SetWindowPos(h, NULL, 0, 0, newW, newH, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastWindowImpl_nSetMaximizable(JNIEnv* env, jobject obj, jlong hwnd, jboolean enabled) {
    HWND h = (HWND)hwnd;
    LONG_PTR style = GetWindowLongPtr(h, GWL_STYLE);
    if (enabled) style |= WS_MAXIMIZEBOX;
    else style &= ~WS_MAXIMIZEBOX;
    SetWindowLongPtr(h, GWL_STYLE, style);
    SetWindowPos(h, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

JNIEXPORT void JNICALL Java_fastwindow_FastWindowImpl_nSetBackgroundColor(JNIEnv* env, jobject obj, jlong hwnd, jint r, jint g, jint b) {
    HWND h = (HWND)hwnd;
    if (g_windowStates.find(h) != g_windowStates.end()) {
        g_windowStates[h].bgR = r; g_windowStates[h].bgG = g; g_windowStates[h].bgB = b;
    }
}

} // extern "C"
