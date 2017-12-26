#ifndef WEAPON_H
#define WEAPON_H
#include <QtCore>

struct Weapon {
    QString name;
    int attack;
    QString element;
    QString elementalAttack;
    int affinity;
    QColor plusZero;
    QColor plusOne;
    QColor plusTwo;
};

#endif // WEAPON_H
