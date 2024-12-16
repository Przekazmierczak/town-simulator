#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resident.h"
#include "town.h"
#include "graveyard.h"

extern int NUMBER_OF_CATEGORY;
extern int NUMBER_OF_NAME_SURNAME;
extern int MAX_NAME_SURNAME;

// Funkcja zapisująca dane miasteczka do pliku binarnego
void save_to_file(struct Town *town) {
    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("TRWA ZAPISYWANIE...\n");

    // Otwórz plik binarny do zapisu
    FILE *file = fopen("zapis.bin", "wb");

    if (!file){
        printf("Nie udało się utworzyć pliku do zapisu");
        exit(EXIT_FAILURE);
    }

    // Zapisz podstawowe informacje o miasteczku
    fwrite(&town->number_of_residents, sizeof(int), 1, file);
    fwrite(&town->year, sizeof(int), 1, file);
    fwrite(&town->budget, sizeof(long long), 1, file);

    // Zapisz dane każdego mieszkańca
    struct Residents *current_resident = town->residents;
    for (int i = 0; i < town->number_of_residents; i++) {
        fwrite(current_resident->val->name, sizeof(char), MAX_NAME_SURNAME, file);
        fwrite(current_resident->val->surname, sizeof(char), MAX_NAME_SURNAME, file);
        fwrite(&(current_resident->val->gender), sizeof(char), 1, file);
        fwrite(&(current_resident->val->age), sizeof(int), 1, file);
        fwrite(&(current_resident->val->salary), sizeof(int), 1, file);
        current_resident = current_resident->next;
    }

    // Zapisz dane cmentarza
    fwrite(&town->graveyard->number_of_positions,sizeof(int), 1, file);
    fwrite(&town->graveyard->number_of_rows, sizeof(int), 1, file);

    bool empty_position;  // Zmienna pomocnicza do sprawdzenia, czy pozycja na cmentarzu jest pusta

    for (int i = 0; i < town->graveyard->number_of_rows; i++) {
        for (int j = 0; j < town->graveyard->number_of_positions; j++) {
            // W przypadku w którym w grobie jest zmarły zapisz od pliku pustą_pozycję jako true a następnie dane zmarłego
            if (town->graveyard->alley[i][j] != NULL) {
                empty_position = false;
                fwrite(&(empty_position), sizeof(bool), 1, file);

                // Zapisz dane zmarłego w grobie
                fwrite(town->graveyard->alley[i][j]->deceased->name, sizeof(char), MAX_NAME_SURNAME, file);
                fwrite(town->graveyard->alley[i][j]->deceased->surname, sizeof(char), MAX_NAME_SURNAME, file);
                fwrite(&(town->graveyard->alley[i][j]->deceased->gender), sizeof(char), 1, file);
                fwrite(&(town->graveyard->alley[i][j]->deceased->age), sizeof(int), 1, file);
                fwrite(&(town->graveyard->alley[i][j]->deceased->salary), sizeof(int), 1, file);
                fwrite(&(town->graveyard->alley[i][j]->year_of_liquidation), sizeof(int), 1, file);
            // W przypadku w którym w grób jest pusty zapisz od pliku pustą_pozycję jako false i kontynuuj
            } else {
                empty_position = true;
                fwrite(&(empty_position), sizeof(bool), 1, file);
            }
        }
    }

    // Zapisz dane dotyczące infrastruktury miejskiej
    fwrite(&town->hospitals, sizeof(int), 1, file);
    fwrite(&town->fire_departments, sizeof(int), 1, file);
    fwrite(&town->schools, sizeof(int), 1, file);

    // Zapisz listę możliwych imion
    for (int i = 0; i < NUMBER_OF_CATEGORY; i++) {
        for (int j = 0; j < NUMBER_OF_NAME_SURNAME; j++) {
            fwrite(town->possible_names[i][j], sizeof(char), MAX_NAME_SURNAME, file);
        }
    }

    fclose(file);  // Zamknij plik

    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("Zapisywanie zakończono sukcesem\n");
    printf("NACIŚNIJ ENTER ABY POWRÓCIĆ DO MENU");
    while (getchar() != '\n');    // Czekaj na naciśnięcie klawisza, aby wrócić do menu
}

// Funkcja wczytująca dane miasteczka z pliku binarnego
void load_from_file(struct Town *town) {
    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("TRWA WCZYTYWANIE...\n");

    // Otwórz plik binarny do odczytu
    FILE *file = fopen("zapis.bin", "rb");

    if (!file){
        // Wyczyść ekran
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear");  // Linux/macOS
        #endif
        
        printf("Wczytywanie zakończono niepowodzeniem - brak pliku\n");
        printf("NACIŚNIJ ENTER ABY POWRÓCIĆ DO MENU");
        while (getchar() != '\n');    // Czekaj na naciśnięcie klawisza, aby wrócić do menu
        return;
    }

    // Zwolnij wcześniej zarezerwowaną pamięć
    free_graveyard(town->graveyard);
    free_residents(town);


    // Wczytaj dane podstawowe miasteczka
    fread(&town->number_of_residents, sizeof(int), 1, file);
    fread(&town->year, sizeof(int), 1, file);
    fread(&town->budget, sizeof(long long), 1, file);

    town->residents = NULL;

    struct Residents *current = NULL;
    struct Residents **next = &town->residents;

     // Wczytaj dane mieszkańców
    for (int i = 0; i < town->number_of_residents; i++) {
        current = malloc(sizeof(struct Residents));
        if (current == NULL) {
            printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
            exit(EXIT_FAILURE);
        }
        current->val = malloc(sizeof(struct Resident));
        if (current->val == NULL) {
            printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
            exit(EXIT_FAILURE);
        }

        current->val->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (current->val->name == NULL) {
            printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
            exit(EXIT_FAILURE);
        }
        fread(current->val->name, sizeof(char), MAX_NAME_SURNAME, file);

        current->val->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (current->val->surname == NULL) {
            printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
            exit(EXIT_FAILURE);
        }
        fread(current->val->surname, sizeof(char), MAX_NAME_SURNAME, file);

        fread(&(current->val->gender), sizeof(char), 1, file);
        fread(&(current->val->age), sizeof(int), 1, file);
        fread(&(current->val->salary), sizeof(int), 1, file);

        *next = current;
        next = &current->next;
    }

    *next = NULL;

    // Wczytaj dane cmentarza
    fread(&town->graveyard->number_of_positions,sizeof(int), 1, file);
    fread(&town->graveyard->number_of_rows, sizeof(int), 1, file);

    // Alokuj pamięć dla alei cmentarza
    town->graveyard->alley = malloc(town->graveyard->number_of_rows * sizeof(struct Grave**));
    if (town->graveyard->alley == NULL) {
        printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
        exit(EXIT_FAILURE);
    }

    bool empty_position;  // Zmienna pomocnicza do sprawdzenia, czy pozycja na cmentarzu jest pusta

    // Wczytaj dane grobów
    for (int i = 0; i < town->graveyard->number_of_rows; i++) {
        town->graveyard->alley[i] = malloc(town->graveyard->number_of_positions * sizeof(struct Grave*));
        if (town->graveyard->alley[i] == NULL) {
            printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < town->graveyard->number_of_positions; j++) {
            // Wczytaj wartość dla pusta_pozycja z pliku
            fread(&empty_position, sizeof(bool), 1, file);

            // W przypadku w którym wartość dla pusta_pozycja jest false następne bity dotyczą danych zmarłego
            if (empty_position == false) {
                // Wczytaj dane zmarłego w grobie
                town->graveyard->alley[i][j] = malloc(sizeof(struct Grave));
                if (town->graveyard->alley[i][j] == NULL) {
                    printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
                    exit(EXIT_FAILURE);
                }

                town->graveyard->alley[i][j]->deceased = malloc(sizeof(struct Resident));
                if (town->graveyard->alley[i][j]->deceased == NULL) {
                    printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
                    exit(EXIT_FAILURE);
                }

                town->graveyard->alley[i][j]->deceased->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
                if (town->graveyard->alley[i][j]->deceased->name == NULL) {
                    printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
                    exit(EXIT_FAILURE);
                }
                fread(town->graveyard->alley[i][j]->deceased->name, sizeof(char), MAX_NAME_SURNAME, file);

                town->graveyard->alley[i][j]->deceased->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
                if (town->graveyard->alley[i][j]->deceased->surname == NULL) {
                    printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
                    exit(EXIT_FAILURE);
                }
                fread(town->graveyard->alley[i][j]->deceased->surname, sizeof(char), MAX_NAME_SURNAME, file);

                fread(&(town->graveyard->alley[i][j]->deceased->gender), sizeof(char), 1, file);
                fread(&(town->graveyard->alley[i][j]->deceased->age), sizeof(int), 1, file);
                fread(&(town->graveyard->alley[i][j]->deceased->salary), sizeof(int), 1, file);
                fread(&(town->graveyard->alley[i][j]->year_of_liquidation), sizeof(int), 1, file);
            // W przypadku w którym wartość dla pusta_pozycja jest true brak danych dotyczących zmarłego więc pętlę należy kontynuować
            } else {
                town->graveyard->alley[i][j] = NULL;
            }
        }
    }

    // Wczytaj dane dotyczące infrastruktury miejskiej
    fread(&town->hospitals, sizeof(int), 1, file);
    fread(&town->fire_departments, sizeof(int), 1, file);
    fread(&town->schools, sizeof(int), 1, file);

    // Wczytaj listę możliwych imion
    town->possible_names = malloc(NUMBER_OF_CATEGORY * sizeof(char**));
    if (town->possible_names == NULL) {
        printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUMBER_OF_CATEGORY; i++) {
        town->possible_names[i] = malloc(NUMBER_OF_NAME_SURNAME * sizeof(char*));
        if (town->possible_names[i] == NULL) {
            printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < NUMBER_OF_NAME_SURNAME; j++) {
            town->possible_names[i][j] = malloc(MAX_NAME_SURNAME * sizeof(char));
            if (town->possible_names[i][j] == NULL) {
                printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
                exit(EXIT_FAILURE);
            }
            fread(town->possible_names[i][j], sizeof(char), MAX_NAME_SURNAME, file);
        }
    }

    fclose(file);

    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
    
    printf("Wczytywanie zakończono sukcesem\n");
    printf("NACIŚNIJ ENTER ABY POWRÓCIĆ DO MENU");
    while (getchar() != '\n');    // Czekaj na naciśnięcie klawisza, aby wrócić do menu
}