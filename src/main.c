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
    
    // Wyczyść ekran
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif

    printf("Witaj w symulacji miasta!\n");

    // Poberz liczbę mieszkańców od użytkownika
    printf("Podaj początkową liczbę mieszkańców: ");
    int input_number_residents = 0;
    while (input_number_residents != 1) {
        input_number_residents = scanf("%i", &number_of_residents);  // Odczytaj liczbę mieszkańców
        if (input_number_residents != 1) {
            printf("Podaj początkową liczbę mieszkańców (w formacie liczbowym): ");
            int c;
            // Oczyść bufor wejściowy, aby uniknąć zapętlenia
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    // Pobierz początkowy budżet
    printf("Podaj budżet początkowy: ");
    int input_budget = 0;
    while (input_budget != 1) {
        input_budget = scanf("%i", &budget);  // Odczytaj budżet
        if (input_budget != 1) {
            printf("Podaj budżet początkowy (w formacie liczbowym): ");
            int c;
            // Oczyść bufor wejściowy, aby uniknąć zapętlenia
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
    
    // Stwórz strukturę Miasteczko z danymi użytkownika
    struct Town *town = create_town(number_of_residents, budget);

    // Wywołaj funkcję menu, aby pozwolić użytkownikowi na interakcję z symulacją
    menu(town);

    return EXIT_SUCCESS;  // Zakończ program
}