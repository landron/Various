// Compilation:  PASSED
// Style:        PASSED
// Findbugs:     No potential bugs found.
// API:          PASSED

// Correctness:  17/20 tests passed
// Memory:       8/8 tests passed
// Timing:       34/44 tests passed

// Raw score: 84.57% [Correctness: 65%, Memory: 10%, Timing: 25%, Style: 0%]

//  !!! nearest: still incorrect
/*
Test 6a: Insert N distinct points and call nearest() with random query points
  *  100000 random points in 100000-by-100000 grid
      -  failed on trial 3 of 100000
      -  student   nearest()    = (0.9529, 0.10727)
      -  reference nearest()    = (0.94899, 0.10794)
      -  student   distanceTo() = 0.002941462221412994
      -  reference distanceTo() = 0.002224410034143885
  *  100000 random points in 10000-by-10000 grid
      -  failed on trial 9 of 100000
      -  student   nearest()    = (0.3934, 0.0827)
      -  reference nearest()    = (0.4003, 0.0842)
      -  student   distanceTo() = 0.004964876634922548
      -  reference distanceTo() = 0.002202271554554504
  *  100000 random points in 1000-by-1000 grid
      -  failed on trial 2 of 100000
      -  student   nearest()    = (0.289, 0.392)
      -  reference nearest()    = (0.29, 0.394)
      -  student   distanceTo() = 0.0014142135623730963
      -  reference distanceTo() = 0.0010000000000000009
  *  100000 random points in 100-by-100 grid
      -  failed on trial 2 of 100000
      -  student   nearest()    = (0.32, 0.5)
      -  reference nearest()    = (0.31, 0.5)
      -  student   distanceTo() = 0.010000000000000009
      -  reference distanceTo() = 0.0
  *  100000 random points in 10-by-10 grid
      -  failed on trial 2 of 100000
      -  student   nearest()    = (0.3, 0.8)
      -  reference nearest()    = (0.4, 0.8)
      -  student   distanceTo() = 0.10000000000000003
      -  reference distanceTo() = 0.0
==> FAILED
*/

public class KdTree {
    private Node mRoot;
    private int mSize;

    public KdTree() {
        mRoot = null;
    }

    public boolean isEmpty() {
        return (0 == size());
    }
    
    public int size() {
        return mSize;
    }

    public void insert(Point2D p) {
        mRoot = insert(mRoot, p, true);
    }

    private Node insert(Node where, Point2D p, boolean horizontal) {
        if (null == where) {
            ++mSize;
            return new Node(p, horizontal);
        }
        if (where.horizontal) {
            if (p.x() < where.p.x())
                where.lb = insert(where.lb, p, !where.horizontal);
            else if (!where.p.equals(p))
                where.rt = insert(where.rt, p, !where.horizontal);
        }
        else {
            if (p.y() < where.p.y())
                where.lb = insert(where.lb, p, !where.horizontal);
            else if (!where.p.equals(p))
                where.rt = insert(where.rt, p, !where.horizontal);
        }
        return where;
    }

    public boolean contains(Point2D p) {
        return contains(mRoot, p);
    }

    private boolean contains(Node where, Point2D p) {
        if (null == where)
            return false;
        // StdOut.println(where.p);
        if (where.horizontal) {
            if (p.x() < where.p.x())
                return contains(where.lb, p);
            else {
                if (where.p.equals(p))
                    return true;
                return contains(where.rt, p);
            }
        }
        else {
            if (p.y() < where.p.y())
                return contains(where.lb, p);
            else {
                if (where.p.equals(p))
                    return true;
                return contains(where.rt, p);
            }
        }
    }

    public void draw() {
        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.setPenRadius(.01);

        draw(mRoot, true, 0);
    }

    //  TODO
    //      it doesn't know how to limit the segment to previous segments
    private void draw(Node n, boolean direction, double origin) {
        if (null == n)
            return;

        Point2D a = null;
        Point2D b = null;
        double newOrigin = 0;
        if (n.horizontal) {
            StdDraw.setPenColor(StdDraw.RED);
            if (direction) {
                a = new Point2D(n.p.x(), origin);
                b = new Point2D(n.p.x(), 1);  
            }
            else {
                a = new Point2D(n.p.x(), 0);
                b = new Point2D(n.p.x(), origin);
            }
            newOrigin = n.p.x();
        } else {
            StdDraw.setPenColor(StdDraw.BLUE);
            if (direction) {
                a = new Point2D(origin, n.p.y());
                b = new Point2D(1, n.p.y());
            }
            else {
                a = new Point2D(0, n.p.y());
                b = new Point2D(origin, n.p.y());
            }
            newOrigin = n.p.y();
        }
        a.draw();
        a.drawTo(b);

        draw(n.lb, false, newOrigin);
        draw(n.rt, true, newOrigin);
    }

    public Iterable<Point2D> range(RectHV rect) {
        Queue<Point2D> q = new Queue<Point2D>();
        range(mRoot, q, rect);
        return q;
    }

    private void range(Node n, Queue<Point2D> q, RectHV rect) {
        if (null == n)
            return;
        if (rect.contains(n.p)) {
            q.enqueue(n.p);
            range(n.lb, q, rect);
            range(n.rt, q, rect);
        }
        else if (n.horizontal) {
            if (rect.xmin() < n.p.x())
                range(n.lb, q, rect);
            if (rect.xmax() >= n.p.x())
                range(n.rt, q, rect);
        }
        else {
            if (rect.ymin() < n.p.y())
                range(n.lb, q, rect);
            if (rect.ymax() >= n.p.y())
                range(n.rt, q, rect);
        }
    }

    public Point2D nearest(Point2D p) {
        Nearest found = new Nearest();
        found = nearest(found, mRoot, p);
        return found.point;
    }

    private Nearest nearest(Nearest foundIn, Node n, Point2D p) {
        Nearest found = foundIn;
        found.closer = false;

        if (null == n)
            return found;

        boolean closer = false;
        double distance = n.p.distanceSquaredTo(p);
        if (distance <= found.distance) {
            found.distance = distance;
            found.point = n.p;
            closer = true;
        }

        if (n.horizontal) {
            if (p.x() < n.p.x()) {
                found = nearest(found, n.lb, p);
                if (!found.closer)
                    found = nearest(found, n.rt, p);
            }
            else {
                found = nearest(found, n.rt, p);
                if (!found.closer)
                    found = nearest(found, n.lb, p);
            }
        } 
        else {
            if (p.y() < n.p.y()) {
                found = nearest(found, n.lb, p);
                if (!found.closer)
                    found = nearest(found, n.rt, p);
            }
            else {
                found = nearest(found, n.rt, p);
                if (!found.closer)
                    found = nearest(found, n.lb, p);
            }
        }

        found.closer = closer;
        return found;
    }

    private static class Nearest {
        private Point2D point; 
        private double distance;
        private boolean closer;

        Nearest() {
            point = null;
            distance = Double.MAX_VALUE;
            closer = false;
        }
    }

    private static class Node {
        private Point2D p; 
        private Node lb; 
        private Node rt;
        private boolean horizontal;

        Node(Point2D pt, boolean isHorizontal) {
            assert null != pt;
            p = pt;
            horizontal = isHorizontal;
        }
    }

    // public String toString() {
    //     StringBuilder s = new StringBuilder();
    //     s.append(mSize + " nodes\n");
    //     toString(mRoot, s);
    //     return s.toString();
    // }

    // public void toString(Node n, StringBuilder s) {
    //     if (null == n)
    //         return;
    //     s.append("(" + n.p.x() + ", " + n.p.y() + ") ");
    //     toString(n.lb, s);
    //     toString(n.rt, s);
    // }
}