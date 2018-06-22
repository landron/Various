//	The file was initially a pattern class for a homework, my code: connected components counter & test units

import java.util.LinkedList;
import java.util.Collection;
import java.util.Arrays;

// import java.lang.Math;


public class Test {

    // les variables où sera stocké le graphe
    public static Ville[] villes;
    public static LinkedList<Integer>[] voisins;
    public static int[] m_composantes;


    // pour charger les cartes
    static Carte carte;
    static final String chemin ="./"; // data file location
    Carte ens = new Carte(chemin+"mf.txt");
    
    // variables qui serviront au test
    public static int v1, v2, v3, v4, v5;

    static void construitGraphe(Collection<Ville> cv, final double minDist, boolean debug) {
    	final int n = cv.size();
    	villes = new Ville[n];
    	voisins = new LinkedList[n]; // cette instruction cause un warning à la compilation
    	// que vous pouvez ignorer
        m_composantes = null;

    	int k = 0;
    	for (Ville v : cv) {
    	    villes[k] = v;
    	    k++;
    	}

    	// ces deux instructions suivantes ne sont utiles que pour la version optimisée
    	// de construitGraphe : deux villes dont les latitudes diffèrent de plus de latDist
    	// sont forcément à une distance de plus de minDist 
    	double R = 6371000;
    	double latDist = minDist * 180.0 / Math.PI / R;
    	// on rappelle aussi qu'on peut trier le tableau villes par Arrays.sort(villes)

        // if (debug)
        // System.out.println("Construction started: " + minDist);

    	// à compléter
        for (int i = 0; i < n; ++i)
            voisins[i] = new LinkedList();
        for (int i = 0; i < n; ++i) {
            for (int j = i+1; j < n; ++j) {
                final double distance = villes[i].distance(villes[j]);
                if (minDist < distance)
                    continue;
                // System.out.println("Distance ("+i+","+j+"): "+distance);
                voisins[i].addLast(j);
                voisins[j].addLast(i);
            }
        }

        if (false)
        for (k = 0; k < n; ++k) {
            if (voisins[k].isEmpty())
                continue;
            System.out.print("Voisins ("+k+"): ");
            for (int j : voisins[k])
                System.out.print(j+", ");
            System.out.println();
        }

        // if (debug)
        // System.out.println("Construction done");
    }

    //  direct method
    static int compteCC_direct(Ville[] villes, LinkedList<Integer>[] voisins, boolean debug) {

        int[] cc = new int[villes.length];
        for (int i = 0; i < villes.length; ++i)
            cc[i] = i;

        boolean retry = false;
        do {
            retry = false;
            for (int i = 0; i < villes.length; ++i)
                for (int j : voisins[i]) {
                    assert cc[j] <= j;

                    // wrong
                    // if (cc[j] < j)
                    //     continue;

                    if ((j > i) && (cc[i] != cc[j])) {
                        if (debug)
                            System.out.println("Composantes ("+i+","+j+"): "+cc[i]+","+cc[j]);

                        // cc[j] = Math.min(cc[j], cc[i]);

                        // wrong
                        // assert cc[i] <= cc[j];
                        // cc[j] = cc[i];

                        if (cc[i] > cc[j]) {
                            cc[i] = cc[j];
                            retry = true;
                            if (debug)
                                System.out.println("Composantes ("+i+","+j+"): retry");
                        }
                        else 
                            cc[j] = cc[i];
                    }
                }
        }
        while (retry);

        assert null == m_composantes;
        m_composantes = new int[cc.length];
        for (int i = 0; i < villes.length; ++i)
            m_composantes[i] = cc[i];

        int composantesConnexes = 0;
        for (int i = 0; i < villes.length; ++i) {
            assert cc[i] <= i;
            if (cc[i] == i)
                ++composantesConnexes;
        }

        return composantesConnexes;
    }

    static boolean relie_composantes(int v1, int v2) {
        if (null == m_composantes)
            return false;
        return (m_composantes[v1] == m_composantes[v2]);
    }

    public static int compteCC(Ville[] villes, LinkedList<Integer>[] voisins) {
    	return compteCC_direct(villes, voisins, false);
    }

    public static boolean relie(Ville[] villes, LinkedList<Integer>[] voisins, int v1, int v2) {
    	// à compléter
    	return relie_composantes(v1, v2);
    }

    // trouve l'indice de la première ville nommée s
    static int premiereVille(String s) {
        for (int i = 0; i<villes.length; i++)
            if (s.equals(villes[i].getNom())) return (i);
        return(-1);
    }

    public static void initMayotte(double minDist){
        Carte ens = new Carte(chemin+"mf.txt");
        construitGraphe(ens.villes(), minDist, false);
    }

    public static void initFrance(double minDist){
        Carte ens = new Carte(chemin+"fr.txt");
        construitGraphe(ens.villes(), minDist, true);
    }

    // cette fonction teste vos fonctions sur Mayotte
    public static int test1(double minDist) {
    	System.out.println("Mayotte, pas de "+(int)minDist);
    	initMayotte(minDist);
        final int composantes = compteCC(villes, voisins);

        v1 = premiereVille("Accua") ;
        v2 = premiereVille("Moutsamoudou");
        v3 = premiereVille("Bandraboua");
        v4 = premiereVille("Mambutzou");


    	System.out.println("nb composantes : "+composantes);
    	System.out.println(relie(villes, voisins, v1, v2));
    	System.out.println(relie(villes, voisins, v1, v3));
    	System.out.println(relie(villes, voisins, v2, v3));
    	System.out.println(relie(villes, voisins, v2, v4));

        return composantes;
    }


    // cette fonction teste vos fonctions sur la France
    // construitGraphe peut prendre du temps s'il n'est pas optimisé
    // par ailleurs, on doit sans doute augmenter la taille de la pile

    public static int test2(double minDist) {

    	System.out.println("France, pas de "+minDist);
    	initFrance(minDist);
        final int composantes = compteCC(villes, voisins);
    	System.out.println("composantes : "+composantes);
    	
    	v1 = premiereVille("Paris") ;
    	v2 = premiereVille("Rouen");
    	v3 = premiereVille("Ajaccio");
    	v4 = premiereVille("Narbonne");
    	v5 = premiereVille("La Testa");

    	System.out.println(relie(villes, voisins, v1, v2));
    	System.out.println(relie(villes, voisins, v1, v3));
    	System.out.println(relie(villes, voisins, v1, v4));
    	System.out.println(relie(villes, voisins, v3, v5));
    	System.out.println(relie(villes, voisins, v1, v5));

        return composantes;
    }

    static void Connect(LinkedList<Integer>[] voisins, int first, int second) {
        voisins[first].addLast(second);
    }

    static int UnitTest1() {
        Ville[] villes = new Ville[5];
        for (int i = 0; i < villes.length; ++i)
            villes[i] = new Ville(String.valueOf(i+1),0,i);

        LinkedList<Integer>[] voisins = new LinkedList[villes.length];
        for (int i = 0; i < villes.length; ++i)
            voisins[i] = new LinkedList<Integer>();

        Connect(voisins,0,1);
        Connect(voisins,1,4);
        Connect(voisins,2,3);
        Connect(voisins,3,4);

        final int composantes = compteCC(villes, voisins);
        // System.out.println("composantes : "+composantes);
        return composantes;
    }

    static void UnitTestsMayotte() {
        initMayotte(1500);
        assert 45 == compteCC(villes, voisins);
        
        initMayotte(1000);
        assert 69 == compteCC(villes, voisins);
        
        initMayotte(666);
        assert 79 == compteCC(villes, voisins);
        
        initMayotte(500);
        assert 85 == compteCC(villes, voisins);
        
        initMayotte(300);
        assert 87 == compteCC(villes, voisins);
        assert 87 == villes.length;
        
    }

    public static void UnitTests() {
        assert 1 == UnitTest1();
        UnitTestsMayotte();
    }

    public static void main (String[] args) {
        UnitTests();

        // test1(1);   //   87

    	assert 31 == test1(1850);   // 31
    	assert 24 == test1(2000);   // 24
    	assert 4 == test1(3000);   // 4
    	assert 2 == test1(3400);   // 2
    	assert 1 == test1(4000);   // 1

    	// // tests sur la carte de France
    	// // peuvent être longs voire demander d'augmenter la taille de la pile
    	// test2(2000);   //  29437
    	// test2(5000);   //  2157
    	// test2(7000);   //  54
    	// test2(12000);  //  11



    }
}
