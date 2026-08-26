package fastwindow.benchmark;

import fastwindow.FastWindow;
import org.openjdk.jmh.annotations.*;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Benchmark)
@Warmup(iterations = 2, time = 1, timeUnit = TimeUnit.SECONDS)
@Measurement(iterations = 3, time = 1, timeUnit = TimeUnit.SECONDS)
@Fork(1)
public class Benchmark {

    private JFrame frame;
    private FastWindow fastWindow;

    @Setup
    public void setup() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            frame = new JFrame("FastWindow Benchmark");
            frame.setSize(800, 600);
            frame.setVisible(true);
        });
        try {
            fastWindow = FastWindow.attach(frame);
        } catch (Exception e) {
            fastWindow = null;
        }
    }

    @TearDown
    public void tearDown() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            if (frame != null) {
                frame.dispose();
            }
        });
    }

    @org.openjdk.jmh.annotations.Benchmark
    public void benchmarkSetConstraints() {
        if (fastWindow != null) {
            fastWindow.setConstraints(400, 300, 1200, 800);
        }
    }

    @org.openjdk.jmh.annotations.Benchmark
    public void benchmarkSetBackgroundColor() {
        if (fastWindow != null) {
            fastWindow.setBackgroundColor(30, 30, 30);
        }
    }
}
