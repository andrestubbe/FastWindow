package fastwindow;

import java.awt.Component;

/**
 * FastWindow — Native Windows Window Engine for Java.
 * 
 * Provides high-performance resizing, constraints, and management 
 * of the native window handle (HWND).
 */
public interface FastWindow {
    
    /**
     * Attaches FastWindow logic to an existing Java Component (JFrame, Canvas, etc.)
     * 
     * @param component The Java component to supercharge.
     * @return A FastWindow instance controlling the native handle.
     */
    static FastWindow attach(Component component) {
        return new FastWindowImpl(component);
    }

    /**
     * Sets strict native constraints on window size.
     * These are enforced by the Windows Kernel, preventing jittery resizing.
     */
    void setConstraints(int minWidth, int minHeight, int maxWidth, int maxHeight);

    /**
     * Enables or disables the native maximize button and functionality.
     */
    void setMaximizable(boolean enabled);

    /**
     * Sets the native background color used to fill the window during resizing.
     * This should match your Java UI color to eliminate flickering/blackness.
     */
    void setBackgroundColor(int r, int g, int b);

    /**
     * Returns the native window handle (HWND).
     */
    long getHWND();
}
