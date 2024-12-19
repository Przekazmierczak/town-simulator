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
        printf("Error: Failed to allocate memory for the town in create_town.\n");
        exit(EXIT_FAILURE);
    }
    town->residents = NULL;  // Initialize the list of residents
    town->number_of_residents = 0; // Initial number of residents
    town->year = 0; // Initial year
    town->budget = budget; // Initial budget

    int number_of_positions = 10;
    town->graveyard = build_graveyard(number_of_positions);

    town->hospitals = 1;  // Initial number of hospitals
    town->fire_departments = 1;  // Initial number of fire departments
    town->schools = 1;  // Initial number of schools

    town->possible_names = load_list_of_names_from_file();

    // Add initial residents to the town
    for (int i = 0; i < number_of_residents; i++) {
        add_resident(town, false);
    }

    return town;
}

void add_resident(struct Town *town, bool newborn) {
    struct Resident *resident = create_resident(newborn, town->possible_names);
    struct Residents *current_resident = malloc(sizeof(struct Residents));
    if (current_resident == NULL) {
        printf("Error: Failed to allocate memory for the node in add_resident.\n");
        exit(EXIT_FAILURE);
    }
    current_resident->val = resident;  // Assign a new resident
    current_resident->next = town->residents; // Add a new resident to the beginning of the list
    town->residents = current_resident;
    town->number_of_residents += 1; // Increase the number of residents
}

// Increase age, assign jobs, send to retirement, and collect taxes from all resident
void manage_residents(struct Town *town) {
    town->year += 1; // Increment the current year
    struct Residents *current_resident = town->residents;

    while (current_resident != NULL) {
        current_resident->val->age += 1; // Increase the age of the current resident

        // Assign a job if the current resident is 18 years old or older and is unemployed
        if (current_resident->val->age >= 18 && current_resident->val->salary == 0) {
            job(current_resident->val);
        }

        // Terminate employment when the current resident has reached the retirement age
        if (current_resident->val->age >= 65 && current_resident->val->salary != 0) {
            current_resident->val->salary = 0;
        }
        // Collect taxes - the tax amount is additionally increased based on the ratio of the number of residents to the number of schools
        town->budget += (int)(current_resident->val->salary * 0.3 * school(town->number_of_residents / town->schools));
        current_resident = current_resident->next;
    }
}

void town_informations(struct Town *town) {
    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
    printf("Year: %i\n", town->year);
    printf("Budget: %lli\n", town->budget);
    printf("Number of residents: %i\n", town->number_of_residents);
    printf("Graveyard size: %i rows\n", town->graveyard->number_of_rows);
    printf("Number of hospitals: %i\n", town->hospitals);
    printf("Number of fire department buildings: %i\n", town->fire_departments);
    printf("Number of school buildings: %i\n", town->schools);
    printf("PRESS ENTER TO RETURN TO THE MENU");
    while (getchar() != '\n');  // Wait for a key press to return to the menu
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
        // Print the current resident's data
        printf("%i: %s %s Gender: %s Age: %i Salary: %i\n",
            count,
            current_resident->val->name,
            current_resident->val->surname,
            current_resident->val->gender == Man ? "Man" : "Woman",
            current_resident->val->age,
            current_resident->val->salary
            );
        current_resident = current_resident->next; // Move to the next resident
        count++;
    }
    printf("PRESS ENTER TO RETURN TO THE MENU");
    while (getchar() != '\n');  // Wait for a key press to return to the menu
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