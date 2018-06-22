public class PointSETTest {

    private static void testUnit1() {
        PointSET set = new PointSET();
        assert set.isEmpty();
        assert 0 == set.size();
    }

    private static void testUnit2() {
        PointSET set = new PointSET();

        Point2D a = new Point2D(1, -1);
        set.insert(a);
        assert !set.isEmpty();
        assert 1 == set.size();

        set.insert(a);
        assert !set.isEmpty();
        assert 1 == set.size();

        Point2D a2 = new Point2D(1, -1);
        set.insert(a2);
        assert !set.isEmpty();
        assert 1 == set.size();

        Point2D b = new Point2D(1, 0);
        set.insert(b);
        assert !set.isEmpty();
        assert 2 == set.size();

        assert set.contains(a);
        assert set.contains(b);
        Point2D c = new Point2D(0, 0);
        assert !set.contains(c);
        set.insert(c);
        assert set.contains(c);
        assert !set.isEmpty();
        assert 3 == set.size();

        // StdOut.println(set);
    }

    private static void testUnit3() {
        PointSET set = new PointSET();
        Point2D p1 = new Point2D(0.2, 0.3);
        Point2D p2 = new Point2D(0.5, 0.7);
        Point2D p3 = new Point2D(0.9, 0.3);
        set.insert(p1);
        set.insert(p2);
        set.insert(p3);

        RectHV rect = new RectHV(0.1, 0.2, 0.9, 0.5);
        Iterable<Point2D> inside = set.range(rect);
        int size = 0;
        for (Point2D p : inside)
            ++size;
        assert 2 == size;

        Point2D res = set.nearest(new Point2D(0.8, 0.4));
        assert p3.equals(res);
        res = set.nearest(new Point2D(0.7, 0.5));
        assert p3.equals(res) || p2.equals(res);
        res = set.nearest(new Point2D(0.1, 0.1));
        assert p1.equals(res);
    }

    public static void main(String[] args) {
        testUnit1();
        testUnit2();
        testUnit3();

        if (0 < args.length) {
            In in = new In(args[0]);
            double[] values = in.readAllDoubles();
            assert 0 < values.length;
            PointSET set = new PointSET();
            for (int i = 0; i < values.length; i += 2)
                set.insert(new Point2D(values[i], values[i+1]));
            set.draw();
        }
    }
}