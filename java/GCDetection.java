/**
 * Derek Hessinger and Sumira Naroola
 * Prof. Ying Li
 * CS 333
 * 11/20/24
 * 
 * This program experiments and demonstrates the garbage collection mechanism in Java
 * 
 * javac task3.java
 * java task3
 */
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

public class GCDetection {
    // set variables to hold iterations, objs, list size
    private static final int ITERATIONS = 1000;
    private static final int OBJECTS_PER_ITERATION = 100000;
    private static final int LIST_SIZE = 10000;
    
    private static List<byte[]> l = new ArrayList<>();
    
    // helper function to create a bunch of objects
    public static void createObjects() {
        List<byte[]> tempList = new ArrayList<>();
        
        for (int i = 0; i < OBJECTS_PER_ITERATION; i++) {
            tempList.add(new byte[1024]);
        }
        
        l.clear();
        for (int i = 0; i < LIST_SIZE; i++) {
            l.add(new byte[1024]);
        }
    }
    
    public static void main(String[] args) {
        List<Long> timings = new ArrayList<>();
        double totalTime = 0;
        long maxTime = 0;
        
        // create objects to begin
        System.out.println("Warming up...");
        for (int i = 0; i < 10; i++) {
            createObjects();
        }
        
        System.out.println("Starting measurements...");
        
        // time the length it takes to create objects for each iteration
        for (int i = 0; i < ITERATIONS; i++) {
            long startTime = System.nanoTime();
            createObjects();
            long duration = System.nanoTime() - startTime;
            timings.add(duration);
            
            totalTime += duration;
            maxTime = Math.max(maxTime, duration);
            
            if ((i + 1) % 100 == 0) {
                System.out.printf("Completed %d iterations%n", i + 1);
            }
        }
        
        double avgTime = totalTime / ITERATIONS;
        double stdDev = calculateStdDev(timings, avgTime);
        double threshold = avgTime + (3 * stdDev);
        
        // determine garbage collection events by calculating if duration of iteration is greater than std
        System.out.println("\nPotential Garbage Collection Events:");
        for (int i = 0; i < timings.size(); i++) {
            long timing = timings.get(i);
            if (timing > threshold) {
                System.out.printf("Iteration %d: %d ms (%.2fx average)%n", 
                    i, 
                    TimeUnit.NANOSECONDS.toMillis(timing),
                    (double)timing / avgTime);
            }
        }
        
        System.out.println("\nSummary Statistics:");
        System.out.printf("Average time: %.2f ms%n", TimeUnit.NANOSECONDS.toMillis((long)avgTime) * 1.0);
        System.out.printf("Maximum time: %d ms%n", TimeUnit.NANOSECONDS.toMillis(maxTime));
        System.out.printf("Standard deviation: %.2f ms%n", TimeUnit.NANOSECONDS.toMillis((long)stdDev) * 1.0);
        System.out.printf("Detection threshold: %.2f ms%n", TimeUnit.NANOSECONDS.toMillis((long)threshold) * 1.0);
    }
    
    private static double calculateStdDev(List<Long> values, double mean) {
        double sumSquaredDiff = 0;
        for (long value : values) {
            double diff = value - mean;
            sumSquaredDiff += diff * diff;
        }
        return Math.sqrt(sumSquaredDiff / values.size());
    }
}