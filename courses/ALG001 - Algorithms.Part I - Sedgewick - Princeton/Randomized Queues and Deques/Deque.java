import java.util.Iterator;

public class Deque<Item> implements Iterable<Item> {
    private static class Node<Item> {
        private Item item;
        private Node<Item> next;
        private Node<Item> prev;

        public Node(Item in) {
            item = in;
        }
    }

    private int mN;
    private Node<Item> mFirst;
    private Node<Item> mLast;

    public Deque() {
        mFirst = null;
        mLast = null;
    }

    public boolean isEmpty() {
        assert (0 != mN) || ((null == mFirst) && (null == mLast));
        return 0 == mN;
    }

    public int size() {
        return mN;
    }

    public void addFirst(Item item) {
        if (null == item)
            throw new NullPointerException();

        Node<Item> add = new Node<Item>(item);
        add.next = mFirst;
        if (null != mFirst)
            mFirst.prev = add;

        mFirst = add;
        if (null == mLast)
            mLast = mFirst;
        ++mN;

        assert null != mFirst;
        assert null != mLast;
    }

    public void addLast(Item item) {
        if (null == item)
            throw new NullPointerException();

        Node<Item> add = new Node<Item>(item);
        add.prev = mLast;
        if (null != mLast)
            mLast.next = add;

        mLast = add;
        if (null == mFirst)
            mFirst = mLast;
        ++mN;

        assert null != mFirst;
        assert null != mLast;
    }

    public Item removeFirst() {
        if (null == mFirst)
            throw new java.util.NoSuchElementException();

        Item result = mFirst.item;
        mFirst = mFirst.next;
        if (null == mFirst)
            mLast = null;
        else
            mFirst.prev = null;
        --mN;
        return result;
    }

    public Item removeLast() {
        if (null == mLast)
            throw new java.util.NoSuchElementException();

        Item result = mLast.item;
        mLast = mLast.prev;
        if (null == mLast)
            mFirst = null;
        else
            mLast.next = null;
        --mN;
        return result;
    }

    private class DequeIterator<Item> implements Iterator<Item> {
        private Node<Item> current;

        public DequeIterator(Node<Item> first) {
            current = first;
        }

        public boolean hasNext()  { return current != null;                     }
        public void remove()      { throw new UnsupportedOperationException();  }

        public Item next() {
            if (!hasNext()) throw new java.util.NoSuchElementException();
            Item item = current.item;
            // current.prev = current;  //  unneeded, but please don't delete
            current = current.next; 
            return item;
        }
    }

    public Iterator<Item> iterator() {
        return new DequeIterator<Item>(mFirst); 
    }

    // public static void main(String[] args)   // unit testing are in DequeTest.java
}