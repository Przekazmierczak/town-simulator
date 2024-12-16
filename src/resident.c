#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resident.h"

const int NUMBER_OF_CATEGORY = 4;
const int NUMBER_OF_NAME_SURNAME = 40;
const int MAX_NAME_SURNAME = 20;

char*** load_list_of_names_from_file() {
    // Stwórz tabelę do przechowania możliwych imion oraz nazwisk
    char ***list_of_possible_names = malloc(NUMBER_OF_CATEGORY * sizeof(char**));
    for (int i = 0; i < 4; i++) {
        list_of_possible_names[i] = malloc(NUMBER_OF_NAME_SURNAME * sizeof(char*));
        for (int j = 0; j < 40; j++) {
            list_of_possible_names[i][j] = malloc(MAX_NAME_SURNAME * sizeof(char));
        }
    }
    
    char buffer[10000]; // Bufor do odczytu linii z pliku
    int line = 0;

    FILE *file = fopen("imiona.txt", "r");

    if (file == NULL) {
        printf("Błąd: Nie udało otworzyć się pliku imiona.txt\n");
        exit(EXIT_FAILURE);
    } else {
        // Odczytaj plik linia po linii
        while (!feof(file) && !ferror(file)) {
            if (fgets(buffer, 10000, file) != NULL) {
                int letter_line = 0;
                int letter_name = 0;
                int name = 0;
                // Przejdź po wszystkich literach w danej linii
                while (buffer[letter_line] != '\0') {
                    if (buffer[letter_line] != ' ' && buffer[letter_line] != '\n') {
                        list_of_possible_names[line][name][letter_name] = buffer[letter_line];
                        letter_name++;
                    } else {
                        list_of_possible_names[line][name][letter_name] = '\0';  // Zakończy imię/nazwisko
                        name ++;  // Przejdź do kolejnego imienia/nazwiska
                        letter_name = 0;
                    }
                    letter_line++;
                }
                line++;  // Przejdź do następnej linii
            }
        }
    }

    fclose(file);
    return list_of_possible_names;
}

struct Resident* create_resident(bool newborn, char*** list_of_possible_names) {
    struct Resident* resident = malloc(sizeof(struct Resident));
    if (resident == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla struktury mieszkaniecw stwórz_mieszkańca.\n");
        exit(EXIT_FAILURE);
    }

    // Losuj płeć mieszkańca
    resident->gender = rand() % 2 ? Man : Woman;

    // Przydziel imię i nazwisko w zależności od płci
    if (resident->gender == Man) {
        char* name = list_of_possible_names[0][rand() % 40];
        resident->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->name == NULL) {
            printf("Błąd: Nie udało się przydzielić pamięci dla mieszkaniec->imię w stwórz_mieszkańca.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->name, name);
    
        char* surname = list_of_possible_names[2][rand() % 40];
        resident->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->surname == NULL) {
            printf("Błąd: Nie udało się przydzielić pamięci dla mieszkaniec->nazwisko w stwórz_mieszkańca.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->surname, surname);
    } else {
        char* name = list_of_possible_names[1][rand() % 40];
        resident->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->name == NULL) {
            printf("Błąd: Nie udało się przydzielić pamięci dla mieszkaniec->imię w stwórz_mieszkańca.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->name, name);

        char* surname = list_of_possible_names[3][rand() % 40];
        resident->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->surname == NULL) {
            printf("Błąd: Nie udało się przydzielić pamięci dla mieszkaniec->nazwisko w stwórz_mieszkańca.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->surname, surname);
    }

    // Ustaw wiek i pensję mieszkańca w zależności od tego, czy jest noworodkiem
    if (newborn) {
        resident->age = 0;
        resident->salary = 0;
    } else {
        resident->age = rand() % 70;
        resident->salary = 0;
        // W przypadku w którym mieszkaniec ma co najmniej 18 lat, losuj czy posiada pracę
        if (resident->age >= 18) {
            int i = resident->age - 18;
            while (i >= 0 && resident->salary == 0) {
                job(resident);
                i--;
            }
        }
    }
    return resident;
}

// Spróbuj przydzielić pracę mieszkańcowi, oraz ustawial losową pensję
void job(struct Resident* resident) {
    if (rand() % 5 == 0) resident->salary = 5000 + (rand() % 10000);
}

void free_resident(struct Resident* resident) {
    free(resident->name);
    free(resident->surname);
    free(resident);
}