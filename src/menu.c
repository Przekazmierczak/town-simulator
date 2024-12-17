#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "town.h"
#include "simulation.h"
#include "save_load.h"

void menu(struct Town *town) {
    for (;;) {
        // Clear the screen
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear");  // Linux/macOS
        #endif
        printf("MENU\n");
        printf("Choose an option:\n");
        printf("1. Continue simulation\n");
        printf("2. View basic town information\n");
        printf("3. View the list of residents\n");
        printf("4. View the graveyard\n");
        printf("5. Save\n");
        printf("6. Load\n");
        printf("7. Exit program\n");

        int option;  // Variable to store the selected option
        int input = scanf("%i", &option);  // Read the user's input (option)

        // If the entered value is invalid, clear the input buffer
        if (input != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);  // Clear the input buffer
        } else {
            getchar();  // Remove the newline character from the buffer
        }

        switch (option) {
            case 1:
                simulation(town);
                break;
            case 2:
                town_informations(town);
                break;
            case 3:
                residents_informations(town);
                break;
            case 4:
                list_of_deceased(town->graveyard);
                break;
            case 5:
                save_to_file(town);
                break;
            case 6:
                load_from_file(town);
                break;
            case 7:
                return;
        }
    }
}