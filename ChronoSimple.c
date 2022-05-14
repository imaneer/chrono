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
    int intervalle, heure, minute, seconde, centieme;
    struct timeval debut;
    struct timeval fin;
    gettimeofday(&debut, NULL);
    while (1) {
        gettimeofday(&fin, NULL);
        intervalle = intervalle_ms(debut, fin);
        heure = nb_ms_vers_heures(intervalle);
        minute = nb_ms_vers_minutes(intervalle);
        seconde = nb_ms_vers_secondes(intervalle);
        centieme = nb_ms_vers_centiemes(intervalle);
        printf("%d : %d: %d : %d\n", heure, minute, seconde, centieme);
        usleep(500000);
    }
    return 0;
}