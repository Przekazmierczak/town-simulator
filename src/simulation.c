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
    int next_planned_building = rand() % 3;  // Losowy wybór budynku, który będzie budowany

    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    int number_of_years;
    int simulation_speed;

    // Pobierz ilość lat do zasymulowania od użytkownika
    printf("Podaj ilość lat do zasymulowania: ");
    int input_number_of_years = 0;
    while (input_number_of_years != 1) {
        input_number_of_years = scanf("%i", &number_of_years);  // Odczytaj ilość lat do zasymulowania
        if (input_number_of_years != 1) {
            printf("Podaj ilość lat do zasymulowania (w formacie liczbowym): ");
            int c;
            // Oczyść bufor wejściowy, aby uniknąć zapętlenia
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    // Pobierz szybkość symulacji od użytkownika
    printf("Podaj szybkość symulacji (w ms): ");
    int input_simulation_speed = 0;
    while (input_simulation_speed != 1) {
        input_simulation_speed = scanf("%i", &simulation_speed);  // Odczytaj szybkość symulacji
        if (input_simulation_speed != 1) {
            printf("Podaj szybkość symulacji (w formacie liczbowym): ");
            int c;
            // Oczyść bufor wejściowy, aby uniknąć zapętlenia
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    // Główna pętla symulacji
    for (int i = 0; i < number_of_years; i++) {
        // Wyczyść ekran
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear");  // Linux/macOS
        #endif
        
        printf("----------------------------Rok:%i-----------------------------------\n",town->year);
        printf("Budżet: %lli\n", town->budget);

        manage_residents(town);  // Zwiększ wiek, przydziel pracę, wyślij na emeryturę oraz zbierz podatki od wszystkich mieszkańców
        maintain_buildings(town);  // Opłać utrzymanie istniejących budynków
        next_planned_building = build_a_building(town, next_planned_building); // Buduj nowe budynków - o ile są fundusze

        // Losowe wystąpienie katastrof
        bool fire = (rand() % 25 == 0);
        bool flood = (rand() % 25 == 0);
        bool earthquake = (rand() % 25 == 0);

        // Sprawdź, czy któryś z mieszkańców umarł, jeżeli tak, usuń go z listy mieszkańców i dodaj na cmentarz
        death_of_residents(town, town->graveyard, fire, flood, earthquake);

        // Dodaj nowych mieszkańców do miasteczka, liczba zależy od aktualnej populacji i liczby szpitali
        for (int i = 0; i < (int)(((town->number_of_residents) / 50) * hospital(town->number_of_residents / town->hospitals)) + 1; i++) {
            add_resident(town, true);
        }

        // Wyświetl informację o mieszkańcach
        printf("----------------------------Mieszkańcy---------------------------------\n");
        printf("Ilość mieszkańców: %i\n", town->number_of_residents);

        // Wyświetl informację o cmentarzu
        printf("----------------------------Cmentarz-----------------------------------\n");
        printf("Ilość rzędów: %i, ilość pozycji: %i\n", town->graveyard->number_of_rows, town->graveyard->number_of_positions);

        // Wyświetl informację o budynkach
        printf("----------------------------Budynki------------------------------------\n");
        printf("Ilość szpitali: %i\n", town->hospitals);
        printf("Ilość budynków straży pożarnej: %i\n", town->fire_departments);
        printf("Ilość budynków szkolnych: %i\n", town->schools);

        // Wyświetl informację o katastrofach
        printf("----------------------------Katastrofy---------------------------------\n");
        if (!fire && !flood && !earthquake) printf("Brak\n");
        if (fire) printf(
            "                    (                                                \n"
            "                    )\\ )                                             \n"
            "                   (()/(             )  (                            \n"
            "                    /(_)) (   (   ( /(  )(                           \n"
            "                   (_))   )\\  )\\  )(_))(()\\                          \n"
            "                   | _ \\ ((_)((_)((_)_  ((_)                         \n"
            "                   |  _// _ \\|_ // _` || '_|                         \n"
            "                   |_|  \\___//__|\\__,_||_|                           \n"
            );
        if (flood) printf(
            "                    O                                                \n"
            "                     o O                 O                           \n"
            "                    O o     O  O      o   o                          \n"
            "                    O  O  O   o  O   O    O   O                      \n"
            "                   O___  O   o  O   O    O _O o                      \n"
            "                   | _ \\___O_ __ _O___o __| |___                     \n"
            "                   |  _/ _ \\ V  V / _ \\/ _` |_ /                     \n"
            "                   |_| \\___/\\_/\\_/\\___/\\__,_/__|                     \n"
            );
        if (earthquake) printf(
            "                                                                     \n"
            "___\\/____\\/______\\/_______\\/____________\\/_____\\/_\\/_\\/_\\/_________\\/\n"
            "                                                                     \n"
            " _____                  _            _            _                _ \n"
            "|_   _| __ _______  ___(_) ___ _ __ (_) ___   ___(_) ___ _ __ ___ (_)\n"
            "  | || '__|_  / _ \\/ __| |/ _ \\ '_ \\| |/ _ \\ |_  / |/ _ \\ '_ ` _ \\| |\n"
            "  | || |   / /  __/\\__ \\ |  __/ | | | |  __/  / /| |  __/ | | | | | |\n"
            "  |_||_|  /___\\___||___/_|\\___|_| |_|_|\\___| /___|_|\\___|_| |_| |_|_|\n"
            );

        // printf("NACIŚNIJ PRZYCISK ABY ZATRZYMAĆ SYMULACJĘ");
        // if (kbhit()) {  // Nasłuchuj wciśnięcia przycisku
        //     getchar();   // Wczytaj wciśnięty klawisz aby nie wyświetlić go w terminalu
        //     system("cls");
        //     return;
        // }

        #ifdef _WIN32
            Sleep(szybkość_symulacji);
        #else
            sleep(simulation_speed / 1000);
        #endif
    }
}
    