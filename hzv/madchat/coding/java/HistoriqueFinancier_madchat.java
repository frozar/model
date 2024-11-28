/************************************************************************/
/* Ceci est un programme assurant la gestion d'un historique financier. */
/* Un historique comporte plusieurs postes (loyer, voiture, nourriture, */
/* voyages, sante, ... bref, ce que vous voulez). Chaque poste comporte */
/* une liste de mouvements (retrait ou depot) caracterises par une date */
/* et une somme.							*/
/* Une interface graphique est egalement disponible dans un autre 	*/
/* fichier qui n'est pas tout a fait au point (au 15/12/02) mais qui	*/
/* sera uploadee sur Madchat des qu'elle le sera (au point). 		*/
/*									*/
/*	Auteur : Erwann Thoraval (eochu@free.fr)			*/
/************************************************************************/


package historiquefinancier;

import java.io.*;
import java.util.*;
import javax.swing.*;


/************************************************************************/
/*  Definition de la classe abstraite Mouvement				*/
/************************************************************************/

// Classe abstraite factorisant les comportements des
// classes Depot et Retrait
abstract class Mouvement {
	Date date;	// un mouvement est caracterise par une date
	int somme;	// et une somme

	// Constructeur de la classe
	public Mouvement(int i) {
		date = new Date();
		somme = i;
	}

	// Retourne le montant de la depense si la somme est negative
	// ie, si le mouvement est un retrait
	public int getDepense() {
		if (somme>0) return 0;
		return -somme;
	}

	// Retourne le montant de la recette si la somme est positive
	// ie, si le mouvement est un depot.
	public int getRecette() {
		if (somme<0) return 0;
		return somme;
	}

	// Methode utilisee par l'interface graphique pour l'affichage 
	// des mouvements (définie dans les classe Depot et Retrait)
        public abstract String toString();
}


/************************************************************************/
/*  Definition de la classe Retrait (derivee de Mouvement)		*/
/************************************************************************/

// Classe Retrait derivant de la classe abstraite Mouvement
class Retrait extends Mouvement {

	// Constructeur. On appelle le constructeur de la classe Mouvement
	// en passant en parametre le montant negatif (car c'est un retrait)
	public Retrait(int valeur) {
		super(-valeur);
	}

	// Methode d'affichage pour l'interface graphique
	// (appelee inplicitement)
        public String toString() {
          return ("Retrait : " + somme);
        }
}


/************************************************************************/
/*  Definition de la classe Depot (derivee de Mouvement)		*/
/************************************************************************/

// Classe Depot derivant de la classe abstraite Mouvement
class Depot extends Mouvement {

	// Constructeur. On appelle le constructeur de la classe Mouvement
	// en passant en parametre le montant positif (car c'est un depot)
	public Depot(int valeur) {
		super(valeur);
	}

	// Methode d'affichage pour l'interface graphique
	// (appelee inplicitement)
        public String toString() {
          return ("Depot : " + somme);
        }
}


/************************************************************************/
/*  Definition de la classe PosteBudgetaire				*/
/************************************************************************/

class PosteBudgetaire {
	// Un poste budgetaire est caracterise par un nom, une liste de
	// mouvements ainsi que le montant total recu et depense pour ce
	// poste. La variable mouvements est public car elle va etre
	// utilisee par l'interface graphique.
	String label;
	int totalRecu;
	int totalDepense;
	public DefaultListModel mouvements;

	// Constructeur
	public PosteBudgetaire(String NomDuPoste) {
		label = NomDuPoste;
		totalRecu = 0;
		totalDepense = 0;
		mouvements = new DefaultListModel();
	}

	// On credite le poste. On ajoute le montant credite a la 
	// variable totalRecu et on cree une nouvelle operation de
	// type Depot que l'on va ajouter a la liste des mouvements.
	public void recevoirSomme(int montant) {
		totalRecu += montant;
		Depot operation = new Depot(montant);
		mouvements.addElement(operation);
	}

	// idem que pour la methode recevoirSomme mais pour un retrait.
	public void depenserSomme(int montant) {
		totalDepense += montant;
		Retrait operation = new Retrait(montant);
		mouvements.addElement(operation);
	}

	// Les 4 methodes qui suivent permettent d'acceder aux variables
	// d'instances de la classe.
	public int getTotalRecu() {
		return totalRecu;
	}

	public int getTotalDepense() {
		return totalDepense;
	}

	public int getSolde() {
		return (totalRecu - totalDepense);
	}

	public String getNom() {
		return label;
	}

	// Methode utilisee par l'interface graphique pour afficher
	// le nom du poste.
	public String toString() {
		return label;
	}

	// Affichage des mouvements (utilisee pour le mode texte)
	public void afficherPoste() {
		int nb_elements = this.mouvements.getSize();
		int i = 0;
		Mouvement un_mouvement;
		// on affiche le nom du poste ainsi que son solde
		System.out.println("Nom du poste   : " + this.getNom());
		System.out.println("Solde du poste : " + this.getSolde());
		
		// on boucle sur le nombre d'elements pour afficher tous les
		// mouvements concernant ce poste.
		while(i != nb_elements) {
			un_mouvement = (Mouvement) this.mouvements.elementAt(i);

			System.out.println("Date : " + un_mouvement.date + " Somme : " +
			un_mouvement.somme);
			i = i+1;
		}
	}
}



/************************************************************************/
/*  Definition de la classe PosteBudgetaire				*/
/************************************************************************/

// Un historique financier est caracterise par un solde et une liste de
// postes budgetaires.
public class HistoriqueFinancier {
	int solde;
	public DefaultListModel postes;

	// Constructeur
	public HistoriqueFinancier() {
		solde = 0;
		postes = new DefaultListModel();
	}

	// Retourne le solde de l'historique
	public int getSolde() {
		return solde;
	}


	// Recherche d'un poste Budgetaire en fonction de son nom
	public PosteBudgetaire getPoste(String s) {
		int nb_elements = this.postes.getSize();
		int i = 0;
		boolean trouve = false;
		PosteBudgetaire monPoste = null;
		
		// On boucle sur la liste des postes budgetaires tant que
		// n'a pas trouve le bon poste.
		while (i != nb_elements && !trouve) {
			monPoste = (PosteBudgetaire) this.postes.elementAt(i);
			trouve = s.equals(monPoste.getNom());
			i = i+1;
		}

		// Si le poste n'existe pas, on retourne null
		if (trouve) {
			return monPoste;
		}
		else {
			return null;
		}
	}




	// Creation d'un nouveau Poste Budgetaire. On ajoute un poste
	// a la liste des postes.
	public void NouveauPoste(String unNom) {
		PosteBudgetaire unPoste = new PosteBudgetaire(unNom);
		postes.addElement(unPoste);
	}


	// Methode utilisee par le mode texte. Affiche tous les
	// mouvements concernant tous les postes budgetaires.
	public void afficher() {
		int nb_elements = this.postes.getSize();
		int i = 0;
		PosteBudgetaire monPoste;

		while (i != nb_elements) {
			monPoste = (PosteBudgetaire) this.postes.elementAt(i);
			monPoste.afficherPoste();
			i = i+1;
		}
	}


	// On depense une somme pour un poste budgetaire donne.
	// On n'oublie pas de decrementer le solde de l'historique.
	public void depenserSommePour(int i, String s) {
		solde -= i;
		// On verifie que le poste existe. S'il n'existe pas
		// on en cree un nouveau.
		PosteBudgetaire unposte = getPoste(s);
		if(unposte == null) {
			this.NouveauPoste(s);
		}
                unposte = getPoste(s);
		// On appelle la methode depenserSomme de la classe
		// PosteBudgetaire
		unposte.depenserSomme(i);
	}



	// idem que la methode ci-dessus pour un depot.
	public void recevoirSommePour(int i, String s) {
		solde += i;
		PosteBudgetaire unposte = getPoste(s);
		if(unposte == null) {
			NouveauPoste(s);
		}
		unposte = getPoste(s);
                unposte.recevoirSomme(i);
	}


	// Methode principale utilisee pour tester l'interface texte. N'est pas utilisee
	// par l'interface graphique.
	// throws IOException : pour recevoir des chaines de caracteres lues au clavier.
	
	/*public static void main (String[] args)  throws IOException {
		HistoriqueFinancier histo = new HistoriqueFinancier();
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		PosteBudgetaire test;
		
		System.out.println("Entrez le nom d'un poste budgetaire :");
		String tmp = in.readLine();
		histo.NouveauPoste(tmp);
		histo.recevoirSommePour(500,tmp);
		histo.depenserSommePour(400,tmp);
		test=histo.getPoste(tmp);
		System.out.println("Entrez le nom d'un poste budgetaire :");
		tmp = in.readLine();
		histo.NouveauPoste(tmp);
		histo.recevoirSommePour(1500,tmp);
		histo.depenserSommePour(1500,tmp);
		histo.afficher();
		System.out.println("Le poste Budgetaire " + tmp + " a un solde de" + test.getSolde());
	}*/
}




