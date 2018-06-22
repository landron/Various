// Assessment Summary
// Compilation:  PASSED
// Style:        PASSED
// Findbugs:     No potential bugs found.
// API:          PASSED

// Correctness:  36/36 tests passed
// Memory:       1/1 tests passed
// Timing:       15/17 tests passed

// Raw score: 97.06% [Correctness: 65%, Memory: 10%, Timing: 25%, Style: 0%]

//  !!! Correctness (HashMap use) loses timing

import java.util.HashMap;
import java.util.Arrays;

public class Fast {

//  http://stackoverflow.com/questions/27581/overriding-equals-and-hashcode-in-java
    private static class Slopes {
        private final Point mOrigin;
        private final double mSlope;

        public Slopes(Point origin, double slope) {
            mOrigin = origin;
            mSlope = slope;
        }

        public int hashCode() {
            return (int) (1000000*mSlope);
        }

        public boolean equals(Object obj) {
            if (obj == null)
                return false;
            if (obj == this)
                return true;
            if (!(obj instanceof Slopes))
                return false;

            Slopes rhs = (Slopes) obj;
            if (mSlope != rhs.mSlope)
                return false;
            return (mSlope == rhs.mOrigin.slopeTo(mOrigin));
        }
    }

    public static void main(String[] args) {
     
        String filename;
        if (1 > args.length)
            throw new java.lang.IllegalArgumentException();
            // filename = "collinear/input8.txt";
        else
            filename = args[0]; 

        In in = new In(filename);
        int N = in.readInt();
        if (4 > N)
            return;
        Point[] p = new Point[N];
        for (int i = 0; i < N; i++) {
            int x = in.readInt();
            int y = in.readInt();
            p[i] = new Point(x, y);

            // StdOut.print(p[i]);
            // StdOut.print(", ");
        }
        // StdOut.println();
        Arrays.sort(p);   //  really needed, although not explicitly demanded

        // final boolean draw = (1 < args.length) && (args[1].contains("draw"));
        final boolean draw = true;
        if (draw) {
            // rescale coordinates and turn on animation mode
            StdDraw.setXscale(0, 32768);
            StdDraw.setYscale(0, 32768);
            StdDraw.show(0);

            for (int i = 0; i < N; i++)
                p[i].draw();
        }

        HashMap<Slopes, Boolean> slopes = new HashMap<Slopes, Boolean>();

        for (int i = 0; i < N-3; ++i) {
            Point[] coll = new Point[N-i];
            for (int j = 0; j < N-i; ++j) 
                coll[j] = p[i+j];
            Arrays.sort(coll, coll[0].SLOPE_ORDER);
            if (false) 
            {
                for (int j = 0; j < N-i; ++j) {
                    StdOut.print(coll[j]);
                    StdOut.print(": ");
                    StdOut.print(coll[j].slopeTo(coll[0]));
                    StdOut.print(", ");
                }
                StdOut.println();
            }

            int k;
            for (int j = 1; j < N-i-2; j = k) {

                final double s = coll[j].slopeTo(coll[0]);                
                for (k = j+1; k < N-i; ++k)
                    if (s != coll[k].slopeTo(coll[0]))
                        break;

                if (1 < (k-(j+1))) {
                    Slopes line = new Slopes(coll[0], s);
                    if (slopes.containsKey(line)) {
                        assert k > j;
                        continue;
                    }
                    slopes.put(line, true);

                    StdOut.print(coll[0]);
                    StdOut.print(" -> ");
                    // Point last = coll[0];
                    for (int l = j; l < (k-1); ++l) {           
                        StdOut.print(coll[l]);
                        StdOut.print(" -> ");
                        // if (draw) {
                        //     last.drawTo(coll[l]);
                        //     last = coll[l];
                        // }
                    }
                    StdOut.print(coll[k-1]);
                    StdOut.println();
                    if (draw)
                        coll[0].drawTo(coll[k-1]);
                }

                assert k > j;
            }
        }

        if (draw)
            StdDraw.show(0);

    }   //  main

}