 import java.util.HashMap;

 class BoardTest {

    static void testUnit1() {
        Board b1 = new Board(generate(3));
        assert 3 == b1.dimension(); // findbugs

        boolean exc = false;
        try { 
            Board b2 = new Board(generate(1)); 
            assert 3 == b2.dimension(); // findbugs
        } 
        catch (IllegalArgumentException e) 
        { exc = true; }
        assert exc;

        exc = false;
        try { 
            int[][] b3 = new int[3][4];
            for (int i = 0; i < b3.length; ++i)
                for (int j = 0; j < b3[i].length; ++j)
                    b3[i][j] = 1+random(7);
            b3[random(3)][random(4)] = 0;
            Board b4 = new Board(b3);
            //  exception anyway
            assert 666 == b4.dimension();
        } 
        catch (IllegalArgumentException e) 
        { exc = true; }
        assert exc;
    }

    static void testUnit2() {
        Board b1 = new Board(generate(3));
        assert 3 == b1.dimension();

        for (int i = 0; i < 10; ++i) {
            final int N = 5;
            Board b = new Board(generate(N));
            assert 0 <= b.hamming();
            assert N*N > b.hamming();
            assert 0 <= b.manhattan();
            // 2(N-1): max score for an item
            assert 2*(N-1)*N*N > b.manhattan();
        }

        int[][] c4 = new int[3][3];
        for (int i = 0; i < c4.length; ++i)
            for (int j = 0; j < c4[i].length; ++j)
                c4[i][j] = i*c4.length+j+1;
        c4[2][2] = 0;
        Board b2 = new Board(c4);
        assert 0 == b2.hamming();
        assert 0 == b2.manhattan();
        assert b2.isGoal();

        int[][] c5 = new int[3][3];
        int[] c6 = {8, 1, 3, 4, 0, 2, 7, 6, 5};
        for (int i = 0; i < c6.length; ++i)
            c5[i/3][i % 3] = c6[i];
        Board b3 = new Board(c5);
        assert 5 == b3.hamming();
        assert 10 == b3.manhattan();
        assert !b3.isGoal();
    }

    static void testUnit3() {
        Board b = new Board(generate(7));
        assert 7 == b.dimension();
        Board bt = b.twin();
        assert bt.dimension() == b.dimension();
        assert !bt.equals(b);

        int size = 0;
        for (Board i : b.neighbors()) {
            ++size;
            assert !i.equals(b);
        }
        assert 2 <= size;
        assert 4 >= size;

        for (int i = 0; i < 100; ++i) {
            final int N = 5;
            Board bu = new Board(generate(N));

            size = 0;
            for (Board j : bu.neighbors()) {
                ++size;
                assert !j.equals(bu);
            }
            assert 2 <= size;
            assert 4 >= size;
        }

        for (int i = 0; i < 100; ++i) {
            final int N = 5;
            b = new Board(generate(N));
            bt = b.twin();
            // if (areNeighbors(b, bt)) {
            //     StdOut.println(b);
            //     StdOut.println(bt);
            // }
            assert !areNeighbors(b, bt);
        }
    }

    static void testUnit4() {
        Board b = new Board(generate(11));
        assert 11 == b.dimension();

        int[] c1 = {0, 1, 3, 4, 2, 5, 7, 8, 6};
        int[][] c2 = new int[3][3];
        for (int i = 0; i < c1.length; ++i)
            c2[i/3][i % 3] = c1[i];
        b = new Board(c2);
        assert 3 == b.dimension();
        Solver s = new Solver(b);
        assert 5 == s.moves();
        // Iterable<Board> solution = s.solution();
        // for (Board j : solution)
        //     StdOut.println(j);
    }

    static void testUnit5() {
        Board b = new Board(generate(3));
        StdOut.println(b);
        assert 3 == b.dimension();
        Solver s = new Solver(b);
        
        Iterable<Board> solution = s.solution();
        if (null != solution)
            for (Board j : solution)
                StdOut.println(j);
        StdOut.print("Steps: ");
        StdOut.println(s.moves());
    }

    private static boolean areNeighbors(Board a, Board b) {
        Iterable<Board> neighbors = a.neighbors();
        for (Board i : neighbors)
            if (i.equals(b))
                return true;
        return false;
    }

    private static int[][] generate(int N, int max) {
        // StdOut.print(N);StdOut.print(" ");StdOut.println(max);
        assert N*N <= max;

        HashMap<Integer, Boolean> uniqueness = new HashMap<Integer, Boolean>();

        int[][] board = new int[N][N];
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                int temp;
                for (temp = random(max); 
                    uniqueness.containsKey(temp); 
                    temp = random(max)) 
                    //  increment: fool checkstyle
                    ++temp;
                board[i][j] = temp;
                uniqueness.put(temp, true);
            }
        // board[random(N)][random(N)] = 0;

        // StdOut.println(new Board(board));
        return board;
    }

    private static int[][] generate(int N) {
        return generate(N, N*N);
    }

    // private static int[][] generate2(int N, int M) {
    //     return generate(N, M*M);
    // }

    private static int random(int N) {
        return (int) (StdRandom.uniform()*N);
    }

    public static void main(String[] args) {
        testUnit1();
        testUnit2();
        testUnit3();
        testUnit4();
        testUnit5();
    }
}