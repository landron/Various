public class Board {
    private int[][] mBlocks;
    private int mScore;

    public Board(int[][] blocks) {
        if (2 > blocks.length)
            throw new java.lang.IllegalArgumentException();

        boolean foundZero = false;
        // mPrevious = new int[blocks.length][blocks.length];
        mBlocks = new int[blocks.length][blocks.length];
        for (int i = 0; i < mBlocks.length; ++i)
            for (int j = 0; j < mBlocks.length; ++j) {
                if (blocks[i].length != blocks.length)
                    throw new java.lang.IllegalArgumentException(); 
                if (blocks[i][j] >= (blocks.length*blocks.length))
                    throw new java.lang.IllegalArgumentException();
                mBlocks[i][j] = blocks[i][j];
                // mPrevious[i][j] = 0;

                if (0 == blocks[i][j]) {
                    assert !foundZero;
                    foundZero = true;
                }
            }
        assert foundZero;

        mScore = -1;
    }

    public int dimension() {
        return mBlocks.length;
    }

    public int hamming() {
        int score = 0;
        for (int i = 0; i < mBlocks.length; ++i)
            for (int j = 0; j < mBlocks.length; ++j)
                if ((0 != mBlocks[i][j]) 
                    && ((i*mBlocks.length+j+1) != mBlocks[i][j]))
                    ++score;
        // score += mSteps;
        return score;
    }

    public int manhattan() {
        if (0 <= mScore)
            return mScore;

        int score = 0;
        for (int i = 0; i < mBlocks.length; ++i)
            for (int j = 0; j < mBlocks.length; ++j)
                if (0 != mBlocks[i][j]) {
                    score += Math.abs((mBlocks[i][j]-1)/mBlocks.length-i);
                    score += Math.abs((mBlocks[i][j]-1) % mBlocks.length-j);
                    // StdOut.println(score);
                }
        // score += mSteps;
        // StdOut.println(score);
        // StdOut.println(this);

        mScore = score;
        return score;
    }

    public boolean isGoal() {
        return (0 == hamming());
    }

    // a board obtained by exchanging two adjacent blocks in the same row
    //  by swapping any pair of adjacent (!!! non-blank) blocks in the same row.
    public Board twin() {
        int row = 0;
        int column = 0;
        do {
            row = random(mBlocks.length);
            column = random(mBlocks.length-1);
        } while ((0 == mBlocks[row][column]) || (0 == mBlocks[row][column+1]));

        Board twin = copy(this);
        twin.mBlocks[row][column] = mBlocks[row][column+1];
        twin.mBlocks[row][column+1] = mBlocks[row][column];
        return twin;
    }  

    public boolean equals(Object y)
    {
        if (y == this) return true;
        if (y == null) return false;
        if (y.getClass() != this.getClass())
            return false;

        Board that = (Board) y;
        if (!equals(this, that))
            return false;
        //  TODO    mPrevious, mSteps
        return true;
    }

    public Iterable<Board> neighbors() {
        int row = -1;
        int column = -1;
        for (int i = 0; i < mBlocks.length; ++i)
            for (int j = 0; j < mBlocks.length; ++j)
                if (0 == mBlocks[i][j]) {
                    row = i;
                    column = j;
                    break;
                }
        assert -1 != row;
        assert -1 != column;

        //  TODO   consider "mPrevious"

        Queue<Board> neighbors = new Queue<Board>();
        if (0 != row) {
            Board temp = copy(this);
            temp.mBlocks[row-1][column] = this.mBlocks[row][column];
            temp.mBlocks[row][column] = this.mBlocks[row-1][column];
            neighbors.enqueue(temp);
        }
        if ((mBlocks.length-1) != row) {
            Board temp = copy(this);
            temp.mBlocks[row+1][column] = this.mBlocks[row][column];
            temp.mBlocks[row][column] = this.mBlocks[row+1][column];
            neighbors.enqueue(temp);
        }
        if (0 != column) {
            Board temp = copy(this);
            temp.mBlocks[row][column-1] = this.mBlocks[row][column];
            temp.mBlocks[row][column] = this.mBlocks[row][column-1];
            neighbors.enqueue(temp);
        }
        if ((mBlocks.length-1) != column) {
            Board temp = copy(this);
            temp.mBlocks[row][column+1] = this.mBlocks[row][column];
            temp.mBlocks[row][column] = this.mBlocks[row][column+1];
            neighbors.enqueue(temp);
        }
        return neighbors;
    }
    
    public String toString() {
        StringBuilder str = new StringBuilder();
        str.append(mBlocks.length);
        str.append("\n");
        for (int i = 0; i < mBlocks.length; ++i) {
            for (int j = 0; j < mBlocks.length; ++j) {
                // str.append(" ");
                // str.append(mBlocks[i][j]);
                str.append(String.format("%2d ", mBlocks[i][j]));
            }
            str.append("\n");
        }
        return str.toString();
    }

    private static Board copy(Board v) {
        int[][] copy = new int[v.mBlocks.length][v.mBlocks.length];
        for (int i = 0; i < v.mBlocks.length; ++i)
            for (int j = 0; j < v.mBlocks.length; ++j)
                copy[i][j] = v.mBlocks[i][j];
        return new Board(copy);
    }

    private static boolean equals(Board x, Board y) {
        if (x.mBlocks.length != y.mBlocks.length)
            return false;
        for (int i = 0; i < x.mBlocks.length; ++i)
            for (int j = 0; j < x.mBlocks.length; ++j)
                if (x.mBlocks[i][j] != y.mBlocks[i][j])
                    return false;
        return true;
    }

    private static int random(int N) {
        return (int) (StdRandom.uniform()*N);
    }
}