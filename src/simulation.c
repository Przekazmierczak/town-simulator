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

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>  // For Sleep() on Windows
// #include <conio.h>
#else
#include <unistd.h>   // For sleep() on Linux/macOS

#endif

void simulation(struct Town *town) {
    int next_planned_building = rand() % 3;  // Randomly select a building to be constructed

    // Clear the screen
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    int number_of_years;
    int simulation_speed;

    // Get the number of years to simulate from the user
    printf("Enter the number of years to simulate: ");
    int input_number_of_years = 0;
    while (input_number_of_years != 1) {
        input_number_of_years = scanf("%i", &number_of_years);  // Read the number of years to simulate
        if (input_number_of_years != 1) {
            printf("Enter the number of years to simulate (in numeric format): ");
            int c;
            // Clear the input buffer to avoid looping
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    // Get the simulation speed from the user
    printf("Enter the simulation speed (in ms): ");
    int input_simulation_speed = 0;
    while (input_simulation_speed != 1) {
        input_simulation_speed = scanf("%i", &simulation_speed);  // Read the simulation speed
        if (input_simulation_speed != 1) {
            printf("Enter the simulation speed (in numerical format): ");
            int c;
            // Clear the input buffer to avoid looping
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    // Main simulation loop
    for (int i = 0; i < number_of_years; i++) {
        // Clear the screen
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear");  // Linux/macOS
        #endif
        
        printf("----------------------------Year:%i----------------------------------\n",town->year);
        printf("Budżet: %lli\n", town->budget);

        manage_residents(town);  // Increase age, assign jobs, send to retirement, and collect taxes from all residents
        maintain_buildings(town);  // Pay maintenance for existing buildings
        next_planned_building = build_a_building(town, next_planned_building); // Build new buildings - if there are sufficient funds

        // Random occurrence of disasters
        bool fire = (rand() % 25 == 0);
        bool flood = (rand() % 25 == 0);
        bool earthquake = (rand() % 25 == 0);

        // Check if any resident has died; if so, remove them from the list of residents and add them to the graveyard
        death_of_residents(town, town->graveyard, fire, flood, earthquake);

        // Add new residents to the town, the number depends on the current population and the number of hospitals
        for (int i = 0; i < (int)(((town->number_of_residents) / 50) * hospital(town->number_of_residents / town->hospitals)) + 1; i++) {
            add_resident(town, true);
        }

        // Display information about the residents
        printf("----------------------------Residents---------------------------------\n");
        printf("Number of residents: %i\n", town->number_of_residents);

        // Display information about the cemetery
        printf("----------------------------Graveyard---------------------------------\n");
        printf("Number of rows: %i, number of positions: %i\n", town->graveyard->number_of_rows, town->graveyard->number_of_positions);

        // Display information about the buildings
        printf("----------------------------Buildings---------------------------------\n");
        printf("Number of hospitals: %i\n", town->hospitals);
        printf("Number of fire departments: %i\n", town->fire_departments);
        printf("Number of school buildings: %i\n", town->schools);

        // Display information about the disasters
        printf("----------------------------Disasters---------------------------------\n");
        if (!fire && !flood && !earthquake) printf("None\n");
        if (fire) printf(
            "                          (                                          \n"
            "                          )\\ )                                       \n"
            "                         (()/( (  (     (                            \n"
            "                          /(_)))\\ )(   ))\\                            \n"
            "                         (_))_((_|()\\ /((_)                          \n"
            "                         | |_  (_)((_|_))                             \n"
            "                         | __| | | '_/ -_)                            \n"
            "                         |_|   |_|_| \\___|                            \n"
            );
            
        if (flood) printf(
            "                      O                                              \n"
            "                       o O                 O                         \n"
            "                      O o     O  O      o   o                        \n"
            "                       ___  O   o  O   O    O   O                    \n"
            "                      |  _| _     o    o   _                         \n"
            "                      | |_ | |__O  _O_  __| |                        \n"
            "                      | __|| / _ \\/ _ \\/ _` |                        \n"
            "                      |_|  |_\\___/\\___/\\__,_|                        \n"
            );
        if (earthquake) printf(
            "                                                                     \n"
            "___\\/____\\/______\\/_______\\/____________\\/_____\\/_\\/_\\/_\\/_________\\/\n"
            "       ______           _   _                       _                \n"
            "      |  ____|         | | | |                     | |               \n"
            "      | |__   __ _ _ __| |_| |__   __ _ _   _  __ _| | _____         \n"
            "      |  __| / _` | '__| __| '_ \\ / _` | | | |/ _` | |/ / _ \\        \n"
            "      | |___| (_| | |  | |_| | | | (_| | |_| | (_| |   <  __/        \n"
            "      |______\\__,_|_|   \\__|_| |_|\\__, |\\__,_|\\__,_|_|\\_\\___|        \n"
            "                                      | |                            \n"
            "                                      |_|                            \n"
            );

        #ifdef _WIN32
            Sleep(simulation_speed);
        #else
            sleep(simulation_speed / 1000);
        #endif
    }
}
    