#include "graveyard.h"

#ifndef TOWN_H
#define TOWN_H

struct Residents {
    struct Resident* val;
    struct Residents* next;
};

struct Town {
    struct Residents* residents;
    int number_of_residents;
    int year;
    long long budget;
    struct Graveyard* graveyard;
    int hospitals;
    int fire_departments;
    int schools;
    char*** possible_names;
};

#endif

struct Town* create_town(int liczba_mieszkańców, int budżet);
void add_resident(struct Town *miasteczko, bool noworodek);
void manage_residents(struct Town *miasteczko);
void town_informations(struct Town *miasteczko);
void residents_informations(struct Town *miasteczko);
void free_residents(struct Town *miasteczko);