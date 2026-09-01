package fastwindow;

import fasttheme.FastTheme;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

public class Demo {

    private static BufferedImage createRoundIcon() {
        BufferedImage icon = new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = icon.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g.setColor(Color.WHITE);
        g.fillOval(4, 4, 56, 56);
        g.dispose();
        return icon;
    }

    public static void main(String[] args) {
        // Auto-hide console if started from batch
        long consoleHwnd = FastTheme.getConsoleWindowHandle();
        if (consoleHwnd != 0) {
            FastTheme.setWindowTransparency(consoleHwnd, 0);
        }

        try (FastNativeWindow window = FastWindow.create("FastWindow — Native Black Window Demo", 1024, 600)) {
            // Set native round window icon
            window.setIconImage(createRoundIcon());

            long hwnd = window.getHWND();
            if (hwnd != 0) {
                // Apply Full Black Theme and Dark Mode via FastTheme
                FastTheme.setTitleBarDarkMode(hwnd, true);
                FastTheme.setTitleBarColor(hwnd, 20, 20, 20);
                FastTheme.setTitleBarTextColor(hwnd, 240, 240, 240);
                FastTheme.setWindowBackgroundColor(hwnd, 20, 20, 20);
                FastTheme.setCornerStyle(hwnd, 2); // Windows 11 Rounded corners
            }

            // Show window seamlessly once styled
            window.setVisible(true);

            long lastFpsTime = System.nanoTime();
            int frames = 0;

            while (window.pollEvents()) {
                frames++;
                long now = System.nanoTime();
                if (now - lastFpsTime >= 1_000_000_000L) {
                    window.setTitle("FastWindow Native Black Window - FPS: " + frames);
                    frames = 0;
                    lastFpsTime = now;
                }

                try {
                    Thread.sleep(16); // ~60 Hz tick
                } catch (InterruptedException ignored) {}
            }
        }

        System.out.println("FastWindow demo closed successfully.");
    }
}
