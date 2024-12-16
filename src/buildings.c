#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "town.h"

// Wybuduj nowy budynek w zależności od rodzaju planowanego budynku w przypadku posiadania wystarczającego budżetu
// W przypadku wybudowania nowego budynku zmień rodzaj planowanego budynku na kolejny losowy
int build_a_building(struct Town *town, int next_planned_building) {
    while (
        (next_planned_building == 0 && town->budget > 40000000) ||  // Budżet na szpital
        (next_planned_building == 1 && town->budget > 25000000) ||  // Budżet na straż pożarną
        (next_planned_building == 2 && town->budget > 20000000)     // Budżet na szkołę
    ) {
        if (next_planned_building == 0) {
            town->hospitals += 1;
            town->budget -= 8000000;
        } else if (next_planned_building == 1) {
            town->fire_departments += 1;
            town->budget -= 5000000;
        } else if (next_planned_building == 2) {
            town->schools += 1;
            town->budget -= 4000000;
        }
        next_planned_building = rand() % 3;  // Wybierz kolejny budynek losowo
    }
    return next_planned_building;  // W przypadku braku budżetu zwróc ten sam budynek
}

// Odejmij koszty utrzymania wszystkich typów budynków od budżetu miasteczka
void maintain_buildings(struct Town *town) {
    town->budget -= town->hospitals * 200000;
    town->budget -= town->fire_departments * 100000;
    town->budget -= town->schools * 50000;
}

// Określ efektywność straży pożarnej w zależności od liczby mieszkańców przypadających na budynek straży
float fire_department(int resident_fire_department_ratio) {
    if (resident_fire_department_ratio < 100) return 0.2;
    if (resident_fire_department_ratio < 200) return 0.4;
    if (resident_fire_department_ratio < 500) return 0.6;
    if (resident_fire_department_ratio < 1000) return 0.8;
    return 1;
}

// Określa efektywność szpitala w zależności od liczby mieszkańców przypadających na budynek szpitalny
float hospital(int resident_hospital_ratio) {
    if (resident_hospital_ratio < 100) return 1.5;
    if (resident_hospital_ratio < 200) return 1.4;
    if (resident_hospital_ratio < 500) return 1.3;
    if (resident_hospital_ratio < 1000) return 1.2;
    return 1;
}

// Określa efektywność szkoły w zależności od liczby mieszkańców przypadających na budynek szkolny
float school(int resident_school_ratio) {
    if (resident_school_ratio < 100) return 1.5;
    if (resident_school_ratio < 200) return 1.4;
    if (resident_school_ratio < 500) return 1.3;
    if (resident_school_ratio < 1000) return 1.2;
    return 1;
}