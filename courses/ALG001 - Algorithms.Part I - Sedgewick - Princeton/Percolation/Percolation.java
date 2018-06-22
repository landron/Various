//  23.II.2013: 
//      passed: java-algs4 -ea Percolation
//              checkstyle-algs4 Percolation.java
//              findbugs-algs4 Percolation.class
//              tests (!final: see input6.txt)
//      backwash + mSite, mTop, mBottom can be probably eliminated

public class Percolation
{
    private int mN;
    //  the columns that give the top & bottom components
    private int mTop, mBottom;    
    private boolean[][] mSite;
    private WeightedQuickUnionUF mFindPercolates;
    
    // create N-by-N grid, with all sites blocked
    public Percolation(int N)              
    {
        assert 0 != N;
        
        mTop = -1;
        mBottom = -1;
        
        mN = N;
        mSite = new boolean[N][N];
        mFindPercolates = new WeightedQuickUnionUF(N*N);
    }
   
    // open site (row i, column j) if it is not already
    public void open(int iI, int jI)         
    {
        if (outOfBounds(iI, jI))
            throw new java.lang.IndexOutOfBoundsException();
        
        final int i = iI-1;
        final int j = jI-1;
        
        if (mSite[i][j])
            return;
        mSite[i][j] = true;
        
        // up
        if (i != 0) 
            unionAddToSecond(i, j, i-1, j);
        else if (-1 == mTop)
            mTop = j;
        else
            unionAddToSecond(i, j, 0, mTop);
        
        // down
        if (i != (mN-1)) 
            unionAddToSecond(i, j, i+1, j);
        else if (-1 == mBottom)
            mBottom = j;
        else
            unionAddToSecond(i, j, mN-1, mBottom);
        
        // left and right
        if (j != 0) 
            union(i, j, i, j-1);
        if (j != (mN-1)) 
            union(i, j, i, j+1);
    }
   
    public boolean isOpen(int i, int j)    // is site (row i, column j) open?
    {
        if (outOfBounds(i, j))
            throw new java.lang.IndexOutOfBoundsException();
        return mSite[i-1][j-1];
    }
   
    public boolean isFull(int i, int j)    // is site (row i, column j) full?
    {
        if (outOfBounds(i, j))
            throw new java.lang.IndexOutOfBoundsException();
        return (-1 != mTop) && mFindPercolates.connected(mTop, ID(i-1, j-1));
    }
   
    public boolean percolates()            // does the system percolate?
    {
        return  (-1 != mTop)
                && (-1 != mBottom) 
                && mFindPercolates.connected(mTop, ID(mN-1, mBottom));
    }
    
    private void union(int i, int j, int i1, int j1)
    {
        assert 0 <= i && 0 <= j && i < mN && j < mN;
        assert 0 <= 1 && 0 <= j1 && i1 < mN && j1 < mN;
        if (mSite[i1][j1])   
            mFindPercolates.union(ID(i, j), ID(i1, j1));
    }

    private void unionAddToSecond(int i, int j, int i1, int j1)
    {
        assert 0 <= i && 0 <= j && i < mN && j < mN;
        assert 0 <= 1 && 0 <= j1 && i1 < mN && j1 < mN;
        if (mSite[i1][j1])   
            mFindPercolates.union(ID(i1, j1), ID(i, j));
    }

    private boolean outOfBounds(int i, int j)
    {
        assert 0 < i && 0 < j && i <= mN && j <= mN;
        return !(0 < i && 0 < j && i <= mN && j <= mN);
    }

    private int ID(int i, int j)
    {
        assert 0 <= i && 0 <= j && 0 < mN && j < mN;
        return mN*i+j;
    }
    
    private static void test1()
    {
        Percolation test = new Percolation(1);
        assert !test.isOpen(1, 1) && !test.isFull(1, 1) && !test.percolates();
        test.open(1, 1);
        assert test.isOpen(1, 1) && test.isFull(1, 1) && test.percolates();
    }
    
    private static void test2()
    {
        int N = 3;
        Percolation test = new Percolation(N);
        for (int i = 1; i <= N; i++) 
            for (int j = 1; j <= N; j++) 
                assert  !test.isOpen(i, j) 
                        && !test.isFull(i, j) 
                        && !test.percolates();
        test.open(2, 2);
        assert !test.percolates();
        test.open(1, 2);
        assert !test.percolates();
        test.open(3, 1);
        assert !test.percolates();
        test.open(3, 2);
        assert test.percolates();
    }
    
    private static void test3()
    {
        int N = 3;
        Percolation test = new Percolation(N);
        test.open(1, 3);
        assert !test.percolates();
        test.open(3, 3);
        assert !test.percolates();
        test.open(1, 1);
        assert !test.percolates();
        test.open(2, 1);
        assert !test.percolates();
        test.open(3, 1);
        assert test.percolates();
    }
    
    private static void test4()
    {
        int N = 3;
        Percolation test = new Percolation(N);
        test.open(1, 3);
        assert !test.percolates();
        test.open(3, 3);
        test.open(1, 1);
        test.open(3, 1);
        test.open(2, 2);
        assert !test.percolates();
        test.open(3, 2);
        assert !test.percolates();
    }

    private static void test5()
    {
        int N = 6;
        Percolation test = new Percolation(N);
        for (int i = 1; i <= N; i++) 
            test.open(i, 6);
        assert test.percolates();
        test.open(5, 5);
        assert test.percolates();
        assert !test.isFull(4, 5);
    }
    
    public static void main(String[] args) 
    { 
        test1();
        test2();
        test3();
        test4();
        test5();
    }
}