// Compilation:  PASSED
// Style:        PASSED
// Findbugs:     No potential bugs found.
// API:          PASSED

// Correctness:  21/25 tests passed
// Memory:       8/8 tests passed
// Timing:       17/17 tests passed

// Raw score: 89.60% [Correctness: 65%, Memory: 10%, Timing: 25%, Style: 0%]

/*
    It never finishes:  puzzle32.txt
*/

// import java.util.Comparator;
import java.util.Arrays;

public class Solver {
    private final Board mCurrent;
    private int mSize;
    private Iterable<Board> mSolution;

    public Solver(Board initial) 
    {
        mCurrent = initial;
        mSize = -2;
        mSolution = null;
    }

    //  not the good solution:  the item must keep its previous step
    private static class SmartArray<Item> {
        private Item[] mArr;
        private int mSize;

        public SmartArray() {
        }

        //  TODO  "error: non-static type variable Item 
        //      cannot be referenced from a static context"
        private Item[] reallocate(SmartArray<Item> in, int newlength) {
            assert (null == in.mArr) || (in.mArr.length >= in.mSize);
            assert (newlength >= in.mSize);

            // StdOut.print(in.mSize); StdOut.print(", "); StdOut.println(newlength);

            Item[] arr = (Item[]) new Object[newlength];
            for (int i = 0; i < in.mSize; ++i)
                arr[i] = in.mArr[i];
            return arr;
        }

        public void set(Item toAdd, int position) {
            assert (null == mArr) || (mArr.length >= mSize);

            // StdOut.print(position); StdOut.print(", "); StdOut.println(mSize);

            if (mSize <= position) {
                // if (false)
                if (mSize != position) {
                    StdOut.println(mSize);
                    StdOut.println(position);
                }
                assert ((0 == mSize) || (mSize == position));
                int newlength = 2 * mSize;
                if (newlength < 8)
                    newlength = 8;
                assert (newlength > position);
                mArr = reallocate(this, newlength);
                mArr[position] = toAdd;
            }
            else {
                if ((mArr.length > 16) && (position < mArr.length/4))
                    mArr = reallocate(this, mArr.length/4);
                mArr[position] = toAdd;
                for (int i = position+1; i < mSize; ++i)
                    mArr[i] = null;
            }
            mSize = position + 1;
        }

        public void print() {
            assert (mArr.length >= mSize);
            for (int i = 0; i < mSize; ++i)
                StdOut.println(mArr[i]);
        }

        public Iterable<Item> items() {
            Item[] result = reallocate(this, mSize);
            return Arrays.asList(result);
        }

        // public Item[] itemsArray() {
        //     // for (int i = 0; i < mArr.length; ++i)
        //     //     StdOut.println(mArr[i]);
        //     Item[] result = reallocate(this, mSize);
        //     for (int i = 0; i < result.length; ++i)
        //         StdOut.println(result[i]);
        //     StdOut.println(result);
        //     return result;
        // }
    }
    
    private static Stack<Board> findSolution(Board start)   
    // private Board[] findSolution(Board start) 
    {
        // StdOut.println(start);

        // SmartArray<Board> curr = new SmartArray<Board>();
        // Queue<Board> twin = new Queue<Board>(); //   necessary ??

        MinPQ<BoardStep> currPQ = new MinPQ<BoardStep>();
        MinPQ<BoardStep> twinPQ = new MinPQ<BoardStep>();

        // Board prevCurr = null, prevTwin = null;
        BoardStep i, j;
        for (i = new BoardStep(start), j = new BoardStep(start.twin()); 
            !i.mBoard.isGoal() && !j.mBoard.isGoal(); 
                // curr.enqueue(i), twin.enqueue(j), 
                // prevCurr = i.mBoard, prevTwin = j.mBoard,
                i = currPQ.delMin(), j = twinPQ.delMin()) {
            // StdOut.print("i: ");StdOut.println(i.mBoard);
            for (Board n : i.mBoard.neighbors()) {
                // StdOut.println(n);
                // if (!n.equals(prevCurr))
                if (!n.equals(i.previous()))
                    currPQ.insert(new BoardStep(n, i.mSteps+1, i));
            }
            // StdOut.print("j: ");StdOut.println(j.mBoard);
            for (Board n : j.mBoard.neighbors()) {
                // if (!n.equals(prevTwin))
                if (!n.equals(j.previous()))
                    twinPQ.insert(new BoardStep(n, i.mSteps+1, j));
            }

            // StdOut.println(i.mSteps);StdOut.println(i.mBoard);
            // curr.set(i.mBoard, i.mSteps);
        }

        if (!i.mBoard.isGoal()) 
            return null;

        Stack<Board> solution = new Stack<Board>();
        for (; null != i; i = i.mPrevious)
            solution.push(i.mBoard);
        return solution;
    }

    private void setSolution()   
    {
        Stack<Board> solution = findSolution(mCurrent);
        // StdOut.println(solution);
        if (null != solution)
            // mSolution = solution.items();
            mSolution = solution;
        if (null != mSolution) {
            assert 0 < solution.size();
            mSize =  solution.size() - 1;
        }
        else
            mSize = -1;
    }

    public Iterable<Board> solution()   
    {
        if (-2 == mSize)
            setSolution();
        return mSolution;
    }

    public boolean isSolvable() 
    {
        if (-2 == mSize)
            setSolution();
        return (null != mSolution);
    }

    // min number of moves to solve initial board; -1 if no solution
    public int moves()   
    {
        if (-2 == mSize)
            setSolution();
        return mSize;
    }   

    // private static class BoardStep implements Comparator<BoardStep> {
    // private class BoardStep implements Comparator<BoardStep> {
    private static class BoardStep implements Comparable<BoardStep> {
        private final Board mBoard;
        private final BoardStep mPrevious;
        private final int mSteps;

        public BoardStep(Board board, int steps, BoardStep previous) {
            mBoard = board;
            mSteps = steps;
            mPrevious = previous;
        }

        public BoardStep(Board board) {
            mBoard = board;
            mPrevious = null;
            mSteps = 0;
        }

        private int score() {
            return mBoard.manhattan() + mSteps;
        }

        public Board previous() {
            if (null == mPrevious)
                return null;
            return mPrevious.mBoard;
        }

        public int compare(BoardStep first, BoardStep second) {
            final int score1 = first.score();
            final int score2 = second.score();
            if (score1 < score2) return -1;
            else if (score1 > score2) return 1;
            else return 0;
        }

        public int compareTo(BoardStep second) {
            final int score1 = this.score();
            final int score2 = second.score();
            if (score1 < score2) return -1;
            else if (score1 > score2) return 1;
            else return 0;
        }
    }

    public static void main(String[] args) {
        // create initial board from file
        In in = new In(args[0]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                blocks[i][j] = in.readInt();
        Board initial = new Board(blocks);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }
}