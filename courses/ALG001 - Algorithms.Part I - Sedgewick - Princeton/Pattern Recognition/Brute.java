import java.util.Arrays;

public class Brute {

 public static void main(String[] args) {
     
  String filename;
  if (1 > args.length)
    throw new java.lang.IllegalArgumentException();
    // filename = "collinear/input8.txt";
  else
    filename = args[0];

  // final boolean draw = (1 < args.length) && (args[1].contains("draw"));
  final boolean draw = true;

  In in = new In(filename);
  int N = in.readInt();
  Point[] p = new Point[N];
  for (int i = 0; i < N; i++) {
    int x = in.readInt();
    int y = in.readInt();
    p[i] = new Point(x, y);
    // StdOut.print(p[i]);
  }
  // StdOut.println();
  Arrays.sort(p);   //  really needed, although not explicitly demanded

  if (draw) {
    // rescale coordinates and turn on animation mode
    StdDraw.setXscale(0, 32768);
    StdDraw.setYscale(0, 32768);
    StdDraw.show(0);

    for (int i = 0; i < N; i++)
      p[i].draw();
  }

  for (int i = 0; i < N; ++i)
         for (int j = N-1; j > i; --j) {
          final double s = p[i].slopeTo(p[j]);
          for (int k = N-1; k > j; --k) 
           if (p[i].slopeTo(p[k]) == s)
            for (int l = N-1; l > k; --l) 
             if (p[i].slopeTo(p[l]) == s) {
              StdOut.print(p[i]);
              StdOut.print(" -> ");
              StdOut.print(p[j]);
              StdOut.print(" -> ");
              StdOut.print(p[k]);
              StdOut.print(" -> ");
              StdOut.print(p[l]);
              StdOut.println();

              if (draw)
                  p[i].drawTo(p[l]);
             }
         }

  if (draw)
    StdDraw.show(0);
 }

}