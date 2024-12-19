#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "graveyard.h"

struct Graveyard* build_graveyard(int number_of_positions) {
    struct Graveyard *graveyard = malloc(sizeof(struct Graveyard));
    if (graveyard == NULL) {
        printf("Error: Failed to allocate memory for the graveyard in build_graveyard.\n");
        exit(EXIT_FAILURE);
    }
    graveyard->number_of_positions = number_of_positions;
    graveyard->number_of_rows = 1;

    // Allocate memory for the pointer to the array of rows
    graveyard->alley = malloc(sizeof(struct Grave**));
    if (graveyard->alley == NULL) {
        printf("Error: Failed to allocate memory for the graveyard->alley in build_graveyard.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the first row in the cemetery
    graveyard->alley[0] = malloc(number_of_positions * sizeof(struct Grave*));
    if (graveyard->alley[0] == NULL) {
        printf("Error: Failed to allocate memory for the graveyard->alley[0] in build_graveyard.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize grave spaces as NULL
    for (int i = 0; i < number_of_positions; i++) {
        graveyard->alley[0][i] = NULL;
    }
    return graveyard;
};

void expand_graveyard(struct Graveyard *graveyard) {
    graveyard->number_of_rows += 1;

    // Reallocate memory for the pointer to the array of rows
    graveyard->alley = realloc(graveyard->alley, graveyard->number_of_rows * sizeof(struct Grave**));
    if (graveyard->alley == NULL) {
        printf("Error: Failed to allocate memory for the graveyard->alley in expand_graveyard.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the new row
    graveyard->alley[graveyard->number_of_rows - 1] = malloc(graveyard->number_of_positions * sizeof(struct Grave*));
    if (graveyard->alley[graveyard->number_of_rows - 1] == NULL) {
        printf("Error: Failed to allocate memory for the graveyard->alley[graveyard->number_of_rows - 1] in expand_graveyard.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize grave spaces in the new row as NULL
    for (int i = 0; i < graveyard->number_of_positions; i++) {
        graveyard->alley[graveyard->number_of_rows - 1][i] = NULL;
    }
}

void add_deceased(struct Graveyard *graveyard, struct Resident *resident, int year_of_death, int *row, int *position) {
    for (; *row < graveyard->number_of_rows; (*row)++) {
        for (; *position < graveyard->number_of_positions; (*position)++) {
            // Check if the space is empty or if the grave qualifies for liquidation
            if (graveyard->alley[*row][*position] == NULL || graveyard->alley[*row][*position]->year_of_liquidation < year_of_death) {
                // If the space is empty, allocate memory for a new grave
                if (graveyard->alley[*row][*position] == NULL) {
                    graveyard->alley[*row][*position] = malloc(sizeof(struct Grave));
                    if (graveyard->alley[*row][*position] == NULL) {
                        printf("Error: Failed to allocate memory for the graveyard->alley[*row][*position] in add_deceased.\n");
                        exit(EXIT_FAILURE);
                    }
                // If the grave is already occupied, free the memory of the previous deceased
                } else {
                    free_resident(graveyard->alley[*row][*position]->deceased);
                }
                // Place the new deceased and calculate the year of grave liquidation
                graveyard->alley[*row][*position]->deceased = resident;
                graveyard->alley[*row][*position]->year_of_liquidation = specify_the_year_of_liquidation(resident, year_of_death);
                (*position)++;
                return;
            }
        }
        *position = 0;
    }
    // If no space is found, expand the cemetery and add the deceased to the new row
    expand_graveyard(graveyard);
    *position = 0;
    add_deceased(graveyard, resident, year_of_death, row, position);
    return;
}

int specify_the_year_of_liquidation(struct Resident *resident, int year_of_death) {
    // Calculate the year of grave liquidation based on the deceased's salary
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
    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
    
    for (int i = 0; i < graveyard->number_of_rows; i++) {
        for (int j = 0; j < graveyard->number_of_positions; j++) {
            printf("In row number %i, in spot number %i, rests: ", i, j);
            if (graveyard->alley[i][j] != NULL) {
                // Display the deceased's data if the grave is occupied
                printf("%s %s, ", graveyard->alley[i][j]->deceased->name, graveyard->alley[i][j]->deceased->surname);
                printf("Year of liquidation: %i\n", graveyard->alley[i][j]->year_of_liquidation);
            } else {
                // Display information that the space is empty
                printf("%s\n", "The spot is currently empty");
            }
        }
    }
    printf("PRESS ENTER TO RETURN TO THE MENU");
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