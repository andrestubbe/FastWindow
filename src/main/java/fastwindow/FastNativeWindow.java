package fastwindow;

import fastcore.FastCore;
import java.awt.image.BufferedImage;

/**
 * FastNativeWindow — Standalone, ultra-high performance native Win32 window.
 * 
 * <p>Created via {@link FastWindow#create(String, int, int)} without AWT/Swing overhead.</p>
 */
public class FastNativeWindow implements AutoCloseable {

    static {
        FastCore.loadLibrary("fastwindow");
    }

    private long nativeHandle;
    private final long hwnd;

    public FastNativeWindow(String title, int width, int height) {
        this.nativeHandle = nCreateWindow(title, width, height);
        if (this.nativeHandle == 0) {
            throw new RuntimeException("Failed to create native Win32 window");
        }
        this.hwnd = nGetHWND(this.nativeHandle);
    }

    public boolean pollEvents() {
        if (nativeHandle == 0) return false;
        return nPollEvents(nativeHandle);
    }

    public boolean isOpen() {
        return nativeHandle != 0;
    }

    public long getHWND() {
        return hwnd;
    }

    public void setTitle(String title) {
        if (nativeHandle != 0) {
            nSetTitle(nativeHandle, title);
        }
    }

    public int getWidth() {
        if (nativeHandle == 0) return 0;
        return nGetWidth(nativeHandle);
    }

    public int getHeight() {
        if (nativeHandle == 0) return 0;
        return nGetHeight(nativeHandle);
    }

    public void setLocation(int x, int y) {
        if (nativeHandle != 0) {
            nSetLocation(nativeHandle, x, y);
        }
    }

    public int getX() {
        if (nativeHandle == 0) return 0;
        return nGetX(nativeHandle);
    }

    public int getY() {
        if (nativeHandle == 0) return 0;
        return nGetY(nativeHandle);
    }

    public void setSize(int width, int height) {
        if (nativeHandle != 0) {
            nSetDimensions(nativeHandle, width, height);
        }
    }

    public void setBounds(int x, int y, int width, int height) {
        if (nativeHandle != 0) {
            nSetBounds(nativeHandle, x, y, width, height);
        }
    }

    public void centerOnScreen() {
        if (nativeHandle != 0) {
            nCenterOnScreen(nativeHandle);
        }
    }

    public void setVisible(boolean visible) {
        if (nativeHandle != 0) {
            nSetVisible(nativeHandle, visible);
        }
    }

    public void setResizable(boolean resizable) {
        if (nativeHandle != 0) {
            nSetResizable(nativeHandle, resizable);
        }
    }

    public void setAlwaysOnTop(boolean alwaysOnTop) {
        if (nativeHandle != 0) {
            nSetAlwaysOnTop(nativeHandle, alwaysOnTop);
        }
    }

    public void setFullscreen(boolean fullscreen) {
        if (nativeHandle != 0) {
            nSetFullscreen(nativeHandle, fullscreen);
        }
    }

    public boolean isFullscreen() {
        if (nativeHandle == 0) return false;
        return nIsFullscreen(nativeHandle);
    }

    public void minimize() {
        if (nativeHandle != 0) {
            nMinimize(nativeHandle);
        }
    }

    public void maximize() {
        if (nativeHandle != 0) {
            nMaximize(nativeHandle);
        }
    }

    public void restore() {
        if (nativeHandle != 0) {
            nRestore(nativeHandle);
        }
    }

    public void setMinimumSize(int minWidth, int minHeight) {
        if (nativeHandle != 0) {
            nSetMinSize(nativeHandle, minWidth, minHeight);
        }
    }

    public void setMaximumSize(int maxWidth, int maxHeight) {
        if (nativeHandle != 0) {
            nSetMaxSize(nativeHandle, maxWidth, maxHeight);
        }
    }

    public void setIconImage(BufferedImage image) {
        if (nativeHandle != 0 && image != null) {
            int w = image.getWidth();
            int h = image.getHeight();
            int[] pixels = new int[w * h];
            image.getRGB(0, 0, w, h, pixels, 0, w);
            nSetIcon(nativeHandle, pixels, w, h);
        }
    }

    @Override
    public void close() {
        if (nativeHandle != 0) {
            nDestroyWindow(nativeHandle);
            nativeHandle = 0;
        }
    }

    // JNI Native methods
    private static native long nCreateWindow(String title, int width, int height);
    private static native void nDestroyWindow(long handle);
    private static native boolean nPollEvents(long handle);
    private static native void nSetTitle(long handle, String title);
    private static native long nGetHWND(long handle);
    private static native int nGetWidth(long handle);
    private static native int nGetHeight(long handle);
    private static native void nSetLocation(long handle, int x, int y);
    private static native int nGetX(long handle);
    private static native int nGetY(long handle);
    private static native void nSetDimensions(long handle, int width, int height);
    private static native void nSetBounds(long handle, int x, int y, int width, int height);
    private static native void nCenterOnScreen(long handle);
    private static native void nSetVisible(long handle, boolean visible);
    private static native void nSetResizable(long handle, boolean resizable);
    private static native void nSetAlwaysOnTop(long handle, boolean alwaysOnTop);
    private static native void nSetFullscreen(long handle, boolean fullscreen);
    private static native boolean nIsFullscreen(long handle);
    private static native void nMinimize(long handle);
    private static native void nMaximize(long handle);
    private static native void nRestore(long handle);
    private static native void nSetMinSize(long handle, int minW, int minH);
    private static native void nSetMaxSize(long handle, int maxW, int maxH);
    private static native void nSetIcon(long handle, int[] pixels, int width, int height);
}
