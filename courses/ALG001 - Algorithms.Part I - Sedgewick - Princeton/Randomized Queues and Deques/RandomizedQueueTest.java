/*
    javac-algs4 *.java
    java-algs4 -ea RandomizedQueueTest
    checkstyle-algs4 *.java
    findbugs-algs4 *.class
*/

import java.util.Iterator;
import java.util.Hashtable;

public class RandomizedQueueTest {

    static void testUnit1() {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();
        assert d.isEmpty();
        assert 0 == d.size();
    }

    static void testUnit2() {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();
        assert d.isEmpty();
        assert 0 == d.size();

        d.enqueue(3);
        assert !d.isEmpty();
        assert 1 == d.size();

        for (int i = 0; i < 9; ++i) {
            d.enqueue(i);
            assert i+2 == d.size();
        }
        assert 10 == d.size();
    }

    static void testUnit3() {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();

        boolean exc = false;
        try { d.dequeue(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;
        exc = false;
        try { d.sample(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;
        exc = false;
        try { d.enqueue(null); } catch (NullPointerException e) 
            { exc = true; }
        assert exc;

        d.enqueue(3);
        assert !d.isEmpty();
        assert 1 == d.size();

        int res = d.dequeue();
        assert 3 == res;
    }

    static void testUnit4() {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();
        Hashtable<Integer, Boolean> hit = new Hashtable<Integer, Boolean>();

        for (int i = 0; i < 100; ++i) {
            d.enqueue(i);
            hit.put(i, true);
        }

        for (int i = 0; i < 100; ++i) {
            int res = d.dequeue().intValue();
            // StdOut.print(res);StdOut.print(" ");
            assert hit.get(res);
            hit.remove(res);
        }
        // StdOut.println();
        assert d.isEmpty();
    }

    static void testUnit5() {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();
        Hashtable<Integer, Boolean> hit = new Hashtable<Integer, Boolean>();

        for (int i = 0; i < 100; ++i) {
            d.enqueue(i);
            hit.put(i, true);
        }

        for (int i = 0; i < 100; ++i) {
            int res = d.dequeue();
            // StdOut.print(res);StdOut.print(" ");
            assert hit.get(res);
            hit.remove(res);
        }
        // StdOut.println();
        assert d.isEmpty();
    }

    static void testUnit6() {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();
        Hashtable<Integer, Boolean> hit = new Hashtable<Integer, Boolean>();

        for (int i = 0; i < 100; ++i) {
            d.enqueue(i);
            hit.put(i, true);
        }

        for (int i : d) {
            // StdOut.print(i);StdOut.print(" ");
            assert hit.get(i);
            hit.remove(i);
        }
        // StdOut.println();
        assert hit.isEmpty();

        for (int i : d)
            hit.put(i, true);

        Iterator<Integer> i = d.iterator();
        while (i.hasNext()) {
            int next = i.next();
            // StdOut.print(next);StdOut.print(" ");
            assert hit.get(next);
            hit.remove(next);
        }
        // StdOut.println();
        assert hit.isEmpty();
    }

    static void testUnit7() {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();

        Iterator<Integer> i = d.iterator();
        boolean exc = false;
        try { i.remove(); } catch (UnsupportedOperationException e) 
            { exc = true; }
        assert exc;

        exc = false;
        try { i.next(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;

        d.enqueue(5);
        i = d.iterator();
        i.next();
        exc = false;
        try { i.next(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;
    }

    // N random calls to: enqueue(), sample(), dequeue(), isEmpty(), and size()
    static void testUnit8(int N) {
        RandomizedQueue<Integer> d = new RandomizedQueue<Integer>();

        for (int i = 0; i < N; ++i) {
            switch ((int) (StdRandom.uniform()*4))
            {
                case 0:
                    d.enqueue(i);
                break;

                case 1:
                    if (!d.isEmpty())
                        d.sample();
                break;

                case 2:
                    if (!d.isEmpty())
                        d.dequeue();
                break;

                case 3:
                    d.size();
                break;

                default:
                    assert false;
            }
        }
    }

    static void testUnit8PrintAndMeasure(int index, int N) {
        Stopwatch watch = new Stopwatch();
        testUnit8(N);
        StdOut.print(index);
        StdOut.print(". Seconds for "); 
        StdOut.print(N); 
        StdOut.print(" : "); 
        StdOut.print(watch.elapsedTime()); 
        StdOut.println();
    }

    static void testUnit8() {

        int[] N = {8, 666, 999};

        int step = 1;
        for (int i = 0; i < N.length; ++i) {
            testUnit8PrintAndMeasure(step++, N[i]);
        }

        int ops = 1024;
        for (int i = 0; i < 16; ++i, ops *= 2) {
            testUnit8PrintAndMeasure(step++, ops);
        }
    }

    public static void main(String[] args) {
        testUnit1();
        testUnit2();  
        testUnit3();
        testUnit4();
        testUnit5();
        testUnit6();
        testUnit7();
        testUnit8();
    }
}