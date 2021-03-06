#include <stdio.h>   /* pour les entr�es-sorties */
#include <string.h>  /* pour les manipulations de cha�nes de caract�res */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include "rep.h"

#define VERSION 3.0
#define SQUELET
/**************************************************************************/
/* Compl�ter votre nom ici                                                */
/*   Nom :   payen                      Pr�nom :   florian                            */
/**************************************************************************/

extern bool modif;


/**********************************************************************/
/*  Ajout d'un contact dans le r�pertoire stock� en m�moire           */
/**********************************************************************/

int ajouter_un_contact_dans_rep(Repertoire* rep, Enregistrement enr)
{
#ifdef IMPL_TAB
	// compl�ter code ici pour tableau
	int idx;

	if (rep->nb_elts < MAX_ENREG)
	{
		*(rep->tab + rep->nb_elts) = enr; // on definie le dernier element de la liste avec enr
		rep->nb_elts++;                   // on a incremente le nombre d' element 
		rep->est_trie = false;            // la liste n'est pas triee
		modif = true;                     // on a effectue une modification
		return(OK);


	}
	else {
		return(ERROR);
	}


#else
#ifdef IMPL_LIST

	bool inserted = false;
	if (rep->nb_elts == 0) {          //si le repertoire est vide
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) {
			rep->nb_elts += 1;
			modif = true;
			rep->est_trie = true;
			return(OK);
		}

	}
	else {               // si il n'est pas vide on ajoute a la bonne  place
		int i = 0;
		while ((i < rep->nb_elts) && (est_sup(enr, (GetElementAt(rep->liste, i)->pers)))) {
			i++;
		} // on augmente un compteur tant qu'on est dans la liste et que enr est superieur � l'element compteur de la liste
		InsertElementAt(rep->liste, i, enr); //on insere donc l elment a l empacement compteur, ainsi deja classe
		rep->nb_elts++;                      // on augmente de 1 le nombre d'element
		modif = true;                        // on a modifie
		rep->est_trie = true;                 // la liste est triee
		return(OK);

	}


#endif

#endif


	return(OK);

} /* fin ajout */
  /**********************************************************************/
  /* supprime du r�pertoire l'enregistrement dont l'indice est donn� en */
  /*   param�tre       et place modif = true                            */
  /**********************************************************************/
#ifdef IMPL_TAB
void supprimer_un_contact_dans_rep(Repertoire* rep, int indice) {

	// compl�ter code ici pour tableau
	if (rep->nb_elts >= 1)		/* s'il y a au moins un element ds le tableau */
	{						/* et que l'indice pointe a l'interieur */


		if (indice < rep->nb_elts) {             // si l'indice est dans le tableau
			for (int i = indice; i < rep->nb_elts; i++) {   //on d�cale chaque element de la liste � partir de celui qu'on veut supprimer vers la gauche
				*(rep->tab + i) = *(rep->tab + i + 1);     // on va supprimer la velur a l'indice souhait�
			}
		}



		rep->nb_elts -= 1;		/* ds ts les cas, il y a un element en moins */
		modif = true;
	}

	return;
} /* fin supprimer */

#else
#ifdef IMPL_LIST
  /************************************************************************/
  /* supprime du r�pertoire l'enregistrement contenu dans le maillon elem */
  /*                   et fixe modif � vrai                              */
  /************************************************************************/
  // complet

int supprimer_un_contact_dans_rep_liste(Repertoire* rep, SingleLinkedListElem* elem) {

	int ret = DeleteLinkedListElem(rep->liste, elem);
	if (ret == 1) {
		rep->nb_elts--;
		modif = true;
	}

	return (0);
}
#endif
#endif


/**********************************************************************/
/*  fonction d'affichage d'un enregistrement sur une ligne � l'�cran  */
/* ex Dupont, Jean                 0320304050                         */
/**********************************************************************/
void affichage_enreg(Enregistrement enr)
{
	printf("le r�pertoire est:");
	printf("%s, %s,  %s", enr.nom, enr.prenom, enr.tel);  // on affiche simplement avec  le filtre %s


} /* fin affichage_enreg */
  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement avec alignement des mots */
  /* pour les listes                                                    */
  /* ex | Dupont                |Jean                  |0320304050      */
  /**********************************************************************/
void affichage_enreg_frmt(Enregistrement enr)
{
	// code � compl�ter ici
	// comme fonction affichage_enreg, mais avec pr�sentation align�es des infos
	//on utilise le filtre %-30s
	printf("\n| %-30s|%-30s|%-20s", enr.nom, enr.prenom, enr.tel); // permet de forcer l'aafichage sur 30 caract�res, coll� � gauche avec le -


} /* fin affichage_enreg */


  /**********************************************************************/
  /* test si dans l'ordre alphabetique, un enregistrement est apres     */
  /* un autre                                                           */
  /**********************************************************************/
bool est_sup(Enregistrement enr1, Enregistrement enr2)
{                                    // strcmp permet de comparer deux cha�nes de caract�res et de savoir si la premi�re est inf�rieure,
									  //�gale ou sup�rieure � la seconde

	if (strcmp(enr1.nom, enr2.nom) > 0) {
		return(true);
	} // on compare d'abord les noms, on retourne vrai si le premier est plus grand que le deuxieme

	else if (strcmp(enr1.nom, enr2.nom) < 0) {
		return(false);
	} // on retourne faux sinon

	else {

		if (strcmp(enr1.prenom, enr2.prenom) > 0) { return(true); } // si on arrive l�, les noms sont �gaux, on recommence donc avec les pr�noms
		if (strcmp(enr1.prenom, enr2.prenom) < 0) { return(false); }

	}


	return(false);

}

/*********************************************************************/
/*   Tri Alphabetique du tableau d'enregistrements                   */
/*********************************************************************/

void trier(Repertoire* rep) {
	Enregistrement tmp;

	for (int i = 0; i < rep->nb_elts - 1; i++) {    // on realise un tri a bulle
		for (int j = 0; j < rep->nb_elts - i - 1; j++) {
			if (est_sup(*(rep->tab + j), *(rep->tab + j + 1))) { //on utilise notre fonction estsup
				tmp = *(rep->tab + j);                           //si la fonction renvoie true on permutte les 2 valeurs avec un tmp intermediare
				*(rep->tab + j + 1) = *(rep->tab + j);
				*(rep->tab + j) = tmp;
			}
		}
	}



#ifdef IMPL_TAB
	Enregistrement tmp2;

	for (int i = 0; i < rep->nb_elts - 1; i++) {
		for (int j = 0; j < rep->nb_elts - i - 1; j++) {
			if (est_sup(*(rep->tab + j), *(rep->tab + j + 1))) {
				tmp2 = *(rep->tab + j);
				*(rep->tab + j + 1) = *(rep->tab + j);
				*(rep->tab + j) = tmp2;
			}
		}
	}





#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// rien � faire !
	// la liste est toujours tri�e
#endif
#endif


	rep->est_trie = true;

} /* fin trier */

  /**********************************************************************/
  /* recherche dans le r�pertoire d'un enregistrement correspondant au  */
  /*   nom � partir de l'indice ind                                     */
  /*   retourne l'indice de l'enregistrement correspondant au crit�re ou*/
  /*   un entier n�gatif si la recherche est n�gative				    */
  /**********************************************************************/

int rechercher_nom(Repertoire* rep, char nom[], int ind)
{
	int i = ind;		    // position  de d�but de recherche 
	int ind_fin;			//position  de fin 

	char tmp_nom[MAX_NOM];
	char tmp_nom2[MAX_NOM];

	bool trouve = false;
	ind_fin = rep->nb_elts - 1; // indice de fin � ne pas d�passer
	strncpy_s(tmp_nom, _countof(tmp_nom), nom, _TRUNCATE);//met nom dans tmp-nom

#ifdef IMPL_TAB
	while ((i <= ind_fin) && (!trouve))
	{

		strncpy_s(tmp_nom2, _countof(tmp_nom2), rep->tab[i].nom, _TRUNCATE);
		for (unsigned i = 0; i < strlen(tmp_nom); i++) {
			tmp_nom[i] = toupper(tmp_nom[i]);
		}
		for (unsigned i = 0; i < strlen(tmp_nom2); i++) {
			tmp_nom2[i] = toupper(tmp_nom2[i]);
		}



		if (strcmp(tmp_nom, tmp_nom2) == 0) {
			trouve = true;
		}
		else {
			i++;
		}
	}

#else
#ifdef IMPL_LIST
	ind_fin = rep->nb_elts - 1;
	strcpy_s(tmp_nom, _countof(tmp_nom), nom);
	for (int j = 0; j < (int)strlen(tmp_nom); j++) {	// Conversion en majuscules
		if (tmp_nom[j] >= 97) { tmp_nom[j] -= 32; }
	}

	while (i <= ind_fin && trouve == false) {
		strcpy_s(tmp_nom2, _countof(tmp_nom2), GetElementAt(rep->liste, i)->pers.nom);
		for (int j = 0; j < (int)strlen(tmp_nom2); j++) {	// Conversion en majuscules
			if (tmp_nom2[j] >= 97) { tmp_nom2[j] -= 32; }
		}

		if (strlen(tmp_nom) == strlen(tmp_nom2)) {
			int j = 0;
			while (tmp_nom[j] == tmp_nom2[j] && j < (int)strlen(tmp_nom)) {
				j++;
			}
			if (j == (int)strlen(tmp_nom)) {
				trouve = true;
			}
		}
		i++;
	}
	i--;

#endif
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_nom */

  /*********************************************************************/
  /* Supprimer tous les caracteres non num�riques de la chaines        */
  /*********************************************************************/
void compact(char* s) {
	int j = 0;
	for (int i = 0; i <= (int)strlen(s); i++) {
		if ((s[i] >= 48 && s[i] <= 57) || s[i] == '\0') { //on regarde si le caractere n'est pas une lettre
			s[j] = s[i];
			j++;
		}
	}
	return;


}

/**********************************************************************/
/* sauvegarde le r�pertoire dans le fichier dont le nom est pass� en  */
/* argument                                                           */
/* retourne OK si la sauvegarde a fonctionn� ou ERROR sinon           */
/**********************************************************************/
int sauvegarder(Repertoire* rep, char nom_fichier[])
{
	FILE* fic_rep;					/* le fichier */
#ifdef IMPL_TAB
	errno_t err = fopen_s(&fic_rep, nom_fichier, "w");
	if (err == 0) {
		for (int i = 0; i < rep->nb_elts; i++) {
			fputs("\n", fic_rep);
			fputs(rep->tab[i].nom, fic_rep);
			fputs(";", fic_rep);
			fputs(rep->tab[i].prenom, fic_rep);
			fputs(";", fic_rep);
			fputs(rep->tab[i].tel, fic_rep);
		}
		fclose(fic_rep);
	}
	else {
		return ERROR;
	}


#else
#ifdef IMPL_LIST
	errno_t err = fopen_s(&fic_rep, nom_fichier, "w");
	if (err == 0) {
		for (int i = 0; i < rep->nb_elts; i++) {
			fputs(GetElementAt(rep->liste, i)->pers.nom, fic_rep);
			fputs(";", fic_rep);
			fputs(GetElementAt(rep->liste, i)->pers.prenom, fic_rep);
			fputs(";", fic_rep);
			fputs(GetElementAt(rep->liste, i)->pers.tel, fic_rep);
			fputs("\n", fic_rep);
		}
		fclose(fic_rep);
	}
	else {
		return ERROR;
	}
#endif
#endif

	return(OK);
} /* fin sauvegarder */


  /**********************************************************************/
  /*   charge dans le r�pertoire le contenu du fichier dont le nom est  */
  /*   pass� en argument                                                */
  /*   retourne OK si le chargement a fonctionn� et ERROR sinon         */
  /**********************************************************************/

int charger(Repertoire* rep, char nom_fichier[])
{
	FILE* fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Enregistrement);
	char buffer[sizeof(Enregistrement) + 1];
	int idx = 0;

	char* char_nw_line;

	_set_errno(0);
	if (((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL))
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (rep->nb_elts < MAX_ENREG))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				//memorisation de l'enregistrement lu dans le tableau 
				buffer[long_max_rec] = 0;

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			// suppression du fin de_ligne eventuel 

				idx = 0;								// analyse depuis le debut de la ligne 
#ifdef IMPL_TAB
				if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;
					if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		// element correct, on le comptabilise 
					}
				}
#else
#ifdef IMPL_LIST
				Enregistrement nouveau;
				if (lire_champ_suivant(buffer, &idx, nouveau.nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, nouveau.prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, nouveau.tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		/* element � priori correct, on le comptabilise */
					}
				}
				InsertElementAt(rep->liste, num_rec, nouveau);

#endif
#endif




			}

		}
		rep->nb_elts = num_rec;
		fclose(fic_rep);
		return(OK);
	}


} /* fin charger */