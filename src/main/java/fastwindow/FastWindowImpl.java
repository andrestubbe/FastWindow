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
        // Ensure the component is displayable to get the HWND
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
    public void setDarkTheme(boolean enabled) {
        nSetDarkTheme(hwnd, enabled);
    }

    @Override
    public void enableMica(boolean enabled) {
        nEnableMica(hwnd, enabled);
    }

    @Override
    public void setCornerStyle(int style) {
        nSetCornerStyle(hwnd, style);
    }

    @Override
    public void setMaximizable(boolean enabled) {
        nSetMaximizable(hwnd, enabled);
    }

    @Override
    public long getHWND() {
        return hwnd;
    }

    // --- Native Methods ---
    private native long nGetHWND(Component c);
    private native void nSetConstraints(long hwnd, int minW, int minH, int maxW, int maxH);
    private native void nSetDarkTheme(long hwnd, boolean enabled);
    private native void nEnableMica(long hwnd, boolean enabled);
    private native void nSetCornerStyle(long hwnd, int style);
    private native void nSetMaximizable(long hwnd, boolean enabled);
}
