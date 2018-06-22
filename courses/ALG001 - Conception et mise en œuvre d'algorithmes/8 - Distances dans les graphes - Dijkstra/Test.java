//	The file was initially a pattern class for a homework, my code: the Dijkstra functions & test units

import java.util.LinkedList;
import java.util.Collection;
import java.util.PriorityQueue;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Arrays;
import java.text.DecimalFormat;

import java.util.Map;
import java.util.SortedMap;
import java.util.Comparator;
import java.util.Collections;

public class Test {

    // pour charger les cartes
    static Carte carte;
    static final String chemin ="./"; // data file location
    Carte ens = new Carte(chemin+"mf.txt");
    


    static HashMap<Ville, HashSet<Ville>> voisins;
    static HashMap<String, HashSet<Ville>> nom;


    static void construitGraphe(Collection<Ville> cv, double minDist) {

	final double R = 6371000;
	final double latDist = minDist * 180.0 / Math.PI / R;
	// des villes à moins de minDist l'une de l'autre auront au plus une différence de
	// latitude (ou de longitude)  de latDist 
	// System.out.println(latDist+" , "+minDist);

	nom = new HashMap<String, HashSet<Ville>>();

	final int n = cv.size();
	VilleDist[] dist = new VilleDist[n];
	// Ville first = cv.iterator().next();
	int i = 0;

	for (Ville v : cv) {
		// System.out.println(v);
		HashSet<Ville> where = nom.get(v.getNom());
		if (null == where)
			where = new HashSet<Ville>();
		boolean result = where.add(v);
		assert result;
		nom.put(v.getNom(), where);

		dist[i++] = new VilleDist(v, v.getLatitude());
	}

	// indication : on peut trier un tableau de villes par Array.sort
	Arrays.sort(dist);
	// for (VilleDist d : dist)
	// 	System.out.println(d);

	// ici construire le graphe des villes en mettant les bonnes valeurs 
	// dans les tables voisins et nom
		voisins = new HashMap<Ville, HashSet<Ville>>();
		for (i = 0; i < n; ++i) 
			voisins.put(dist[i].v, new HashSet<Ville>());
		for (i = 0; i < n; ++i) {
			HashSet<Ville> next = voisins.get(dist[i].v);
			for (int j = i+1; j < n; ++j) {

				// if ((dist[i].v.getNom() == "B"))
				// 	System.out.println(dist[j].v.getNom());

				assert dist[j].v.getLatitude() >= dist[i].v.getLatitude();
				if (dist[j].v.getLatitude() -  dist[i].v.getLatitude() > latDist)
					break;

				//	Error 2:	add it, don't break it
				if (dist[j].v.distance(dist[i].v) > minDist)
					continue;

				{
					next.add(dist[j].v);

					HashSet<Ville> other = voisins.get(dist[j].v);
					other.add(dist[i].v);
					voisins.put(dist[j].v, other);
				}
			}
			voisins.put(dist[i].v, next);

			if (false) 
			{
				System.out.print(dist[i].v.getNom()+" : ");
				for (Ville k : next)
					System.out.print(k.getNom()+",");
				System.out.println();
			}
		}
    }






    static Ville premiereVille(String s) {
	return(nom.get(s).iterator().next());
    }

    static boolean AreEquals(double d1, double d2) {
    	return Math.abs(d1-d2) < 0.000001;
    }

    static double Dijkstra1(Ville orig, Ville dest) {
	// utiliser Dijkstra pour calculer la longueur du plus court chemin
	// entre v1 et v2
	// rendre -1 s'il n'y a pas de chemin

    	if (false) {
	    	System.out.println();
	    	System.out.println("Dijkstra:");
	    	System.out.println("\t"+orig);
	    	System.out.println("\t"+dest);
	    }

    	HashMap<Ville, Double> unvisited = new HashMap<Ville, Double>();
    	// if (false)
    	// for (String i : nom.keySet()) {
    	// 	// System.out.println(i);
    	// 	HashSet<Ville> villes = nom.get(i);
    	// 	for (Ville v : villes) {
    	// 		// System.out.println(v);
    	// 		unvisited.put(v, Double.MAX_VALUE);
    	// 	}
    	// 	// System.out.println();
    	// }

    	//	Error 5:	very slow because we have all the cities here, even if the ones with no score yet aren't needed
    	for (HashSet<Ville> villes : nom.values()){
    		for (Ville v : villes) {
    			// System.out.println(v);
    			unvisited.put(v, Double.MAX_VALUE);
    		}
    	}
    	unvisited.put(orig, 0.0);

    	Double distance = -1.0;
    	//	maybe more villages are to the same distance
    	// Double lastDistance = 0.0;
    	boolean found = false;
    	do {
    		Map.Entry<Ville, Double> next = Collections.min(unvisited.entrySet(), new Comparator<Map.Entry<Ville, Double>>() {
			    public int compare(Map.Entry<Ville, Double> entry1, Map.Entry<Ville, Double> entry2) {
			        return entry1.getValue().compareTo(entry2.getValue());
			    }
			});
			unvisited.remove(next.getKey());
			// if (found && (!AreEquals(next.getValue(),lastDistance)))
			// 	break;

			// if (false)
			// if (0 == unvisited.size() % 1000)
			// 	System.out.println(unvisited.size()+": "+next.getKey());
			// System.out.println(next.getKey()+" : "+RoundIt(next.getValue()));
			// System.out.println(voisins.get(next.getKey()).size());

			if (AreEquals(next.getValue(), Double.MAX_VALUE))
				break;
			// lastDistance = next.getValue();

			for (Ville v : voisins.get(next.getKey())) {
				Double dist = unvisited.get(v);
				if (null == dist)
					continue;
				final double d = next.getKey().distance(v);
				// System.out.println(next.getKey().getNom()+" - "+v.getNom()+" : "+RoundIt(next.getValue())+"+"+RoundIt(d)+" m");
				// System.out.println(next.getKey().getNom()+" - "+v.getNom()+" : "+RoundIt(next.getValue()+d)+" m");
				if (next.getValue() + d < dist)
					unvisited.put(v, next.getValue() + d);

				if (v == dest)
				{
					distance = unvisited.get(v);
					//	Error 3: we must evaluate all the nodes to be sure that we get the shortest path
					// found = true;

					//	Error 4 ??
					break;
				}
			}
    	}
    	//	Error 1: double comparison doesn't work like this: distance != -1.0 
    	while (!found && !unvisited.isEmpty());
    	// while (!unvisited.isEmpty());

      return distance;
    }

    static double Dijkstra2(Ville orig, Ville dest) {
	// utiliser Dijkstra pour calculer la longueur du plus court chemin
	// entre v1 et v2
	// rendre -1 s'il n'y a pas de chemin

    	HashSet<Ville> visited = new HashSet<Ville>();
    	HashMap<Ville, Double> unvisited = new HashMap<Ville, Double>();
    	unvisited.put(orig, 0.0);

    	Double distance = -1.0;
    	while (!unvisited.isEmpty()) {

    		Map.Entry<Ville, Double> next = Collections.min(unvisited.entrySet(), new Comparator<Map.Entry<Ville, Double>>() {
			    public int compare(Map.Entry<Ville, Double> entry1, Map.Entry<Ville, Double> entry2) {
			        return entry1.getValue().compareTo(entry2.getValue());
			    }
			});
			unvisited.remove(next.getKey());
			visited.add(next.getKey());

			for (Ville v : voisins.get(next.getKey())) {
				if (visited.contains(v))
					continue;
				final Double score = unvisited.get(v);
				final Double d = next.getKey().distance(v);
				if ((null == score) || (next.getValue() + d < score)) {
					unvisited.put(v, next.getValue() + d);
				}

				if (v == dest)
					distance = unvisited.get(v);
			}
    	}

      return distance;
    }

    static double Dijkstra(Ville orig, Ville dest) {
    	// return Dijkstra1(orig, dest);
    	return Dijkstra2(orig, dest);
    }


  public static void initMayotte(double minDist){
	Carte ens = new Carte(chemin+"mf.txt");
	construitGraphe(ens.villes(), minDist);
  }

  public static void initFrance(double minDist){
	Carte ens = new Carte(chemin+"fr.txt");
	construitGraphe(ens.villes(), minDist);

  }


    public static void test1(double minDist) {
	System.out.println();
    	System.out.println("Mayotte, pas de "+minDist);
    	initMayotte(minDist);

    	 Ville v1 = premiereVille("Accua") ;
    	 Ville v2 = premiereVille("Moutsamoudou");
    	 Ville v3 = premiereVille("Bandraboua");
    	 Ville v4 = premiereVille("Mambutzou");
	afficheDijkstra(v1, v2);
	afficheDijkstra(v2, v1);
	afficheDijkstra(v1, v3);
	afficheDijkstra(v3, v1);
	afficheDijkstra(v1, v4);
	afficheDijkstra(v4, v1);
	afficheDijkstra(v2, v3);

	// afficheDijkstra(v3, v2);
	// System.out.println(v1.getNom()+" - "+v3.getNom()+" : "+RoundIt(v1.distance(v3))+" m");
    }


    public static double afficheDijkstra(Ville v1, Ville v2) {
	DecimalFormat df = new DecimalFormat("#.000");
	double d = Dijkstra(v1,v2);
	String s = "  pas de chemin";
	// if (d > 0) s = df.format(Dijkstra(v1,v2) / 1000);
	if (d > 0) s = df.format(d / 1000);

	System.out.println(v1.getNom()+" "+v2.getNom()+" "+s);
	return d;
    }


    public static void test2(double minDist) {
	System.out.println();
    	System.out.println("France, pas de "+minDist);

    	initFrance(minDist);

    	Ville paris = premiereVille("Paris") ;
    	Ville rouen = premiereVille("Rouen");
	Ville palaiseau = premiereVille("Palaiseau");
	Ville perpignan = premiereVille("Perpignan");
	Ville strasbourg = premiereVille("Strasbourg");
	Ville hagenau = premiereVille("Hagenau");
	Ville brest = premiereVille("Brest");
	Ville hendaye = premiereVille("Hendaye");
		
    	afficheDijkstra(paris, rouen);
    	afficheDijkstra(palaiseau, rouen);
    	afficheDijkstra(palaiseau, paris);
	afficheDijkstra(paris, perpignan);
	afficheDijkstra(hendaye, perpignan);
	afficheDijkstra(paris, strasbourg);
	afficheDijkstra(hagenau, strasbourg);
	afficheDijkstra(hagenau, brest);


    } 

    static double RoundIt(double nb) {
    	return (double)Math.round(nb*100)/100;
    }

    static void UnitTests_1() {
    	LinkedList<Ville> villes = new LinkedList<Ville>();
    	villes.addLast(new Ville("A", 0, 0));
    	villes.addLast(new Ville("B", 0.01, 0));
    	villes.addLast(new Ville("C", 0.01, 0.01));

    	if (false)
    	for (int i = 0; i < villes.size(); ++i)
    		for (int j = i+1; j < villes.size(); ++j)
    			System.out.println(villes.get(i).getNom()+" - "+villes.get(j).getNom()+" : "+RoundIt(villes.get(i).distance(villes.get(j)))+" m");

    	construitGraphe(villes, 1500);
    	assert 1 == voisins.get(premiereVille("A")).size();
    	assert 2 == voisins.get(premiereVille("B")).size();
    	assert 1 == voisins.get(premiereVille("C")).size();

    	if (false)
    	{
	    	for (int i = 0; i < villes.size(); ++i)
	    		for (int j = i+1; j < villes.size(); ++j)
	    			afficheDijkstra(villes.get(i), villes.get(j));
	    }

    	double d = Dijkstra(premiereVille("A"),premiereVille("B"))/1000;	// km
    	assert 1 < d && d < 2;
    	d = Dijkstra(premiereVille("B"),premiereVille("C"))/1000;	// km
    	assert 1 < d && d < 2;
	    d = Dijkstra(premiereVille("A"),premiereVille("C"))/1000;
	    assert 2 < d && d < 3;

    	construitGraphe(villes, 1600);
    	assert 2 == voisins.get(premiereVille("A")).size();
    	assert 2 == voisins.get(premiereVille("B")).size();
    	assert 2 == voisins.get(premiereVille("C")).size();
    	d = Dijkstra(premiereVille("A"),premiereVille("C"))/1000;
	    assert 1 < d && d < 2;
    }

    static void UnitTests_2() {
    	LinkedList<Ville> villes = new LinkedList<Ville>();
    	villes.addLast(new Ville("A", 0, 0));
    	villes.addLast(new Ville("B", 0.01, 0));
    	villes.addLast(new Ville("C", 0.01, 0.01));
    	villes.addLast(new Ville("D", 0.02, 0.01));
    	villes.addLast(new Ville("E", 0.02, 0));

    	if (false)
    	for (int i = 0; i < villes.size(); ++i)
    		for (int j = i+1; j < villes.size(); ++j)
    			System.out.println(villes.get(i).getNom()+" - "+villes.get(j).getNom()+" : "+RoundIt(villes.get(i).distance(villes.get(j)))+" m");

    	construitGraphe(villes, 1500);
    	assert 1 == voisins.get(premiereVille("A")).size();
    	assert 3 == voisins.get(premiereVille("B")).size();
    	assert 2 == voisins.get(premiereVille("C")).size();
    	assert 2 == voisins.get(premiereVille("D")).size();
    	assert 2 == voisins.get(premiereVille("E")).size();

    	double d = Dijkstra(premiereVille("A"),premiereVille("D"))/1000;
	    assert 3 < d && d < 4;
	    d = Dijkstra(premiereVille("A"),premiereVille("E"))/1000;
	    assert 2 < d && d < 3;
	    for (int i = 0; i < villes.size(); ++i)
    		for (int j = i+1; j < villes.size(); ++j)
    			assert AreEquals(Dijkstra(villes.get(i),villes.get(j)), Dijkstra(villes.get(j),villes.get(i)));
    }

    static void UnitTests() {
    	UnitTests_1();
    	UnitTests_2();
    }

    public static void main (String[] args) {

    	UnitTests();

    	// test1(1850);

    	// test1(2000);

    	// test1(3000);
    	// test1(3400);
    	// test1(4000);

	// //tests sur la carte de France
	// test2(2000);
	// test2(5000);
	// test2(7000);
	// test2(10000);

    }



}
