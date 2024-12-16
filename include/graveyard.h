#ifndef GRAVEYARD_H
#define GRAVEYARD_H

struct Graveyard {
    int number_of_positions;
    int number_of_rows;
    struct Grave ***alley;
};

struct Grave {
    struct Resident *deceased;
    int year_of_liquidation;
};

#endif

struct Graveyard* build_graveyard(int number_of_positions);
void expand_graveyard(struct Graveyard *graveyard);
void add_deceased(struct Graveyard *graveyard, struct Resident *resident, int year_of_death, int *row, int *position);
int specify_the_year_of_liquidation(struct Resident *resident, int year_of_death);
void list_of_deceased(struct Graveyard *graveyard);
void free_graveyard(struct Graveyard *graveyard);