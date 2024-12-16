#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "town.h"
#include "graveyard.h"
#include "death.h"
#include "buildings.h"

void death_of_residents(struct Town *town, struct Graveyard *graveyard, bool fire, bool flood, bool earthquake) {
    if (town->residents == NULL) return; // Powróć gdy brak jest mieszkańców

    int row = 0;
    int position = 0;

    // Sprawdź mieszkańców na początku listy
    bool death_of_first_resident = true;
    while (town->residents != NULL && death_of_first_resident) {
        int chance_of_death = chance_of_natural_death(town->residents->val->age);
        death_of_first_resident = false;
        if (fire) {
            chance_of_death += chance_of_death_from_fire(town);
        }
        if (flood) {
            chance_of_death += chance_of_death_from_flooding(town);
        }
        if (earthquake) {
            chance_of_death += chance_of_death_from_earthquake(town);
        }
        if (chance_of_death > rand() % 1000) {
            add_deceased(graveyard, town->residents->val, town->year, &row, &position);
            struct Residents *temp = town->residents;
            town->residents = town->residents->next; // Usuń aktualnego mieszkańca z listy
            free(temp); // Zwolnij pamięć zarezerwowaną na aktualnego mieszkańca
            town->number_of_residents -= 1; // Zmniejsz liczbę mieszkańców
            death_of_first_resident = true;
        }
    }

    // Sprawdź mieszkańców w pozostałej części listy
    struct Residents *current_resident = town->residents;
    while (current_resident != NULL && current_resident->next != NULL) {
        int chance_of_death = chance_of_natural_death(town->residents->val->age);
        if (fire) {
            chance_of_death += chance_of_death_from_fire(town);
        }
        if (flood) {
            chance_of_death += chance_of_death_from_flooding(town);
        }
        if (earthquake) {
            chance_of_death += chance_of_death_from_earthquake(town);
        }
        if (chance_of_death > rand() % 1000) {
            add_deceased(graveyard, current_resident->next->val, town->year, &row, &position); // Dodaje zmarłego na cmentarz
            struct Residents *temp = current_resident->next;
            current_resident->next = current_resident->next->next; // Usuń aktualnego mieszkańca z listy
            free(temp); // Zwolnij pamięć zarezerwowaną na aktualnego mieszkańca
            town->number_of_residents -= 1; // Zmniejsza liczbę mieszkańców
        } else {
            current_resident = current_resident->next; // Przejdź do kolejnego mieszkańca
        }
    }
}

int chance_of_natural_death(int age) {
    int range = age / 20; // Podziel wiek na przedziały co 20 lat
    switch (range) {
        case 0:  // Wiek 0-19
            return 1;
        case 1:  // Wiek 20-39
            return 2;
        case 2:  // Wiek 40-59
            return 4;
        case 3:  // Wiek 60-79
            return 50;
        case 4:  // Wiek 80-99
            return 300;
        case 5:  // Wiek 100-119
            return 800;
        default: // Wiek 120 i wyżej
            return 950;
    }
}

// Określ dodatkowe ryzyko śmierci w wyniku pożaru - wartość zależy od stosunku ilości mieszkańców do straży pożarnej
int chance_of_death_from_fire(struct Town *town) {
    return 150 * fire_department(town->number_of_residents / town->fire_departments);
}

// Określ dodatkowe ryzyko śmierci w wyniku powodzi - wartość zależy od stosunku ilości mieszkańców do straży pożarnej
int chance_of_death_from_flooding(struct Town *town) {
    return 100 * fire_department(town->number_of_residents / town->fire_departments);
}

// Określ dodatkowe ryzyko śmierci w wyniku trzęsienia ziemi - wartość zależy od stosunku ilości mieszkańców do straży pożarnej
int chance_of_death_from_earthquake(struct Town *town) {
    return 50 * fire_department(town->number_of_residents / town->fire_departments);
}