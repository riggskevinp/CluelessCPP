#include "CardDictionary.h"

CardDictionary::CardDictionary()
{
    CharacterMap->insert(0, "player1");
    CharacterMap->insert(1, "player2");
    CharacterMap->insert(2, "player3");
    CharacterMap->insert(3, "player4");
    CharacterMap->insert(4, "player5");
    CharacterMap->insert(5, "player6");

    LocationMap->insert(0, "study");
    LocationMap->insert(1, "hallway1");
    LocationMap->insert(2, "hall");
    LocationMap->insert(3, "hallway2");
    LocationMap->insert(4, "lounge");
    LocationMap->insert(10, "hallway3");
    LocationMap->insert(11, "inaccess1");
    LocationMap->insert(12, "hallway4");
    LocationMap->insert(13, "inaccess2");
    LocationMap->insert(14, "hallway5");
    LocationMap->insert(20, "library");
    LocationMap->insert(21, "hallway6");
    LocationMap->insert(22, "billiard");
    LocationMap->insert(23, "hallway7");
    LocationMap->insert(24, "dining");
    LocationMap->insert(30, "hallway8");
    LocationMap->insert(31, "inaccess3");
    LocationMap->insert(32, "hallway9");
    LocationMap->insert(33, "inaccess4");
    LocationMap->insert(34, "hallway10");
    LocationMap->insert(40, "conservatory");
    LocationMap->insert(41, "hallway11");
    LocationMap->insert(42, "ball");
    LocationMap->insert(43, "hallway12");
    LocationMap->insert(44, "kitchen");


    WeaponMap->insert(0, "rope");
    WeaponMap->insert(1, "pipe");
    WeaponMap->insert(2, "knife");
    WeaponMap->insert(3, "wrench");
    WeaponMap->insert(4, "candelstick");
    WeaponMap->insert(5, "revolver");

    // todo set random solution Map.size() and QRand
    characterSolution = 3;
    locationSolution = 42;
    weaponSolution = 0;


}

bool CardDictionary::checkSolution(qint64 t_char, qint64 t_loc, qint64 t_weap)
{
    if(t_char == getCharacterSolution() && t_loc == getLocationSolution() && t_weap == getWeaponSolution()){
        return true;
    }
    return false;
}

bool CardDictionary::checkCharacter(qint64 t_char)
{
    if(t_char == getCharacterSolution()){
        return true;
    }
    return false;
}

bool CardDictionary::checkLocation(qint64 t_char)
{
    if(t_char == getLocationSolution()){
        return true;
    }
    return false;
}

bool CardDictionary::checkWeapon(qint64 t_char)
{
    if(t_char == getWeaponSolution()){
        return true;
    }
    return false;
}
