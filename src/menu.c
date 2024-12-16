#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "town.h"
#include "simulation.h"
#include "save_load.h"

void menu(struct Town *town) {
    for (;;) {
        // Wyczyść ekran
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear");  // Linux/macOS
        #endif
        printf("MENU\n");
        printf("Wybierz opcję:\n");
        printf("1. Kontynuuj symulacje\n");
        printf("2. Zobacz podstawowe informacje o miasteczku\n");
        printf("3. Zobacz listę mieszkańców\n");
        printf("4. Zobacz cmentarz\n");
        printf("5. Zapisz\n");
        printf("6. Wczytaj\n");
        printf("7. Zakończ program\n");

        int option;  // Zmienna do przechowania wybranej opcji
        int input = scanf("%i", &option);  // Odczytaj wejście użytkownika (opcję)

        // Jeśli podana wartość nie jest poprawna, czyści bufor wejściowy
        if (input != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);  // Oczyść bufor wejścia
        } else {
            getchar();  // Usuń enter z bufforu
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