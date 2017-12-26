#ifndef ATTACK_H
#define ATTACK_H
#include <QtCore>

namespace Attack {
    int effectiveAttack(int attack, int additiveMod, float multiMod, float affinity, QColor sharpness, bool critBoost);
    int effectiveElement(QString element, float elementMod, float affinity, QColor sharpness, bool critElement);
}

#endif // ATTACK_H
