#ifndef SOLUTION_H
#define SOLUTION_H

#include "cards.h"


class Solution
{
public:
    Solution(){characterSol = Character::player1; locationSol = Location::A; weaponSol = Weapon::a;}
    ~Solution(){}
    inline Character getCharacter(){return characterSol;}
    inline Location getLocation(){return locationSol;}
    inline Weapon getWeapon(){return weaponSol;}

private:
    Character characterSol;
    Location locationSol;
    Weapon  weaponSol;

};



#endif // SOLUTION_H
