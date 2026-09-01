package fastwindow;

/**
 * FastWindow — Ultra-Fast Win32 Native Window Engine for Java.
 * 
 * <p>FastWindow provides kernel-level, zero-overhead standalone native Win32 window creation
 * specifically designed as the primary window foundation for <b>FastVulkan</b>, <b>FastGraphics</b>,
 * DirectX, and hardware-accelerated composition pipelines.</p>
 * 
 * <p><b>Usage:</b></p>
 * <pre>
 * try (FastNativeWindow window = FastWindow.create("FastVulkan Window", 1024, 600)) {
 *     long hwnd = window.getHWND();
 *     // Initialize Vulkan surface on hwnd...
 * }
 * </pre>
 * 
 * @author FastJava Team
 * @version 0.1.1
 * @see <a href="https://github.com/andrestubbe/FastWindow">FastWindow GitHub</a>
 */
public final class FastWindow {
    
    private FastWindow() {}

    /**
     * Creates a high-performance standalone Win32 Native Window.
     * 
     * @param title  Window title in UTF-16 Unicode.
     * @param width  Client area width in pixels.
     * @param height Client area height in pixels.
     * @return A standalone FastNativeWindow instance.
     */
    public static FastNativeWindow create(String title, int width, int height) {
        return new FastNativeWindow(title, width, height);
    }
}
