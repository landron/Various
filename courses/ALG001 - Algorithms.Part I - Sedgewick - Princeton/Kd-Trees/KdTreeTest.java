public class KdTreeTest {

    private static void testUnit1() {
        KdTree set = new KdTree();
        assert set.isEmpty();
        assert 0 == set.size();
    }

    private static void testUnit2() {
        KdTree set = new KdTree();

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

        // StdOut.println(set);
    }

    private static void testUnit3() {
        KdTree set = new KdTree();

        Point2D a = new Point2D(1, -1);
        set.insert(a);
        assert set.contains(a);

        Point2D b = new Point2D(1, 0);
        set.insert(b);
        assert set.contains(b);

        Point2D c = new Point2D(0, 0);
        assert !set.contains(c);
        set.insert(c);
        assert set.contains(c);

        //  important since it is the first guaranteed to be on the third level
        Point2D d = new Point2D(-1, 0);
        assert !set.contains(d);
        set.insert(d);
        assert set.contains(d);

        assert !set.isEmpty();
        assert 4 == set.size();
    }

    private static void testUnit4() {
        KdTree set1 = new KdTree();
        SET<Point2D> set2 = new SET<Point2D>();

        for (int i = 0; i < 100; ++i) {
            Point2D p = new Point2D(random(100), random(100));
            // StdOut.println(p);
            set1.insert(p);
            set2.add(p);
        }
        // StdOut.println();
        // StdOut.println(set1);
        // StdOut.println();

        assert set1.size() == set2.size();
        // if (false)
        for (Point2D p : set2) {
            // if (!set1.contains(p)) {
            //     StdOut.println(p);
            // }
            assert set1.contains(p);
        }
    }

    private static void testUnit5() {
        KdTree set = new KdTree();
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
    }

    //  circleN.txt contains N points on the circumference of the circle 
    //      centered on (0.5, 0.5) of radius 0.5.
    private static void testUnit6() {
        KdTree set = new KdTree();
        set.insert(new Point2D(0.206107, 0.095492));    //  A
        set.insert(new Point2D(0.975528, 0.654508));    //  B
        set.insert(new Point2D(0.024472, 0.345492));    //  C
        set.insert(new Point2D(0.793893, 0.095492));    //  D
        set.insert(new Point2D(0.793893, 0.904508));    //  E
        set.insert(new Point2D(0.975528, 0.345492));    //  F
        set.insert(new Point2D(0.206107, 0.904508));    //  G
        set.insert(new Point2D(0.500000, 0.000000));    //  H
        set.insert(new Point2D(0.024472, 0.654508));    //  I
        set.insert(new Point2D(0.500000, 1.000000));    //  J

        RectHV rect = new RectHV(0.75, 0, 1, 1);
        int size = size(set, rect);
        assert 4 == size;

        rect = new RectHV(0.793893, 0, 1, 1);
        size = size(set, rect);
        assert 4 == size;

        rect = new RectHV(0.8, 0, 1, 1);
        size = size(set, rect);
        assert 2 == size;

        rect = new RectHV(0.21, 0.66, 0.78, 0.9);
        size = size(set, rect);
        assert 0 == size;
    }

    private static void testUnit7() {
        KdTree set = new KdTree();
        Point2D p1 = new Point2D(0.2, 0.3);
        Point2D p2 = new Point2D(0.5, 0.7);
        Point2D p3 = new Point2D(0.9, 0.3);
        set.insert(p1);
        set.insert(p2);
        set.insert(p3);

        Point2D res = set.nearest(new Point2D(0.8, 0.4));
        assert p3.equals(res);
        res = set.nearest(new Point2D(0.7, 0.5));
        assert p3.equals(res) || p2.equals(res);
        res = set.nearest(new Point2D(0.1, 0.1));
        assert p1.equals(res);
    }

    private static void testUnit8() {
        KdTree set = new KdTree();
        set.insert(new Point2D(0.206107, 0.095492));    //  A
        Point2D B = new Point2D(0.975528, 0.654508);
        set.insert(B);    //  B
        set.insert(new Point2D(0.024472, 0.345492));    //  C
        set.insert(new Point2D(0.793893, 0.095492));    //  D
        set.insert(new Point2D(0.793893, 0.904508));    //  E
        Point2D F = new Point2D(0.975528, 0.345492);
        set.insert(F);    //  F
        set.insert(new Point2D(0.206107, 0.904508));    //  G
        set.insert(new Point2D(0.500000, 0.000000));    //  H
        set.insert(new Point2D(0.024472, 0.654508));    //  I
        set.insert(new Point2D(0.500000, 1.000000));    //  J

        Point2D res = set.nearest(new Point2D(0.5, 0.5));
        // StdOut.println(res);
        assert F.equals(res);

        res = set.nearest(new Point2D(0.81, 0.30));
        assert F.equals(res);
    }

    private static int size(KdTree set, RectHV rect) {
        Iterable<Point2D> inside = set.range(rect);
        int size = 0;
        for (Point2D p : inside)
            ++size;
        return size;
    }

    private static int random(int N) {
        return (int) (StdRandom.uniform()*N);
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

        if (0 < args.length) {
            In in = new In(args[0]);
            double[] values = in.readAllDoubles();
            assert 0 < values.length;
            KdTree set = new KdTree();
            for (int i = 0; i < values.length; i += 2)
                set.insert(new Point2D(values[i], values[i+1]));
            set.draw();
        }
    }
}