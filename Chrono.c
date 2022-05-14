/*
 * Auteur : Sarah ENG, Imane ERRAHMANI
 * Creation : 17-02-2022
 * Modification : 22-02-2022
 */
#include <sys/time.h>
#include <ncurses.h>
#include <assert.h>
#include <unistd.h>

#define TAILLE 20


typedef struct {
    int etat;
    int duree_totale;
    int avertissement;
    int dernier_tour;
    int nb_tours;
    int temps_tour[TAILLE];
}Chronometre;

/* Reçoit deux timeval, et renvoie l'intervalle de temps (en ms) écoulé entre les deux. */
int intervalle_ms(struct timeval debut, struct timeval fin){
    int usec, sec, resultat;
    usec = fin.tv_usec - debut.tv_usec;
    sec = fin.tv_sec - debut.tv_sec;
    resultat = (usec / 1000) + (sec * 1000);
    return resultat;
}

/* Reçoit un nombre de millisecondes, et renvoie son nombre de centiemes de secondes modulo 100. */
int nb_ms_vers_centiemes(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / 10) % 100;
}

/* Reçoit un nombre de millisecondes, et renvoie son nombre de secondes modulo 60. */
int nb_ms_vers_secondes(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / 1000) % 60;
}

/* Reçoit un nombre de millisecondes, et renvoie son nombre de minutes modulo 60. */
int nb_ms_vers_minutes (int nb_ms) {
    int resultat;
    assert(nb_ms >= 0);
    resultat = (nb_ms / (1000 * 60)) % 60;
    return resultat;
}

/* Reçoit un nombre de millisecondes, et renvoie son nombre d'heures. */
int nb_ms_vers_heures (int nb_ms) {
    int resultat;
    assert(nb_ms >= 0);
    resultat = (nb_ms / (1000 * 60 * 60));
    return resultat;
}

/* Renvoie un chronometre avec ces champs initialises selon des conditions initiales imposees */
Chronometre initialiser_chronometre(){
    Chronometre chrono;
    chrono.etat = 0;
    chrono.duree_totale = 0;
    chrono.avertissement = 5000;
    chrono.dernier_tour = 0;
    chrono.nb_tours = 0;
    return chrono;
}

/* Affiche, sur l'interface NCURSES, aux coordonnees (y, x), la duree de nb_ms, sous le format HH : MM : SS : CC. */
void afficher_duree (int y, int x, int nb_ms){
    int heure, minute, seconde, centieme;
    assert(y >= 0);
    assert(x >= 0);
    assert(nb_ms >= 0);
    heure = nb_ms_vers_heures(nb_ms);
    minute = nb_ms_vers_minutes(nb_ms);
    seconde = nb_ms_vers_secondes(nb_ms);
    centieme = nb_ms_vers_centiemes(nb_ms);
    mvprintw(y, x, "\t%d : %d : %d : %d\n", heure, minute, seconde, centieme);
    refresh();
}

/* Affiche, sur l'interface NCURSES, le chronometre, ses fonctionnalites, et son menu. */
void afficher_interface(Chronometre chrono) {
    int i, j, x;
    assert(LINES >= 14);
    assert(COLS >= 58);

    x = COLS / 2;
    mvprintw(1, x-7, "== Chronometre ==");
    afficher_duree(8, x, chrono.duree_totale);
    mvprintw(9, x-14, "Avertissement ");
    afficher_duree(9, x, chrono.avertissement);

    for (j = 0; j < COLS ; j++)
        mvprintw(10, j, "-");

    for(i = 0; i < chrono.nb_tours; i++){
        if(6-i > 1){
            mvprintw(7-i, x - 10, "Tours %d", chrono.dernier_tour-i);
            afficher_duree(7-i, x, chrono.temps_tour[i]);
        }
    }
    
    mvprintw(12, 2, "Espace : Lancer / Mettre en pause.");
    mvprintw(14, 2, "r      : Réinitialiser.");
    mvprintw(16, 2, "t      : Marquer tour.");
    mvprintw(18, 2, "F1/F2  : Incrémenter / Décrementer heure avertissement.");
    mvprintw(20, 2, "F3/F4  : Incrémenter / Décrementer minute avertissement.");
    mvprintw(22, 2, "F5/F6  : Incrémenter / Décrementer seconde avertissement.");
    mvprintw(24, 2, "q      : Quitter.");

    refresh();
    
}

/* Reçoit un chronometre et lui ajoute un nouveau tour. */
void ajouter_tours(Chronometre * chrono) {
    int i;
    for (i = chrono->nb_tours ; i > 0 ; i--)
        chrono->temps_tour[i] = chrono->temps_tour[i - 1];
    chrono->temps_tour[i] = chrono->duree_totale;
    chrono->dernier_tour += 1;
    if (chrono->nb_tours < TAILLE)
        chrono->nb_tours += 1;
}

/* Affiche un clignotant. */
void afficher_flash () {
    
    attron(COLOR_PAIR(1));
    mvprintw(8, 0, "*************************");
    attroff(COLOR_PAIR(1));
    usleep(2000);
    refresh();

    attron(COLOR_PAIR(2));
    mvprintw(8, 0, "*************************");
    attroff(COLOR_PAIR(2));
    usleep(2000);
    refresh();
    
}

int main () {
    int touche;
    int intervalle;
    struct timeval debut;
    struct timeval fin;
    Chronometre chrono;
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    chrono = initialiser_chronometre();

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    gettimeofday(&debut, NULL);
    while (1) {
        touche = getch();
        if (touche == ' ') {
            if (chrono.etat == 0) {
                gettimeofday(&debut, NULL);
            }
            chrono.etat = (chrono.etat + 1) % 2;
            
        }
        usleep(50000);
        if (chrono.etat != 0) {
            gettimeofday(&fin, NULL);
            intervalle = intervalle_ms(debut, fin);
            chrono.duree_totale += intervalle;
            debut = fin;
            
        }
        if (touche == 't')
            ajouter_tours(&chrono);
        if (touche == 'q')
            break;
        if (touche == 'r'){
            chrono.duree_totale = 0;
            chrono.nb_tours = 0;
            chrono.dernier_tour = 0;
            chrono.etat = 0;
            clear();
        }
        if (touche == KEY_F(1)){
            if (chrono.avertissement <= 99 * 1000 * 60 * 60)
                chrono.avertissement += 1000 * 60 * 60;
            clear();
        }
        if (touche == KEY_F(2)){
            if (chrono.avertissement >= 1000 * 60 * 60)
                chrono.avertissement -= 1000 * 60 * 60;
            clear();
        }
        if (touche == KEY_F(3)){
            if (chrono.avertissement <= 59 * 1000 * 60)
                chrono.avertissement += 1000 * 60;
            clear();
        }
        if (touche == KEY_F(4)){
            if (chrono.avertissement >= 1000 * 60)
                chrono.avertissement -= 1000 * 60;
            clear();
        }
        if (touche == KEY_F(5)){
            if (chrono.avertissement < 59 * 1000)
                chrono.avertissement += 1000;
            clear();
        }
        if (touche == KEY_F(6)){
            if (chrono.avertissement > 1000)
                chrono.avertissement -= 1000;
            clear();
        }
        if (chrono.avertissement < chrono.duree_totale){
            afficher_flash();
        }
        if (chrono.duree_totale > (99 * 1000 * 60 * 60) + (59 * 1000 * 60) + (59 * 1000) + 990)
            chrono.duree_totale = 0;
        afficher_interface(chrono);
    }

    refresh();
    endwin();
    return 0;
}