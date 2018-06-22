public class PointSET {
    private SET<Point2D> mPoints;

    public PointSET() {
        mPoints = new SET<Point2D>();
    }

    public boolean isEmpty() {
        return (0 == size());
    }
    
    public int size() {
        return mPoints.size();
    }

    public void insert(Point2D p) {
        mPoints.add(p);
    }

    public boolean contains(Point2D p) {
        return mPoints.contains(p);
    }

    public void draw() {
        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.setPenRadius(.01);

        for (Point2D pt : mPoints)
            pt.draw();
    }

    public Iterable<Point2D> range(RectHV rect) {
        // StdOut.println(rect);
        Queue<Point2D> result = new Queue<Point2D>();
        for (Point2D pt : mPoints) {
            // StdOut.print(pt);StdOut.print(": ");StdOut.println(rect.contains(pt));
            if (rect.contains(pt)) 
                result.enqueue(pt);
        }
        return result;
    }

    public Point2D nearest(Point2D p) {
        if (isEmpty())
            return null;
        Point2D result = mPoints.min();
        double distance = p.distanceSquaredTo(result);
        for (Point2D pt : mPoints) {
            final double distance2 = p.distanceSquaredTo(pt);
            // StdOut.print(pt);StdOut.print(": ");StdOut.println(distance2);
            if (distance > distance2) {
                distance = distance2;
                result = pt;
            }
        }
        return result;
    }

    // public String toString() {
    //     return mPoints.toString();
    // }
}