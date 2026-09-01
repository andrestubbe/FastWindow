/**
 * @file fastwindow.cpp
 * @brief Native Windows Window Engine for Java (FastWindow).
 * 
 * Implements pure standalone Win32 Native Windows specifically designed
 * as the window foundation for FastVulkan, FastGraphics, DirectX and DWM.
 * 
 * @author FastJava Team
 * @version 0.1.1
 */

#include <jni.h>
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

static const wchar_t* PURE_WINDOW_CLASS_NAME = L"FastNativeWindowClass";

struct StandaloneWindowContext {
    HWND hwnd = nullptr;
    HINSTANCE hInstance = nullptr;
    bool shouldClose = false;
    bool resized = false;
    bool isFullscreen = false;
    RECT savedWindowRect{};
    DWORD savedStyle = 0;
    DWORD savedExStyle = 0;
    int minWidth = 0, minHeight = 0;
    int maxWidth = 0, maxHeight = 0;
    int width = 0, height = 0;
};

// -------------------------------------------------------------
// Pure Native Win32 Window Procedure
// -------------------------------------------------------------
static LRESULT CALLBACK StandaloneWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto ctx = (StandaloneWindowContext*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg) {
    case WM_ERASEBKGND:
        return 1;

    case WM_SIZE:
        if (ctx) {
            ctx->resized = true;
            ctx->width = LOWORD(lParam);
            ctx->height = HIWORD(lParam);
        }
        return 0;

    case WM_SIZING:
        if (ctx) {
            ctx->resized = true;
        }
        return 0;

    case WM_GETMINMAXINFO:
        if (ctx) {
            LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
            if (ctx->minWidth > 0) mmi->ptMinTrackSize.x = ctx->minWidth;
            if (ctx->minHeight > 0) mmi->ptMinTrackSize.y = ctx->minHeight;
            if (ctx->maxWidth > 0) mmi->ptMaxTrackSize.x = ctx->maxWidth;
            if (ctx->maxHeight > 0) mmi->ptMaxTrackSize.y = ctx->maxHeight;
            return 0;
        }
        break;

    case WM_CLOSE:
        if (ctx) ctx->shouldClose = true;
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

// -------------------------------------------------------------
// JNI Exports
// -------------------------------------------------------------
extern "C" {

JNIEXPORT jlong JNICALL Java_fastwindow_FastNativeWindow_nCreateWindow(
    JNIEnv* env, jclass clazz, jstring title, jint width, jint height) {

    if (!title) return 0;
    const jchar* chars = env->GetStringChars(title, nullptr);
    if (!chars) return 0;

    auto ctx = new StandaloneWindowContext();
    ctx->width = width;
    ctx->height = height;
    ctx->hInstance = GetModuleHandle(nullptr);

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = StandaloneWndProc;
    wc.hInstance = ctx->hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = PURE_WINDOW_CLASS_NAME;

    RegisterClassExW(&wc);

    RECT wr = { 0, 0, width, height };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    int winW = wr.right - wr.left;
    int winH = wr.bottom - wr.top;
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int posX = (std::max)(0, (screenW - winW) / 2);
    int posY = (std::max)(0, (screenH - winH) / 2);

    ctx->hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        PURE_WINDOW_CLASS_NAME,
        (LPCWSTR)chars,
        WS_OVERLAPPEDWINDOW,
        posX, posY,
        winW, winH,
        nullptr, nullptr, ctx->hInstance, nullptr
    );

    env->ReleaseStringChars(title, chars);

    if (!ctx->hwnd) {
        delete ctx;
        return 0;
    }

    SetWindowLongPtr(ctx->hwnd, GWLP_USERDATA, (LONG_PTR)ctx);
    return (jlong)ctx;
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nDestroyWindow(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx->hwnd) {
            DestroyWindow(ctx->hwnd);
        }
        delete ctx;
    }
}

JNIEXPORT jboolean JNICALL Java_fastwindow_FastNativeWindow_nPollEvents(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (!handle) return JNI_FALSE;
    auto ctx = (StandaloneWindowContext*)handle;
    if (!ctx->hwnd) return JNI_FALSE;

    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            ctx->shouldClose = true;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return !ctx->shouldClose;
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetTitle(
    JNIEnv* env, jclass clazz, jlong handle, jstring title) {
    if (handle && title) {
        const jchar* chars = env->GetStringChars(title, nullptr);
        if (chars) {
            auto ctx = (StandaloneWindowContext*)handle;
            if (ctx && ctx->hwnd) {
                SetWindowTextW(ctx->hwnd, (LPCWSTR)chars);
            }
            env->ReleaseStringChars(title, chars);
        }
    }
}

JNIEXPORT jlong JNICALL Java_fastwindow_FastNativeWindow_nGetHWND(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (!handle) return 0;
    return (jlong)((StandaloneWindowContext*)handle)->hwnd;
}

JNIEXPORT jint JNICALL Java_fastwindow_FastNativeWindow_nGetWidth(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (!handle) return 0;
    auto ctx = (StandaloneWindowContext*)handle;
    RECT rc;
    GetClientRect(ctx->hwnd, &rc);
    return (jint)(rc.right - rc.left);
}

JNIEXPORT jint JNICALL Java_fastwindow_FastNativeWindow_nGetHeight(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (!handle) return 0;
    auto ctx = (StandaloneWindowContext*)handle;
    RECT rc;
    GetClientRect(ctx->hwnd, &rc);
    return (jint)(rc.bottom - rc.top);
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetLocation(
    JNIEnv* env, jclass clazz, jlong handle, jint x, jint y) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx && ctx->hwnd) {
            SetWindowPos(ctx->hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}

JNIEXPORT jint JNICALL Java_fastwindow_FastNativeWindow_nGetX(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (!handle) return 0;
    auto ctx = (StandaloneWindowContext*)handle;
    RECT rc;
    GetWindowRect(ctx->hwnd, &rc);
    return (jint)rc.left;
}

JNIEXPORT jint JNICALL Java_fastwindow_FastNativeWindow_nGetY(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (!handle) return 0;
    auto ctx = (StandaloneWindowContext*)handle;
    RECT rc;
    GetWindowRect(ctx->hwnd, &rc);
    return (jint)rc.top;
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetDimensions(
    JNIEnv* env, jclass clazz, jlong handle, jint width, jint height) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx && ctx->hwnd) {
            RECT wr = { 0, 0, width, height };
            AdjustWindowRect(&wr, GetWindowLong(ctx->hwnd, GWL_STYLE), FALSE);
            SetWindowPos(ctx->hwnd, nullptr, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetBounds(
    JNIEnv* env, jclass clazz, jlong handle, jint x, jint y, jint width, jint height) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx && ctx->hwnd) {
            RECT wr = { 0, 0, width, height };
            AdjustWindowRect(&wr, GetWindowLong(ctx->hwnd, GWL_STYLE), FALSE);
            SetWindowPos(ctx->hwnd, nullptr, x, y, wr.right - wr.left, wr.bottom - wr.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nCenterOnScreen(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx && ctx->hwnd) {
            RECT rc;
            GetWindowRect(ctx->hwnd, &rc);
            int winW = rc.right - rc.left;
            int winH = rc.bottom - rc.top;
            int screenW = GetSystemMetrics(SM_CXSCREEN);
            int screenH = GetSystemMetrics(SM_CYSCREEN);
            int x = (std::max)(0, (screenW - winW) / 2);
            int y = (std::max)(0, (screenH - winH) / 2);
            SetWindowPos(ctx->hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetVisible(
    JNIEnv* env, jclass clazz, jlong handle, jboolean visible) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx && ctx->hwnd) {
            ShowWindow(ctx->hwnd, visible ? SW_SHOW : SW_HIDE);
            if (visible) UpdateWindow(ctx->hwnd);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetResizable(
    JNIEnv* env, jclass clazz, jlong handle, jboolean resizable) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx && ctx->hwnd) {
            LONG_PTR style = GetWindowLongPtr(ctx->hwnd, GWL_STYLE);
            if (resizable) style |= (WS_THICKFRAME | WS_MAXIMIZEBOX);
            else style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
            SetWindowLongPtr(ctx->hwnd, GWL_STYLE, style);
            SetWindowPos(ctx->hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetAlwaysOnTop(
    JNIEnv* env, jclass clazz, jlong handle, jboolean alwaysOnTop) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        if (ctx && ctx->hwnd) {
            SetWindowPos(ctx->hwnd, alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetFullscreen(
    JNIEnv* env, jclass clazz, jlong handle, jboolean fullscreen) {
    if (!handle) return;
    auto ctx = (StandaloneWindowContext*)handle;
    if (!ctx || !ctx->hwnd || ctx->isFullscreen == (bool)fullscreen) return;

    if (fullscreen) {
        GetWindowRect(ctx->hwnd, &ctx->savedWindowRect);
        ctx->savedStyle = GetWindowLong(ctx->hwnd, GWL_STYLE);
        ctx->savedExStyle = GetWindowLong(ctx->hwnd, GWL_EXSTYLE);

        MONITORINFO mi{ sizeof(mi) };
        HMONITOR hMon = MonitorFromWindow(ctx->hwnd, MONITOR_DEFAULTTOPRIMARY);
        GetMonitorInfo(hMon, &mi);

        SetWindowLong(ctx->hwnd, GWL_STYLE, ctx->savedStyle & ~(WS_CAPTION | WS_THICKFRAME));
        SetWindowLong(ctx->hwnd, GWL_EXSTYLE, ctx->savedExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

        SetWindowPos(ctx->hwnd, HWND_TOP,
                     mi.rcMonitor.left, mi.rcMonitor.top,
                     mi.rcMonitor.right - mi.rcMonitor.left,
                     mi.rcMonitor.bottom - mi.rcMonitor.top,
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        ctx->isFullscreen = true;
    } else {
        SetWindowLong(ctx->hwnd, GWL_STYLE, ctx->savedStyle);
        SetWindowLong(ctx->hwnd, GWL_EXSTYLE, ctx->savedExStyle);

        SetWindowPos(ctx->hwnd, nullptr,
                     ctx->savedWindowRect.left, ctx->savedWindowRect.top,
                     ctx->savedWindowRect.right - ctx->savedWindowRect.left,
                     ctx->savedWindowRect.bottom - ctx->savedWindowRect.top,
                     SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        ctx->isFullscreen = false;
    }
}

JNIEXPORT jboolean JNICALL Java_fastwindow_FastNativeWindow_nIsFullscreen(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (!handle) return JNI_FALSE;
    return ((StandaloneWindowContext*)handle)->isFullscreen ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nMinimize(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (handle) ShowWindow(((StandaloneWindowContext*)handle)->hwnd, SW_MINIMIZE);
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nMaximize(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (handle) ShowWindow(((StandaloneWindowContext*)handle)->hwnd, SW_MAXIMIZE);
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nRestore(
    JNIEnv* env, jclass clazz, jlong handle) {
    if (handle) ShowWindow(((StandaloneWindowContext*)handle)->hwnd, SW_RESTORE);
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetMinSize(
    JNIEnv* env, jclass clazz, jlong handle, jint minW, jint minH) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        ctx->minWidth = minW;
        ctx->minHeight = minH;
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetMaxSize(
    JNIEnv* env, jclass clazz, jlong handle, jint maxW, jint maxH) {
    if (handle) {
        auto ctx = (StandaloneWindowContext*)handle;
        ctx->maxWidth = maxW;
        ctx->maxHeight = maxH;
    }
}

JNIEXPORT void JNICALL Java_fastwindow_FastNativeWindow_nSetIcon(
    JNIEnv* env, jclass clazz, jlong handle, jintArray pixels, jint width, jint height) {
    if (!handle || !pixels || width <= 0 || height <= 0) return;
    auto ctx = (StandaloneWindowContext*)handle;
    if (!ctx->hwnd) return;

    jint* rawPixels = env->GetIntArrayElements(pixels, nullptr);
    if (!rawPixels) return;

    BITMAPV5HEADER bi{};
    bi.bV5Size = sizeof(BITMAPV5HEADER);
    bi.bV5Width = width;
    bi.bV5Height = -height;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask   = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask  = 0x000000FF;
    bi.bV5AlphaMask = 0xFF000000;

    HDC hdc = GetDC(ctx->hwnd);
    void* lpBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &lpBits, nullptr, 0);
    HBITMAP hMonoMask = CreateBitmap(width, height, 1, 1, nullptr);
    ReleaseDC(ctx->hwnd, hdc);

    if (hBitmap && lpBits) {
        memcpy(lpBits, rawPixels, (size_t)width * height * 4);

        ICONINFO ii{};
        ii.fIcon = TRUE;
        ii.hbmMask = hMonoMask;
        ii.hbmColor = hBitmap;

        HICON hIcon = CreateIconIndirect(&ii);
        if (hIcon) {
            SendMessageW(ctx->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessageW(ctx->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
        DeleteObject(hBitmap);
        DeleteObject(hMonoMask);
    }
    env->ReleaseIntArrayElements(pixels, rawPixels, JNI_ABORT);
}

} // extern "C"
