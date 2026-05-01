package fastwindow;

import javax.swing.*;
import java.awt.*;

/**
 * FastWindow Step 1 Demo — Testing HWND Capture and Dark Mode.
 */
public class Demo {
    public static void main(String[] args) {
        // Use a Dark Theme for the content to match the title bar
        try {
            UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
        } catch (Exception e) {}

        JFrame frame = new JFrame("FastWindow — Native Turbocharger");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 600);
        frame.setLocationRelativeTo(null);

        // Add some content
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBackground(new Color(30, 30, 30));
        
        JLabel label = new JLabel("FastWindow is active!", SwingConstants.CENTER);
        label.setForeground(Color.WHITE);
        label.setFont(new Font("Segoe UI", Font.BOLD, 24));
        panel.add(label, BorderLayout.CENTER);

        frame.add(panel);

        // --- LIVE REPAINT HOOK ---
        // Force Java to repaint the text immediately during native resize
        frame.addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent e) {
                panel.paintImmediately(0, 0, panel.getWidth(), panel.getHeight());
            }
        });
        
        // --- PRO INITIALIZATION ---
        // Create the native peer without showing the window yet
        frame.addNotify();

        System.out.println("[ENGINE] Attaching FastWindow (Pre-Visibility)...");
        FastWindow nativeWin = FastWindow.attach(frame);
        
        System.out.println("[ENGINE] Enabling Immersive Dark Mode...");
        nativeWin.setDarkTheme(true);

        System.out.println("[ENGINE] Setting Hard Constraints...");
        nativeWin.setConstraints(400, 300, 1500, 960);

        System.out.println("[ENGINE] Syncing Native Background Color...");
        nativeWin.setBackgroundColor(30, 30, 30);

        System.out.println("[ENGINE] Disabling Maximize...");
        nativeWin.setMaximizable(false);

        // Now show the window - it will appear already dark and constrained!
        frame.setVisible(true);

        System.out.println("[ENGINE] Success. No more startup jump!");
    }
}
