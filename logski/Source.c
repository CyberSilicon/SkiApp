    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    // Structure pour représenter un skieur
    struct Skieur {
        char nom[50];
        int numero_dossard;
        double temps_premiere_manche;
        double temps_deuxieme_manche;
        double temps_total;
        struct Skieur* suivant; // Pointeur vers le prochain skieur dans la liste
    };

    // Fonction pour créer un nouveau skieur 
    struct Skieur* creer_skieur() {
        struct Skieur* skieur = (struct Skieur*)malloc(sizeof(struct Skieur));
        skieur->suivant = NULL;
        return skieur;
    }

    // Fonction pour insérer un skieur dans la liste chaînée
    void inserer_skieur(struct Skieur** liste, struct Skieur* skieur) {
        if (*liste == NULL || skieur->temps_total < (*liste)->temps_total) {
            skieur->suivant = *liste;
            *liste = skieur;
        }
        else {
            struct Skieur* temp = *liste;
            while (temp->suivant != NULL && skieur->temps_total >= temp->suivant->temps_total) {
                temp = temp->suivant;
            }
            skieur->suivant = temp->suivant;
            temp->suivant = skieur;
        }
    }

    // Fonction pour afficher la liste des skieurs
    void afficher_liste(struct Skieur* liste) {
        while (liste != NULL) {
            printf("Nom: %s, Dossard: %d, Temps Total: %.2lf\n", liste->nom, liste->numero_dossard, liste->temps_total);
            liste = liste->suivant;
        }
    }

    // Fonction pour afficher la liste des skieurs ayant abandonné
    void afficher_abandonnes(struct Skieur* abandonnes) {
        if (abandonnes == NULL) {
            printf("\nAucun skieur n'a abandonner dans la premiere manche.\n\n");
        }
        else {
            printf("\nListe des skieurs ayant abandonne :\n");
            afficher_liste(abandonnes);
            printf("\n");
        }
    }

    // Fonction pour rechercher un skieur par son numéro de dossard
    struct Skieur* rechercher_skieur_par_dossard(struct Skieur* liste, int numero_dossard) {
        while (liste != NULL) {
            if (liste->numero_dossard == numero_dossard) {
                return liste;
            }
            liste = liste->suivant;
        }
        return NULL; // Le skieur n'a pas été trouvé
    }

    // Fonction pour marquer un skieur comme abandonné
    void marquer_abandon(struct Skieur** skieurs_ayant_termine, struct Skieur** skieurs_abandonnes) {
        int abandon_choice;
        printf("Ya t'il des Skieurs ayant abandonnée ? si oui Entrez le numéro de dossard du skieur ayant abandonné si non tapez 0: ");
        scanf("%d", &abandon_choice);

        if (abandon_choice != 0) {
            // Rechercher le skieur dans la liste des skieurs ayant terminé
            struct Skieur* skieur = rechercher_skieur_par_dossard(*skieurs_ayant_termine, abandon_choice);

            if (skieur != NULL) {
                // Retirer le skieur de la liste des skieurs ayant terminé
                // et l'ajouter à la liste des skieurs ayant abandonné
                struct Skieur* temp = *skieurs_ayant_termine;
                if (temp == skieur) {
                    *skieurs_ayant_termine = skieur->suivant;
                }
                else {
                    while (temp->suivant != NULL && temp->suivant != skieur) {
                        temp = temp->suivant;
                    }
                    if (temp->suivant != NULL) {
                        temp->suivant = skieur->suivant;
                    }
                }
                skieur->suivant = NULL;
                inserer_skieur(skieurs_abandonnes, skieur);
                printf("Le skieur a été marqué comme abandonné.\n");
            }
            else {
                printf("Aucun skieur trouvé avec ce numéro de dossard.\n");
            }
        }
        else {
            printf("Aucun skieur marqué comme abandonné.\n");
        }
    }

    void inverser_ordre(struct Skieur** liste) {
        struct Skieur* prev = NULL;
        struct Skieur* current = *liste;
        struct Skieur* next = NULL;

        while (current != NULL) {
            next = current->suivant;
            current->suivant = prev;
            prev = current;
            current = next;
        }

        *liste = prev;
    }


    int main() {
        int nombre_skieurs;

        printf("Entrez le nombre de skieurs : ");
        scanf("%d", &nombre_skieurs);

        // Initialisation des listes
        struct Skieur* skieurs_ayant_termine = NULL;
        struct Skieur* skieurs_abandonnes = NULL;

        // Saisie des données de la première manche
        for (int i = 0; i < nombre_skieurs; ++i) {
            struct Skieur* skieur = creer_skieur();

            printf("Nom du skieur %d : ", i + 1);
            scanf("%s", skieur->nom);

            printf("Numéro de dossard du skieur %d : ", i + 1);
            scanf("%d", &skieur->numero_dossard);

            printf("Temps de la première manche du skieur %d (en minutes) : ", i + 1);
            scanf("%lf", &skieur->temps_premiere_manche);


            if (skieur->temps_premiere_manche <= 0) {
                inserer_skieur(&skieurs_abandonnes, skieur);
            }
            else {
                inserer_skieur(&skieurs_ayant_termine, skieur);
            }

        }

   

        // Affichage de la liste des skieurs de la première manche
        printf("\nListe des skieurs de la première manche :\n");
        afficher_liste(skieurs_ayant_termine);
        afficher_liste(skieurs_abandonnes);

        // Affichage de la liste des skieurs de la première manche
        printf("\nListe des skieurs de la première manche :\n");
        afficher_liste(skieurs_ayant_termine);
        afficher_abandonnes(skieurs_abandonnes);

        // Demander à l'utilisateur de spécifier le skieur ayant abandonné
        marquer_abandon(&skieurs_ayant_termine, &skieurs_abandonnes);

        // Inverser l'ordre de la liste des skieurs ayant terminé après la première manche
        inverser_ordre(&skieurs_ayant_termine);

        // Affichage de la liste des skieurs de la première manche (maintenant dans l'ordre inverse)
        printf("\nListe des skieurs de la première manche (ordre inverse) :\n");
        afficher_liste(skieurs_ayant_termine);
        printf("\n");

 

        // Saisie des données de la deuxième manche
        for (struct Skieur* skieur = skieurs_ayant_termine; skieur != NULL; skieur = skieur->suivant) {
            printf("Temps de la deuxieme manche pour %s : ", skieur->nom);
            scanf("%lf", &skieur->temps_deuxieme_manche);

        // Calcul du temps total
            skieur->temps_total = skieur->temps_premiere_manche + skieur->temps_deuxieme_manche;
        }

        // Trouver le skieur ayant le temps total le plus bas (vainqueur)
        struct Skieur* vainqueur = skieurs_ayant_termine;
        struct Skieur* temp = skieurs_ayant_termine->suivant;

        while (temp != NULL) {
            if (temp->temps_total < vainqueur->temps_total) {
                vainqueur = temp;
            }
            temp = temp->suivant;
        }

        // Affichage de la liste classée des skieurs ayant terminé la course
        printf("\nListe classée des skieurs ayant terminé la course :\n");
        afficher_liste(skieurs_ayant_termine);

        // Afficher le vainqueur et l'écart de temps pour les autres skieurs
        printf("\n***************************************************************************************\n");
        printf("\nLe vainqueur de la course est : %s avec un temps total de %.2lf minutes.\n", vainqueur->nom, vainqueur->temps_total);

        //Graphe ASCII
        printf("\n");
        printf("   /\\ \n");
        printf("  /  \\ \n");
        printf(" /____\\  %s remporte la course!\n", vainqueur->nom);

        printf("\n***************************************************************************************\n");
        // Afficher le vainqueur de la course
        printf("\nLe vainqueur de la course est : %s avec un temps total de %.2lf minutes.\n", vainqueur->nom, vainqueur->temps_total);

        // Afficher l'écart de temps pour les autres skieurs s'il y en a
        if (skieurs_ayant_termine != NULL && skieurs_ayant_termine->suivant != NULL) {
            printf("\nÉcart de temps pour les autres skieurs :\n");

            temp = skieurs_ayant_termine;
            while (temp != NULL) {
                if (temp != vainqueur) {
                    double ecart_de_temps = temp->temps_total - vainqueur->temps_total;

                    // Afficher une flèche vers le bas pour indiquer que le skieur est derrière le vainqueur
                    printf("%s a un écart de temps de %.2lf minutes par rapport au vainqueur. v\n", temp->nom, ecart_de_temps);
                }
                temp = temp->suivant;
            }
        }

        printf("\n");

        // Libération de la mémoire allouée pour les skieurs abandonnés
        while (skieurs_abandonnes != NULL) {
            struct Skieur* temp = skieurs_abandonnes;
            skieurs_abandonnes = skieurs_abandonnes->suivant;
            free(temp);
        }

        // Libération de la mémoire allouée pour les skieurs ayant terminé
        while (skieurs_ayant_termine != NULL) {
            struct Skieur* temp = skieurs_ayant_termine;
            skieurs_ayant_termine = skieurs_ayant_termine->suivant;
            free(temp);
        }

        return 0;
    }
