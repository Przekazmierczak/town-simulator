#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resident.h"

const int NUMBER_OF_CATEGORY = 4;
const int NUMBER_OF_NAME_SURNAME = 40;
const int MAX_NAME_SURNAME = 20;

char*** load_list_of_names_from_file() {
    // Create an array to store possible first names and last names
    char ***list_of_possible_names = malloc(NUMBER_OF_CATEGORY * sizeof(char**));
    for (int i = 0; i < 4; i++) {
        list_of_possible_names[i] = malloc(NUMBER_OF_NAME_SURNAME * sizeof(char*));
        for (int j = 0; j < 40; j++) {
            list_of_possible_names[i][j] = malloc(MAX_NAME_SURNAME * sizeof(char));
        }
    }
    
    char buffer[10000];  // Buffer for reading a line from a file
    int line = 0;

    FILE *file = fopen("names.txt", "r");

    if (file == NULL) {
        printf("Error: Could not open the file names.txt\n");
        exit(EXIT_FAILURE);
    } else {
        // Read the file line by line
        while (!feof(file) && !ferror(file)) {
            if (fgets(buffer, 10000, file) != NULL) {
                int letter_line = 0;
                int letter_name = 0;
                int name = 0;
                // Iterate through all the characters in the given line
                while (buffer[letter_line] != '\0') {
                    if (buffer[letter_line] != ' ' && buffer[letter_line] != '\n') {
                        list_of_possible_names[line][name][letter_name] = buffer[letter_line];
                        letter_name++;
                    } else {
                        list_of_possible_names[line][name][letter_name] = '\0';  // End of first name/last name
                        name ++;  // Move to the next first name/last name
                        letter_name = 0;
                    }
                    letter_line++;
                }
                line++;  // Move to the next line
            }
        }
    }

    fclose(file);
    return list_of_possible_names;
}

struct Resident* create_resident(bool newborn, char*** list_of_possible_names) {
    struct Resident* resident = malloc(sizeof(struct Resident));
    if (resident == NULL) {
        printf("Error: Failed to allocate memory for the resident structure in create_resident.\n");
        exit(EXIT_FAILURE);
    }

    // Randomly assign the gender of the resident
    resident->gender = rand() % 2 ? Man : Woman;

    // Assign first name and last name based on gender
    if (resident->gender == Man) {
        char* name = list_of_possible_names[0][rand() % 40];
        resident->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->name == NULL) {
            printf("Error: Failed to allocate memory for the resident->name structure in create_resident.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->name, name);
    
        char* surname = list_of_possible_names[2][rand() % 40];
        resident->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->surname == NULL) {
            printf("Error: Failed to allocate memory for the resident->surname structure in create_resident.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->surname, surname);
    } else {
        char* name = list_of_possible_names[1][rand() % 40];
        resident->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->name == NULL) {
            printf("Error: Failed to allocate memory for the resident->name structure in create_resident.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->name, name);

        char* surname = list_of_possible_names[3][rand() % 40];
        resident->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (resident->surname == NULL) {
            printf("Error: Failed to allocate memory for the resident->surname structure in create_resident.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(resident->surname, surname);
    }

    // Set the age and salary of the resident based on whether they are a newborn
    if (newborn) {
        resident->age = 0;
        resident->salary = 0;
    } else {
        resident->age = rand() % 70;
        resident->salary = 0;
        // If the resident is at least 18 years old, randomly assign whether they have a job
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

// Try to assign a job to the resident and set a random salary
void job(struct Resident* resident) {
    if (rand() % 5 == 0) resident->salary = 5000 + (rand() % 10000);
}

void free_resident(struct Resident* resident) {
    free(resident->name);
    free(resident->surname);
    free(resident);
}