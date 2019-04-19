#ifndef CARDDICTIONARY_H
#define CARDDICTIONARY_H

#include <QMap>

class CardDictionary
{
public:
    CardDictionary();
    QMap<qint64, QString>* getCharacterMap() {return CharacterMap;}
    QMap<qint64, QString>* getLocationMap() {return LocationMap;}
    QMap<qint64, QString>* getWeaponMap() {return WeaponMap;}

    QString translateCharacter(qint64 t_char) {return CharacterMap->value(t_char);}
    QString translateLocation(qint64 t_char) {return LocationMap->value(t_char);}
    QString translateWeapon(qint64 t_char) {return WeaponMap->value(t_char);}


    bool checkSolution(qint64 t_char, qint64 t_loc, qint64 t_weap);

    // use these if we don't want to follow clue rules, ie we just let the player know what they got wrong
    // otherwise we need to disprove in order etc, which becomes a todo
    bool checkCharacter(qint64 t_char);
    bool checkLocation(qint64 t_char);
    bool checkWeapon(qint64 t_char);

    qint64 getCharacterSolution(){return characterSolution;}
    qint64 getLocationSolution(){return locationSolution;}
    qint64 getWeaponSolution(){return weaponSolution;}

private:
    QMap<qint64, QString>* CharacterMap;
    QMap<qint64, QString>* LocationMap;
    QMap<qint64, QString>* WeaponMap;
    qint64 characterSolution;
    qint64 locationSolution;
    qint64 weaponSolution;

};

#endif // CARDDICTIONARY_H
