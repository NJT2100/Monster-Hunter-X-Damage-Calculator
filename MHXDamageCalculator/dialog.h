#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QMap>
#include "weapon.h"
#include "modifiers.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

    void initTable(std::string filename);
    void initButtonGroups();
    void loadAttributes(QString filename);

public:
    char *weaponPath;
    Weapon weapon;
    CurrentModifier totalMods;
    ElementalModifier totalElem;
    QMap<QString, Modifier> modifiers;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_comboBox_activated(const QString &arg1);
    void setModifiers(bool toggled);
    void setExclusive(int id);
    void setSharpness();

    void on_Calculate_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_CritBoost_toggled(bool checked);

    void on_CritElement_toggled(bool checked);

    void on_comboBox_2_activated(const QString &arg1);

    void on_EAU_toggled(bool checked);

    void on_Bludgeoner_toggled(bool checked);

private:
    Ui::Dialog *ui;
    int calculate_dualblades();
};

#endif // DIALOG_H
