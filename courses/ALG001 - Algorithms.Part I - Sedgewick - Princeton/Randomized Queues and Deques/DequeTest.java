import java.util.Iterator;

public class DequeTest {

    static void testUnit1() {
        Deque<Integer> d = new Deque<Integer>();
        assert d.isEmpty();
        assert 0 == d.size();
        boolean exc = false;

        try { d.removeFirst(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;
        exc = false;
        try { d.removeLast(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;

        d.addFirst(3);
        assert !d.isEmpty();
        assert 1 == d.size();

        exc = false;
        try { d.addLast(null); } catch (NullPointerException e) 
            { exc = true; }
        assert exc;
        exc = false;
        try { d.addFirst(null); } catch (NullPointerException e) 
            { exc = true; }
        assert exc;
    }

    static void testUnit2() {
        Deque<Integer> d = new Deque<Integer>();

        d.addFirst(3);
        int result = d.removeLast();
        assert d.isEmpty();
        assert 3 == result;

        d.addLast(5);
        result = d.removeFirst();
        assert d.isEmpty();
        assert 5 == result;

        d.addLast(7);
        d.removeLast();
        assert d.isEmpty();

        d.addFirst(11);
        d.removeFirst();
        assert d.isEmpty();

        d.addLast(5);
        d.addFirst(3);
        d.addLast(7);
        result = d.removeFirst();
        assert 3 == result;
        result = d.removeFirst();
        assert 5 == result;
        result = d.removeFirst();
        assert 7 == result;
        assert d.isEmpty();
    }

    static void testUnit3() {
        Deque<Integer> d = new Deque<Integer>();

        int N = 7;
        int[] a = new int[N];

        for (int i = 0; i < N; ++i) {
            a[i] = random(99);
            d.addFirst(a[i]);
        }
        for (int i = 0; i < N; ++i) {
            int result = d.removeLast();
            assert result == a[i];
        }
        assert d.isEmpty();

        for (int i = 0; i < N; ++i)
            d.addLast(a[i]);
        for (int i = 0; i < N; ++i) {
            int result = d.removeFirst();
            assert result == a[i];
        }
        assert d.isEmpty();
    }

    static void testUnit4() {
        Deque<Integer> d = new Deque<Integer>();

        int N = 7;
        int[] a = new int[N];

        for (int i = 0; i < N; ++i) {
            a[i] = random(99);
            d.addFirst(a[i]);
            // StdOut.print(a[i]); StdOut.print(" ");
        }
        // StdOut.println();

        int j = 0;
        for (int i : d) {
            // StdOut.print(i.intValue()); StdOut.print(" "); StdOut.println(a[j]);
            assert i == a[N-(j++)-1];
        }
        assert j == N;

        j = 0;
        Iterator<Integer> i = d.iterator();
        while (i.hasNext()) {
            int next = i.next();
            assert next == a[N-(j++)-1];
        }
        assert j == N;
    }

    static void testUnit5() {
        Deque<Integer> d = new Deque<Integer>();

        Iterator<Integer> i = d.iterator();
        boolean exc = false;
        try { i.remove(); } catch (UnsupportedOperationException e) 
            { exc = true; }
        assert exc;

        exc = false;
        try { i.next(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;

        d.addLast(5);
        i = d.iterator();
        i.next();
        exc = false;
        try { i.next(); } catch (java.util.NoSuchElementException e) 
            { exc = true; }
        assert exc;
    }

    // Test 10: Check iterator() after intermixed calls to addFirst(), addLast(),
    //      removeFirst(), and removeLast()
    //   -  student   length = 8
    //   -  reference length = 7
    //   iterator failed after applying operation 12
    // ==> FAILED
    static void testUnit6() {
        Deque<Integer> d = new Deque<Integer>();

        int N = 70;
        int[] a = new int[N];

        for (int i = 0; i < N; ++i) {
            a[i] = random(99);
            d.addFirst(a[i]);
            // StdOut.print(a[i]); StdOut.print(" ");
        }
        // StdOut.println();

        int j = 0;
        for (int i : d) {
            // StdOut.print(i.intValue()); StdOut.print(" "); StdOut.println(a[j]);
            assert i == a[N-(j++)-1];
            d.addLast(5);
            d.addFirst(8);
            d.removeFirst();
            d.removeLast();
        }
        assert j == N;

        j = 0;
        Iterator<Integer> i = d.iterator();
        while (i.hasNext()) {
            int next = i.next();
            assert next == a[N-(j++)-1];
            d.addFirst(j);
        }
        assert j == N;
    }

    private static int random(int N) {
        return 1+(int) (StdRandom.uniform()*N);
    }

    public static void main(String[] args) {
        testUnit1();    
        testUnit2();
        testUnit3();
        testUnit4();
        testUnit5();
        testUnit6();
    }

}