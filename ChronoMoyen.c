/*
 * Auteur : Sarah ENG, Imane ERRAHMANI
 * Creation : 17-02-2022
 * Modification : 22-02-2022
 */
#include <sys/time.h>
#include <ncurses.h>
#include <assert.h>
#include <unistd.h>

/* question 1 */
int intervalle_ms(struct timeval debut, struct timeval fin){
    int usec, sec, resultat;
    usec = fin.tv_usec - debut.tv_usec;
    sec = fin.tv_sec - debut.tv_sec;
    resultat = (usec / 1000) + (sec * 1000);
    return resultat;
}

/* question 2 */
int nb_ms_vers_centiemes(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / 10) % 100;
}

int nb_ms_vers_secondes(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / 1000) % 60;
}

int nb_ms_vers_minutes (int nb_ms) {
    int resultat;
    assert(nb_ms >= 0);
    resultat = (nb_ms / (1000 * 60)) % 60;
    return resultat;
}

int nb_ms_vers_heures (int nb_ms) {
    int resultat;
    assert(nb_ms >= 0);
    resultat = resultat = (nb_ms / (1000 * 60 * 60));
    return resultat;
}


int main () {
    /* question 3 */
    int duree_totale, pause, touche;
    int intervalle, heure, minute, seconde, centieme;
    struct timeval debut;
    struct timeval fin;
    pause = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    gettimeofday(&debut, NULL);
    while (1) {
        touche = getch();
        if ( touche == ' ') {
            if (pause == 0) {
                gettimeofday(&debut, NULL);
            }
            pause = (pause + 1) % 2;
            
        }
        usleep(50000);
        if (pause != 0) {
            gettimeofday(&fin, NULL);
            intervalle = intervalle_ms(debut, fin);
            duree_totale += intervalle;
            debut = fin;
        }
        heure = nb_ms_vers_heures(duree_totale);
        minute = nb_ms_vers_minutes(duree_totale);
        seconde = nb_ms_vers_secondes(duree_totale);
        centieme = nb_ms_vers_centiemes(duree_totale);
        mvprintw(5, 5, "%d : %d: %d : %d\n", heure, minute, seconde, centieme);
        refresh();
        
    }


    refresh();
    getch();
    endwin();
    return 0;
}