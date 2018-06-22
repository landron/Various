// Compilation:  PASSED
// Style:        PASSED
// Findbugs:     No potential bugs found.
// API:          PASSED

// Correctness:  35/35 tests passed
// Memory:       49/49 tests passed
// Timing:       12/24 tests passed

// Raw score: 87.50% [Correctness: 65%, Memory: 10%, Timing: 25%, Style: 0%]


// Test 3 (bonus): Check that maximum size of any or Deque or RandomizedQueue object
//                 created is <= k
//  failed

import java.util.Iterator;

public class Subset {
    public static void main(String[] args) {
        if (1 > args.length)
            throw new java.lang.IllegalArgumentException();
        int k = Integer.parseInt(args[0]);
        assert 0 < k;
        if (0 > k)
            throw new java.lang.IllegalArgumentException();

        RandomizedQueue<String> q = new RandomizedQueue<String>();
        while (!StdIn.isEmpty()) {
            // StdOut.print(StdIn.readString()); 
            // StdOut.println();
            q.enqueue(StdIn.readString());
        }
        assert k <= q.size();
        if (k > q.size())
            throw new java.lang.IllegalArgumentException();

        Iterator<String> it = q.iterator();
        for (int i = 0; i < k; ++i) {
            StdOut.print(it.next()); 
            StdOut.println();
        }
    }
}