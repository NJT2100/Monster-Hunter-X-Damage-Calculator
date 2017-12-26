#include "dialog.h"
#include "ui_dialog.h"
#include "csvreader.h"
#include "attack.h"
#include "modifiers.h"
#include <vector>
#include <iterator>
#include <QDebug>
#include <QtCore>
#include <QFile>
//#include <QtWidgets>
#include <QtXml/QtXml>
#include <QMap>
#include <QSignalMapper>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QWidget::setWindowTitle ("MHX Attack Calculator");
    QWidget::setWindowIcon(QIcon("images/ItemIcon029.png"));
    QWidget::setWindowFlags(Qt::Window | Qt::WindowTitleHint);

    totalMods.additiveTotal = 0;
    totalMods.multiTotal = 1;
    totalMods.affinityTotal = 0;
    totalMods.critBoost = false;
    totalMods.critElem = false;
    totalMods.sharpnessLvl = 0;
    totalMods.bludgeoner = false;

    totalElem.element = "";
    totalElem.effect = 1;
    totalElem.EAU = false;

    initButtonGroups();
    initTable("Weapons/Greatsword.csv");
    loadAttributes("Modifiers.xml");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::loadAttributes(QString filename)
{
    QDomDocument document;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file";
        return;
    } else {
        if (!document.setContent(&file)){
            qDebug() << "Failed to set document";
            return;
        }
    }
    file.close();

    QDomElement root = document.firstChildElement();
    QDomNodeList items = root.elementsByTagName("mod");

    for (int i = 0; i < items.count(); i++){
        QDomNode itemNode = items.at(i);
        if (itemNode.isElement()){
            QDomElement itemElem = itemNode.toElement();
            QDomNode elemChild = itemElem.firstChild();
            Modifier mod;
            for (int i = 0; !elemChild.isNull(); i++){
                QDomElement perData = elemChild.toElement();
                switch(i){
                    case 0:
                        mod.name = perData.text();
                    break;
                    case 1:
                        mod.description = perData.text();
                    break;
                    case 2:
                        mod.additiveBonus = perData.text().toInt();
                    break;
                    case 3:
                        mod.multiBonus = perData.text().toFloat();
                    break;
                    case 4:
                        mod.affinityBonus = perData.text().toInt();
                    break;
                }
                modifiers[mod.name] = mod;
                elemChild = elemChild.nextSibling();
            }
        }
    }
}

void Dialog::initButtonGroups()
{
    QList<QButtonGroup *> groups;

    QButtonGroup *a = new QButtonGroup(this);
    groups.append(a);
    a->addButton(ui->PowerCharm);

    QButtonGroup *b = new QButtonGroup(this);
    groups.append(b);
    b->addButton(ui->PowerTalon);

    QButtonGroup *c1 = new QButtonGroup(this);
    groups.append(c1);
    c1->setExclusive(false);
    c1->addButton(ui->Demondrug);
    c1->addButton(ui->MegaDemondrug);

    QButtonGroup *c2 = new QButtonGroup(this);
    groups.append(c2);
    c2->setExclusive(false);
    c2->addButton(ui->AttackUpSMeal);
    c2->addButton(ui->AttackUpMMeal);
    c2->addButton(ui->AttackUpLMeal);

    QButtonGroup *d = new QButtonGroup(this);
    groups.append(d);
    d->setExclusive(false);
    d->addButton(ui->MightSeed);
    d->addButton(ui->MightPill);
    d->addButton(ui->Exciteshroom);
    d->addButton(ui->DemonS);
    d->addButton(ui->DemonHorn);
    d->addButton(ui->DemonAffinity);
    d->addButton(ui->CoolCat);

    QButtonGroup *e = new QButtonGroup(this);
    groups.append(e);
    e->addButton(ui->FelyneBooster);

    QButtonGroup *f = new QButtonGroup(this);
    groups.append(f);
    f->setExclusive(false);
    f->addButton(ui->AttackUpS);
    f->addButton(ui->AttackUpM);
    f->addButton(ui->AttackUpL);

    QButtonGroup *g = new QButtonGroup(this);
    groups.append(g);
    g->setExclusive(false);
    g->addButton(ui->Adrenaline);
    g->addButton(ui->FelyneHeroics);

    QButtonGroup *h = new QButtonGroup(this);
    groups.append(h);
    h->setExclusive(false);
    h->addButton(ui->AttackUpSHorn);
    h->addButton(ui->AttackUpLHorn);

    QButtonGroup *i = new QButtonGroup(this);
    groups.append(i);
    i->setExclusive(false);
    i->addButton(ui->Fortify1);
    i->addButton(ui->Fortify2);

    QButtonGroup *j = new QButtonGroup(this);
    groups.append(j);
    j->setExclusive(false);
    j->addButton(ui->Challenger1);
    j->addButton(ui->Challenger2);

    QButtonGroup *k = new QButtonGroup(this);
    groups.append(k);
    k->setExclusive(false);
    k->addButton(ui->PolarHunter1);
    k->addButton(ui->PolarHunter2);
    k->addButton(ui->PolarHunter3);
    k->addButton(ui->TropicHunter);

    QButtonGroup *m = new QButtonGroup(this);
    groups.append(m);
    m->addButton(ui->Resuscitate);

    QButtonGroup *n = new QButtonGroup(this);
    groups.append(n);
    m->addButton(ui->Furor);

    QButtonGroup *o = new QButtonGroup(this);
    groups.append(o);
    o->setExclusive(false);
    o->addButton(ui->CriticalEye1);
    o->addButton(ui->CriticalEye2);
    o->addButton(ui->CriticalEye3);

    QButtonGroup *q = new QButtonGroup(this);
    groups.append(q);
    q->addButton(ui->RepeatOffender);

    QButtonGroup *p = new QButtonGroup(this);
    groups.append(p);
    p->addButton(ui->WeaknessExploit);

    for (int j = 0; j < groups.size(); j++){
        QList<QAbstractButton *> buttons = groups.at(j)->buttons();
        if (groups.at(j)->buttons().size() > 1){
            connect(groups.at(j), SIGNAL(buttonClicked(int)), this, SLOT(setExclusive(int)));
        } else {
            groups.at(j)->setExclusive(false);
        }
        for (int i = 0; i < buttons.size();i++){
            connect(buttons.at(i), SIGNAL(toggled(bool)), this, SLOT(setModifiers(bool)));
            buttons.at(i)->setToolTip(modifiers[buttons.at(i)->text()].description);
        }
    }

    //Radio Buttons
    connect(ui->Sharp0, SIGNAL(clicked()), this, SLOT(setSharpness()));
    connect(ui->Sharp1, SIGNAL(clicked()), this, SLOT(setSharpness()));
    connect(ui->Sharp2, SIGNAL(clicked()), this, SLOT(setSharpness()));

    //Special Modifiers
    ui->EAU->setToolTip(modifiers[ui->EAU->text()].description);
    ui->CritBoost->setToolTip(modifiers[ui->CritBoost->text()].description);
    ui->CritElement->setToolTip(modifiers[ui->CritElement->text()].description);
    ui->Bludgeoner->setToolTip(modifiers[ui->Bludgeoner->text()].description);
}

void Dialog::initTable(std::string filename)
{
    CSVReader csv (filename);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setColumnWidth(0, 150);
    int rowCount = 0;
    while (!csv.eof()){
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
        std::vector<std::string> row = csv.read();
        int columnCount = 0;
        for (std::vector<std::string>::iterator itr = row.begin(); itr != row.end(); ++itr){
            QTableWidgetItem *itm = new QTableWidgetItem(QString::fromStdString(*itr));
            itm->setFlags(itm->flags() ^ Qt::ItemIsEditable);

            if (*itr == "R"){
                itm->setBackgroundColor(Qt::red);
            } else if (*itr == "O"){
                itm->setBackgroundColor(Qt::darkRed);
            } else if (*itr == "Y"){
                itm->setBackgroundColor(Qt::yellow);
            } else if (*itr == "G"){
                itm->setBackgroundColor(Qt::green);
            } else if (*itr == "B"){
                itm->setBackgroundColor(Qt::blue);
            } else if (*itr == "W"){
                itm->setBackgroundColor(Qt::white);
            } else {
                itm->setBackgroundColor(Qt::lightGray);
            }

             if (itm->text().contains('/') && columnCount == 2){
                QStringList elements = itm->text().split('/');
                itm->setIcon(QIcon("images/elements/" + elements.at(0) + "-" + elements.at(1) + ".png"));
            } else if (columnCount == 2 && itm->text() != ""){
                 itm->setIcon(QIcon("images/elements/" + itm->text() + ".png"));
              }

            if (columnCount > 4){
                itm->setText("");
            }
            ui->tableWidget->setItem(rowCount, columnCount, itm);
            columnCount++;
        }
        rowCount++;
    }
    csv.close();
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() - 1);
    ui->Calculate->setEnabled(false);
}

void Dialog::on_comboBox_activated(const QString &arg1)
{
    std::string path = "Weapons/" + arg1.toStdString() + ".csv";
    initTable(path);
}

void Dialog::setModifiers(bool toggled)
{
    QCheckBox *button = (QCheckBox *)sender();
    if (toggled){
        totalMods.additiveTotal += modifiers[button->text()].additiveBonus;
        totalMods.multiTotal *= modifiers[button->text()].multiBonus;
        totalMods.affinityTotal += modifiers[button->text()].affinityBonus;
    } else {
        totalMods.additiveTotal -= modifiers[button->text()].additiveBonus;
        totalMods.multiTotal /= modifiers[button->text()].multiBonus;
        totalMods.affinityTotal -= modifiers[button->text()].affinityBonus;
    }

}

void Dialog::setExclusive(int id)
{
    QButtonGroup *group = (QButtonGroup *)sender();
    QList<QAbstractButton *> list = group->buttons();
    for (int i = 0; i < list.size(); i++){
        if (group->id(list.at(i)) != id && list.at(i)->isChecked()){
            list.at(i)->setChecked(false);
        }
    }
}

void Dialog::setSharpness()
{
    QRadioButton *button = (QRadioButton *)sender();
    if (button->text() == "+0"){
        totalMods.sharpnessLvl = 0;
    } else if (button->text() == "+1"){
        totalMods.sharpnessLvl = 1;
    } else {
        totalMods.sharpnessLvl = 2;
    }
}

void Dialog::on_Calculate_clicked()
{
    QColor sharpLvl;
    float elementalMod = 1;
    if (totalMods.sharpnessLvl == 2){
        sharpLvl = weapon.plusTwo;
    } else if (totalMods.sharpnessLvl){
        sharpLvl = weapon.plusOne;
    } else {
        sharpLvl = weapon.plusZero;
    }
    int bludg = 0;
    if (totalMods.bludgeoner){
        if (sharpLvl == Qt::green){
            bludg = 15;
        } else if (sharpLvl == Qt::yellow){
            bludg = 25;
        }
    }
    float trueAtk = Attack::effectiveAttack(weapon.attack,
                            totalMods.additiveTotal, totalMods.multiTotal,
                                            weapon.affinity + totalMods.affinityTotal + bludg, sharpLvl, totalMods.critBoost);

    if (totalElem.element == weapon.element){
        if (totalElem.effect == 1){
            elementalMod = 1.1;
        } else if (totalElem.effect == 2){
            elementalMod = 1.15;
        }
    }
    if (totalElem.EAU){
        elementalMod += 0.1;
    }
    float trueElem = Attack::effectiveElement(weapon.elementalAttack,
                                              elementalMod, weapon.affinity + totalMods.affinityTotal, sharpLvl, totalMods.critElem);

    if (weapon.affinity + totalMods.affinityTotal > 100){
        ui->affintyLabel->setText((QString::number(100)));
    } else {
        ui->affintyLabel->setText((QString::number(weapon.affinity + totalMods.affinityTotal)));
    }

    ui->calcAttackLabel->setText(QString::number(trueAtk));

    if (weapon.element != "Blast" && weapon.element != "Sleep" && weapon.element != "Poison" && weapon.element != "Paralysis"){
        ui->calcElementLabel->setText(QString::number(trueElem));
    } else {
        ui->calcElementLabel->setText(QString::number(0));
    }
    if (ui->comboBox->currentText() == "Dual Blades" && weapon.element.contains('/')){
        calculate_dualblades();
    }
}

int Dialog::calculate_dualblades(){
    QStringList elements = weapon.element.split('/');
    QStringList power = weapon.elementalAttack.split('/');
    QString element1 = elements.at(0);
    QString element2 = elements.at(1);
    QString attack1 = power.at(0);
    QString attack2 = power.at(1);
    int trueElem1 = 0;
    int trueElem2 = 0;
    QColor sharpLvl;

    float elementalMod1 = 1;
    float elementalMod2 = 1;
    if (totalMods.sharpnessLvl == 2){
        sharpLvl = weapon.plusTwo;
    } else if (totalMods.sharpnessLvl){
        sharpLvl = weapon.plusOne;
    } else {
        sharpLvl = weapon.plusZero;
    }

    if (totalElem.element == element1){
        if (totalElem.effect == 1){
            elementalMod1 = 1.1;
        } else if (totalElem.effect == 2){
            elementalMod1 = 1.15;
        }
    }
    if (totalElem.element == element2){
        if (totalElem.effect == 1){
            elementalMod2 = 1.1;
        } else if (totalElem.effect == 2){
            elementalMod2 = 1.15;
        }
    }
    if (totalElem.EAU){
        elementalMod1 += 0.1;
        elementalMod2 += 0.1;
    }
    if (element1 != "Blast" && element1 != "Sleep" && element1 != "Poison" && element1 != "Paralysis"){
        trueElem1 = Attack::effectiveElement(attack1,
                                              elementalMod1, weapon.affinity + totalMods.affinityTotal, sharpLvl, totalMods.critElem);
    }

    if (element2 != "Blast" && element2 != "Sleep" && element2 != "Poison" && element2 != "Paralysis"){
        trueElem2 = Attack::effectiveElement(attack2,
                                              elementalMod2, weapon.affinity + totalMods.affinityTotal, sharpLvl, totalMods.critElem);
    }
    ui->calcElementLabel->setText(QString::number(trueElem1).append("/").append(QString::number(trueElem2)));
    return 0;
}

void Dialog::on_tableWidget_cellClicked(int row, int column)
{
    weapon.name = ui->tableWidget->item(row, 0)->text();
    weapon.attack = ui->tableWidget->item(row, 1)->text().toInt();
    weapon.element = ui->tableWidget->item(row, 2)->text();
    weapon.elementalAttack = ui->tableWidget->item(row, 3)->text();
    weapon.affinity = ui->tableWidget->item(row, 4)->text().toInt();
    weapon.plusZero = ui->tableWidget->item(row, 5)->background().color();
    weapon.plusOne = ui->tableWidget->item(row, 6)->background().color();
    weapon.plusTwo = ui->tableWidget->item(row, 7)->background().color();

    ui->nameLabel->setText(weapon.name);
    ui->affintyLabel->setText((QString::number(weapon.affinity)));
    ui->attackLabel->setText(QString::number(weapon.attack));
    ui->calcAttackLabel->setText("");
    ui->elementLabel->setText(weapon.elementalAttack);
    ui->calcElementLabel->setText("");

    ui->Calculate->setEnabled(true);
}

void Dialog::on_CritBoost_toggled(bool checked)
{
    if (checked){
        totalMods.critBoost = true;
    } else {
        totalMods.critBoost = false;
    }
}

void Dialog::on_CritElement_toggled(bool checked)
{
    if (checked){
        totalMods.critElem = true;
    } else {
        totalMods.critElem = false;
    }
}


void Dialog::on_EAU_toggled(bool checked)
{
    if (checked){
        totalElem.EAU = true;
    } else {
        totalElem.EAU = false;
    }
}

void Dialog::on_Bludgeoner_toggled(bool checked)
{
    if (checked){
        totalMods.bludgeoner = true;
    } else {
        totalMods.bludgeoner = false;
    }
}

void Dialog::on_comboBox_2_activated(const QString &arg1)
{
    QString element = arg1;
    if (element == "None"){
        totalElem.element = "";
        totalElem.effect = 0;
        return;
    } else {
        element = element.remove(QRegExp("Atk \\+"));
    }
    int effect;
    QStringList list;
    list = element.split(' ');
    element = list.at(0);
    effect = ((QString)list.at(1)).toInt();
    totalElem.element = element;
    totalElem.effect = effect;
}
