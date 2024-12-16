void death_of_residents(struct Town *town, struct Graveyard *graveyard, bool fire, bool flood, bool earthquake);
int chance_of_natural_death(int age);
int chance_of_death_from_flooding(struct Town *town);
int chance_of_death_from_fire(struct Town *town);
int chance_of_death_from_earthquake(struct Town *town);