package fastwindow.benchmark;

import fastwindow.FastNativeWindow;
import fastwindow.FastWindow;
import org.openjdk.jmh.annotations.*;

import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Benchmark)
@Warmup(iterations = 2, time = 1, timeUnit = TimeUnit.SECONDS)
@Measurement(iterations = 3, time = 1, timeUnit = TimeUnit.SECONDS)
@Fork(1)
public class Benchmark {

    private FastNativeWindow nativeWindow;

    @Setup
    public void setup() {
        nativeWindow = FastWindow.create("FastWindow JMH Benchmark", 800, 600);
    }

    @TearDown
    public void tearDown() {
        if (nativeWindow != null) {
            nativeWindow.close();
        }
    }

    @org.openjdk.jmh.annotations.Benchmark
    public boolean benchmarkPollEvents() {
        return nativeWindow.pollEvents();
    }

    @org.openjdk.jmh.annotations.Benchmark
    public void benchmarkSetTitle() {
        nativeWindow.setTitle("FastWindow Benchmark Title");
    }

    @org.openjdk.jmh.annotations.Benchmark
    public void benchmarkSetBounds() {
        nativeWindow.setBounds(100, 100, 800, 600);
    }

    @org.openjdk.jmh.annotations.Benchmark
    public void benchmarkSetConstraints() {
        nativeWindow.setMinimumSize(400, 300);
        nativeWindow.setMaximumSize(1200, 800);
    }
}
