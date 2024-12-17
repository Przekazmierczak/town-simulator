#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "town.h"
#include "graveyard.h"
#include "death.h"
#include "buildings.h"

void death_of_residents(struct Town *town, struct Graveyard *graveyard, bool fire, bool flood, bool earthquake) {
    if (town->residents == NULL) return; // Return if there are no residents 

    int row = 0;
    int position = 0;

    // Check the residents at the beginning of the list
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
            town->residents = town->residents->next; // Remove the current resident from the list  
            free(temp); // Free the current resident memory
            town->number_of_residents -= 1; // Decrease the number of residents
            death_of_first_resident = true;
        }
    }

    // Check the residents in the remaining part of the list 
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
            add_deceased(graveyard, current_resident->next->val, town->year, &row, &position); // Add the deceased to the cemetery
            struct Residents *temp = current_resident->next;
            current_resident->next = current_resident->next->next; // Remove the current resident from the list
            free(temp); // Free the current resident memory
            town->number_of_residents -= 1; // Decrease the number of residents
        } else {
            current_resident = current_resident->next; // Move to the next resident
        }
    }
}

int chance_of_natural_death(int age) {
    int range = age / 20; // Divide age into 20-year intervals
    switch (range) {
        case 0:  // Age 0-19
            return 1;
        case 1:  // Age 20-39
            return 2;
        case 2:  // Age 40-59
            return 4;
        case 3:  // Age 60-79
            return 50;
        case 4:  // Age 80-99
            return 300;
        case 5:  // Age 100-119
            return 800;
        default: // Age 120 and more
            return 950;
    }
}

// Determine the additional risk of death due to fire - the value depends on the ratio of residents to fire stations
int chance_of_death_from_fire(struct Town *town) {
    return 150 * fire_department(town->number_of_residents / town->fire_departments);
}

// Determine the additional risk of death due to flood - the value depends on the ratio of residents to fire stations
int chance_of_death_from_flooding(struct Town *town) {
    return 100 * fire_department(town->number_of_residents / town->fire_departments);
}

// Determine the additional risk of death due to earthquake - the value depends on the ratio of residents to fire stations
int chance_of_death_from_earthquake(struct Town *town) {
    return 50 * fire_department(town->number_of_residents / town->fire_departments);
}