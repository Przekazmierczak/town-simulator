#include <stdbool.h>

#ifndef RESIDENT_H
#define RESIDENT_H

enum Gender {
    Man,
    Woman
};

struct Resident {
    char* name;
    char* surname;
    enum Gender gender;
    int age;
    int salary;
};

#endif

char*** load_list_of_names_from_file();
struct Resident* create_resident(bool newborn, char*** list_of_possible_names);
void job(struct Resident* resident);
void free_resident(struct Resident* resident);