package fastwindow;

import java.awt.Component;

/**
 * FastWindow — Native Windows Window Engine for Java.
 * 
 * <p>FastWindow provides kernel-level control over Win32 window handles (HWND).
 * It is designed to solve the performance and aesthetic limitations of standard 
 * Swing/AWT components, specifically targeting resizing flicker and boundary jitter.</p>
 * 
 * <p><b>Usage:</b></p>
 * <pre>
 * FastWindow engine = FastWindow.attach(myJFrame);
 * engine.setConstraints(400, 300, 1200, 800);
 * engine.setBackgroundColor(30, 30, 30);
 * </pre>
 * 
 * @author FastJava Team
 * @version 0.1.0
 * @see <a href="https://github.com/andrestubbe/FastWindow">FastWindow GitHub</a>
 */
public interface FastWindow {
    
    /**
     * Attaches the native engine to an existing Java Component.
     * <p>This method finds the top-level window ancestor of the component and 
     * subclasses its native message loop.</p>
     * 
     * @param component The Java component (JFrame, JPanel, etc.) to supercharge.
     * @return A FastWindow instance controlling the native handle.
     * @throws IllegalStateException If the component is not displayable.
     */
    static FastWindow attach(Component component) {
        return new FastWindowImpl(component);
    }

    /**
     * Sets strict native constraints on the window size.
     * <p>These constraints are enforced by the Windows Kernel (WM_GETMINMAXINFO), 
     * ensuring that the window cannot be resized beyond these limits, preventing 
     * the "snap-back" effect seen with standard Java setMinimumSize.</p>
     * 
     * @param minWidth  The minimum width in pixels (0 for no limit).
     * @param minHeight The minimum height in pixels (0 for no limit).
     * @param maxWidth  The maximum width in pixels (0 for no limit).
     * @param maxHeight The maximum height in pixels (0 for no limit).
     */
    void setConstraints(int minWidth, int minHeight, int maxWidth, int maxHeight);

    /**
     * Enables or disables the native maximize button and functionality.
     * <p>Toggles the WS_MAXIMIZEBOX style bit and refreshes the window frame.</p>
     * 
     * @param enabled True to enable the maximize button, false to hide/disable it.
     */
    void setMaximizable(boolean enabled);

    /**
     * Sets the native background color used for flicker-free resizing.
     * <p>The native engine intercepts WM_ERASEBKGND and fills the invalidated 
     * area with this color immediately, preventing the white/black flash before 
     * Java draws its contents.</p>
     * 
     * @param r Red component (0-255).
     * @param g Green component (0-255).
     * @param b Blue component (0-255).
     */
    void setBackgroundColor(int r, int g, int b);

    /**
     * Returns the stable native window handle (HWND).
     * <p>This handle can be passed to other FastJava modules (e.g., FastTheme) 
     * for advanced styling and effects.</p>
     * 
     * @return The 64-bit native window handle.
     */
    long getHWND();
}
