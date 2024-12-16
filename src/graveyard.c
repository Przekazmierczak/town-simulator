#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "graveyard.h"

struct Graveyard* build_graveyard(int number_of_positions) {
    struct Graveyard *graveyard = malloc(sizeof(struct Graveyard));
    if (graveyard == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla cmentarz w stwórz_cmentarz.\n");
        exit(EXIT_FAILURE);
    }
    graveyard->number_of_positions = number_of_positions;
    graveyard->number_of_rows = 1;

    // Alokuj pamięci dla wskaźnika na tablicę rzędów
    graveyard->alley = malloc(sizeof(struct Grave**));
    if (graveyard->alley == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla cmentarz->aleja w stwórz_cmentarz.\n");
        exit(EXIT_FAILURE);
    }

    // Alokuj pamięć dla pierwszego rzędu na cmentarzu
    graveyard->alley[0] = malloc(number_of_positions * sizeof(struct Grave*));
    if (graveyard->alley[0] == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla cmentarz->aleja[0] w stwórz_cmentarz.\n");
        exit(EXIT_FAILURE);
    }

    // Inicjalizuj miejsca na groby jako NULL
    for (int i = 0; i < number_of_positions; i++) {
        graveyard->alley[0][i] = NULL;
    }
    return graveyard;
};

void expand_graveyard(struct Graveyard *graveyard) {
    graveyard->number_of_rows += 1;

    // Realokuj pamięć dla wskaźnika na tablicę rzędów
    graveyard->alley = realloc(graveyard->alley, graveyard->number_of_rows * sizeof(struct Grave**));
    if (graveyard->alley == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla cmentarz->aleja w powiększ_cmentarz.\n");
        exit(EXIT_FAILURE);
    }

    // Alokuj pamięĆ dla nowego rzędu
    graveyard->alley[graveyard->number_of_rows - 1] = malloc(graveyard->number_of_positions * sizeof(struct Grave*));
    if (graveyard->alley[graveyard->number_of_rows - 1] == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla cmentarz->aleja[cmentarz->ilość_rzędów - 1] w powiększ_cmentarz.\n");
        exit(EXIT_FAILURE);
    }

    // Inicjalizuj miejsca na groby w nowym rzędzie jako NULL
    for (int i = 0; i < graveyard->number_of_positions; i++) {
        graveyard->alley[graveyard->number_of_rows - 1][i] = NULL;
    }
}

void add_deceased(struct Graveyard *graveyard, struct Resident *resident, int year_of_death, int *row, int *position) {
    for (; *row < graveyard->number_of_rows; (*row)++) {
        for (; *position < graveyard->number_of_positions; (*position)++) {
            // Sprawdź, czy miejsce jest puste lub czy grób kwalifikuje się do likwidacji
            if (graveyard->alley[*row][*position] == NULL || graveyard->alley[*row][*position]->year_of_liquidation < year_of_death) {
                // Jeśli miejsce jest wolne, alokuj pamięć dla nowego grobu
                if (graveyard->alley[*row][*position] == NULL) {
                    graveyard->alley[*row][*position] = malloc(sizeof(struct Grave));
                    if (graveyard->alley[*row][*position] == NULL) {
                        printf("Błąd: Nie udało się przydzielić pamięci dla cmentarz->aleja[i][j] w dodaj_zmarłego.\n");
                        exit(EXIT_FAILURE);
                    }
                // Jeśli grób jest już zajęty, uwalnij pamięć poprzedniego zmarłego
                } else {
                    free_resident(graveyard->alley[*row][*position]->deceased);
                }
                // Umieść nowego zmarłego i oblicz roku likwidacji grobu
                graveyard->alley[*row][*position]->deceased = resident;
                graveyard->alley[*row][*position]->year_of_liquidation = specify_the_year_of_liquidation(resident, year_of_death);
                (*position)++;
                return;
            }
        }
        *position = 0;
    }
    // Jeśli nie znaleziono miejsca, powiększ cmentarz i dodaJ zmarłego do nowego rzędu
    expand_graveyard(graveyard);
    *position = 0;
    add_deceased(graveyard, resident, year_of_death, row, position);
    return;
}

int specify_the_year_of_liquidation(struct Resident *resident, int year_of_death) {
    // Oblicz rok likwidacji grobu na podstawie pensji zmarłego
    if (resident->salary > 13000) {
        return year_of_death + 100;
    } else if (resident->salary > 11000) {
        return year_of_death + 80;
    } else if (resident->salary > 9000) {
        return year_of_death + 60;
    } else if (resident->salary > 7000) {
        return year_of_death + 40;
    }
    return year_of_death + 20;
}

void list_of_deceased(struct Graveyard *graveyard) {
    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
    
    for (int i = 0; i < graveyard->number_of_rows; i++) {
        for (int j = 0; j < graveyard->number_of_positions; j++) {
            printf("W rzędzie nr %i w miejscu nr %i spoczywa: ", i, j);
            if (graveyard->alley[i][j] != NULL) {
                // Wyświetl dane o zmarłym, jeśli grób jest zajęty
                printf("%s %s ", graveyard->alley[i][j]->deceased->name, graveyard->alley[i][j]->deceased->surname);
                printf("Rok likwidacji: %i\n", graveyard->alley[i][j]->year_of_liquidation);
            } else {
                // Wyświetl informacje, że miejsce jest puste
                printf("%s\n", "Miejsce jest aktualnie puste");
            }
        }
    }
    printf("NACIŚNIJ ENTER ABY POWRÓCIĆ DO MENU");
    while (getchar() != '\n');
}

void free_graveyard(struct Graveyard * graveyard) {
    for (int i = 0; i < graveyard->number_of_rows; i++) {
        for (int j = 0; j < graveyard->number_of_positions; j++) {
            if (graveyard->alley[i][j] != NULL) {
                free(graveyard->alley[i][j]->deceased->name);
                free(graveyard->alley[i][j]->deceased->surname);
                free(graveyard->alley[i][j]->deceased);
                free(graveyard->alley[i][j]);
            }
        }
        free(graveyard->alley[i]);
    }
    free(graveyard->alley);
}