public class PointTest {

        //  TODO
    private static void unitTestSlopeOrder() {
    }

    // unit test
    public static void main(String[] args) {
        
        Point a = new Point(0, 0);
        Point b = new Point(1, 1);
        Point c = new Point(0, 1);
        assert 0 < b.compareTo(a);
        assert 0 == b.compareTo(b);
        assert ((0 < b.compareTo(a)) == (0 > a.compareTo(b)));
        assert Double.NEGATIVE_INFINITY == b.slopeTo(b);
        assert Double.POSITIVE_INFINITY == a.slopeTo(c);
        assert 1 == a.slopeTo(b);
        assert 1 == b.slopeTo(a);

        unitTestSlopeOrder();
    }
}   