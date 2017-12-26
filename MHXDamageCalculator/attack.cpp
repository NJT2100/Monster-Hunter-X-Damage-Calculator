#include "attack.h"
#include <QtCore>
#include <QColor>

int Attack::effectiveAttack(int attack, int additiveMod, float multiMod, float affinity, QColor sharpness, bool critBoost)
{

    if (affinity > 100){
        affinity = 100;
    }
    affinity /= 100;
    float crit = 1.25;
    if (critBoost == true){
        crit = 1.4;
    }
    float sharpBonus;
    if (sharpness == Qt::yellow){
        sharpBonus = 1.0;
    } else if (sharpness == Qt::green){
        sharpBonus = 1.05;
    } else if (sharpness == Qt::blue){
        sharpBonus = 1.2;
    } else if (sharpness == Qt::white){
        sharpBonus = 1.32;
    } else {
        sharpBonus = 0.75;
    }
    return (((attack + additiveMod) * multiMod * crit * affinity) + (attack + additiveMod) * multiMod * (1 - affinity)) * sharpBonus;
}

int Attack::effectiveElement(QString element, float elementMod, float affinity, QColor sharpness, bool critElement)
{
    if (affinity > 100){
        affinity = 100;
    }
    affinity /= 100;
    float crit = 1;
    if (critElement == true){
        crit = 1.3;
    }
    if (elementMod > 1.2){
        elementMod = 1.2;
    }
    float sharpBonus = 0;
    if (sharpness == Qt::yellow){
        sharpBonus = 0.75;
    } else if (sharpness == Qt::green){
        sharpBonus = 1.0;
    } else if (sharpness == Qt::blue){
        sharpBonus = 1.0625;
    } else if (sharpness == Qt::white){
        sharpBonus = 1.125;
    }
    return ((element.toInt() * elementMod * crit * affinity) + (element.toInt() * elementMod * (1 - affinity))) * sharpBonus;
}
