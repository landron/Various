//  TODO    not good enough for the time tests 
//          => sparse buffer implementation (does not need to move the buffer)
import java.util.Iterator;

public class RandomizedQueue<Item> implements Iterable<Item> {
    private int mLength;
    private Item[] mItems;

    public RandomizedQueue() {
        mItems = (Item[]) new Object[8];
        assert 8 == mItems.length;
    }

    public boolean isEmpty() {
        return 0 == mLength;
    }

    public int size() {
        assert mItems.length >= mLength;
        return mLength;
    }

    public void enqueue(Item item) {
        if (null == item)
            throw new NullPointerException();

        assert mItems.length >= mLength;
        if (mItems.length == mLength)
            mItems = resizeUp(mItems);
        assert mItems.length > mLength;

        mItems[mLength++] = item;
    }

    public Item dequeue() {
        if (0 == mLength)
            throw new java.util.NoSuchElementException();
        assert !isEmpty();
        assert mItems.length >= mLength;

        int i = random(mLength);
        assert i < mLength;
        Item result = mItems[i];
        //  TODO this operation is costly
        for (; i < mLength-1; i++)
            mItems[i] = mItems[i+1];
        mItems[i] = null;
        --mLength;

        if (mLength == mItems.length/4) //  only when equal (not smaller: limited to 8 entries)
            mItems = resizeDown(mItems, mLength);

        return result;
    }

    public Item sample() {
        if (0 == mLength)
            throw new java.util.NoSuchElementException();
        assert !isEmpty();
        assert mItems.length >= mLength;

        int i = random(mLength);
        assert i < mLength;
        return mItems[i];
    }

    private class MyIterator<Item> implements Iterator<Item> {
        private int mCurrent;
        private final RandomizedQueue<Item> mQueue;
        private int[] mOrder;

        public MyIterator(RandomizedQueue<Item> queue) {
            mQueue = queue;
            mCurrent = 0;

            mOrder = new int[mQueue.mLength];
            for (int i = 0; i < mOrder.length; i++)
                mOrder[i] = i;
            StdRandom.shuffle(mOrder);
        }

        public boolean hasNext()  { return mCurrent != mOrder.length;           }
        public void remove()      { throw new UnsupportedOperationException();  }

        public Item next() {
            if (!hasNext()) throw new java.util.NoSuchElementException();
            Item item = mQueue.mItems[mOrder[mCurrent]];
            ++mCurrent;
            return item;
        }
    }

    public Iterator<Item> iterator() {
        return new MyIterator<Item>(this); 
    }

    // TODO error when using static: 
    //  non-static type variable Item cannot be referenced from a static context
    private Item[] resizeUp(Item[] in) {
        assert 0 != in.length;
        Item[] out = (Item[]) new Object[2*in.length];
        for (int i = 0; i < in.length; i++)
            out[i] = in[i];
        return out;
    }

    private Item[] resizeDown(Item[] in, int length) {
        assert 0 != in.length;
        if (8 >= in.length)
            return in;
        Item[] out = (Item[]) new Object[in.length/2];
        for (int i = 0; i < length; i++)
            out[i] = in[i];
        return out;
    }

    private static int random(int N) {
        return (int) (StdRandom.uniform()*N);
    }

   // public static void main(String[] args)   // unit testing
}