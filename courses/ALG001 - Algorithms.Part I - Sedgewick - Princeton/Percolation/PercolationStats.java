public class PercolationStats
{
    private int mT;
    private int mN;
    private double[] mX;
    private double mMean;
    private double mStddev;
    
    // perform T independent computational experiments on an N-by-N grid
    public PercolationStats(int N, int T) 
    {
        if ((T <= 0) || (N <= 0))
            throw new java.lang.IllegalArgumentException();
        
        mT = T;
        mN = N;
        mX = new double[mT];
        
    }

    // sample mean of percolation threshold   
    public double mean()   
    {
        if (0 != mMean)
            return mMean;
        mMean = StdStats.mean(mX);
        return mMean;
    }

    // sample standard deviation of percolation threshold                  
    public double stddev() 
    {
        if (0 != mStddev)
            return mStddev;
        mStddev = StdStats.stddev(mX);
        return mStddev;
    }

    // returns lower bound of the 95% confidence interval                  
    public double confidenceLo()  
    {
        assert 0 != mStddev;
        assert 0 != mMean;

        return (mMean - (1.96*mStddev)/Math.sqrt(mT));
    }

    // returns upper bound of the 95% confidence interval           
    public double confidenceHi()   
    {
        assert 0 != mStddev;
        assert 0 != mMean;

        return (mMean + (1.96*mStddev)/Math.sqrt(mT));
    }

    private int random()
    {
        return 1+(int) (StdRandom.uniform()*mN);
    }

    private int percolate()
    {
        assert 0 < mN;

        Percolation percolation = new Percolation(mN);
        int i = 0;
        for (; !percolation.percolates(); ++i)
        {
            int x = random();
            int y = random();
            while (percolation.isOpen(x, y))
            {
                x = random();
                y = random();
            }
            percolation.open(x, y);
            // System.out.println(x+" "+y);
        }
        // System.out.println(i);
        return i;
    }   

    private void percolateAll()
    {
        assert 0 < mT;

        for (int i = 0; i < mT; ++i)
            mX[i] = (double) percolate()/(mN*mN);
    }       

    public static void main(String[] args) 
    {
        if (2 > args.length)
            throw new java.lang.IllegalArgumentException();
        
        PercolationStats stats = new PercolationStats(
            Integer.parseInt(args[0]), Integer.parseInt(args[1]));
        stats.percolateAll();
        StdOut.print("mean                    = "); 
        StdOut.print(stats.mean());
        StdOut.println();
        StdOut.print("stddev                  = "); 
        StdOut.print(stats.stddev());
        StdOut.println();
        StdOut.print("95% confidence interval = "); 
        StdOut.print(stats.confidenceLo());
        StdOut.print(", ");
        StdOut.print(stats.confidenceHi());
        StdOut.println();
    }
};