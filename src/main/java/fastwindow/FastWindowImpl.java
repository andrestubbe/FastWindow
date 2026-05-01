package fastwindow;

import fastcore.FastCore;
import java.awt.Component;
import java.awt.Window;
import javax.swing.SwingUtilities;

public class FastWindowImpl implements FastWindow {
    
    static {
        FastCore.loadLibrary("fastwindow");
    }

    private final long hwnd;

    public FastWindowImpl(Component component) {
        Window window = SwingUtilities.getWindowAncestor(component);
        if (window == null && component instanceof Window) {
            window = (Window) component;
        }
        
        if (window == null || !window.isDisplayable()) {
            throw new IllegalStateException("Component must be displayable to attach FastWindow!");
        }

        this.hwnd = nGetHWND(window);
        if (this.hwnd == 0) {
            throw new RuntimeException("Failed to capture native HWND!");
        }
    }

    @Override
    public void setConstraints(int minW, int minH, int maxW, int maxH) {
        nSetConstraints(hwnd, minW, minH, maxW, maxH);
    }

    @Override
    public void setMaximizable(boolean enabled) {
        nSetMaximizable(hwnd, enabled);
    }

    @Override
    public void setBackgroundColor(int r, int g, int b) {
        nSetBackgroundColor(hwnd, r, g, b);
    }

    @Override
    public long getHWND() {
        return hwnd;
    }

    // --- Native Methods ---
    private native long nGetHWND(Component c);
    private native void nSetConstraints(long hwnd, int minW, int minH, int maxW, int maxH);
    private native void nSetMaximizable(long hwnd, boolean enabled);
    private native void nSetBackgroundColor(long hwnd, int r, int g, int b);
}
