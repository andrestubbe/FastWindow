package fastwindow;

import java.awt.Component;

/**
 * Native Windows Window Engine for Java.
 * Provides high-performance resizing, constraints, and modern aesthetics (Mica/Dark Mode).
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
     * Enables or disables Immersive Dark Mode on the window title bar.
     */
    void setDarkTheme(boolean enabled);

    /**
     * Enables Windows 11 Mica material effect on the window background.
     */
    void enableMica(boolean enabled);

    /**
     * Forces the Windows 11 corner radius style.
     * @param style 0 = Default, 1 = None, 2 = Rounded, 3 = Small Rounded
     */
    void setCornerStyle(int style);

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
