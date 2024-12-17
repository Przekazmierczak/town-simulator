#include <stdio.h>
#include <stdlib.h>
#include "resident.h"
#include "town.h"

// Build a new building depending on the type of the planned building if the budget is sufficient  
// If a new building is built, change the type of the planned building to the next random one
int build_a_building(struct Town *town, int next_planned_building) {
    while (
        (next_planned_building == 0 && town->budget > 40000000) ||  // Budget for the hospital
        (next_planned_building == 1 && town->budget > 25000000) ||  // Budget for the fire department
        (next_planned_building == 2 && town->budget > 20000000)     // Budget for the school
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
        next_planned_building = rand() % 3;  // Select the next building randomly
    }
    return next_planned_building;  // If the budget is insufficient, return the same building
}

// Subtract the maintenance costs of all building types from the town's budget
void maintain_buildings(struct Town *town) {
    town->budget -= town->hospitals * 200000;
    town->budget -= town->fire_departments * 100000;
    town->budget -= town->schools * 50000;
}

// Determine the efficiency of the fire department based on the number of residents per fire station building
float fire_department(int resident_fire_department_ratio) {
    if (resident_fire_department_ratio < 100) return 0.2;
    if (resident_fire_department_ratio < 200) return 0.4;
    if (resident_fire_department_ratio < 500) return 0.6;
    if (resident_fire_department_ratio < 1000) return 0.8;
    return 1;
}

// Determine the efficiency of the hospital based on the number of residents per hospital
float hospital(int resident_hospital_ratio) {
    if (resident_hospital_ratio < 100) return 1.5;
    if (resident_hospital_ratio < 200) return 1.4;
    if (resident_hospital_ratio < 500) return 1.3;
    if (resident_hospital_ratio < 1000) return 1.2;
    return 1;
}

// Determine the efficiency of the school based on the number of residents per school building
float school(int resident_school_ratio) {
    if (resident_school_ratio < 100) return 1.5;
    if (resident_school_ratio < 200) return 1.4;
    if (resident_school_ratio < 500) return 1.3;
    if (resident_school_ratio < 1000) return 1.2;
    return 1;
}