#ifndef PLAYERKNOWLEDGE_H
#define PLAYERKNOWLEDGE_H

#include <QSet>
#include "cards.h"

class PlayerKnowledge
{
public:
    PlayerKnowledge();
    void addCharacter(Character* m_c) {characters.insert(m_c);}
    void addLocation(Location* m_l) {locations.insert(m_l);}
    void addWeapon(Weapon* m_w) {weapons.insert(m_w);}

private:
    QSet<Character*> characters;
    QSet<Location*> locations;
    QSet<Weapon*> weapons;


};

#endif // PLAYERKNOWLEDGE_H
