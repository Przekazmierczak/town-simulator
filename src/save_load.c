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

// Function to save town data to a binary file
void save_to_file(struct Town *town) {
    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("SAVING IN PROGRESS...\n");

    // Open the binary file for writing
    FILE *file = fopen("save.bin", "wb");

    if (!file){
        printf("Failed to create file for saving");
        exit(EXIT_FAILURE);
    }

    // Save basic information about the town
    fwrite(&town->number_of_residents, sizeof(int), 1, file);
    fwrite(&town->year, sizeof(int), 1, file);
    fwrite(&town->budget, sizeof(long long), 1, file);

    // Save data for each resident
    struct Residents *current_resident = town->residents;
    for (int i = 0; i < town->number_of_residents; i++) {
        fwrite(current_resident->val->name, sizeof(char), MAX_NAME_SURNAME, file);
        fwrite(current_resident->val->surname, sizeof(char), MAX_NAME_SURNAME, file);
        fwrite(&(current_resident->val->gender), sizeof(char), 1, file);
        fwrite(&(current_resident->val->age), sizeof(int), 1, file);
        fwrite(&(current_resident->val->salary), sizeof(int), 1, file);
        current_resident = current_resident->next;
    }

    // Save graveyeard data
    fwrite(&town->graveyard->number_of_positions,sizeof(int), 1, file);
    fwrite(&town->graveyard->number_of_rows, sizeof(int), 1, file);

    bool empty_position;  // Helper variable to check if a cemetery position is empty

    for (int i = 0; i < town->graveyard->number_of_rows; i++) {
        for (int j = 0; j < town->graveyard->number_of_positions; j++) {
            // If there is a deceased in the grave, save to the file empty_position as false and then the deceased's data
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
            // If the grave is empty, save to the file empty_position as true and continue
            } else {
                empty_position = true;
                fwrite(&(empty_position), sizeof(bool), 1, file);
            }
        }
    }

    // Save data related to the town's infrastructure
    fwrite(&town->hospitals, sizeof(int), 1, file);
    fwrite(&town->fire_departments, sizeof(int), 1, file);
    fwrite(&town->schools, sizeof(int), 1, file);

    // Save the list of possible names
    for (int i = 0; i < NUMBER_OF_CATEGORY; i++) {
        for (int j = 0; j < NUMBER_OF_NAME_SURNAME; j++) {
            fwrite(town->possible_names[i][j], sizeof(char), MAX_NAME_SURNAME, file);
        }
    }

    fclose(file);  // Close the file

    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("Saving completed successfully\n");
    printf("PRESS ENTER TO RETURN TO THE MENU");
    while (getchar() != '\n');  // Wait for a key press to return to the menu
}

// Function to load town data from a binary file
void load_from_file(struct Town *town) {
    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("LOADING IN PROGRESS...\n");

    // Open the binary file for reading
    FILE *file = fopen("save.bin", "rb");

    if (!file){
        // Clear the screen
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear");  // Linux/macOS
        #endif
        
        printf("Loading failed - file not found\n");
        printf("PRESS ENTER TO RETURN TO THE MENU");
        while (getchar() != '\n');  // Wait for a key press to return to the menu
        return;
    }

    // Free previously allocated memory
    free_graveyard(town->graveyard);
    free_residents(town);


    // Load basic town data
    fread(&town->number_of_residents, sizeof(int), 1, file);
    fread(&town->year, sizeof(int), 1, file);
    fread(&town->budget, sizeof(long long), 1, file);

    town->residents = NULL;

    struct Residents *current = NULL;
    struct Residents **next = &town->residents;

    // Load resident data
    for (int i = 0; i < town->number_of_residents; i++) {
        current = malloc(sizeof(struct Residents));
        if (current == NULL) {
            printf("Error: Failed to allocate memory in load_from_file.\n");
            exit(EXIT_FAILURE);
        }
        current->val = malloc(sizeof(struct Resident));
        if (current->val == NULL) {
            printf("Error: Failed to allocate memory in load_from_file.\n");
            exit(EXIT_FAILURE);
        }

        current->val->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (current->val->name == NULL) {
            printf("Error: Failed to allocate memory in load_from_file.\n");
            exit(EXIT_FAILURE);
        }
        fread(current->val->name, sizeof(char), MAX_NAME_SURNAME, file);

        current->val->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
        if (current->val->surname == NULL) {
            printf("Error: Failed to allocate memory in load_from_file.\n");
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

    // Load graveyard data
    fread(&town->graveyard->number_of_positions,sizeof(int), 1, file);
    fread(&town->graveyard->number_of_rows, sizeof(int), 1, file);

    // Allocate memory for the graveyard alley
    town->graveyard->alley = malloc(town->graveyard->number_of_rows * sizeof(struct Grave**));
    if (town->graveyard->alley == NULL) {
        printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
        exit(EXIT_FAILURE);
    }

    bool empty_position;  // Helper variable to check if a position in the graveyard is empty

    // Load grave data
    for (int i = 0; i < town->graveyard->number_of_rows; i++) {
        town->graveyard->alley[i] = malloc(town->graveyard->number_of_positions * sizeof(struct Grave*));
        if (town->graveyard->alley[i] == NULL) {
            printf("Error: Failed to allocate memory in load_from_file.\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < town->graveyard->number_of_positions; j++) {
            // Load the value for empty_position from the file
            fread(&empty_position, sizeof(bool), 1, file);

            // If the value of empty_position is false, the next bits contain the deceased's data
            if (empty_position == false) {
                // Load the deceased data from the grave
                town->graveyard->alley[i][j] = malloc(sizeof(struct Grave));
                if (town->graveyard->alley[i][j] == NULL) {
                    printf("Error: Failed to allocate memory in load_from_file.\n");
                    exit(EXIT_FAILURE);
                }

                town->graveyard->alley[i][j]->deceased = malloc(sizeof(struct Resident));
                if (town->graveyard->alley[i][j]->deceased == NULL) {
                    printf("Błąd: Nie udało się przydzielić w wczytaj_z_pliku.\n");
                    exit(EXIT_FAILURE);
                }

                town->graveyard->alley[i][j]->deceased->name = malloc(sizeof(char) * MAX_NAME_SURNAME);
                if (town->graveyard->alley[i][j]->deceased->name == NULL) {
                    printf("Error: Failed to allocate memory in load_from_file.\n");
                    exit(EXIT_FAILURE);
                }
                fread(town->graveyard->alley[i][j]->deceased->name, sizeof(char), MAX_NAME_SURNAME, file);

                town->graveyard->alley[i][j]->deceased->surname = malloc(sizeof(char) * MAX_NAME_SURNAME);
                if (town->graveyard->alley[i][j]->deceased->surname == NULL) {
                    printf("Error: Failed to allocate memory in load_from_file.\n");
                    exit(EXIT_FAILURE);
                }
                fread(town->graveyard->alley[i][j]->deceased->surname, sizeof(char), MAX_NAME_SURNAME, file);

                fread(&(town->graveyard->alley[i][j]->deceased->gender), sizeof(char), 1, file);
                fread(&(town->graveyard->alley[i][j]->deceased->age), sizeof(int), 1, file);
                fread(&(town->graveyard->alley[i][j]->deceased->salary), sizeof(int), 1, file);
                fread(&(town->graveyard->alley[i][j]->year_of_liquidation), sizeof(int), 1, file);
            // If the value for empty_position is true, there are no data for the deceased, so the loop should continue
            } else {
                town->graveyard->alley[i][j] = NULL;
            }
        }
    }

    // Load data related to the town's infrastructure
    fread(&town->hospitals, sizeof(int), 1, file);
    fread(&town->fire_departments, sizeof(int), 1, file);
    fread(&town->schools, sizeof(int), 1, file);

    // Load the list of possible names
    town->possible_names = malloc(NUMBER_OF_CATEGORY * sizeof(char**));
    if (town->possible_names == NULL) {
        printf("Error: Failed to allocate memory in load_from_file.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUMBER_OF_CATEGORY; i++) {
        town->possible_names[i] = malloc(NUMBER_OF_NAME_SURNAME * sizeof(char*));
        if (town->possible_names[i] == NULL) {
            printf("Error: Failed to allocate memory in load_from_file.\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < NUMBER_OF_NAME_SURNAME; j++) {
            town->possible_names[i][j] = malloc(MAX_NAME_SURNAME * sizeof(char));
            if (town->possible_names[i][j] == NULL) {
                printf("Error: Failed to allocate memory in load_from_file.\n");
                exit(EXIT_FAILURE);
            }
            fread(town->possible_names[i][j], sizeof(char), MAX_NAME_SURNAME, file);
        }
    }

    fclose(file);

    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
    
    printf("Loading completed successfully\n");
    printf("PRESS ENTER TO RETURN TO THE MENU");
    while (getchar() != '\n');  // Wait for a key press to return to the menu
}