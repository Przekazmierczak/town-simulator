#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "resident.h"
#include "town.h"
#include "graveyard.h"
#include "death.h"
#include "buildings.h"
#include "simulation.h"
#include "menu.h"

int main() {
    setlocale(LC_ALL, "pl_PL.UTF-8");
    srand(time(NULL));

    int number_of_residents;
    int budget;
    
    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("Welcome to the city simulation!\n");

    // Get the number of residents from the user
    printf("Enter the initial number of residents: ");
    int input_number_residents = 0;
    while (input_number_residents != 1) {
        input_number_residents = scanf("%i", &number_of_residents);  // Read the number of residents
        if (input_number_residents != 1) {
            printf("Enter the initial number of residents (in numeric format): ");
            int c;
            // Clear the input buffer to avoid loop
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    // Get the initial budget
    printf("Enter the initial budget: ");
    int input_budget = 0;
    while (input_budget != 1) {
        input_budget = scanf("%i", &budget);  // Read the budget
        if (input_budget != 1) {
            printf("Enter the initial budget (in numeric format): ");
            int c;
            // Clear the input buffer to avoid loop
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
    
    // Create the Town structure with user data
    struct Town *town = create_town(number_of_residents, budget);

    // Call the menu function to allow the user to interact with the simulation
    menu(town);

    return EXIT_SUCCESS;  // End the program
}