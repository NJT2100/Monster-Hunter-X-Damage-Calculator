#ifndef MODIFIERS_H
#define MODIFIERS_H

#include <QColor>

struct Modifier{
    QString name;
    QString description;
    int additiveBonus;
    float multiBonus;
    int affinityBonus;
};

struct CurrentModifier{
    int additiveTotal;
    float multiTotal;
    int affinityTotal;
    bool critBoost;
    bool critElem;
    bool bludgeoner;
    int sharpnessLvl;
};

struct ElementalModifier{
    QString element;
    float effect;
    bool EAU;
};

#endif // MODIFIERS_H
