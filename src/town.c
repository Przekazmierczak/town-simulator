#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "town.h"
#include "graveyard.h"
#include "buildings.h"
#include "menu.h"

struct Town* create_town(int number_of_residents, int budget) {
    struct Town *town = malloc(sizeof(struct Town));
    if (town == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla miasteczka w stwórz_miasteczko.\n");
        exit(EXIT_FAILURE);
    }
    town->residents = NULL;  // Inicjalizacja listy mieszkańców
    town->number_of_residents = 0; // Początkowa liczba mieszkańców
    town->year = 0; // Rok początkowy
    town->budget = budget; // Budżet początkowy

    int number_of_positions = 10;
    town->graveyard = build_graveyard(number_of_positions);

    town->hospitals = 1;  // Początkowa liczba szpitali
    town->fire_departments = 1;  // Początkowa liczba straży pożarnej
    town->schools = 1;  // Początkowa liczba szkół

    town->possible_names = load_list_of_names_from_file();

    // Dodaj początkowych mieszkańców do miasteczka
    for (int i = 0; i < number_of_residents; i++) {
        add_resident(town, false);
    }

    return town;
}

void add_resident(struct Town *town, bool newborn) {
    struct Resident *resident = create_resident(newborn, town->possible_names);
    struct Residents *current_resident = malloc(sizeof(struct Residents));
    if (current_resident == NULL) {
        printf("Błąd: Nie udało się przydzielić pamięci dla węzła w dodaj_mieszkańca.\n");
        exit(EXIT_FAILURE);
    }
    current_resident->val = resident;  // Przypisz nowego mieszkańca
    current_resident->next = town->residents; // Dodaj nowego mieszkańca na początek listy
    town->residents = current_resident;
    town->number_of_residents += 1; // Zwiększ liczbę mieszkańców
}

// Zwiększ wiek, przydziel pracę, wyślij na emeryturę oraz zbierz podatki od wszystkich mieszkańców
void manage_residents(struct Town *town) {
    town->year += 1; // Zwiększ aktualny rok
    struct Residents *current_resident = town->residents;

    while (current_resident != NULL) {
        current_resident->val->age += 1; // Zwiększ wiek aktualnego mieszkańca

        // Przyznaj pracę, jeśli aktualny mieszkaniec ma 18 lat lub więcej i nie pracuje
        if (current_resident->val->age >= 18 && current_resident->val->salary == 0) {
            job(current_resident->val);
        }

        // Zakończ pracę gdy, aktualny mieszkaniec osiągnął wiek emerytalny
        if (current_resident->val->age >= 65 && current_resident->val->salary != 0) {
            current_resident->val->salary = 0;
        }
        // Zbierz podatki - wysokość podatków zwiększona jest dodatkowo od wartość stosunku ilości mieszkańców do ilości szkół
        town->budget += (int)(current_resident->val->salary * 0.3 * school(town->number_of_residents / town->schools));
        current_resident = current_resident->next;
    }
}

void town_informations(struct Town *town) {
    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
    printf("Rok: %i\n", town->year);
    printf("Budżet: %lli\n", town->budget);
    printf("Ilość mieszkańców: %i\n", town->number_of_residents);
    printf("Wielkość cmentarza: %i rzędów\n", town->graveyard->number_of_rows);
    printf("Ilość szpitali: %i\n", town->hospitals);
    printf("Ilość budynków straży pożarnej: %i\n", town->fire_departments);
    printf("Ilość budynków szkolnych: %i\n", town->schools);
    printf("NACIŚNIJ ENTER ABY POWRÓCIĆ DO MENU");
    while (getchar() != '\n');    // Czekaj na naciśnięcie klawisza, aby wrócić do menu
}

void residents_informations(struct Town *town) {
    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    struct Residents *current_resident = town->residents;
    int count = 1;
    while (current_resident != NULL) {
        // Wydrukuj dane aktualnego mieszkańca
        printf("%i: %s %s Płeć: %s Wiek: %i Pensja: %i\n",
            count,
            current_resident->val->name,
            current_resident->val->surname,
            current_resident->val->gender == Man ? "Mężczyna" : "Kobieta",
            current_resident->val->age,
            current_resident->val->salary
            );
        current_resident = current_resident->next; // Przejdź do kolejnego mieszkańca
        count++;
    }
    printf("NACIŚNIJ ENTER ABY POWRÓCIĆ DO MENU");
    while (getchar() != '\n');    // Czekaj na naciśnięcie klawisza, aby wrócić do menu
}

void free_residents(struct Town *town) {
    while (town->residents != NULL) {
        struct Residents *następny_mieszkaniec = town->residents->next;
        free(town->residents->val->name);
        free(town->residents->val->surname);
        free(town->residents->val);
        free(town->residents);
        town->residents = następny_mieszkaniec;
    }
}